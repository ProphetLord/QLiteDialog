cmake_minimum_required(VERSION 3.12)

project(QLiteDialogProject)

option(QLiteDialog_Test "enable qlitedialog test" OFF)

if(QLiteDialog_Test)
add_subdirectory(test)
endif ()
add_subdirectory(src)