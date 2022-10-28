cmake_minimum_required(VERSION 3.18.1)

project("ModMenu")

set(CMAKE_CXX_FLAGS "-O3 -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++17 -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions")
set(CMAKE_C_FLAGS "-O3 -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions")

set(DOBBY_DIR ${CMAKE_SOURCE_DIR}/LIBS/Dobby)

# Dobby Options
option(DOBBY_GENERATE_SHARED "Build shared library" ON)
option(DOBBY_DEBUG "Enable debug logging" OFF)
option(NearBranch "Enable near branch trampoline" ON)
option(FullFloatingPointRegisterPack "Save and pack all floating-point registers" OFF)
option(Plugin.SymbolResolver "Enable symbol resolver" ON)
option(Plugin.ImportTableReplace "Enable import table replace " ON)
option(Plugin.Android.BionicLinkerUtil "Enable android bionic linker util" ON)
option(BUILD_EXAMPLE "Build example" OFF)
option(BUILD_TEST "Build test" OFF)

include_directories(
        LIBS/imgui LIBS/imgui/backends LIBS/KittyMemory
        LIBS/BNM LIBS/Substrate LIBS/And64InlineHook
)

add_library(
        ModMenu

        SHARED

        native-lib.cpp

        # ImGui
        LIBS/imgui/imgui.cpp
        LIBS/imgui/imgui_draw.cpp
        LIBS/imgui/imgui_tables.cpp
        LIBS/imgui/imgui_widgets.cpp
        LIBS/imgui/backends/imgui_impl_android.cpp
        LIBS/imgui/backends/imgui_impl_opengl3.cpp

        # KittyMemory
        LIBS/KittyMemory/KittyArm64.cpp
        LIBS/KittyMemory/KittyMemory.cpp
        LIBS/KittyMemory/KittyScanner.cpp
        LIBS/KittyMemory/MemoryBackup.cpp
        LIBS/KittyMemory/KittyUtils.cpp
        LIBS/KittyMemory/MemoryPatch.cpp

        # ByNameModding
        LIBS/BNM/ByNameModding/BNM.cpp

        # Substrate
        LIBS/Substrate/hde64.c
        LIBS/Substrate/SubstrateDebug.cpp
        LIBS/Substrate/SubstrateHook.cpp
        LIBS/Substrate/SubstratePosixMemory.cpp
        LIBS/Substrate/SymbolFinder.cpp

        LIBS/And64InlineHook/And64InlineHook.cpp
)

target_include_directories(ModMenu PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

if(NOT TARGET dobby)
    macro(SET_OPTION option value)
        set(${option} ${value} CACHE INTERNAL "" FORCE)
    endmacro()
    SET_OPTION(DOBBY_DEBUG OFF)
    SET_OPTION(DOBBY_GENERATE_SHARED OFF)
    add_subdirectory(${DOBBY_DIR} dobby)
    get_property(DOBBY_INCLUDE_DIRECTORIES
            TARGET dobby
            PROPERTY INCLUDE_DIRECTORIES)
    include_directories(
            .
            ${DOBBY_INCLUDE_DIRECTORIES}
            $<TARGET_PROPERTY:dobby,INCLUDE_DIRECTORIES>
    )
endif()

target_link_libraries(
        ${PROJECT_NAME}
        log
        dobby
        android
        EGL
        GLESv1_CM
        GLESv2
        GLESv3
)