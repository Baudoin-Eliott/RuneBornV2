cmake_minimum_required(VERSION 3.15)

project(RuneBorn VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Chemins SDL2
set(SDL2_INCLUDE_DIR "C:/Dev/SDL2/include")
set(SDL2_LIBRARY "C:/Dev/SDL2/lib/libSDL2.dll.a")
set(SDL2MAIN_LIBRARY "C:/Dev/SDL2/lib/libSDL2main.a")

# Fichiers source
set(SOURCES
    Main.cpp
    Game.cpp
    DataManager.cpp
    ECS/Utils/Vector2D.cpp
)

set(HEADERS
    Game.h
    DataManager.h
    ECS/ECS.h
    ECS/Utils/Vector2D.h
    ECS/Components/TransformComponent.h
    ECS/Components/SpriteComponent.h
    ECS/Components/AnimationComponent.h
    ECS/Systems/RenderSystem.h
    ECS/Systems/AnimationSystem.h
)

add_executable(RuneBorn ${SOURCES} ${HEADERS})

target_include_directories(RuneBorn PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIR}
)

target_link_libraries(RuneBorn PRIVATE
    mingw32
    ${SDL2MAIN_LIBRARY}
    ${SDL2_LIBRARY}
)

# Copier SDL2.dll
add_custom_command(TARGET RuneBorn POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "C:/Dev/SDL2/bin/SDL2.dll"
    $<TARGET_FILE_DIR:RuneBorn>
)

message(STATUS "RuneBorn SDL2 configured!")