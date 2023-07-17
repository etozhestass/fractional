cmake_minimum_required(VERSION 3.22)
project(fractional_never_ever_live_git)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

set_source_files_properties(
        PROPERTIES COMPILE_FLAGS include/test/test.cpp)

find_package(Catch2 REQUIRED)
add_executable(test include/test/test.cpp src/expression.cpp src/fractional.cpp)
target_link_libraries(test Catch2::Catch2 Catch2::Catch2WithMain)

if (TESTS_BUILD_TYPE MATCHES ASAN)
    set(COMPILE_OPTS -Wall -Wextra -Werror -pedantic -pedantic-errors -O1 -fsanitize=address -fno-omit-frame-pointer
            -fno-inline -fno-sanitize-recover=all)
    set(LINK_OPTS -fsanitize=address)
endif()
if (TESTS_BUILD_TYPE MATCHES MSAN)
    set(COMPILE_OPTS -Wall -Wextra -Werror -pedantic -pedantic-errors -O1 -fsanitize=leak
            -fno-omit-frame-pointer
            -fno-sanitize-recover=all)
    set(LINK_OPTS -fsanitize=leak)
endif()
if (TESTS_BUILD_TYPE MATCHES USAN)
    set(COMPILE_OPTS -Wall -Wextra -Werror -pedantic -pedantic-errors -O1
            -fsanitize=undefined,float-cast-overflow,float-divide-by-zero
            -fno-omit-frame-pointer -fno-sanitize-recover=all
            -fsanitize-recover=alignment)
    set(LINK_OPTS
            -fsanitize=undefined,float-cast-overflow,float-divide-by-zero)
endif()

if (${USE_CLANG_TIDY})
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()