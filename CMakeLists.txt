cmake_minimum_required(VERSION 3.20)

# O nome do projeto raiz é a sua Biblioteca
project(GamepadCore LANGUAGES CXX)

# --- Configurações Globais de C++ ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    add_compile_definitions(UNICODE _UNICODE)
endif()

add_definitions(-DBUILD_GAMEPAD_CORE_TESTS)

# --- Adiciona a Biblioteca Principal (Core) ---
# Isso vai processar o CMakeLists.txt dentro da pasta Source
add_subdirectory(Source)

# --- Configuração dos Testes ---
# Cria uma "checkbox" no CMake (padrão ON) para você poder desativar os testes se quiser apenas a lib
option(BUILD_TESTS "Build integration tests" ON)

if(BUILD_TESTS)
    # Adiciona a subpasta onde criamos o teste de integração
    add_subdirectory(Tests/Integration)
endif()