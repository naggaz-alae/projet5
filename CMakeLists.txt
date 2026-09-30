cmake_minimum_required(VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(game_text C)
#project(takuzu-05d)

include(CTest)
enable_testing()
include(SDL2.cmake)
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

set(CMAKE_C_FLAGS "-Wall -std=c99") # options de compilation génériques
set(CMAKE_C_FLAGS_DEBUG "-g -O0 --coverage") # options de compilation en mode Debug
set(CMAKE_C_FLAGS_RELEASE "-O3") # options de compilation en mode Release


##################################################################

#positionne la variable
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

include_directories(${SDL2_ALL_INC})
link_directories(${CMAKE_SOURCE_DIR})

add_library(game game.c game_aux.c game_ext.c queue.c game_tools.c game_private.c)
add_executable(game_text game_text.c)
target_link_libraries(game_text game m)

add_executable(game_solve game_solve.c)
target_link_libraries(game_solve game)

add_executable(game_sdl game_sdl.c model.c)
target_link_libraries(game_sdl game ${SDL2_ALL_LIBS} m)

add_executable(game_test_anaggaz game_test_anaggaz.c)
target_link_libraries(game_test_anaggaz game )
add_executable(game_test_mlalaouirhal game_test_mlalaouirhal.c)
target_link_libraries(game_test_mlalaouirhal game )
add_executable(game_test_yfadl game_test_yfadl.c)
target_link_libraries(game_test_yfadl game )


#*********************************************anaggaz**********************
add_test(test_anaggaz_game_copy ./game_test_anaggaz game_copy)
add_test(test_anaggaz_game_equal ./game_test_anaggaz game_equal)
add_test(test_anaggaz_game_get_square ./game_test_anaggaz game_get_square)
add_test(test_anaggaz_game_get_next_number ./game_test_anaggaz game_get_next_number)
add_test(test_anaggaz_game_has_error ./game_test_anaggaz game_has_error)
add_test(test_anaggaz_game_is_over ./game_test_anaggaz game_is_over)
add_test(test_anaggaz_game_default ./game_test_anaggaz game_default)
add_test(test_anaggaz_game_load ./game_test_anaggaz game_load)
add_test(test_anaggaz_game_save ./game_test_anaggaz game_save)



#*********************************************mlalaouirhal**********************
add_test(test_mlalaouirhal_game_new ./game_test_mlalaouirhal game_new)
add_test(test_mlalaouirhal_game_set_square ./game_test_mlalaouirhal game_set_square)
add_test(test_mlalaouirhal_game_get_next_square ./game_test_mlalaouirhal game_get_next_square)
add_test(test_mlalaouirhal_game_is_immutable ./game_test_mlalaouirhal game_is_immutable)
add_test(test_mlalaouirhal_game_play_move ./game_test_mlalaouirhal game_play_move)
add_test(test_mlalaouirhal_game_restart ./game_test_mlalaouirhal game_restart)
add_test(test_mlalaouirhal_game_print ./game_test_mlalaouirhal game_print)

#*********************************************yfadl**********************
add_test(test_yfadl_game_default_solution ./game_test_yfadl game_default_solution)
add_test(test_yfadl_game_new_empty ./game_test_yfadl game_new_empty)
add_test(test_yfadl_game_game_delete ./game_test_yfadl game_delete)
add_test(test_yfadl_game_get_number ./game_test_yfadl game_get_number)
add_test(test_yfadl_game_is_empty ./game_test_yfadl game_is_empty)
add_test(test_yfadl_game_check_move ./game_test_yfadl game_check_move)
#*****************************************************************************
#*****************************EXT TESTS**************************************
#*****************************************************************************
add_test(test_game_new_ext ./game_test_yfadl game_new_ext)
add_test(test_game_new_empty_ext ./game_test_anaggaz game_new_empty_ext)
add_test(test_game_nb_rows ./game_test_mlalaouirhal game_nb_rows)
add_test(test_game_nb_cols ./game_test_mlalaouirhal game_nb_cols)
add_test(test_game_is_unique ./game_test_yfadl game_is_unique)
add_test(test_game_is_wrapping ./game_test_anaggaz game_is_wrapping)
add_test(test_game_undo ./game_test_anaggaz game_undo)
add_test(test_game_redo ./game_test_anaggaz game_redo)

# copy useful data files in build dir
foreach(file "images/MainBackground.jpg" "images/empty.jpg" "images/erreur.jpg"
"images/Help.jpg" "images/immutableW.jpg" "images/immutableB.jpg" "images/squareB.jpg" "images/squareW.jpg")
  file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file)

