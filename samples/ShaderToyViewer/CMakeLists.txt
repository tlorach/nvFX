Project(ShaderToyViewer)

SET(SHADER_DIR_LOCAL "${ShaderToyViewer_SOURCE_DIR}/shared")
SET(NV_MATH_DIR_LOCAL "${ShaderToyViewer_SOURCE_DIR}/nv_math")
set(NV_MATH_HEADERS_LOCAL "${NV_MATH_DIR_LOCAL}/nv_math.h" "${NV_MATH_DIR}/nv_algebra.h" "${NV_MATH_DIR}/nv_mathdecl.h")
set(NV_MATH_SOURCE_LOCAL "${NV_MATH_DIR_LOCAL}/nv_math.cpp")

set(PLATFORM_LIBRARIES
    ${NVFX_LIBS}
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
    ${NSIGHT_LIB}
    ${CUDA_LIB}
)
include_directories(
    ${nvFx_SOURCE_DIR}/include
    ${GLEW_INCLUDE_DIR}
    ${SHADER_DIR_LOCAL}
    ${NV_MATH_DIR_LOCAL}
)
set(SOURCE_FILES 
  ShaderToyViewer.cpp
  ${SHADER_DIR_LOCAL}/main_win32.cpp
  ${SHARED_DIR}/nv_dds/nv_dds.h
  ${SHARED_DIR}/nv_dds/nv_dds.cpp
  ShaderToyEffect.glslfx
)
#####################################################################################
# UI
if(SVCMFCUI_FOUND AND USE_SVCUI)
  include_directories(${SVCMFCUI_INCLUDE_DIR})
  LIST(APPEND SOURCE_FILES ${SVCMFCUI_HEADERS})
endif()
#####################################################################################
# Glew : source or lib
if(GLEW_SOURCE)
  message(STATUS "found Glew source code. Using it instead of library")
  LIST(APPEND SOURCE_FILES ${GLEW_SOURCE})
  add_definitions(-DGLEW_STATIC)
else()
    message(STATUS "using GLEW library")
    LIST(APPEND PLATFORM_LIBRARIES ${GLEW_LIBRARY})
endif()
add_definitions(-DGLEW_NO_GLU)
#####################################################################################
# GLUT
if(USE_GLUT AND GLUT_FOUND)
  LIST(APPEND PLATFORM_LIBRARIES ${GLUT_LIBRARIES} )
  include_directories(${GLUT_INCLUDE_DIR})
endif()
set(PUBLIC_HEADERS
  ${nvFx_SOURCE_DIR}/include/FxLib.h
  ${nvFx_SOURCE_DIR}/include/FxLibEx.h
  ${nvFx_SOURCE_DIR}/include/FxParser.h
)
source_group(nvFx FILES
  ${PUBLIC_HEADERS}
)
source_group(nv_math FILES
  ${NV_MATH_SOURCE_LOCAL}
  ${NV_MATH_HEADERS_LOCAL}
)
if(SVCMFCUI_FOUND AND USE_SVCUI)
  source_group(SvcUI FILES 
    ${SVCMFCUI_HEADERS}
  )
endif()
source_group(bk3d_headers FILES
  ${BK3D_HEADERS}
)

#if(APPLE)
#    _add_glut_executable(ShaderToyViewer ShaderToyViewer.cpp)
#    add_definitions(-DAPPLE)
#else()
#_add_glut_executable(ShaderToyViewer
#    ShaderToyViewer.cpp
#    ${INC_FILES}
#)
#endif()

if(NOT USE_GLUT)
  set(WIN_SRC
    ${SHARED_DIR}/resources.h
    ${SHARED_DIR}/resources.rc
  )
endif()

if(NVFXCC_ON_EFFECTS)
  add_custom_command(
    OUTPUT ShaderToyEffect_Log.txt
    COMMAND ${NVFX_BINARY_PATH}/nvFxcc
            -oShaderToyEffect_Log.txt
            ${ShaderToyViewer_SOURCE_DIR}/ShaderToyEffect.glslfx
    DEPENDS ${NVFX_BINARY_PATH}/nvFxcc ShaderToyEffect.glslfx
    WORKING_DIRECTORY ${ShaderToyViewer_SOURCE_DIR}
    )
  add_custom_target(EFFECTS_ShaderToyEffect ALL DEPENDS ShaderToyEffect_Log.txt)
endif()

set(SRC_FOR_EXCUTABLE
  ${SOURCE_FILES}
  ${PUBLIC_HEADERS}
  ${SOURCES}
  ${NV_MATH_SOURCE_LOCAL}
  ${NV_MATH_HEADERS_LOCAL}
  ${BK3D_HEADERS}
  ${WIN_SRC}
  ${SHADER_DIR_LOCAL}/InertiaCamera.h
  ${SHADER_DIR_LOCAL}/TimeSampler.h
)

if(WIN32)
  add_executable(ShaderToyViewer WIN32 ${SRC_FOR_EXCUTABLE} )
  if((NOT USE_GLUT) OR (NOT GLUT_FOUND))
    # strange: not working... but WIN32 above solve the issue
    #set_target_properties(ShaderToyViewer PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    #set_target_properties(ShaderToyViewer PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    add_definitions(-DNOGLUT)
  endif()
else()
  add_executable(ShaderToyViewer ${SRC_FOR_EXCUTABLE} )
endif()
#list(APPEND PLATFORM_LIBRARIES ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libFxLib.a)
target_link_libraries(ShaderToyViewer
    ${PLATFORM_LIBRARIES}
)

# if we wanted to have the effect close to the exe
# but let's use the effect from the resource directory instead : 
# the sample will pull the effect from RESOURCE_DIR
#_copy_files_to_target(ShaderToyViewer ${PROJECT_SOURCE_DIR}/effect.glslfx)

if(USE_SVCUI AND SVCMFCUI_FOUND)
  # Copy the resources where the executable file will be created
  _copy_files_to_target( ShaderToyViewer "${SVCMFCUI_DLL}")
endif()
if(NSIGHT_DLL)
  _copy_files_to_target( ShaderToyViewer "${NSIGHT_DLL}")
endif()
#_copy_files_to_target( simpleOpenGL "${RESOURCE_DIR}/NV_Shaderball_v133.bk3d.gz")
add_definitions(-DRESOURCE_DIR="${RESOURCE_DIR}" -DSOURCE_DIR="${ShaderToyViewer_SOURCE_DIR}")
