cmake_minimum_required(VERSION 3.20)
project(NasCode VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 启用Qt语言工具
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/forms)

# Qt6配置
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Widgets 
    Network
    SerialPort
    LinguistTools
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

# 翻译文件
set(TS_FILES
    ${CMAKE_SOURCE_DIR}/translations/nascode_zh_CN.ts
    ${CMAKE_SOURCE_DIR}/translations/nascode_en_US.ts
)

# 生成.qm文件
qt6_add_translation(QM_FILES ${TS_FILES})

# 添加翻译文件到构建
add_custom_target(translations ALL DEPENDS ${QM_FILES})

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

# 安装翻译文件
install(FILES ${QM_FILES}
    DESTINATION bin/translations
)
