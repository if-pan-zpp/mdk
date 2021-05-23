set(TARGET mdk)

add_library(${TARGET})

file(GLOB_RECURSE PUBLIC_SOURCES include/*)
file(GLOB_RECURSE PRIVATE_SOURCES src/*)

target_sources(${TARGET}
    PUBLIC ${PUBLIC_SOURCES}
    PRIVATE ${PRIVATE_SOURCES})

target_include_directories(${TARGET}
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/mdk>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include/mdk>
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_compile_options(${TARGET}
    PRIVATE
    -Wall -Wextra -fopenmp
    $<$<CONFIG:Debug>:-pg -Og -march=native>
    $<$<CONFIG:Release>:-Ofast -march=native>)

target_compile_features(${TARGET}
    PUBLIC cxx_std_17)

target_compile_definitions(${TARGET}
    PRIVATE $<$<CONFIG:Debug>:EIGEN_NO_DEBUG>)

find_package(Eigen3 REQUIRED)
find_package(Catch2 REQUIRED)

target_link_libraries(${TARGET}
    PRIVATE
    Eigen3::Eigen
    Catch2::Catch2)
