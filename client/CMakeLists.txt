cmake_minimum_required(VERSION 3.10.0)

add_executable(client_test test.cpp lib.cpp rsa.cpp)
# ^ Взять test.cpp и lib.cpp, собрать вместе, назвать client_test (client_test.exe)
target_link_libraries(client_test PUBLIC doctest::doctest default)
# ^ Использовать в client_test библиотеки doctest::doctest и default
doctest_discover_tests(client_test "client.cpp" "lib.cpp" "lib.hpp" "rsa.cpp" "test.cpp")
# ^ Прочитать из client_test список тестов

add_executable(client client.cpp lib.cpp rsa.cpp)
target_link_libraries(client PUBLIC doctest::doctest default Boost::system Boost::coroutine CONAN_PKG::fmt CONAN_PKG::nlohmann_json)
# fix error on Windows
target_link_directories(client PUBLIC "${Boost_LIBRARY_DIRS}")