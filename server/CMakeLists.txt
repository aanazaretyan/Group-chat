cmake_minimum_required(VERSION 3.10.0)

add_executable(server_test test.cpp  server.hpp)
# Взять test.cpp и lib.cpp, собрать вместе, назвать server_test (server_test.exe)
target_link_libraries(server_test PUBLIC doctest::doctest default Boost::system Boost::coroutine)
# Использовать в server_test библиотеки doctest::doctest и default
doctest_discover_tests(server_test)
# Прочитать из server_test список 
target_link_directories(server_test PUBLIC "${Boost_LIBRARY_DIRS}")

add_executable(server server.cpp  server.hpp)
target_link_libraries(server PUBLIC doctest::doctest default Boost::system Boost::coroutine CONAN_PKG::fmt CONAN_PKG::nlohmann_json)

# fix error on Windows
target_link_directories(server PUBLIC "${Boost_LIBRARY_DIRS}")