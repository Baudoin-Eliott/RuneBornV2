cmake_minimum_required(VERSION 3.15)

project(RuneBorn VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SDL2_ROOT_DIR "C:/Dev/SDL2" CACHE PATH "SDL2 root directory")

set(SDL2_INCLUDE_DIR "${SDL2_ROOT_DIR}/include")
set(SDL2_LIBRARY "${SDL2_ROOT_DIR}/lib/libSDL2.dll.a")
set(SDL2_IMAGE_LIBRARY "${SDL2_ROOT_DIR}/lib/libSDL2_image.dll.a")
set(SDL2_MIXER_LIBRARY "${SDL2_ROOT_DIR}/lib/libSDL2_mixer.dll.a")
set(SDL2MAIN_LIBRARY "${SDL2_ROOT_DIR}/lib/libSDL2main.a")
set(SDL2_TTF_LIBRARY "${SDL2_ROOT_DIR}/lib/libSDL2_ttf.dll.a")

# DLLs à copier
set(SDL2_RUNTIME_DLLS
    "${SDL2_ROOT_DIR}/bin/SDL2.dll"
    "${SDL2_ROOT_DIR}/bin/SDL2_image.dll"
    "${SDL2_ROOT_DIR}/bin/SDL2_mixer.dll"
    "${SDL2_ROOT_DIR}/bin/SDL2_ttf.dll"
)


set(SOURCES
    Main.cpp
    Game.cpp
    tinyxml2.cpp
    
    # === ECS Systems ===
    ECS/Systems/CollisionSystem.cpp
    ECS/Systems/MovementSystem.cpp
    ECS/Systems/TriggerSystem.cpp
    ECS/Systems/CameraSystem.cpp
    ECS/Systems/TileMapRenderSystem.cpp
    ECS/Systems/RenderSystem.cpp
    ECS/Systems/DebugRenderSystem.cpp
    
    # === UI ===
    src/UI/MainMenu.cpp
    src/UI/PauseMenu.cpp
    src/UI/OptionsMenu.cpp
    src/UI/CreditsMenu.cpp
    src/UI/RunesMenu.cpp
    
    # === Managers ===
    src/Managers/AudioManager.cpp
    src/Managers/UIThemeManager.cpp
    
    # === Game Systems ===
    src/Systems/DirectionalAnimationSystem.cpp
    src/Systems/PlayerInputSystem.cpp
    
    # === Spells ===
    src/Spells/SpellFactory.cpp
    src/Spells/SpellDatabase.cpp
    src/Spells/SpellRecognizer.cpp
    
    # === Spell Effects ===
    src/Spells/Effects/FireBallEffect.cpp
    # src/Spells/Effects/HealEffect.cpp        # TODO
    # src/Spells/Effects/IceSpearEffect.cpp    # TODO
)

# =====================================================
# HEADERS (.h files) - Optionnel mais utile pour l'IDE
# =====================================================
set(HEADERS
    Game.h
    
    # === ECS Core ===
    ECS/ECS.h
    ECS/Utils/Vector2D.h
    
    # === ECS Components ===
    ECS/Components/TransformComponent.h
    ECS/Components/SpriteComponent.h
    ECS/Components/AnimationComponent.h
    
    # === ECS Systems ===
    ECS/Systems/RenderSystem.h
    ECS/Systems/AnimationSystem.h
    ECS/Systems/CollisionSystem.h
    ECS/Systems/MovementSystem.h
    
    # === UI ===
    src/UI/MainMenu.h
    src/UI/PauseMenu.h
    src/UI/OptionsMenu.h
    src/UI/CreditsMenu.h
    src/UI/RunesMenu.h
    
    # === Managers ===
    src/Managers/AudioManager.h
    src/Managers/UIThemeManager.h
    
    # === Game Systems ===
    src/Systems/DirectionalAnimationSystem.h
    src/Systems/PlayerInputSystem.h
    
    # === Spells ===
    src/Spells/SpellEffect.h
    src/Spells/SpellPattern.h
    src/Spells/SpellFactory.h
    src/Spells/SpellDatabase.h
    src/Spells/SpellRecognizer.h
    
    # === Spell Effects ===
    src/Spells/Effects/FireBallEffect.h
    # src/Spells/Effects/HealEffect.h        # TODO
    # src/Spells/Effects/IceSpearEffect.h    # TODO
)

add_executable(RuneBorn ${SOURCES} ${HEADERS})


target_include_directories(RuneBorn PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ECS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIR}
    C:/Dev/nlohmann/include
)

target_link_libraries(RuneBorn PRIVATE
    mingw32
    ${SDL2MAIN_LIBRARY}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_MIXER_LIBRARY}
    ${SDL2_TTF_LIBRARY}
)

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
include_directories(${CMAKE_SOURCE_DIR}/ECS)

message(STATUS "RuneBorn SDL2 configured!")
message(STATUS "SDL2 root: ${SDL2_ROOT_DIR}")