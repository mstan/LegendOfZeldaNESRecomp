cmake_minimum_required(VERSION 3.20)
project(LegendOfZeldaNESRecomp C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(NESRECOMP_ROOT ${CMAKE_SOURCE_DIR}/nesrecomp)

# nesrecomp/ and recomp-ui/ are git submodules (see .gitmodules); the pinned
# commit is the gitlink recorded in this repo. After cloning run setup.sh/.bat
# (git submodule update --init) to fetch them at the recorded commits.

# ---- Nestopia Oracle (optional) ----
option(ENABLE_NESTOPIA_ORACLE "Build with Nestopia emulator oracle for verify mode" ON)
if(ENABLE_NESTOPIA_ORACLE)
    set(NESTOPIA_DIR ${CMAKE_SOURCE_DIR}/nestopia-core)
endif()

include(${NESRECOMP_ROOT}/runner/runner.cmake)

list(APPEND CMAKE_PREFIX_PATH "${NESRECOMP_ROOT}/runner/external/SDL2/cmake")
find_package(SDL2 REQUIRED)

# ---- Pre-boot GUI launcher (recomp-ui / Dear ImGui) ----
# recomp-ui/ is a directory junction -> the shared recomp-ui checkout. Its module
# defines recomp_target_launcher_ui() (RECOMP_LAUNCHER on the target, staging the
# launcher assets); called once per game variant below.
include(${CMAKE_SOURCE_DIR}/recomp-ui/recomp_ui.cmake)

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
    add_executable(${TARGET}
        ${NESRECOMP_RUNNER_SOURCES}
        extras.c
        nes_snapshot.c
        verify_mode.c
        watchdog.c
        generated/${GEN_PREFIX}_full.c
        generated/${GEN_PREFIX}_dispatch.c
    )

    target_compile_definitions(${TARGET} PRIVATE RECOMP_STACK_TRACKING WATCHDOG_ENABLED)
    # Zelda is battery-backed (iNES header byte 6 bit 1) — declare it so the
    # launcher's SAVES panel shows the SRAM row unconditionally, from the very
    # first launch, without waiting for a ROM to have been picked once.
    target_compile_definitions(${TARGET} PRIVATE NESRECOMP_GAME_BATTERY)
    # Single-player (one controller) — hide the Player 2 card.
    target_compile_definitions(${TARGET} PRIVATE NESRECOMP_GAME_PLAYERS=1)
    if(IS_STOCK)
        # Stock CRC (extras.c) + HD packs disabled (launcher panel hidden, runtime
        # never loads — see hdpack.c / launcher.c).
        target_compile_definitions(${TARGET} PRIVATE NESRECOMP_GAME_NO_HDPACK)
    endif()

    target_include_directories(${TARGET} PRIVATE
        ${NESRECOMP_RUNNER_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        ${SDL2_INCLUDE_DIRS}
    )

    target_link_libraries(${TARGET} PRIVATE ${SDL2_LIBRARIES})
    recomp_target_launcher_ui(${TARGET} BOXART ${CMAKE_SOURCE_DIR}/boxart.tga)

    if(WIN32)
        # ws2_32 for the TCP debug server (OpenGL is linked by recomp_ui.cmake).
        target_link_libraries(${TARGET} PRIVATE ws2_32)
    endif()

    if(ENABLE_NESTOPIA_ORACLE)
        target_link_libraries(${TARGET} PRIVATE nestopia_core)
        target_compile_definitions(${TARGET} PRIVATE ENABLE_NESTOPIA_ORACLE)
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
