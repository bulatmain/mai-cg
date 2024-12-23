cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER "/usr/bin/gcc")

project(lab LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_WARNING "Enable warnings" OFF)

if (ENABLE_WARNING)
    if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
        add_compile_options(
            -Wall
            -Wextra
            -Wpedantic

            -Wcast-align
            -Wcast-qual
            -Wconversion
            -Wctor-dtor-privacy
            -Wenum-compare
            -Wfloat-equal
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Woverloaded-virtual
            -Wredundant-decls
            -Wsign-conversion
            -Wsign-promo
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        add_compile_options(/W4 /WX)
    endif()

    if(NOT CMAKE_CXX_EXTENSIONS)
        set(CMAKE_CXX_EXTENSIONS OFF)
    endif()
endif()


find_package(glfw3 REQUIRED)

find_package(glm REQUIRED)

find_package(OpenGL REQUIRED)

# Add GLAD as a separate library
add_library(glad STATIC external/glad/glad.c)
target_include_directories(glad PUBLIC external)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)
target_compile_features(glad PUBLIC c_std_11)

add_library(source INTERFACE)
target_include_directories(source INTERFACE src/cpp)



# Define the executable
add_executable(exec src/cpp/main.cpp)

# Link libraries to the executable
target_link_libraries(exec PRIVATE
    source
    glad
    glfw
    glm::glm
    OpenGL::GL
)

target_compile_features(exec PRIVATE cxx_std_20)

