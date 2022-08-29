cmake_minimum_required(VERSION 3.12)

project(pathtracer)

aux_source_directory(src SRC)
add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE inc)
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

find_package(SDL2 REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(imgui REQUIRED)

find_package(glm REQUIRED)

set(CMAKE_BUILD_TYPE Debug)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    Eigen3::Eigen
    glm
)
