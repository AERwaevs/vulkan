set( VULKAN_SRC 
    vk/Instance.cpp 
    vk/PhysicalDevice.cpp
    vk/LogicalDevice.cpp
    vk/Surface.cpp
    VulkanViewport.cpp
    VulkanRenderer.cpp
)

add_library(                aeon-platform-vulkan STATIC        ${VULKAN_SRC} )
add_dependencies( aeon-platform-vulkan
    aeon-base
    glad_gl46_vk12
)
set_target_properties(      aeon-platform-vulkan PROPERTIES    POSITION_INDEPENDENT_CODE   TRUE )
target_compile_definitions( aeon-platform-vulkan PRIVATE VK_USE_PLATFORM_WIN32_KHR )
target_precompile_headers(  aeon-platform-vulkan   REUSE_FROM  aeon-base                     )
target_link_libraries( aeon-platform-vulkan PUBLIC aeon-base glad_gl46_vk12 )
target_include_directories( aeon-platform-vulkan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )