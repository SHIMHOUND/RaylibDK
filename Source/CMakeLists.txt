# Create executable
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SOURCE_DIR}/Main.cpp
        Game.cpp
        Game.h
        Visuals/AnimatedSprite.cpp
        Visuals/AnimatedSprite.h
        Input/Input.cpp
        Input/Input.h
        Input/InputCodes.h)

include(${CMAKaE_CURRENT_SOURCE_DIR}Sources.cmake)

# Add libraries include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${lua_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${sol2_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${box2d_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        raylib
        box2d
        Lua_static
        sol2
)
