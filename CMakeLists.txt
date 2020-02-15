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
    add_subdirectory(${QUTEPART_SRC_DIR} qutepart)
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

set(CMAKE_CXX_STANDARD 17)

# Find the QtWidgets library
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

# Populate a CMake variable with the sources
set(enki_SRCS
    src/core.cpp
    src/editor.cpp
    src/enki.cpp
    src/file_explorer.cpp
    src/main_window.cpp
    src/workspace.cpp
    src/menu_bar.cpp
    src/project.cpp
    src/settings.cpp
    src/locator/locator.cpp
    src/locator/locator_dialog.cpp
    src/locator/locator_model.cpp
    src/locator/open_file_command.cpp
    src/search_replace/search_controller.cpp
    src/search_replace/search_widget.cpp
    src/search_replace/search_pattern.cpp
    src/file_browser/file_browser.cpp
    src/file_browser/file_tree.cpp
    )

SET(enki_FORMS src/search-replace/SearchWidget.ui)
QT5_WRAP_UI(enki_FORMS_HEADERS ${enki_FORMS})

include_directories(src)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(enki-ng ${enki_SRCS} ${enki_FORMS_HEADERS})

target_link_libraries(enki-ng Qt5::Core Qt5::Widgets qutepart)
target_compile_options(enki-ng PRIVATE -Wall -Wextra -pedantic -Werror)

install(TARGETS enki-ng DESTINATION bin)
