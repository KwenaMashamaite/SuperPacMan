set(SRC_FILES
        main.cpp
        Game.cpp
        animators/GhostAnimator.cpp
        animators/PacManAnimator.cpp
        animators/PelletAnimator.cpp
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
set(SFML_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/extlibs/SFML/include")
set(SFML_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/extlibs/SFML/lib")
set(SFML_DIR "${PROJECT_SOURCE_DIR}/extlibs/SFML/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS REQUIRED system window graphics audio)
message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARY_DIR}")
message(STATUS "SFML_VERSION: ${SFML_VERSION}")

#Find TGUI
set(TGUI_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/extlibs/TGUI/include")
set(TGUI_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/extlibs/TGUI/lib")
set(TGUI_DIR "${PROJECT_SOURCE_DIR}/extlibs/TGUI/lib/cmake/TGUI")

find_package(TGUI 0.9.0 REQUIRED)
message(STATUS "TGUI_INCLUDE_DIR: ${TGUI_INCLUDE_DIR}")
message(STATUS "TGUI_LIBRARIES: ${TGUI_LIBRARY_DIR}")
message(STATUS "TGUI_VERSION: ${TGUI_VERSION}")

#Find IME
set(IME_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME/include")
set(IME_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME/lib")
set(IME_DIR "${PROJECT_SOURCE_DIR}/extlibs/IME/lib/cmake/IME")

find_package(IME 1.0.0 REQUIRED)
message(STATUS "IME_INCLUDE_DIR: ${IME_INCLUDE_DIR}")
message(STATUS "IME_LIBRARIES: ${IME_LIBRARY_DIR}")
message(STATUS "IME_VERSION: ${IME_VERSION}")

# Link TGUI, SFML and IME
target_link_libraries (SuperPacMan PRIVATE tgui sfml-graphics sfml-window sfml-system sfml-audio ime)