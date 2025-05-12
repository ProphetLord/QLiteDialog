project(popup-test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)
find_package(QLogCollectorServer REQUIRED)

add_compile_definitions(TEST_CONFIG_PATH="${CMAKE_CURRENT_SOURCE_DIR}/test-config.json")

if (WIN32)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
endif()

add_compile_definitions(APP_VERSION="v${PROJECT_VERSION}")

add_executable(${PROJECT_NAME} WIN32

    main.cpp

    app.h
    app.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets

    qlogcollectorserver

    qlitedialog
)