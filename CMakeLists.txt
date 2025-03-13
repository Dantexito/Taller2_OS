CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(cliutils)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(SOURCES
    cliutils.c
    cpu.c
)

# Crear el ejecutable
add_executable(cliutils ${SOURCES})

# Incluir directorios
include_directories(${CMAKE_SOURCE_DIR})
