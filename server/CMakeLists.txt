cmake_minimum_required(VERSION 3.10.0)

add_executable(server_test test.cpp lib.cpp lib.hpp)
# Взять test.cpp и lib.cpp, собрать вместе, назвать server_test (server_test.exe)
target_link_libraries(server_test PUBLIC doctest::doctest default Boost::system Boost::coroutine CONAN_PKG::botan)
# Использовать в server_test библиотеки doctest::doctest и default
doctest_discover_tests(server_test)
# Прочитать из server_test список тестов
add_executable(server server.cpp lib.cpp lib.hpp)
target_link_libraries(server PUBLIC doctest::doctest default Boost::system Boost::coroutine CONAN_PKG::fmt CONAN_PKG::nlohmann_json CONAN_PKG::botan)


# fix error on Windows
target_link_directories(server PUBLIC "${Boost_LIBRARY_DIRS}")