# cmake -------------------------------------------------------------------------------------------
cmake_minimum_required( VERSION 3.2.0 )
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
# vulkan_loader -----------------------------------------------------------------------------------
set( GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad" )
set( LOADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/loader")

set( VK_EXTENSIONS
    "VK_KHR_surface"
    "VK_EXT_debug_utils"
    "VK_KHR_win32_surface"
    "VK_KHR_swapchain"
)

if( NOT EXISTS "${GLAD_DIR}/cmake/CMakeLists.txt" )
    message( FATAL_ERROR "Glad submodule not present! GIT_SUBMODULE was turned off or failed. Please update submodules and try again." )
endif()

if( NOT EXISTS "${LOADER_DIR}/include/glad/vulkan.h" )
    message( STATUS "Vulkan header not found, generating a loader" )
endif()

add_subdirectory( "${GLAD_DIR}/cmake" GLAD_DIR )
glad_add_library( loader STATIC
    EXCLUDE_FROM_ALL
    LOADER
    LOCATION    ${LOADER_DIR}
    API         vulkan=1.3
    EXTENSIONS  ${VK_EXTENSIONS}
)
target_compile_definitions( loader PUBLIC $<$<BOOL:${WIN32}>:VK_USE_PLATFORM_WIN32_KHR NOMINMAX> )
# AEON::Vulkan ------------------------------------------------------------------------------------
set( VULKAN_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( VULKAN_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( VULKAN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

set( VULKAN_HEADER    
    ${VULKAN_INC_DIR}/Graphics/VulkanViewport.h
    ${VULKAN_INC_DIR}/Graphics/VulkanRenderer.h
    
    ${VULKAN_INC_DIR}/vk/Instance.h
    ${VULKAN_INC_DIR}/vk/PhysicalDevice.h
    ${VULKAN_INC_DIR}/vk/LogicalDevice.h
    ${VULKAN_INC_DIR}/vk/Surface.h
    ${VULKAN_INC_DIR}/vk/Swapchain.h
    ${VULKAN_INC_DIR}/vk/GraphicsPipeline.h
    ${VULKAN_INC_DIR}/vk/state/Image.h
    ${VULKAN_INC_DIR}/vk/state/ImageView.h
    ${VULKAN_INC_DIR}/vk/state/Pipeline.h
    ${VULKAN_INC_DIR}/vk/state/GraphicsPipelineState.h
    ${VULKAN_INC_DIR}/vk/state/ShaderModule.h
    ${VULKAN_INC_DIR}/vk/state/ShaderStage.h
    ${VULKAN_INC_DIR}/vk/state/DynamicState.h
    ${VULKAN_INC_DIR}/vk/state/VertexInputState.h
    ${VULKAN_INC_DIR}/vk/state/InputAssemblyState.h
    ${VULKAN_INC_DIR}/vk/state/ViewportState.h
)

set( VULKAN_SRC    
    ${VULKAN_SRC_DIR}/VulkanViewport.cpp
    ${VULKAN_SRC_DIR}/VulkanRenderer.cpp
    
    ${VULKAN_SRC_DIR}/vk/Instance.cpp 
    ${VULKAN_SRC_DIR}/vk/PhysicalDevice.cpp
    ${VULKAN_SRC_DIR}/vk/LogicalDevice.cpp
    ${VULKAN_SRC_DIR}/vk/Surface.cpp
    ${VULKAN_SRC_DIR}/vk/Swapchain.cpp
    ${VULKAN_SRC_DIR}/vk/GraphicsPipeline.cpp
    ${VULKAN_SRC_DIR}/vk/state/Image.cpp
    ${VULKAN_SRC_DIR}/vk/state/ImageView.cpp
    ${VULKAN_SRC_DIR}/vk/state/ShaderModule.cpp
    ${VULKAN_SRC_DIR}/vk/state/ShaderStage.cpp
    ${VULKAN_SRC_DIR}/vk/state/DynamicState.cpp
    ${VULKAN_SRC_DIR}/vk/state/VertexInputState.cpp
    ${VULKAN_SRC_DIR}/vk/state/InputAssemblyState.cpp
    ${VULKAN_SRC_DIR}/vk/state/ViewportState.cpp
)

add_library( vulkan STATIC  
    ${VULKAN_HEADER}
    ${VULKAN_SRC}
)

target_include_directories( vulkan 
    PUBLIC
        $<BUILD_INTERFACE:${VULKAN_INC_DIR}>
	    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $<BUILD_INTERFACE:${VULKAN_SRC_DIR}>
        $<BUILD_INTERFACE:${LOADER_DIR}/include>
)
target_link_libraries( vulkan PRIVATE base graphics_interface loader )

# spir-v ------------------------------------------------------------------------------------------
set( SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders )
set( SHADER_COMPILER glslc )

file( GLOB_RECURSE SHADER_CONF ${SHADER_DIR}/*.conf  ) # config file that replaces the default configuration
file( GLOB_RECURSE SHADER_VERT ${SHADER_DIR}/*.vert  ) # vertex shader
file( GLOB_RECURSE SHADER_TESC ${SHADER_DIR}/*.tesc  ) # tessellation control shader
file( GLOB_RECURSE SHADER_TESE ${SHADER_DIR}/*.tese  ) # tessellation evaluation shader
file( GLOB_RECURSE SHADER_GEOM ${SHADER_DIR}/*.geom  ) # geometry shader
file( GLOB_RECURSE SHADER_FRAG ${SHADER_DIR}/*.frag  ) # fragment shader
file( GLOB_RECURSE SHADER_COMP ${SHADER_DIR}/*.comp  ) # compute shader
file( GLOB_RECURSE SHADER_MESH ${SHADER_DIR}/*.mesh  ) # mesh shader
file( GLOB_RECURSE SHADER_TASK ${SHADER_DIR}/*.task  ) # task shader
file( GLOB_RECURSE SHADER_RGEN ${SHADER_DIR}/*.rgen  ) # ray generation shader
file( GLOB_RECURSE SHADER_RINT ${SHADER_DIR}/*.rint  ) # ray intersection shader
file( GLOB_RECURSE SHADER_RAHT ${SHADER_DIR}/*.rahit ) # ray any hit shader
file( GLOB_RECURSE SHADER_RCHT ${SHADER_DIR}/*.rchit ) # ray closest hit shader
file( GLOB_RECURSE SHADER_RMSS ${SHADER_DIR}/*.rmiss ) # ray miss shader
file( GLOB_RECURSE SHADER_RCLL ${SHADER_DIR}/*.rcall ) # ray callable shader
file( GLOB_RECURSE SHADER_GLSL ${SHADER_DIR}/*.glsl  ) # for .vert.glsl, .tesc.glsl, ..., .comp.glsl compound suffixes
file( GLOB_RECURSE SHADER_HLSL ${SHADER_DIR}/*.hlsl  ) # for .hlsl or related compound suffixes

set( SHADER_SRC 
    ${SHADER_CONF}
    ${SHADER_VERT}
    ${SHADER_TESC}
    ${SHADER_TESE}
    ${SHADER_GEOM}
    ${SHADER_FRAG}
    ${SHADER_COMP}
    ${SHADER_MESH}
    ${SHADER_TASK}
    ${SHADER_RGEN}
    ${SHADER_RINT}
    ${SHADER_RAHT}
    ${SHADER_RCHT}
    ${SHADER_RMSS}
    ${SHADER_RCLL}
    ${SHADER_GLSL}
    ${SHADER_HLSL}
)

message( STATUS "Compiling spir-v shaders... ")
foreach( SHADER ${SHADER_SRC} )
    string( CONCAT SHADER_SPV "${SHADER}" ".spv" )
    message(STATUS "Compiling ${SHADER} => ${SHADER_SPV}")
    execute_process(
        COMMAND
            ${SHADER_COMPILER} ${SHADER} -o ${SHADER_SPV}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()