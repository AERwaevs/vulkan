cmake_minimum_required( VERSION 3.2.0 )
project( aeon-platform-vulkan
    VERSION      0.1.0.0 
    DESCRIPTION  "AEON game / simulation framework." 
    HOMEPAGE_URL "https://github.com/AERwaevs/aeon"
    LANGUAGES    C CXX
)

set( VULKAN_SRC 
    vk/Instance.cpp 
    vk/PhysicalDevice.cpp
    vk/LogicalDevice.cpp
    vk/Surface.cpp
    vk/Swapchain.cpp
)

set( GLAD     ../../Vendor/glad )

set( GLAD_INC ${GLAD}/include )
set( GLAD_SRC ${GLAD}/src/vulkan.c )

add_library( aeon-platform-vulkan STATIC  
    VulkanViewport.cpp
    VulkanRenderer.cpp
    ${GLAD_SRC}
    ${VULKAN_SRC}
)
add_dependencies( aeon-platform-vulkan
    aeon-base
)
    #glad_gl46_vk12
#)
set_target_properties(      aeon-platform-vulkan PROPERTIES    POSITION_INDEPENDENT_CODE   TRUE )
target_compile_definitions( aeon-platform-vulkan PUBLIC AEON_DEBUG VK_USE_PLATFORM_WIN32_KHR )
#target_precompile_headers(  aeon-platform-vulkan   REUSE_FROM  aeon-base                     )
target_link_libraries( aeon-platform-vulkan PUBLIC aeon-base )# glad_gl46_vk12 )
target_include_directories( aeon-platform-vulkan PRIVATE ${AEON_DIR} ${GLAD_INC} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )