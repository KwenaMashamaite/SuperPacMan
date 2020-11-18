set(SRC_FILES
        main.cpp
        Game.cpp
        animators/GhostAnimator.cpp
        animators/PacManAnimator.cpp
        animators/PelletAnimator.cpp
        controllers/PacManController.cpp
        controllers/GhostsController.cpp
        view/LoadingView.cpp
        view/MainMenuView.cpp
        view/ScoreView.cpp
        view/IntroView.cpp
        common/SpriteContainer.cpp
        entity/Door.cpp
        entity/DoorLocker.cpp
        entity/Fruit.cpp
        entity/Ghost.cpp
        entity/Key.cpp
        entity/Movable.cpp
        entity/PacMan.cpp
        entity/Pellet.cpp
        entity/Wall.cpp
        scoreboard/Score.cpp
        scoreboard/Scoreboard.cpp
        states/IntroState.cpp
        states/LoadingState.cpp
        states/MainMenuState.cpp
        states/PlayingState.cpp
        )

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