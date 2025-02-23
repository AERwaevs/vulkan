# cmake -------------------------------------------------------------------------------------------
cmake_minimum_required( VERSION 3.20 )

set( CMAKE_CXX_STANDARD              23  )
set( CMAKE_CXX_STANDARD_REQUIRED     ON  )
set( CMAKE_CXX_EXTENSIONS            OFF )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
include( compile_shaders )
# dependencies ------------------------------------------------------------------------------------
find_package( Vulkan REQUIRED )

if( ANDROID )
elseif( WIN32 )
elseif ( APPLE )
elseif( UNIX AND NOT APPLE )
    set( LINUX 1 )
endif()

# AEON::Vulkan ------------------------------------------------------------------------------------
set( LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

set( HEADERS
    ${INC_DIR}/vk/vk.h
    ${INC_DIR}/vk/Instance.h
    ${INC_DIR}/vk/PhysicalDevice.h
    ${INC_DIR}/vk/LogicalDevice.h
    ${INC_DIR}/vk/Surface.h
    ${INC_DIR}/vk/Swapchain.h
    ${INC_DIR}/vk/GraphicsPipeline.h
    ${INC_DIR}/vk/PipelineLayout.h
    ${INC_DIR}/vk/RenderPass.h
    ${INC_DIR}/vk/Framebuffer.h
    ${INC_DIR}/vk/Context.h
    ${INC_DIR}/vk/CommandPool.h
    ${INC_DIR}/vk/CommandBuffer.h
    ${INC_DIR}/vk/Fence.h
    ${INC_DIR}/vk/Semaphore.h

    ${INC_DIR}/vk/state/Image.h
    ${INC_DIR}/vk/state/ImageView.h
    ${INC_DIR}/vk/state/GraphicsPipelineState.h
    ${INC_DIR}/vk/state/ShaderModule.h
    ${INC_DIR}/vk/state/ShaderStage.h
    ${INC_DIR}/vk/state/DynamicState.h
    ${INC_DIR}/vk/state/VertexInputState.h
    ${INC_DIR}/vk/state/InputAssemblyState.h
    ${INC_DIR}/vk/state/ViewportState.h
    ${INC_DIR}/vk/state/RasterizationState.h
    ${INC_DIR}/vk/state/MultisampleState.h
    ${INC_DIR}/vk/state/DepthStencilState.h
    ${INC_DIR}/vk/state/ColorBlendState.h
    ${INC_DIR}/vk/state/TessellationState.h
)

set( SOURCES    
    ${SRC_DIR}/vk/Instance.cpp 
    ${SRC_DIR}/vk/PhysicalDevice.cpp
    ${SRC_DIR}/vk/LogicalDevice.cpp
    ${SRC_DIR}/vk/Surface.cpp
    ${SRC_DIR}/vk/Swapchain.cpp
    ${SRC_DIR}/vk/GraphicsPipeline.cpp
    ${SRC_DIR}/vk/PipelineLayout.cpp
    ${SRC_DIR}/vk/RenderPass.cpp
    ${SRC_DIR}/vk/Framebuffer.cpp
    ${SRC_DIR}/vk/Context.cpp
    ${SRC_DIR}/vk/CommandPool.cpp
    ${SRC_DIR}/vk/CommandBuffer.cpp
    ${SRC_DIR}/vk/Fence.cpp
    ${SRC_DIR}/vk/Semaphore.cpp

    ${SRC_DIR}/vk/state/Image.cpp
    ${SRC_DIR}/vk/state/ImageView.cpp
    ${SRC_DIR}/vk/state/ShaderModule.cpp
    ${SRC_DIR}/vk/state/ShaderStage.cpp
    ${SRC_DIR}/vk/state/DynamicState.cpp
    ${SRC_DIR}/vk/state/VertexInputState.cpp
    ${SRC_DIR}/vk/state/InputAssemblyState.cpp
    ${SRC_DIR}/vk/state/ViewportState.cpp
    ${SRC_DIR}/vk/state/RasterizationState.cpp
    ${SRC_DIR}/vk/state/MultisampleState.cpp
    ${SRC_DIR}/vk/state/DepthStencilState.cpp
    ${SRC_DIR}/vk/state/ColorBlendState.cpp
    ${SRC_DIR}/vk/state/TessellationState.cpp
)

add_library( vulkan ${HEADERS} ${SOURCES} )
add_library( aer::vulkan ALIAS vulkan )

set_target_properties( vulkan PROPERTIES
    CXX_STANDARD                    23
    CXX_STANDARD_REQUIRED           ON
    CXX_EXTENSIONS                  OFF
    CMAKE_POSITION_INDEPENDENT_CODE ON
)

target_include_directories( vulkan 
    PUBLIC
        $<BUILD_INTERFACE:${INC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $<BUILD_INTERFACE:${SRC_DIR}>
)

target_compile_definitions( vulkan PUBLIC
    $<$<BOOL:${WIN32}>:VK_USE_PLATFORM_WIN32_KHR>
    $<$<BOOL:${LINUX}>:VK_USE_PLATFORM_XCB_KHR >
)

target_link_libraries( vulkan 
    PRIVATE
        base
        Vulkan::Vulkan
    $<$<BOOL:${LINUX}>:xcb>
)