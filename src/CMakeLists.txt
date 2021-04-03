set(SRC_FILES
        main.cpp
        views/animations/GridAnimation.cpp
        models/actors/Door.cpp
        models/actors/Wall.cpp
        models/actors/Fruit.cpp
        views/animations/FruitAnimation.cpp
        views/animations/PelletAnimations.cpp
        models/actors/Pellet.cpp
        models/actors/DoorLocker.cpp
        models/scenes/IntroScene.cpp
        views/IntroSceneView.cpp
        models/scenes/MainMenuScene.cpp
        models/scenes/LoadingScene.cpp
        models/scenes/LevelStartScene.cpp
        models/scenes/StartUpScene.cpp
        views/CommonView.cpp
        views/MainMenuSceneView.cpp
        views/IntroSceneView.cpp
        views/StartUpSceneView.cpp
        views/LevelStartSceneView.cpp
        views/LoadingSceneView.cpp
        models/actors/Ghost.cpp
        models/actors/Key.cpp
        models/actors/PacMan.cpp
        views/animations/PacManAnimations.cpp
        views/animations/GhostAnimations.cpp
        models/scenes/GameplayScene.cpp
        models/world/Grid.cpp
        utils/ObjectCreator.cpp
        models/game/Game.cpp
        models/scoreboard/Score.cpp
        models/scoreboard/Scoreboard.cpp
        )

# Set executables output folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Create executable project source files
add_executable(SuperPacMan ${SRC_FILES})

# Find third party dependency
set(IME_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME/lib/cmake/IME")
set(IME_BIN_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME/bin")
find_package(IME 2.0.0 REQUIRED)

# Link third party dependency to executable
target_link_libraries (SuperPacMan PRIVATE ime)

# Add <project>/ as include directory
include_directories(${PROJECT_SOURCE_DIR}/)

# Copy runtime dependencies to the executable output folder
add_custom_command(TARGET SuperPacMan PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${IME_BIN_DIR}/${CMAKE_BUILD_TYPE}/" $<TARGET_FILE_DIR:SuperPacMan>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" $<TARGET_FILE_DIR:SuperPacMan>/assets
)