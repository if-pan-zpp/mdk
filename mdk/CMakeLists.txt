set(LIBRARY_NAME mdk)

add_library(${LIBRARY_NAME})

file(GLOB_RECURSE PUBLIC_SOURCES include/*)
target_sources(${LIBRARY_NAME} PUBLIC
    ${PUBLIC_SOURCES})

file(GLOB_RECURSE PRIVATE_SOURCES src/*)
target_sources(${LIBRARY_NAME} PRIVATE
    ${PRIVATE_SOURCES})

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

