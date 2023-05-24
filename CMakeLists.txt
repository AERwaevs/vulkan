# cmake -------------------------------------------------------------------------------------------
cmake_minimum_required( VERSION 3.2.0 )
# AEON::Vulkan ------------------------------------------------------------------------------------
set( VULKAN_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( VULKAN_INC ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( VULKAN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src )

set( VULKAN_EXTENSIONS
    "VK_KHR_surface"
    "VK_EXT_debug_utils"
    "VK_KHR_win32_surface"
    "VK_KHR_swapchain"
)

add_library( aeon-platform-vulkan STATIC  
    ${VULKAN_SRC}/VulkanViewport.cpp
    ${VULKAN_SRC}/VulkanRenderer.cpp

    ${VULKAN_SRC}/vk/Instance.cpp 
    ${VULKAN_SRC}/vk/PhysicalDevice.cpp
    ${VULKAN_SRC}/vk/LogicalDevice.cpp
    ${VULKAN_SRC}/vk/Surface.cpp
    ${VULKAN_SRC}/vk/Swapchain.cpp
    ${VULKAN_SRC}/vk/Image.cpp
    ${VULKAN_SRC}/vk/ImageView.cpp
)

target_include_directories( aeon-platform-vulkan 
    PUBLIC
        $<BUILD_INTERFACE:${VULKAN_INC}>
	    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $<BUILD_INTERFACE:${VULKAN_SRC}>
)
target_link_libraries( aeon-platform-vulkan PRIVATE aeon-base aeon-graphics )

# submodules --------------------------------------------------------------------------------------
find_package( Git QUIET )
if( GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git" )
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
# vulkan loader -----------------------------------------------------------------------------------
set( GLAD_DIR "${VULKAN_LIB}/glad" )
set( LOADER_DIR "${VULKAN_LIB}/vulkan")

if( NOT EXISTS "${LOADER_DIR}/include/glad/vulkan.h" )
    message( STATUS "Vulkan header not found!" )
endif()

if( NOT EXISTS "${GLAD_DIR}/cmake/CMakeLists.txt" )
    message( FATAL_ERROR "Glad submodule not present! GIT_SUBMODULE was turned off or failed. Please update submodules and try again." )
endif()

add_subdirectory( "${GLAD_DIR}/cmake" GLAD_DIR )

glad_add_library( vulkan SHARED
    EXCLUDE_FROM_ALL
    LOADER
    LOCATION    ${LOADER_DIR}
    API         vulkan=1.3
    EXTENSIONS  ${VULKAN_EXTENSIONS}
)

if( WIN32 )
    target_compile_definitions( vulkan PUBLIC VK_USE_PLATFORM_WIN32_KHR NOMINMAX ) 
endif()

target_link_libraries( aeon-platform-vulkan PRIVATE vulkan )