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

file(GLOB IMGUI_FILES "imgui/*.cpp")

add_executable(${PROJECT_NAME}_editor ${IMGUI_FILES} background_view.cpp  background_view.h  brush_editor.cpp  brush_editor.h  db_editor.cpp  db_editor.h  entity_editor.cpp  entity_editor.h  imgui_utils.cpp  imgui_utils.h  labyrinth_editor.cpp  labyrinth_editor.h  labyrinth_print.cpp  labyrinth_print.h  labyrinth_test.cpp  labyrinth_top_view.cpp  labyrinth_top_view.h  labyrinth_view.cpp  labyrinth_view.h  main.cpp  one_point_perspective.cpp  one_point_perspective.h  perspective_editor.cpp  perspective_editor.h  runner.cpp static_view.cpp  static_view.h  view_utils.cpp  view_utils.h  )

target_link_libraries(${PROJECT_NAME}_editor PUBLIC 
    "sfml-graphics" 
	"sfml-audio"
	${PROJECT_NAME}LOGIC
)

# Does not seem to work?
set_target_properties(${PROJECT_NAME}_editor PROPERTIES  
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

#set(link_src "${CMAKE_SOURCE_DIR}/assets")
#if(UNIX AND NOT APPLE) 
#    set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/assets") 
#else() 
#    set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/assets") 
#endif() 
#add_custom_command( 
#    TARGET ${PROJECT_NAME}_editor
#    POST_BUILD COMMAND ${CMAKE_COMMAND} 
#        -E create_symlink ${link_src} ${link_dst} 
#    COMMENT "symbolic link ${link_src} => ${link_dst}" 
#)

#add_executable(${PROJECT_NAME} ${SRC_FILES})