cmake_minimum_required(VERSION 4.1)
project(todo
    VERSION 0.1.0
    LANGUAGES CXX
)

# CODE
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# PACKAGES
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(nlohmann_json 3.12.0 REQUIRED)

# QT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC OFF)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/task.cpp
    src/task_loader.cpp
    src/task_dialog.cpp
    src/task_dialog.ui
    src/main_window.cpp
    src/main_window.ui
)

# LINKING
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core
    PRIVATE Qt6::Widgets
    PRIVATE nlohmann_json::nlohmann_json
)
