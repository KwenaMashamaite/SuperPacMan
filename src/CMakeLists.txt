file(GLOB_RECURSE SUPER_PACMAN_SRC_ROOT ${PROJECT_SOURCE_DIR}/src *.cpp *.h)

#Remove Cmake main from project source files
file(GLOB_RECURSE CMAKE_MAIN "cmake-build-debug/*")
list(REMOVE_ITEM IN SUPER_PACMAN_SRC_ROOT ${CMAKE_MAIN})

#Set executables output folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#Create executable project source files
add_executable(SuperPacMan ${SUPER_PACMAN_SRC_ROOT})

#Find and link SFML
set(SFML_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/extlibs/SFML/include")
set(SFML_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/extlibs/SFML/lib")
set(SFML_DIR "${PROJECT_SOURCE_DIR}/extlibs/SFML/lib/cmake/SFML")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics audio)

if(SFML_FOUND)
    target_link_libraries (SuperPacMan PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARY_DIR}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

#Find and link TGUI
set(TGUI_INCLUDE_DIR "C:/Users/Brandon/Desktop/IME/extlibs/TGUI-0.9/include")
set(TGUI_LIBRARY_DIR "C:/Users/Brandon/Desktop/IME/extlibs/TGUI-0.9/cmake-build-debug/lib")

link_libraries(${TGUI_LIBRARY_DIR})
include_directories(${TGUI_INCLUDE_DIR})
target_link_libraries(SuperPacMan PUBLIC ${TGUI_LIBRARY_DIR}/libtgui-d.a)

#Find and link IME
set(IME_INCLUDE_DIR "C:/Users/Brandon/Desktop/IME/include")
set(IME_LIBRARY_DIR "C:/Users/Brandon/Desktop/IME/bin")

link_libraries(${IME_LIBRARY_DIR})
include_directories(${IME_INCLUDE_DIR})

target_link_libraries(SuperPacMan PUBLIC ${IME_LIBRARY_DIR}/libime.dll.a)