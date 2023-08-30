cmake_minimum_required(VERSION 3.25.1)

set(MINGW32_DIRECTORY "C:\\Portable\\mingw64") #For windows compilation

project(engine_test C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}") # -Wall -Wextra -Wpedantic  -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers")
set(CMAKE_EXE_LINKER_FLAGS "-pg")


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}\\bin)

link_directories("lib\\p8g")

add_executable(engine_test)

target_link_libraries(engine_test p8g)

configure_file("./lib/chess_font.ttf" ${CMAKE_CURRENT_BINARY_DIR}\\bin\\chess_font.ttf COPYONLY)


IF (WIN32)

    configure_file("./lib/chess_font.ttf" ${CMAKE_CURRENT_BINARY_DIR}\\bin\\chess_font.ttf COPYONLY)
    #configure_file("${MINGW32_DIRECTORY}\\bin\\libgcc_s_seh-1.dll" ${CMAKE_CURRENT_BINARY_DIR}\\bin\\libgcc_s_seh-1.dll COPYONLY)
    #configure_file("${MINGW32_DIRECTORY}\\bin\\libstdc++-6.dll" ${CMAKE_CURRENT_BINARY_DIR}\\bin\\libstdc++-6.dll COPYONLY)

ENDIF()



target_sources(engine_test
    PRIVATE 
        src/main.c

        src/pc/graphics.c

        src/game/movement.c
        src/game/game.c
        
        src/game/engine/board.c
        src/game/engine/interface.c
        src/game/engine/moves.c
        src/game/engine/board.c
        src/game/engine/engine.c
        src/game/lighting/lighting.c
)
