cmake_minimum_required(VERSION 3.12)

project(qlitedialog)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_DEBUG_POSTFIX "d")

if(OPEN_PARTY_DEBUG_MESSAGE)
    add_definitions(-DDEBUG_MESSAGE)
endif ()

add_library(${PROJECT_NAME}
    basedialog.h
    baseanimation.h
    baseanimation.cpp

    windowframelesshelper.h
    windowframelesshelper.cpp

    inner/innerdialog.h
    inner/innerdialog.cpp

    normal/normaldialog.h
    normal/normaldialog.cpp

    weakhint/weakhintdialog.h
    weakhint/weakhintdialog_p.h
    weakhint/weakhintdialog.cpp
    tip/tipsdialog.cpp
    tip/tipsdialog.h
    tip/tipsmanager.cpp
    tip/tipsmanager.h
)

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)

install(TARGETS ${PROJECT_NAME}
    EXPORT QLiteDialogTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY .
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
    PATTERN "*_p.h" EXCLUDE
)

configure_package_config_file(QLiteDialogConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/QLiteDialogConfig.cmake"
    INSTALL_DESTINATION lib/cmake/QLiteDialog
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/QLiteDialogConfig.cmake"
    DESTINATION lib/cmake/QLiteDialog
)

install(EXPORT QLiteDialogTargets
    FILE QLiteDialogTargets.cmake
    DESTINATION lib/cmake/QLiteDialog
)