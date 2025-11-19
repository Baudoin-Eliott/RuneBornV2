cmake_minimum_required(VERSION 3.15)

project(RuneBorn VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ========================================
# Configuration SDL2 (générique)
# ========================================
set(SDL2_ROOT_DIR "C:/Dev/SDL2" CACHE PATH "SDL2 root directory")

set(SDL2_INCLUDE_DIR "${SDL2_ROOT_DIR}/include")
set(SDL2_LIBRARY "${SDL2_ROOT_DIR}/lib/libSDL2.dll.a")
set(SDL2_IMAGE_LIBRARY "${SDL2_ROOT_DIR}/lib/libSDL2_image.dll.a")
set(SDL2MAIN_LIBRARY "${SDL2_ROOT_DIR}/lib/libSDL2main.a")

# DLLs à copier
set(SDL2_RUNTIME_DLLS
    "${SDL2_ROOT_DIR}/bin/SDL2.dll"
    "${SDL2_ROOT_DIR}/bin/SDL2_image.dll"
)

# ========================================
# Fichiers source
# ========================================
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

# ========================================
# Création de l'exécutable
# ========================================
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
    ${SDL2_IMAGE_LIBRARY}
)

# ========================================
# Copie des DLLs (générique et robuste)
# ========================================
foreach(DLL_FILE ${SDL2_RUNTIME_DLLS})
    if(EXISTS "${DLL_FILE}")
        add_custom_command(TARGET RuneBorn POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL_FILE}"
            $<TARGET_FILE_DIR:RuneBorn>
            COMMENT "Copying ${DLL_FILE}"
        )
    else()
        message(WARNING "DLL not found: ${DLL_FILE}")
    endif()
endforeach()

message(STATUS "RuneBorn SDL2 configured!")
message(STATUS "SDL2 root: ${SDL2_ROOT_DIR}")