# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

project ("Group chat")

# Включите подпроекты.
add_subdirectory ("Client")
add_subdirectory ("Server")

# Подключение библиотеки nana
add_subdirectory(nana EXCLUDE_FROM_ALL)
target_compile_definitions(nana PUBLIC NANA_ENABLE_PNG NANA_ENABLE_JPEG)
# target_link_libraries(nana PUBLIC CONAN_PKG::libpng CONAN_PKG::libjpeg)