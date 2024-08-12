add_library(amazon_s3 STATIC
        AwsCommands.cpp
)

find_package(AWSSDK REQUIRED COMPONENTS s3 sts)

target_link_libraries(amazon_s3 PRIVATE ${AWSSDK_LINK_LIBRARIES}
        ${AWSSDK_PLATFORM_DEPS})

target_include_directories(amazon_s3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
