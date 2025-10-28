cmake_minimum_required(VERSION 3.20)
project(NasCode VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6配置
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Widgets 
    Network
    SerialPort
)

# 第三方库
find_package(spdlog REQUIRED)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# 子目录
add_subdirectory(src/core)
add_subdirectory(src/models)
add_subdirectory(src/views)
add_subdirectory(src/controllers)
add_subdirectory(src/compiler)
add_subdirectory(src/communication)
add_subdirectory(src/plugins)
add_subdirectory(src/app)

# 主可执行文件
add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        NasCodeCore
        NasCodeModels
        NasCodeViews
        NasCodeControllers
        spdlog::spdlog
)

# 安装配置
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
