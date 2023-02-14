cmake_minimum_required(VERSION 3.1...3.25)
project(graphics-algebra-benchmark VERSION 0.1
                  DESCRIPTION "Simple software-based graphics pipeline for testing 3D graphics-relevant algebras"
                  LANGUAGES CXX)

find_package(directxmath CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

set(TARGET_EXECUTABLE_NAME "gabench")
add_executable(${TARGET_EXECUTABLE_NAME} src/main.cc src/frame_buffer.h src/geometry.h src/geometry.cc src/pipeline.h src/pipeline.cc)
target_compile_features(${TARGET_EXECUTABLE_NAME} PUBLIC cxx_std_17)
set_target_properties(${TARGET_EXECUTABLE_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(${TARGET_EXECUTABLE_NAME}
    PRIVATE
    Microsoft::DirectXMath
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

if(${MSVC})
  target_compile_options(${TARGET_EXECUTABLE_NAME} PRIVATE /W4)
endif()
