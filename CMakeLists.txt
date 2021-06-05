cmake_minimum_required (VERSION 3.8)


project (Group_chat CXX)
set(CMAKE_CXX_STANDARD 17)
include(cmake/StandardProjectSettings.cmake)
# Link this 'library' to set the c++ standard / compile-time options requested
add_library(default INTERFACE)
target_compile_features(default INTERFACE cxx_std_17)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(default)

enable_testing()
add_subdirectory(external/doctest)
include(external/doctest/doctest.cmake)




SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "D:/libraries/boost_1_76_0")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "D:/libraries/boost_1_76_0/libs")
set(BOOST_ROOT "D:/libraries/boost_1_76_0")
set(BOOST_USE_STATIC_LIBS ON) 
set(BOOST_USE_MULTITHREADED ON)  
set(BOOST_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost 1.76.0 REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS})
#ADD_DEFINITIONS( "-DHAS_BOOST" )




# Подключение подпроектов.
add_subdirectory ("Client")
add_subdirectory ("Server")

# Подключение библиотеки nana
#add_subdirectory(nana EXCLUDE_FROM_ALL)
#target_compile_definitions(nana PUBLIC NANA_ENABLE_PNG NANA_ENABLE_JPEG)
# target_link_libraries(nana PUBLIC CONAN_PKG::libpng CONAN_PKG::libjpeg)