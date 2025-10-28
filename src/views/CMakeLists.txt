add_library(NasCodeViews STATIC
    MainWindow.h
    MainWindow.cpp
    CodeEditor.h
    CodeEditor.cpp
    OutputPanel.h
    OutputPanel.cpp
)

target_link_libraries(NasCodeViews
    PUBLIC
        Qt6::Core
        Qt6::Widgets
        NasCodeCore
        NasCodeModels
        spdlog::spdlog
)

target_include_directories(NasCodeViews
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
