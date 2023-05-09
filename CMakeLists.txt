cmake_minimum_required( VERSION 3.2.0 )

find_package( Git QUIET )
if( GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git" )
# Update submodules as needed
    option( GIT_SUBMODULE "Check submodules during build" ON )
    if( GIT_SUBMODULE )
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

set( LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

# vulkan ------------------------------------------------------------------------------------------

set( VK_EXT
    "VK_KHR_surface"
    "VK_EXT_debug_utils"
    "VK_KHR_win32_surface"
    "VK_KHR_swapchain"
)

set( VK_SRC
    ${SRC_DIR}/vk/Instance.cpp 
    ${SRC_DIR}/vk/PhysicalDevice.cpp
    ${SRC_DIR}/vk/LogicalDevice.cpp
    ${SRC_DIR}/vk/Surface.cpp
    ${SRC_DIR}/vk/Swapchain.cpp
)

# glad --------------------------------------------------------------------------------------------

set( GLAD_DIR "${LIB_DIR}/glad" )

if( NOT EXISTS "${GLAD_DIR}/include/glad/vulkan.h" )
    message( STATUS "Vulkan header not found!" )
endif()

if( NOT EXISTS "${GLAD_DIR}/cmake/CMakeLists.txt" )
    message( FATAL_ERROR "Glad submodule not present! GIT_SUBMODULE was turned off or failed. Please update submodules and try again." )
endif()

add_subdirectory( "${GLAD_DIR}/cmake" GLAD_DIR )

glad_add_library( vulkan STATIC EXCLUDE_FROM_ALL
    LOADER
    LOCATION    ${LIB_DIR}/vulkan
    API         vulkan=1.2
    EXTENSIONS  ${VK_EXT}
)

# AEON::Vulkan ------------------------------------------------------------------------------------

add_library( aeon-platform-vulkan STATIC  
    ${SRC_DIR}/VulkanViewport.cpp
    ${SRC_DIR}/VulkanRenderer.cpp
    ${VK_SRC}
)

add_dependencies( aeon-platform-vulkan
    aeon-base
    aeon-graphics
    vulkan
)

target_compile_definitions( aeon-platform-vulkan PRIVATE VK_USE_PLATFORM_WIN32_KHR NOMINMAX )
target_precompile_headers(  aeon-platform-vulkan PRIVATE ${HEADERS} ${STD_HEADERS} )
target_include_directories( aeon-platform-vulkan 
    PUBLIC
        ${INC_DIR}
    PRIVATE
        ${SRC_DIR}
)
target_link_libraries( aeon-platform-vulkan
    PRIVATE
        aeon-base
        aeon-graphics
        vulkan
)