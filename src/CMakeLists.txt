set(SRC_FILES
    main.cpp
    common/PositionTracker.cpp
    models/actors/states/ghost/GhostState.cpp
    models/actors/states/ghost/ScatterState.cpp
    models/actors/states/ghost/ChaseState.cpp
    models/actors/states/ghost/FrightenedState.cpp
    models/actors/states/ghost/HealState.cpp
    models/actors/states/ghost/RoamState.cpp
    models/actors/states/ghost/GIdleState.cpp
    models/actors/states/ActorStateFSM.cpp
    models/actors/states/ActorStateFSM.h
    models/actors/controllers/GhostGridMover.cpp
    models/actors/controllers/ForwardDirectionalBFS.cpp
    models/actors/Door.cpp
    models/actors/DoorLocker.cpp
    models/actors/Fruit.cpp
    models/actors/Ghost.cpp
    models/actors/Key.cpp
    models/actors/PacMan.cpp
    models/actors/Pellet.cpp
    models/actors/Sensor.cpp
    models/actors/Wall.cpp
    models/game/Game.cpp
    models/scenes/GameplayScene.cpp
    models/scenes/LevelStartScene.cpp
    models/scenes/LoadingScene.cpp
    models/scenes/MainMenuScene.cpp
    models/scenes/StartUpScene.cpp
    models/scoreboard/Score.cpp
    models/scoreboard/Scoreboard.cpp
    models/world/Grid.cpp
    utils/ObjectCreator.cpp
    utils/Utils.cpp
    animations/FruitAnimation.cpp
    animations/GhostAnimations.cpp
    animations/PacManAnimations.cpp
    animations/PelletAnimations.cpp
    animations/GridAnimation.cpp
    views/CommonView.cpp
    views/LevelStartSceneView.cpp
    views/LoadingSceneView.cpp
    views/MainMenuSceneView.cpp
    views/StartUpSceneView.cpp
    models/scenes/GameOverScene.cpp
    views/GameOverSceneView.cpp
    models/scenes/GamePauseScene.cpp
    views/GamePauseSceneView.cpp)

# Resource script files
set(RES_FILES
    ${PROJECT_SOURCE_DIR}/icon.rc
    ${PROJECT_SOURCE_DIR}/version.rc)

# Set executables output folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Create executable project source files
add_executable(SuperPacMan ${SRC_FILES} ${RES_FILES})

# Find third party dependency
set(IME_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME/lib/cmake/IME")
set(IME_BIN_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME/bin")
find_package(IME 2.1.0 REQUIRED)

# Link third party dependency to executable
target_link_libraries (SuperPacMan PRIVATE ime)

# Add <project>/ as include directory
include_directories(${PROJECT_SOURCE_DIR}/)

# Copy runtime dependencies to the executable output folder
add_custom_command(TARGET SuperPacMan PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${IME_BIN_DIR}/${CMAKE_BUILD_TYPE}/" $<TARGET_FILE_DIR:SuperPacMan>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${IME_BIN_DIR}/Runtime/" $<TARGET_FILE_DIR:SuperPacMan>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" $<TARGET_FILE_DIR:SuperPacMan>/assets
)