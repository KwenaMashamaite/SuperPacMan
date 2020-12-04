set(SRC_FILES
        main.cpp
        Game.cpp
        common/Drawer.cpp
        animations/GhostAnimations.cpp
        animations/PacManAnimations.cpp
        animations/PelletAnimations.cpp
        animations/FruitAnimation.cpp
        controllers/PacManController.cpp
        controllers/GhostsController.cpp
        views/LoadingView.cpp
        views/MainMenuView.cpp
        views/LevelStartView.cpp
        views/CommonView.cpp
        views/IntroView.cpp
        entities/states/StateController.cpp
        common/SpriteContainer.cpp
        entities/Door.cpp
        entities/DoorLocker.cpp
        entities/Fruit.cpp
        entities/Ghost.cpp
        entities/Key.cpp
        entities/PacMan.cpp
        entities/Pellet.cpp
        entities/Wall.cpp
        scoreboard/Score.cpp
        scoreboard/Scoreboard.cpp
        states/IntroState.cpp
        states/LoadingState.cpp
        states/MainMenuState.cpp
        states/LevelStartState.cpp
        states/PlayingState.cpp
        utils/Utils.cpp
        entities/states/ghost/GhostIdleState.cpp
        entities/states/pacman/PacManIdleState.cpp
        entities/states/ghost/EatenState.cpp
        entities/states/ghost/ScatterState.cpp
        entities/states/ghost/FrightenedState.cpp
        entities/states/pacman/SuperState.cpp)

#Set executables output folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#Create executable project source files
add_executable(SuperPacMan ${SRC_FILES})

#Find SFML
set(SFML_DIR "${PROJECT_SOURCE_DIR}/extlibs/SFML/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS REQUIRED system window graphics audio)

#Find TGUI
set(TGUI_DIR "${PROJECT_SOURCE_DIR}/extlibs/TGUI/lib/cmake/TGUI")
find_package(TGUI 0.9.0 REQUIRED)

#Find IME
set(IME_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME/lib/cmake/IME")
find_package(IME 1.0.0 REQUIRED)

# Link TGUI, SFML and IME
target_link_libraries (SuperPacMan PRIVATE tgui sfml-graphics sfml-window sfml-system sfml-audio ime)