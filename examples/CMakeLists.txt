# 示例程序

# POU序列化测试
add_executable(test_pou_serialization test_pou_serialization.cpp)
target_link_libraries(test_pou_serialization
    PRIVATE
    NasCodeModels
    Qt6::Core
)

# 设置输出目录
set_target_properties(test_pou_serialization
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)
