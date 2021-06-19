cmake_minimum_required(VERSION 3.10.0)

add_executable(reader_test test.cpp  reader.hpp)
# ^ Взять test.cpp и lib.cpp, собрать вместе, назвать client_test (client_test.exe)
target_link_libraries(reader_test PUBLIC doctest::doctest default Boost::system Boost::coroutine) 
# ^ Использовать в client_test библиотеки doctest::doctest и default
doctest_discover_tests(reader_test)
# ^ Прочитать из client_test список тестов

target_link_directories(reader_test PUBLIC "${Boost_LIBRARY_DIRS}")

add_executable(reader reader.cpp  reader.hpp)
target_link_libraries(reader PUBLIC doctest::doctest default Boost::system Boost::coroutine CONAN_PKG::fmt CONAN_PKG::nlohmann_json)
# fix error on Windows
target_link_directories(reader PUBLIC "${Boost_LIBRARY_DIRS}")
