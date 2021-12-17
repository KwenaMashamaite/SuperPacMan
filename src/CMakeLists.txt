set(SRC_FILES
        main.cpp
        Game.cpp
        Grid.cpp
        AI/ghost/GhostState.cpp
        AI/ghost/ScatterState.cpp
        AI/ghost/ChaseState.cpp
        AI/ghost/FrightenedState.cpp
        AI/ghost/EatenState.cpp
        utils/ObjectCreator.cpp
        utils/Utils.cpp
        Animations/FruitAnimation.cpp
        Animations/GhostAnimations.cpp
        Animations/PacManAnimations.cpp
        Animations/PelletAnimations.cpp
        Animations/GridAnimation.cpp
        Common/ObjectReferenceKeeper.cpp
        GameObjects/Door.cpp
        GameObjects/Fruit.cpp
        GameObjects/Ghost.cpp
        GameObjects/Key.cpp
        GameObjects/PacMan.cpp
        GameObjects/Pellet.cpp
        GameObjects/Sensor.cpp
        GameObjects/Wall.cpp
        PathFinders/PacManGridMover.cpp
        PathFinders/GhostGridMover.cpp
        Scenes/CollisionResponseRegisterer.cpp
        Scenes/StartUpScene.cpp
        Scenes/MainMenuScene.cpp
        Scenes/LoadingScene.cpp
        Scenes/LevelStartScene.cpp
        Scenes/GameplayScene.cpp
        Scenes/PauseMenuScene.cpp
        Scenes/GameOverScene.cpp
        Scoreboard/Score.cpp
        Scoreboard/Scoreboard.cpp
        Views/CommonView.cpp
        Views/LevelStartSceneView.cpp
        Views/LoadingSceneView.cpp
        Views/MainMenuSceneView.cpp
        views/StartUpSceneView.cpp
        Views/PauseMenuSceneView.cpp
        Views/GameOverSceneView.cpp)

# Resource script files
set(RES_FILES
    ${PROJECT_SOURCE_DIR}/icon.rc
    ${PROJECT_SOURCE_DIR}/version.rc)

# Set executables output folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Create executable project source files
add_executable(SuperPacMan ${SRC_FILES} ${RES_FILES})

# Find IME
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/extlibs")
find_package(ime 2.5.0 REQUIRED)

# Link IME
target_link_libraries (SuperPacMan PRIVATE ime)

# Add <project>/src folder as include directory
include_directories(${PROJECT_SOURCE_DIR}/src)

# Copy runtime dependencies to the executable output folder
set(IME_BIN_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME-${ime_VERSION}/bin")

# Copy runtime dependencies to the executable output folder
add_custom_command(TARGET SuperPacMan PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${IME_BIN_DIR}/${CMAKE_BUILD_TYPE}/" $<TARGET_FILE_DIR:SuperPacMan>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${IME_BIN_DIR}/Runtime/" $<TARGET_FILE_DIR:SuperPacMan>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res" $<TARGET_FILE_DIR:SuperPacMan>/res
)