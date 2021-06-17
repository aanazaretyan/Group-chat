cmake_minimum_required(VERSION 3.10.0)

add_executable(client_test test.cpp lib.cpp rsa.cpp client.hpp )
# ^ Взять test.cpp и lib.cpp, собрать вместе, назвать client_test (client_test.exe)
target_link_libraries(client_test PUBLIC doctest::doctest default Boost::system Boost::coroutine CONAN_PKG::botan Boost::math_tr1) 
# ^ Использовать в client_test библиотеки doctest::doctest и default
doctest_discover_tests(client_test)
# ^ Прочитать из client_test список тестов

target_link_directories(client_test PUBLIC "${Boost_LIBRARY_DIRS}")

add_executable(client client.cpp lib.cpp rsa.cpp client.hpp )
target_link_libraries(client PUBLIC doctest::doctest default Boost::system Boost::coroutine CONAN_PKG::fmt CONAN_PKG::nlohmann_json CONAN_PKG::botan Boost::math_tr1)
# fix error on Windows
target_link_directories(client PUBLIC "${Boost_LIBRARY_DIRS}")
