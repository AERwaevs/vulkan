function(compile_shaders TARGET SHADER_DIR)
    find_program(SHADER_COMPILER glslc)
    if(NOT SHADER_COMPILER)
        message(FATAL_ERROR "glslc not found. Please install the Vulkan SDK.")
    endif()

    add_custom_command(
        TARGET ${TARGET}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${TARGET}>/shaders/
    )

    file(GLOB_RECURSE SHADER_CONF ${SHADER_DIR}/*.conf)      # config file that replaces the default configuration
    file(GLOB_RECURSE SHADER_VERT ${SHADER_DIR}/*.vert)      # vertex shader
    file(GLOB_RECURSE SHADER_TESC ${SHADER_DIR}/*.tesc)      # tessellation control shader
    file(GLOB_RECURSE SHADER_TESE ${SHADER_DIR}/*.tese)      # tessellation evaluation shader
    file(GLOB_RECURSE SHADER_GEOM ${SHADER_DIR}/*.geom)      # geometry shader
    file(GLOB_RECURSE SHADER_FRAG ${SHADER_DIR}/*.frag)      # fragment shader
    file(GLOB_RECURSE SHADER_COMP ${SHADER_DIR}/*.comp)      # compute shader
    file(GLOB_RECURSE SHADER_MESH ${SHADER_DIR}/*.mesh)      # mesh shader
    file(GLOB_RECURSE SHADER_TASK ${SHADER_DIR}/*.task)      # task shader
    file(GLOB_RECURSE SHADER_RGEN ${SHADER_DIR}/*.rgen)      # ray generation shader
    file(GLOB_RECURSE SHADER_RINT ${SHADER_DIR}/*.rint)      # ray intersection shader
    file(GLOB_RECURSE SHADER_RAHT ${SHADER_DIR}/*.rahit)     # ray any hit shader
    file(GLOB_RECURSE SHADER_RCHT ${SHADER_DIR}/*.rchit)     # ray closest hit shader
    file(GLOB_RECURSE SHADER_RMSS ${SHADER_DIR}/*.rmiss)     # ray miss shader
    file(GLOB_RECURSE SHADER_RCLL ${SHADER_DIR}/*.rcall)     # ray callable shader
    file(GLOB_RECURSE SHADER_GLSL ${SHADER_DIR}/*.glsl)      # for .vert.glsl, .tesc.glsl, ..., .comp.glsl compound suffixes
    file(GLOB_RECURSE SHADER_HLSL ${SHADER_DIR}/*.hlsl)      # for .hlsl or related compound suffixes

    set(SHADER_SRC 
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

    foreach(SHADER ${SHADER_SRC})
        string(CONCAT SHADER_SPV "${SHADER}" ".spv")
        add_custom_command(
            TARGET ${TARGET}
            POST_BUILD
            COMMAND ${SHADER_COMPILER} ${SHADER} -o ${SHADER_SPV} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy ${SHADER_SPV} $<TARGET_FILE_DIR:${TARGET}>/shaders/
            COMMAND ${CMAKE_COMMAND} -E remove ${SHADER_SPV}
        )
    endforeach()
endfunction()