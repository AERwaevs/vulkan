cmake_minimum_required( VERSION 3.2.0 )
project( aeon-platform-vulkan
    VERSION      0.1.0.0 
    DESCRIPTION  "AEON game / simulation framework." 
    HOMEPAGE_URL "https://github.com/AERwaevs/aeon"
    LANGUAGES    C CXX
)

# vulkan ------------------------------------------------------------------------------------------

set( VK_EXT
    "VK_KHR_surface"
    "VK_EXT_debug_utils"
    "VK_KHR_win32_surface"
    "VK_KHR_swapchain"
)

set( VK_SRC 
    vk/Instance.cpp 
    vk/PhysicalDevice.cpp
    vk/LogicalDevice.cpp
    vk/Surface.cpp
    vk/Swapchain.cpp
)

# glad --------------------------------------------------------------------------------------------

set( GLAD_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/glad" )
add_subdirectory( "${GLAD_DIR}/gen/cmake" GLAD_DIR )

glad_add_library( vulkan STATIC EXCLUDE_FROM_ALL
    LOADER
    LOCATION    ${GLAD_DIR}
    API         vulkan=1.2
    EXTENSIONS  ${VK_EXT}
)

# /vk ---------------------------------------------------------------------------------------------

add_library( aeon-platform-vulkan STATIC  
    VulkanViewport.cpp
    VulkanRenderer.cpp
    ${VK_SRC}
)

add_dependencies( aeon-platform-vulkan
    aeon-base
)

set_target_properties(      aeon-platform-vulkan PROPERTIES    POSITION_INDEPENDENT_CODE   TRUE )
target_compile_definitions( aeon-platform-vulkan PUBLIC AEON_DEBUG VK_USE_PLATFORM_WIN32_KHR )
#target_precompile_headers(  aeon-platform-vulkan   REUSE_FROM  aeon-base                     )
target_link_libraries( aeon-platform-vulkan PUBLIC aeon-base )# glad_gl46_vk12 )
target_include_directories( aeon-platform-vulkan PRIVATE ${AEON_DIR} ${GLAD_INC} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )