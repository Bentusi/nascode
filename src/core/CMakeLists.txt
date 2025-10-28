add_library(NasCodeCore STATIC
    Application.cpp
    Application.h
    EventBus.cpp
    EventBus.h
    PluginManager.cpp
    PluginManager.h
    IPlugin.h
    I18nManager.cpp
    I18nManager.h
)

target_link_libraries(NasCodeCore
    PUBLIC
        Qt6::Core
        Qt6::Widgets
        spdlog::spdlog
)

target_include_directories(NasCodeCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
