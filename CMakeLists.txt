# cmake -------------------------------------------------------------------------------------------
cmake_minimum_required( VERSION 3.20 )

set( CMAKE_CXX_STANDARD              23  )
set( CMAKE_CXX_STANDARD_REQUIRED     ON  )
set( CMAKE_CXX_EXTENSIONS            OFF )
set( CMAKE_POSITION_INDEPENDENT_CODE ON )

# dependencies ------------------------------------------------------------------------------------
find_package( Vulkan REQUIRED )

if( ANDROID )
    set( AER_ANDROID 1 )
elseif( WIN32 )
    set( AER_WIN32 1 )
elseif ( APPLE )
    set( AER_APPLE 1 )
else()
    set( AER_LINUX 1 )
    find_package( PkgConfig REQUIRED )
    pkg_check_modules( xcb REQUIRED xcb )
endif()

# AEON::Vulkan ------------------------------------------------------------------------------------
set( VULKAN_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( VULKAN_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( VULKAN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

set( VULKAN_HEADER
    ${VULKAN_INC_DIR}/vk/vk.h
    ${VULKAN_INC_DIR}/vk/Instance.h
    ${VULKAN_INC_DIR}/vk/PhysicalDevice.h
    ${VULKAN_INC_DIR}/vk/LogicalDevice.h
    ${VULKAN_INC_DIR}/vk/Surface.h
    ${VULKAN_INC_DIR}/vk/Swapchain.h
    ${VULKAN_INC_DIR}/vk/GraphicsPipeline.h
    ${VULKAN_INC_DIR}/vk/PipelineLayout.h
    ${VULKAN_INC_DIR}/vk/RenderPass.h
    ${VULKAN_INC_DIR}/vk/Framebuffer.h
    ${VULKAN_INC_DIR}/vk/Context.h
    ${VULKAN_INC_DIR}/vk/CommandPool.h
    ${VULKAN_INC_DIR}/vk/CommandBuffer.h
    ${VULKAN_INC_DIR}/vk/Fence.h
    ${VULKAN_INC_DIR}/vk/Semaphore.h

    ${VULKAN_INC_DIR}/vk/state/Image.h
    ${VULKAN_INC_DIR}/vk/state/ImageView.h
    ${VULKAN_INC_DIR}/vk/state/GraphicsPipelineState.h
    ${VULKAN_INC_DIR}/vk/state/ShaderModule.h
    ${VULKAN_INC_DIR}/vk/state/ShaderStage.h
    ${VULKAN_INC_DIR}/vk/state/DynamicState.h
    ${VULKAN_INC_DIR}/vk/state/VertexInputState.h
    ${VULKAN_INC_DIR}/vk/state/InputAssemblyState.h
    ${VULKAN_INC_DIR}/vk/state/ViewportState.h
    ${VULKAN_INC_DIR}/vk/state/RasterizationState.h
    ${VULKAN_INC_DIR}/vk/state/MultisampleState.h
    ${VULKAN_INC_DIR}/vk/state/DepthStencilState.h
    ${VULKAN_INC_DIR}/vk/state/ColorBlendState.h
    ${VULKAN_INC_DIR}/vk/state/TessellationState.h
)

set( VULKAN_SRC    
    ${VULKAN_SRC_DIR}/vk/Instance.cpp 
    ${VULKAN_SRC_DIR}/vk/PhysicalDevice.cpp
    ${VULKAN_SRC_DIR}/vk/LogicalDevice.cpp
    ${VULKAN_SRC_DIR}/vk/Surface.cpp
    ${VULKAN_SRC_DIR}/vk/Swapchain.cpp
    ${VULKAN_SRC_DIR}/vk/GraphicsPipeline.cpp
    ${VULKAN_SRC_DIR}/vk/PipelineLayout.cpp
    ${VULKAN_SRC_DIR}/vk/RenderPass.cpp
    ${VULKAN_SRC_DIR}/vk/Framebuffer.cpp
    ${VULKAN_SRC_DIR}/vk/Context.cpp
    ${VULKAN_SRC_DIR}/vk/CommandPool.cpp
    ${VULKAN_SRC_DIR}/vk/CommandBuffer.cpp
    ${VULKAN_SRC_DIR}/vk/Fence.cpp
    ${VULKAN_SRC_DIR}/vk/Semaphore.cpp

    ${VULKAN_SRC_DIR}/vk/state/Image.cpp
    ${VULKAN_SRC_DIR}/vk/state/ImageView.cpp
    ${VULKAN_SRC_DIR}/vk/state/ShaderModule.cpp
    ${VULKAN_SRC_DIR}/vk/state/ShaderStage.cpp
    ${VULKAN_SRC_DIR}/vk/state/DynamicState.cpp
    ${VULKAN_SRC_DIR}/vk/state/VertexInputState.cpp
    ${VULKAN_SRC_DIR}/vk/state/InputAssemblyState.cpp
    ${VULKAN_SRC_DIR}/vk/state/ViewportState.cpp
    ${VULKAN_SRC_DIR}/vk/state/RasterizationState.cpp
    ${VULKAN_SRC_DIR}/vk/state/MultisampleState.cpp
    ${VULKAN_SRC_DIR}/vk/state/DepthStencilState.cpp
    ${VULKAN_SRC_DIR}/vk/state/ColorBlendState.cpp
    ${VULKAN_SRC_DIR}/vk/state/TessellationState.cpp
)

add_library( vulkan STATIC  
    ${VULKAN_HEADER}
    ${VULKAN_SRC}
)

target_compile_definitions( vulkan PUBLIC
    $<$<BOOL:${AER_WIN32}>:VK_USE_PLATFORM_WIN32_KHR>
    $<$<BOOL:${AER_LINUX}>:VK_USE_PLATFORM_XCB_KHR >
)

target_include_directories( vulkan 
    PUBLIC
        $<BUILD_INTERFACE:${VULKAN_INC_DIR}>
	    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $<BUILD_INTERFACE:${VULKAN_SRC_DIR}>
)
target_link_libraries( vulkan 
    PRIVATE
        base
        Vulkan::Vulkan
    $<$<BOOL:${AER_LINUX}>:xcb>
)

add_library( aer::vulkan ALIAS vulkan )
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