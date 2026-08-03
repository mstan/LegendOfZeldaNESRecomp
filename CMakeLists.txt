cmake_minimum_required(VERSION 3.20)
project(LegendOfZeldaNESRecomp C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

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
include(${NESRECOMP_ROOT}/runner/runner.cmake)

list(APPEND CMAKE_PREFIX_PATH "${NESRECOMP_ROOT}/runner/external/SDL2/cmake")
find_package(SDL2 REQUIRED)

# ---- Pre-boot GUI launcher (recomp-ui / Dear ImGui) ----
# recomp-ui/ is a directory junction -> the shared recomp-ui checkout. Its module
# defines recomp_target_launcher_ui() (RECOMP_LAUNCHER on the target, staging the
# launcher assets); called once per game variant below.
include(${RECOMP_UI_ROOT}/recomp_ui.cmake)

# ---- Game variants: stock + HD (two builds from one project) ----
# See _zelda_release.bat / the "Cutting a release" section in CLAUDE.md.
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
        extras.c
        zelda_voxel.c
        mods/zelda_voxel_plugin.c
        nes_snapshot.c
        verify_mode.c
        watchdog.c
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
        ${CMAKE_SOURCE_DIR}
        ${SDL2_INCLUDE_DIRS}
    )

    target_link_libraries(${TARGET} PRIVATE ${SDL2_LIBRARIES})
    recomp_target_launcher_ui(${TARGET}
        CONSOLE nes
        BOXART ${CMAKE_SOURCE_DIR}/boxart.tga)

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
