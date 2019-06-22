cmake_minimum_required(VERSION 3.1.0)

project(enki)

# Qutepart is used as source tree, not as installed library to speedup development
# TODO use installed library
set(QUTEPART_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../qutepart-cpp
    CACHE PATH "Qutepart source tree path")

add_subdirectory(
    ${QUTEPART_SRC_PATH}
    ${CMAKE_CURRENT_BINARY_DIR}/qutepart
    EXCLUDE_FROM_ALL
)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)

# Find the QtWidgets library
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

# Populate a CMake variable with the sources
set(enki_SRCS
    src/editor.cpp
    src/enki.cpp
    src/file_explorer.cpp
    src/main_window.cpp
    src/workspace.cpp
    src/menu_bar.cpp
    )

include_directories(${QUTEPART_SRC_PATH}/include)

# Build example executable
add_executable(enki-ng ${enki_SRCS})


target_link_libraries(enki-ng Qt5::Core Qt5::Widgets qutepart)

install(TARGETS enki-ng DESTINATION bin)
