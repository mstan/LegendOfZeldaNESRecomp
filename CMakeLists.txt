cmake_minimum_required(VERSION 3.20)
project(LegendOfZeldaNESRecomp C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(NESRECOMP_GAME_VERSION "1.12.3" CACHE STRING "Game release version")

set(NESRECOMP_ROOT "${CMAKE_SOURCE_DIR}/nesrecomp" CACHE PATH
    "Path to the NESRecomp source tree")
set(RECOMP_UI_ROOT "${CMAKE_SOURCE_DIR}/recomp-ui" CACHE PATH
    "Path to the recomp-ui source tree")
set(ZELDA_STOCK_GENERATED_ROOT "${CMAKE_SOURCE_DIR}/generated" CACHE PATH
    "Path containing the generated stock Zelda recompilation sources")
set(ZELDA_HD_GENERATED_ROOT "${CMAKE_SOURCE_DIR}/generated" CACHE PATH
    "Path containing the generated Zelda HD recompilation sources")

# nesrecomp/ and recomp-ui/ are git submodules (see .gitmodules); the pinned
# commit is the gitlink recorded in this repo. After cloning run setup.sh/.bat
# (git submodule update --init) to fetch them at the recorded commits.

# Zelda opts into NESRecomp's package runtime. This also opens recomp-ui's Mods
# surface; the feature remains disabled until selected in the launcher.
set(NESRECOMP_ENABLE_MODS ON CACHE BOOL
    "Enable package mods for Zelda" FORCE)
set(NESRECOMP_BACKEND "cycle" CACHE STRING "CPU backend: cycle or legacy")
set_property(CACHE NESRECOMP_BACKEND PROPERTY STRINGS cycle legacy)
# Qualified Windows x64 renderer default; explicit cache choices remain honored.
if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8 AND
   CMAKE_SYSTEM_PROCESSOR MATCHES "^(AMD64|amd64|x86_64|X86_64)$")
    set(NESRECOMP_PPU_IMPL "FRAME" CACHE STRING "PPU implementation: LLE, HLE or FRAME")
    option(NESRECOMP_APU_OPERATION "Use the native APU operation profile" OFF)
    option(NESRECOMP_NATIVE_WAIT_LOOPS "Use qualified native wait operations" ON)
endif()
if(NESRECOMP_BACKEND STREQUAL "cycle")
    set(NESRECOMP_ROM "" CACHE FILEPATH "Original Zelda USA PRG0 ROM, not distributed")
    set(NESRECOMP_HOST_COMPILER "" CACHE FILEPATH "Optional prebuilt compiler")
    # Which cartridge image profile under config/cycle/ this build targets:
    #   stock       - USA PRG0 retail image (payload CRC32 3fe272fb)
    #   relocalized - the SimpleDanny/Snarfblam translation + font mod image
    #                 (payload CRC32 397b56a6)
    # The profile names the recompiler game config and the discovery inputs
    # (config/cycle/game-cycle-<variant>.toml, cyc_seeds_<variant>.txt,
    # cyc_captures_<variant>.txt). The ROM itself is hashed at configure time:
    # its PRG hash, launcher SHA-256 and mod image identity all come from the
    # image passed in NESRECOMP_ROM, so a profile only has to describe how the
    # code is discovered, not which bytes are compiled.
    set(ZELDA_ROM_VARIANT "stock" CACHE STRING
        "Cartridge image profile to build: stock or relocalized")
    set_property(CACHE ZELDA_ROM_VARIANT PROPERTY STRINGS stock relocalized)
    # One executable for several cartridge images. NESRECOMP_ROM is the image
    # the code is generated from (the retail USA data set); every image listed
    # here is accepted at runtime by src/cyc_extras.c (cyc_game_image_accepted)
    # and in the launcher. Each must keep the executable layout: the same
    # [[mod_function_hook]] signatures, the same RAM-code copy and the same
    # seed addresses. Compiled bytes are still verified per block by the PRG
    # guards, so a differing region falls back to the interpreter.
    set(ZELDA_EXTRA_ROMS "" CACHE STRING
        "Additional cartridge images (semicolon separated .nes paths) this executable also runs")
    set(NESRECOMP_CYC_EXTRA_ROM_SHA256 "")
    set(ZELDA_ACCEPTED_IMAGES_C "")
    # Images this executable accepts come from a checked-in manifest of hashes,
    # not from the ROM files a machine happens to have: cartridge data is never
    # part of this repository. The acceptance table and the launcher allowlist
    # are therefore complete on any clone; NESRECOMP_ROM is only the image the
    # native code is generated from and is validated against the same list.
    set(ZELDA_ACCEPTED_MANIFEST "${CMAKE_CURRENT_SOURCE_DIR}/config/cartridges/accepted-images.txt")
    if(NOT EXISTS "${ZELDA_ACCEPTED_MANIFEST}")
        message(FATAL_ERROR "missing ${ZELDA_ACCEPTED_MANIFEST}")
    endif()
    set(_zelda_image_fnv "")
    set(_zelda_extra_hashes "")
    file(STRINGS "${ZELDA_ACCEPTED_MANIFEST}" _zelda_manifest_lines)
    foreach(_zelda_line IN LISTS _zelda_manifest_lines)
        string(STRIP "${_zelda_line}" _zelda_line)
        if(_zelda_line STREQUAL "" OR _zelda_line MATCHES "^#")
            continue()
        endif()
        string(REPLACE "|" ";" _zelda_parts "${_zelda_line}")
        list(LENGTH _zelda_parts _zelda_parts_len)
        if(_zelda_parts_len LESS 4)
            message(FATAL_ERROR "accepted-images.txt: bad line '${_zelda_line}'")
        endif()
        list(GET _zelda_parts 1 _zelda_fnv)
        list(GET _zelda_parts 2 _zelda_sha)
        list(APPEND _zelda_image_fnv "0x${_zelda_fnv}u")
        list(APPEND _zelda_extra_hashes "${_zelda_sha}")
    endforeach()
    list(REMOVE_DUPLICATES _zelda_image_fnv)
    list(REMOVE_DUPLICATES _zelda_extra_hashes)
    list(LENGTH _zelda_image_fnv _zelda_manifest_count)
    message(STATUS "Zelda accepted images: ${_zelda_manifest_count} from accepted-images.txt")
    set(NESRECOMP_CYC_EXTRA_ROM_SHA256 "${_zelda_extra_hashes}")

    # The compiled image should be one of the listed ones. A local image that is
    # not listed is still built, with a warning, so a developer can experiment
    # before adding its hashes to the manifest.
    if(NESRECOMP_ROM OR ZELDA_EXTRA_ROMS)
        find_package(Python3 3.11 REQUIRED COMPONENTS Interpreter)
        set(_zelda_images "${NESRECOMP_ROM}")
        list(APPEND _zelda_images ${ZELDA_EXTRA_ROMS})
        list(REMOVE_DUPLICATES _zelda_images)
        foreach(_zelda_image IN LISTS _zelda_images)
            if(NOT _zelda_image OR NOT EXISTS "${_zelda_image}")
                message(FATAL_ERROR "ZELDA_EXTRA_ROMS: ${_zelda_image} does not exist")
            endif()
            execute_process(COMMAND "${Python3_EXECUTABLE}" -c
                "import sys,hashlib,zlib
d=open(sys.argv[1],'rb').read()
s=16 if len(d)>16 and d[:3] in (b'NES',b'FDS') else 0
p=d[s:]
h=2166136261
for b in p: h=((h^b)*16777619)&0xffffffff
print('fnv=0x%08x crc=%08x sha=%s' % (h, zlib.crc32(p)&0xffffffff, hashlib.sha256(p).hexdigest()))"
                "${_zelda_image}"
                OUTPUT_VARIABLE _zelda_identity OUTPUT_STRIP_TRAILING_WHITESPACE
                RESULT_VARIABLE _zelda_identity_result)
            if(NOT _zelda_identity_result EQUAL 0)
                message(FATAL_ERROR "cannot hash ${_zelda_image}")
            endif()
            string(REGEX MATCH "fnv=0x([0-9a-f]+) crc=([0-9a-f]+) sha=([0-9a-f]+)" _zelda_m "${_zelda_identity}")
            set(_zelda_fnv "${CMAKE_MATCH_1}")
            set(_zelda_crc "${CMAKE_MATCH_2}")
            set(_zelda_sha "${CMAKE_MATCH_3}")
            if(_zelda_image STREQUAL NESRECOMP_ROM)
                message(STATUS "Zelda compiled image: ${_zelda_image} (FNV ${_zelda_fnv}, CRC32 ${_zelda_crc})")
            else()
                message(STATUS "Zelda extra image accepted: ${_zelda_image} (FNV ${_zelda_fnv}, CRC32 ${_zelda_crc})")
            endif()
            list(APPEND _zelda_extra_hashes "${_zelda_sha}")
            if(NOT "0x${_zelda_fnv}u" IN_LIST _zelda_image_fnv)
                message(WARNING "${_zelda_image} (FNV ${_zelda_fnv}) is not listed in accepted-images.txt; other builds will not accept it")
                list(APPEND _zelda_image_fnv "0x${_zelda_fnv}u")
            endif()
        endforeach()
        list(REMOVE_DUPLICATES _zelda_image_fnv)
        list(REMOVE_DUPLICATES _zelda_extra_hashes)
        set(NESRECOMP_CYC_EXTRA_ROM_SHA256 "${_zelda_extra_hashes}")
    endif()

    string(REPLACE ";" ", " _zelda_fnv_list "${_zelda_image_fnv}")
    set(_zelda_images_c "${CMAKE_BINARY_DIR}/zelda_accepted_images.c")
    file(WRITE "${_zelda_images_c}"
"/* Generated by CMakeLists.txt from config/cartridges/accepted-images.txt.
 * Images this executable accepts: FNV-1a of the PRG payload, the value the
 * host compares (cyc_prg_hash()). Images other than the compiled one run on
 * the cycle-accurate interpreter (see docs/ROM_HEADERS.md). */
#include <stdint.h>
#include <stdbool.h>

static const uint32_t zelda_accepted_images[] = { ${_zelda_fnv_list} };

bool cyc_game_image_accepted(uint32_t prg_hash) {
    for (unsigned i = 0; i < sizeof zelda_accepted_images / sizeof zelda_accepted_images[0]; ++i)
        if (zelda_accepted_images[i] == prg_hash) return true;
    return false;
}
")
    set(ZELDA_ACCEPTED_IMAGES_C "${_zelda_images_c}")
    # The image identity the preloaded mod packages advertise: CRC32 of the
    # cartridge payload after its 16-byte iNES header, exactly as
    # runner/src/mod_runtime.cpp crc32_file() computes it. Two dumps of the same
    # retail title can differ here (revision, header), so the packages are
    # filtered by this value rather than by the profile name.
    set(ZELDA_ROM_PAYLOAD_CRC32 "")
    if(NESRECOMP_ROM)
        find_package(Python3 3.11 REQUIRED COMPONENTS Interpreter)
        execute_process(COMMAND "${Python3_EXECUTABLE}" -c
            "import sys,zlib;d=open(sys.argv[1],'rb').read();s=16 if len(d)>16 and d[:3] in (b'NES',b'FDS') else 0;print('%08x'%(zlib.crc32(d[s:])&0xffffffff))"
            "${NESRECOMP_ROM}"
            OUTPUT_VARIABLE ZELDA_ROM_PAYLOAD_CRC32 OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE _zelda_crc_result)
        if(NOT _zelda_crc_result EQUAL 0 OR NOT ZELDA_ROM_PAYLOAD_CRC32)
            message(WARNING "cannot hash the payload of ${NESRECOMP_ROM}; preloaded mod packages are staged unfiltered")
            set(ZELDA_ROM_PAYLOAD_CRC32 "")
        else()
            message(STATUS "Zelda cartridge image identity: payload CRC32 ${ZELDA_ROM_PAYLOAD_CRC32}")
        endif()
    endif()
    include("${NESRECOMP_ROOT}/runner/cyc/project.cmake")
    function(add_zelda_cycle TARGET VARIANT ROM)
        nesrecomp_add_cycle_game(${TARGET}
            ROM "${ROM}" GAME_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/config/cycle/game-cycle-${VARIANT}.toml"
            RECOMPILER "${NESRECOMP_HOST_COMPILER}" SEED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/config/cycle/cyc_seeds_${VARIANT}.txt"
            CAPTURE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/config/cycle/cyc_captures_${VARIANT}.txt"
            MODS HD_PACKS GAME_ID "legend-of-zelda-us" PLAYERS 1 REGION NTSC
            BOXART "${CMAKE_CURRENT_SOURCE_DIR}/assets/boxart.tga"
            HOST_EXTRAS src/cyc_extras.c src/zelda_voxel.c src/zelda_world.c src/zelda_world_canvas.c src/zelda_world_rooms.c src/zelda_voxel_plugin.c
                "${NESRECOMP_ROOT}/runner/cyc/cyc_presentation.c"
                "${NESRECOMP_ROOT}/runner/src/voxel_renderer.c"
                "${NESRECOMP_ROOT}/runner/src/voxel_screen_profile.c"
                "${NESRECOMP_ROOT}/runner/src/keybinds.c")
        target_include_directories(${TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
        target_compile_definitions(${TARGET} PRIVATE ZELDA_CYCLE NESRECOMP_CYCLE_PRESENTATION
              NESRECOMP_HDPACK_LAYER_PLANS=1 NESRECOMP_NATIVE_PRG_GUARDS=1
              NESRECOMP_ISOLATED_NATIVE=1 NESRECOMP_VOXEL_RETAINED_TERRAIN=1)
        if(MSVC)
            # Keep an optimized release while retaining its matching crash symbols.
            target_compile_options(${TARGET} PRIVATE /Zi)
            target_link_options(${TARGET} PRIVATE /DEBUG /OPT:REF /OPT:ICF)
        endif()
        # Stage the preloaded packages this image can actually run. Each package
        # declares its images in [[target]] rom_crc32, and the world/voxel bodies
        # additionally re-check the retail PRG CRC32 themselves
        # (zelda_world_supported(), src/zelda_world.c). Staging a package that
        # targets another image surfaces as an unresolvable Mods conflict on its
        # default-enabled feature and blocks PLAY, so the filter is the image
        # identity computed above — not the profile name.
        file(GLOB _zelda_mod_packages LIST_DIRECTORIES true
            "${CMAKE_CURRENT_SOURCE_DIR}/mods/preloaded/packages/*")
        foreach(_zelda_pkg IN LISTS _zelda_mod_packages)
            get_filename_component(_zelda_pkg_name "${_zelda_pkg}" NAME)
            set(_zelda_pkg_images "")
            file(GLOB _zelda_pkg_manifests "${_zelda_pkg}/*/manifest.toml")
            foreach(_zelda_manifest IN LISTS _zelda_pkg_manifests)
                file(READ "${_zelda_manifest}" _zelda_manifest_text)
                string(REGEX MATCHALL "rom_crc32[ \t]*=[ \t]*\"[0-9a-fA-F]+\"" _zelda_crc_lines
                    "${_zelda_manifest_text}")
                foreach(_zelda_crc_line IN LISTS _zelda_crc_lines)
                    string(REGEX REPLACE ".*\"([0-9a-fA-F]+)\".*" "\\1" _zelda_crc "${_zelda_crc_line}")
                    string(TOLOWER "${_zelda_crc}" _zelda_crc)
                    list(APPEND _zelda_pkg_images "${_zelda_crc}")
                endforeach()
            endforeach()
            if(ZELDA_ROM_PAYLOAD_CRC32 AND NOT ZELDA_ROM_PAYLOAD_CRC32 IN_LIST _zelda_pkg_images)
                message(STATUS "${TARGET}: ${_zelda_pkg_name} targets ${_zelda_pkg_images}, "
                               "not this image (${ZELDA_ROM_PAYLOAD_CRC32}); not staged")
                continue()
            endif()
            add_custom_command(TARGET ${TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${_zelda_pkg}"
                    "$<TARGET_FILE_DIR:${TARGET}>/mods/packages/${_zelda_pkg_name}")
        endforeach()
        # A MinGW/GCC build links the C++ runtime, the unwinder and the threads
        # shim dynamically, so a machine without the toolchain needs those DLLs
        # beside the executable — the same reason nesrecomp_add_cycle_game
        # stages the VC runtime for MSVC. Without them the program dies in the
        # loader with 0xC0000135 (STATUS_DLL_NOT_FOUND) before main().
        if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            get_filename_component(_zelda_cxx_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
            set(_zelda_mingw_runtime_dlls)
            foreach(_zelda_pattern "libgcc_s_*.dll" "libstdc++-*.dll" "libwinpthread-*.dll")
                file(GLOB _zelda_found "${_zelda_cxx_bin}/${_zelda_pattern}")
                list(APPEND _zelda_mingw_runtime_dlls ${_zelda_found})
            endforeach()
            foreach(_zelda_dll IN LISTS _zelda_mingw_runtime_dlls)
                add_custom_command(TARGET ${TARGET} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_zelda_dll}"
                        "$<TARGET_FILE_DIR:${TARGET}>")
            endforeach()
            if(NOT _zelda_mingw_runtime_dlls)
                message(WARNING "${TARGET}: no MinGW runtime DLLs found beside ${CMAKE_CXX_COMPILER}; "
                                "the build will need the toolchain on PATH to start")
            endif()
        endif()
    endfunction()
    # One target, built from the profile's discovery inputs and the image given
    # in NESRECOMP_ROM. A profile other than stock exists because the cartridge
    # data differs, not the code: the build still hashes the supplied image.
    set(_zelda_profile_toml "${CMAKE_CURRENT_SOURCE_DIR}/config/cycle/game-cycle-${ZELDA_ROM_VARIANT}.toml")
    if(NOT EXISTS "${_zelda_profile_toml}")
        message(FATAL_ERROR "ZELDA_ROM_VARIANT=${ZELDA_ROM_VARIANT}: ${_zelda_profile_toml} does not exist "
                            "(profiles: stock, relocalized)")
    endif()
    message(STATUS "Zelda cartridge profile: ${ZELDA_ROM_VARIANT} (${_zelda_profile_toml})")
    add_zelda_cycle(LegendOfZeldaNESRecomp "${ZELDA_ROM_VARIANT}" "${NESRECOMP_ROM}")
    # Windows icon: embedded as a resource so Explorer, the taskbar and the
    # launcher window show the game's icon. The .ico is generated from
    # assets/icon/zelda.png (see launcher/ in the packaged folder).
    set(ZELDA_ICON_ICO "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon/zelda.ico")
    if(WIN32 AND EXISTS "${ZELDA_ICON_ICO}")
        enable_language(RC)
        set(_zelda_icon_rc "${CMAKE_BINARY_DIR}/zelda_icon.rc")
        file(WRITE "${_zelda_icon_rc}"
"// Generated by CMakeLists.txt: the program icon (assets/icon/zelda.ico,
// generated from assets/icon/zelda.png).
IDI_ICON1 ICON \"${ZELDA_ICON_ICO}\"
")
        target_sources(LegendOfZeldaNESRecomp PRIVATE "${_zelda_icon_rc}")
        message(STATUS "Zelda program icon: ${ZELDA_ICON_ICO}")
    endif()
    if(ZELDA_ACCEPTED_IMAGES_C)
        # The accepted-image table is generated from config/cartridges/accepted-images.txt.
        target_sources(LegendOfZeldaNESRecomp PRIVATE "${ZELDA_ACCEPTED_IMAGES_C}")
    endif()
    if(WIN32 AND TARGET LegendOfZeldaNESRecomp)
        # Launcher utilities: IShellLinkW (shell32) and its CLSIDs/IIDs (uuid),
        # used by recomp-ui's "desktop shortcut" / "add to Steam" buttons.
        target_link_libraries(LegendOfZeldaNESRecomp PRIVATE shell32 uuid)
    endif()
    option(ZELDA_TEST_NATIVE_JOBS "Build ROM-backed isolated memory-job oracle" OFF)
    if(ZELDA_TEST_NATIVE_JOBS)
        target_sources(LegendOfZeldaNESRecomp PRIVATE tests/native_sprite_service.c)
        target_compile_definitions(LegendOfZeldaNESRecomp PRIVATE NESRECOMP_TEST_NATIVE_JOBS=1)
    endif()
    option(ZELDA_BUILD_RENDER_TESTS "Build targeted world-canvas differential test" OFF)
    if(ZELDA_BUILD_RENDER_TESTS)
        enable_testing()
        add_executable(zelda_world_canvas_test tests/world_canvas_test.c)
        add_test(NAME zelda_world_canvas_test COMMAND zelda_world_canvas_test)
    endif()
    return()
elseif(NOT NESRECOMP_BACKEND STREQUAL "legacy")
    message(FATAL_ERROR "NESRECOMP_BACKEND must be cycle or legacy")
endif()

include(${NESRECOMP_ROOT}/runner/runner.cmake)

list(APPEND CMAKE_PREFIX_PATH "${NESRECOMP_ROOT}/runner/external/SDL2/cmake")
find_package(SDL2 REQUIRED)

# ---- Pre-boot GUI launcher (recomp-ui / Dear ImGui) ----
# recomp-ui/ is the pinned shared launcher submodule. Its module
# defines recomp_target_launcher_ui() (RECOMP_LAUNCHER on the target, staging the
# launcher assets); called once per game variant below.
include(${RECOMP_UI_ROOT}/recomp_ui.cmake)

# ---- Game variants: stock + HD (two builds from one project) ----
# Legacy reference only; current cycle releases use tools/make_release.ps1.
#   LegendOfZeldaNESRecomp      (default) — recompiled from STOCK zelda.nes: pure
#       stock game, no Remastered enhancements; the launcher hides the HD-pack
#       panel and the runtime never loads a pack (NESRECOMP_GAME_NO_HDPACK).
#   LegendOfZeldaNESRecomp-HD             — recompiled from the patched "Zelda
#       Remastered" ROM: full remaster, HD-pack panel + loading enabled.
# Each variant compiles its OWN generated/<prefix>_{full,dispatch}.c, produced by
# regenning the recompiler against the matching ROM (the release script does both
# and renames the recompiler's "zelda_*" output to the per-variant prefix). They
# are separate executables, so the identical func_* symbols never collide.
function(add_zelda_variant TARGET GEN_PREFIX IS_STOCK)
    if(IS_STOCK)
        set(GENERATED_ROOT "${ZELDA_STOCK_GENERATED_ROOT}")
    else()
        set(GENERATED_ROOT "${ZELDA_HD_GENERATED_ROOT}")
    endif()
    add_executable(${TARGET}
        ${NESRECOMP_RUNNER_SOURCES}
        src/extras.c
        src/zelda_voxel.c
        src/zelda_voxel_plugin.c
        src/nes_snapshot.c
        src/verify_mode.c
        src/watchdog.c
        ${GENERATED_ROOT}/${GEN_PREFIX}_full.c
        ${GENERATED_ROOT}/${GEN_PREFIX}_dispatch.c
    )

    target_compile_definitions(${TARGET} PRIVATE RECOMP_STACK_TRACKING WATCHDOG_ENABLED)
    # Zelda is battery-backed (iNES header byte 6 bit 1) — declare it so the
    # launcher's SAVES panel shows the SRAM row unconditionally, from the very
    # first launch, without waiting for a ROM to have been picked once.
    target_compile_definitions(${TARGET} PRIVATE NESRECOMP_GAME_BATTERY)
    # Single-player (one controller) — hide the Player 2 card.
    target_compile_definitions(${TARGET} PRIVATE NESRECOMP_GAME_PLAYERS=1)
    target_compile_definitions(${TARGET} PRIVATE
        NESRECOMP_GAME_ID="legend-of-zelda-us"
        NESRECOMP_GAME_SDL_EVENT_HOOK=zelda_voxel_handle_event)
    if(IS_STOCK)
        # Stock CRC (extras.c) + HD packs disabled (launcher panel hidden, runtime
        # never loads — see hdpack.c / launcher.c).
        target_compile_definitions(${TARGET} PRIVATE
            NESRECOMP_GAME_NO_HDPACK
            NESRECOMP_GAME_ROM_CRC32="3fe272fb")
    else()
        target_compile_definitions(${TARGET} PRIVATE
            NESRECOMP_GAME_ROM_CRC32="fd9c577f")
    endif()

    target_include_directories(${TARGET} PRIVATE
        ${NESRECOMP_RUNNER_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/src
        ${SDL2_INCLUDE_DIRS}
    )

    target_link_libraries(${TARGET} PRIVATE ${SDL2_LIBRARIES})
    recomp_target_launcher_ui(${TARGET}
        CONSOLE nes
        BOXART ${CMAKE_SOURCE_DIR}/assets/boxart.tga)

    # Ship built-in packages through the same on-disk catalog used for
    # user-installed .nesmod archives. copy_directory is additive, so a rebuild
    # never overwrites mods/state.toml or removes user-installed packages.
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/mods/preloaded"
            "$<TARGET_FILE_DIR:${TARGET}>/mods"
        COMMENT "Staging Zelda's preloaded NES mod packages")

    if(WIN32)
        # ws2_32 for the TCP debug server (OpenGL is linked by recomp_ui.cmake).
        target_link_libraries(${TARGET} PRIVATE ws2_32)
    endif()

    if(WIN32)
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${NESRECOMP_ROOT}/runner/external/SDL2/lib/x64/SDL2.dll"
                "$<TARGET_FILE_DIR:${TARGET}>"
        )
    endif()

    if(MSVC)
        target_compile_options(${TARGET} PRIVATE /W3 /WX- /wd4102)
    endif()
endfunction()

add_zelda_variant(LegendOfZeldaNESRecomp     zelda_stock TRUE)   # default = stock
add_zelda_variant(LegendOfZeldaNESRecomp-HD  zelda_hd    FALSE)  # full Remastered
