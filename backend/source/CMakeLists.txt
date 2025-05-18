cmake_minimum_required(VERSION 3.16)

# Init project

set(project "main")

project(${project})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

# Project files

set(COMPONENTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/components/controllers/TestController.cpp
)

set(COMPONENTS_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/components/controllers/TestController.hpp
)

## Files for Core

set(CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Listener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Session.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Router.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/factories/ResponseFactory.cpp
)

set(CORE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Listener.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Session.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Router.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/factories/ResponseFactory.hpp
)

## Files for Default

set(DEFAULT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/default/components/views/View.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/default/components/Controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/default/web/Routes.cpp
)

set(DEFAULT_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/default/components/Model.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/default/components/views/View.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/default/components/Controller.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/default/web/Routes.hpp
)

## Files for Storage

set(STORAGE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/storage/Manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/storage/Session.cpp
)

set(STORAGE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/storage/Manager.hpp
)

## All files

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Environment.cpp
    ${COMPONENTS_SOURCES}
    ${CORE_SOURCES}
    ${DEFAULT_SOURCES}
    ${STORAGE_SOURCES}
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Environment.hpp
    ${COMPONENTS_HEADERS}
    ${CORE_HEADERS}
    ${DEFAULT_HEADERS}
    ${STORAGE_HEADERS}
)

# Include libraries

find_package(Boost 1.74 REQUIRED COMPONENTS system)
find_package(nlohmann_json 3.10.5 REQUIRED)
find_package(PostgreSQL REQUIRED)

## Bad practice
find_package(libpqxx REQUIRED
    PATHS "/usr/local/lib/cmake/libpqxx"
    NO_DEFAULT_PATH
)

include_directories(${Boost_INCLUDE_DIRS})

# Add executable

add_executable(${project} ${SOURCES} ${HEADERS})

# Link libraries

target_link_libraries(${project} PRIVATE Boost::system)
target_link_libraries(${project} PRIVATE nlohmann_json::nlohmann_json)

## Bad practice
target_link_libraries(${project} PRIVATE /usr/local/lib/libpqxx.a)

target_link_libraries(${project} PRIVATE PostgreSQL::PostgreSQL)

# Add compile options 

target_compile_options(${project} PRIVATE -Wall -Wextra -Werror -Wpedantic)

# Copy .env file to build directory

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/.env ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# Add custom flag for Debug

if(ENABLE_DEBUG)
    target_compile_options(${project} PRIVATE -g)
endif()

# Add custom target for Clang-Format

add_custom_target(
    clang-format
    COMMAND clang-format
    --dry-run
    -Werror
    ${SOURCES}
    ${HEADERS}
)

add_custom_target(
    clang-format-fix
    COMMAND clang-format
    --style=file
    -i
    ${SOURCES}
    ${HEADERS}
)
