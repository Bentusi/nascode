add_library(NasCodeModels STATIC
    Project.h
    Project.cpp
    ProjectConfig.h
    ProjectConfig.cpp
    POU.h
    POU.cpp
    ProjectModel.h
    ProjectModel.cpp
    LibraryManager.h
    LibraryManager.cpp
)

target_link_libraries(NasCodeModels
    PUBLIC
        Qt6::Core
        spdlog::spdlog
)

target_include_directories(NasCodeModels
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
