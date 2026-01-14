cmake_minimum_required(VERSION 3.20)

project(GamepadAppDLL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona o subdiretório do ViGEmClient para compilar junto
add_subdirectory(../Examples/Platform_Windows/ViGEmAdapter/ViGEmClient-master ViGEmClient)

# Adiciona o arquivo de fonte do Mod e os exemplos
add_library(GamepadModCore SHARED
        DualSenseMods.cpp
        ../Examples/Platform_Windows/test_windows_device_info.cpp
        ../Examples/Platform_Windows/ViGEmAdapter/ViGEmAdapter.cpp
)

# Adiciona os diretórios de include do ViGEmClient para o GamepadModCore
target_include_directories(GamepadModCore PRIVATE
        ../Examples/Platform_Windows/ViGEmAdapter/ViGEmClient-master/include
)

# Define a macro necessária para os testes e ativa o ViGEm
target_compile_definitions(GamepadModCore PRIVATE 
    BUILD_GAMEPAD_CORE_TESTS
    USE_VIGEM
)

# --- A PARTE IMPORTANTE ---
# Ao linkar com 'GamepadCore', o CMake herda automaticamente:
# 1. O código binário (estático)
# 2. Os diretórios de include (Public/Private) definidos lá no outro CMake
target_link_libraries(GamepadModCore PRIVATE
        GamepadCore  # <--- Isso injeta o código do Core aqui dentro
        Setupapi
        Hid
        dxgi
        ole32
        oleaut32
        uuid
        winmm
        ViGEmClient::ViGEmClient
)

# Configura o nome final do arquivo para ficar bonitinho na pasta
set_target_properties(GamepadModCore PROPERTIES
        OUTPUT_NAME "DualSenseModCore"
        PREFIX ""
)