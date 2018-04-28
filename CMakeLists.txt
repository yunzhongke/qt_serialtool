cmake_minimum_required(VERSION 3.0)

project(SerialTool)

set(CMAKE_PREFIX_PATH "/home/yunzhongke/work/Qt5.6.2/5.6/gcc_64/lib/cmake/Qt5Widgets")
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5SerialPort)

if(Qt5Widgets_FOUND)
    message("--${Qt5Widgets_VERSION_STRING}")
else()
    message(STATUS "NO FIND!")
endif()

# Tell CMake to create the helloworld executable
add_executable(${PROJECT_NAME} main.cpp widgets.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::SerialPort)
