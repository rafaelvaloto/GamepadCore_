cmake_minimum_required(VERSION 3.20)
if(WIN32)
    add_compile_definitions(UNICODE _UNICODE)
endif()

# Coleta os arquivos
file(GLOB_RECURSE GAMEPADCORE_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/Public/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Public/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.hpp"
)

file(GLOB_RECURSE GAMEPADCORE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cxx"
)

# --- MANTENHA ESTE (Versão Estática) ---
add_library(GamepadCore STATIC
        ${GAMEPADCORE_HEADERS}
        ${GAMEPADCORE_SOURCES}
        Public/GImplementations/Utils/GamepadAudio.h
)

target_include_directories(GamepadCore
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Public"
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

target_compile_features(GamepadCore PUBLIC cxx_std_20)

set_target_properties(GamepadCore PROPERTIES
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
)