cmake_minimum_required(VERSION 3.20)

set(project "main")

project(${project})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

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

set(DEFAULT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/default/components/views/View.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/default/web/Routes.cpp
)

set(DEFAULT_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/default/components/views/View.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/default/web/Routes.hpp
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CORE_SOURCES}
    ${DEFAULT_SOURCES}
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.hpp
    ${CORE_HEADERS}
    ${DEFAULT_HEADERS}
)

find_package(Boost 1.74 REQUIRED COMPONENTS system)
find_package(nlohmann_json 3.11.2 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(${project} ${SOURCES} ${HEADERS})

target_link_libraries(${project} PRIVATE Boost::system)
target_link_libraries(${project} PRIVATE nlohmann_json::nlohmann_json)
target_compile_options(${project} PRIVATE -Wall -Wextra -Werror -Wpedantic)

if(ENABLE_DEBUG)
    target_compile_options(${project} PRIVATE -g)
endif()

add_custom_target(
    clang-format
    COMMAND clang-format
    --dry-run
    -Werror
    ${SOURCES}
    ${HEADERS}
)
