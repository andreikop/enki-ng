cmake_minimum_required(VERSION 3.1.0)

project(enki)

#
#  Integrate Qutepart
#
set(QUTEPART_SRC_DIR ""
    CACHE PATH
    "Qutepart source tree path. If not empty - Qutepart from this path is built as a subproject")

set(QUTEPART_MIN_REQUIRED_VERSION 0.1.0)  # need at least this version
set(QUTEPART_MAX_REQUIRED_VERSION 2.0.0)  # but not this version


if(NOT QUTEPART_SRC_DIR STREQUAL "")
    set(DISABLE_QUTEPART_TESTS ON BOOL)
    add_subdirectory(${QUTEPART_SRC_DIR} qutepart EXCLUDE_FROM_ALL)
    if (NOT DEFINED QUTEPART_VERSION)
        message(FATAL_ERROR
            "Are you sure " ${QUTEPART_SRC_DIR} " directory contains Qutepart? "
            "QUTEPART_VERSION variable is not defined.")
    endif (NOT DEFINED QUTEPART_VERSION)

    if (QUTEPART_VERSION VERSION_LESS QUTEPART_MIN_REQUIRED_VERSION)
        message(FATAL_ERROR
                "Too old Qutepart version " ${QUTEPART_VERSION}
                " found in " ${QUTEPART_SRC_DIR}
                " Required version >= " ${QUTEPART_MIN_REQUIRED_VERSION})
    endif(QUTEPART_VERSION VERSION_LESS QUTEPART_MIN_REQUIRED_VERSION)
    if (QUTEPART_VERSION VERSION_GREATER_EQUAL QUTEPART_MAX_REQUIRED_VERSION)
        message(FATAL_ERROR
                "Too new Qutepart version " ${QUTEPART_VERSION}
                " found in " ${QUTEPART_SRC_DIR}
                " Required version < " ${QUTEPART_MAX_REQUIRED_VERSION})
    endif (QUTEPART_VERSION VERSION_GREATER_EQUAL QUTEPART_MAX_REQUIRED_VERSION)
    message("Using Qutepart version " ${QUTEPART_VERSION} " from source directory " ${QUTEPART_SRC_DIR})
    include_directories(BEFORE ${QUTEPART_SRC_DIR}/include)
else(NOT QUTEPART_SRC_DIR STREQUAL "")
    message("Using Qutepart installed to the system")
    # TODO how to verify version ??
endif(NOT QUTEPART_SRC_DIR STREQUAL "")


#
# Build the package
#

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

# Find the QtWidgets library
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Test CONFIG REQUIRED)

# Populate a CMake variable with the sources
file(GLOB_RECURSE ENKI_SRCS src/*.cpp)
# enki.cpp  is not included to the library, only to the binary
list(REMOVE_ITEM ENKI_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/enki.cpp)

SET(enki_FORMS src/search-replace/SearchWidget.ui)
QT5_WRAP_UI(enki_FORMS_HEADERS ${enki_FORMS})

include_directories(src)
include_directories(src/lib)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Enki library is used by the binary and the tests
add_library(enki STATIC ${ENKI_SRCS} ${enki_FORMS_HEADERS} icons/icons.qrc)
target_link_libraries(enki Qt5::Core Qt5::Widgets qutepart)
target_compile_options(enki PRIVATE -Wall -Wextra -pedantic -Werror)

# The executable
add_executable(enki-ng src/enki.cpp)
target_link_libraries(enki-ng enki)

install(TARGETS enki-ng DESTINATION bin)

# Install desktop files on Linux
if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    install(
        FILES install/enki.desktop
        DESTINATION /usr/share/applications)
    install(
        FILES icons/logo/48x48/enki.png
        DESTINATION /usr/share/pixmaps)
    install(
        FILES icons/logo/32x32/enki.png
        DESTINATION /usr/share/icons/hicolor/32x32/apps)
    install(
        FILES icons/logo/48x48/enki.png
        DESTINATION /usr/share/icons/hicolor/48x48/apps)
    install(
        FILES icons/logo/enki.svg
        DESTINATION /usr/share/icons/hicolor/scalable/apps)
endif()


#
# Tests
#
enable_testing()

function(enki_test name)
    set(full_name test_${name})
    add_executable(${full_name} tests/${full_name}.cpp )
    target_link_libraries(${full_name} enki)
    target_link_libraries(${full_name} Qt5::Test Qt5::Core Qt5::Widgets qutepart)
    add_test(
        NAME ${name}
        COMMAND ${full_name}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/test-project)
endfunction()

enki_test(core)
enki_test(file_operations)
enki_test(editor)
enki_test(main_window)
enki_test(file_actions)
