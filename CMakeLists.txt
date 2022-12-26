CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(MyCoolApplication)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(MIN_QT_VERSION "5.6.0")

set(LIBARCHIVE_PKG_CONFIG "${LIBARCHIVE_PKG_CONFIG}" CACHE STRING "Make CMake find LibArchive under MacOS" FORCE)
add_subdirectory(QArchive)

find_package(Qt5 COMPONENTS Core)

# Include Directories.
include_directories(.)
include_directories(${CMAKE_BINARY_DIR})

SET(source)
list(APPEND source main.cc)

add_executable(MyCoolApplication ${source})

target_link_libraries(MyCoolApplication PUBLIC Qt5::Core QArchive)
