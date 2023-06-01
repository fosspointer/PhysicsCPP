
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/Physics/*/*.hpp)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)

add_library(physicslib ${SOURCES} ${HEADERS})
target_include_directories(physicslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(physicslib sfml-graphics)
target_precompile_headers(physicslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/Physics.hpp)

target_compile_definitions(physicslib PUBLIC
    $<$<CONFIG:Debug>:-DPHYSICS_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:-DPHYSICS_DEBUG>
    $<$<CONFIG:Release>:-DPHYSICS_RELEASE>
    $<$<CONFIG:MinSizeRel>:-DPHYSICS_RELEASE>
    PHYSICS_WORKING_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\"
    PHYSICS_ASSETS_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/assets/\"
)