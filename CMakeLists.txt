cmake_minimum_required(VERSION 3.1.0)

project(enki)

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
    src/search-replace/search_controller.cpp
    src/search-replace/search_widget.cpp
    )

SET(enki_FORMS src/search-replace/SearchWidget.ui)
QT5_WRAP_UI(enki_FORMS_HEADERS ${enki_FORMS})

include_directories(src)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Build example executable
add_executable(enki-ng ${enki_SRCS} ${enki_FORMS_HEADERS})

target_link_libraries(enki-ng Qt5::Core Qt5::Widgets qutepart)

install(TARGETS enki-ng DESTINATION bin)
