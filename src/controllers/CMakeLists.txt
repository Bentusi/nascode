add_library(NasCodeControllers STATIC
    ProjectController.h
    ProjectController.cpp
)

target_link_libraries(NasCodeControllers
    PUBLIC
        Qt6::Core
        NasCodeCore
        NasCodeModels
        spdlog::spdlog
)

target_include_directories(NasCodeControllers
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
