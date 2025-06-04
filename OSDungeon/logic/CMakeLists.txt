cmake_minimum_required(VERSION 3.31) 
project(OSDungeon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(${PROJECT_NAME}LOGIC databases.cpp brush.cpp common.cpp entity.cpp entity_state.cpp entity_template.cpp game_state.cpp ground.cpp labyrinth.cpp labyrinth_background.cpp labyrinth_pov.cpp sprite.cpp texture.cpp wall.cpp )

target_link_libraries(${PROJECT_NAME}LOGIC PUBLIC 
    "sfml-graphics" 
)
