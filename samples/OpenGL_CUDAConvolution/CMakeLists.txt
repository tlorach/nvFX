Project(OpenGL_CUDAConvolution)

set(PLATFORM_LIBRARIES
    ${NVFX_LIBS}
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
    ${ZLIB_LIBRARIES}
    ${CUDA_LIB}
    ${NSIGHT_LIB}
)
include_directories(
    ${nvFx_SOURCE_DIR}/include
    ${GLEW_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
    ${SHARED_DIR}
    ${NV_MATH_DIR}
    ${BK3D_DIR}
    ${NV_IMAGE_DIR}
    ${CUDA_INCLUDE_DIR}
)
set(SOURCE_FILES 
  OpenGL_CUDAConvolution.cpp
  ${SHARED_DIR}/main.cpp
  OpenGL_CUDAConvolution.glslfx
  materialCarPaint.glslfx
  shared_globals.glslfxh
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
  ${NV_MATH_SOURCE}
  ${NV_MATH_HEADERS}
)
source_group(nvImage FILES
  ${NV_IMAGE_SOURCES}
  ${NV_IMAGE_HEADERS}
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
#    _add_glut_executable(OpenGL_CUDAConvolution OpenGL_CUDAConvolution.cpp)
#    add_definitions(-DAPPLE)
#else()
#_add_glut_executable(OpenGL_CUDAConvolution
#    OpenGL_CUDAConvolution.cpp
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
    OUTPUT OpenGL_CUDAConvolution_Log.txt
    COMMAND ${NVFX_BINARY_PATH}/nvFxcc
            -oOpenGL_CUDAConvolution_Log.txt
            ${OpenGL_CUDAConvolution_SOURCE_DIR}/OpenGL_CUDAConvolution.glslfx
    MAIN_DEPENDENCY OpenGL_CUDAConvolution.glslfx
    DEPENDS ${NVFX_BINARY_PATH}/nvFxcc
    WORKING_DIRECTORY ${OpenGL_CUDAConvolution_SOURCE_DIR}
    )
  add_custom_command(
    OUTPUT materialCarPaint_Log.txt
    COMMAND ${NVFX_BINARY_PATH}/nvFxcc
            -omaterialCarPaint_Log.txt
            ${OpenGL_CUDAConvolution_SOURCE_DIR}/materialCarPaint.glslfx
    MAIN_DEPENDENCY materialCarPaint.glslfx
    DEPENDS ${NVFX_BINARY_PATH}/nvFxcc
    WORKING_DIRECTORY ${OpenGL_CUDAConvolution_SOURCE_DIR}
    )
  add_custom_target(EFFECTS_OpenGL_CUDAConvolution ALL DEPENDS OpenGL_CUDAConvolution_Log.txt materialCarPaint_Log.txt)
endif()

set(SRC_FOR_EXCUTABLE
  ${SOURCE_FILES}
  ${PUBLIC_HEADERS}
  ${SOURCES}
  ${NV_MATH_SOURCE}
  ${NV_MATH_HEADERS}
  ${NV_IMAGE_SOURCES}
  ${NV_IMAGE_HEADERS}
  ${BK3D_HEADERS}
  ${WIN_SRC}
  ${SHARED_DIR}/Camera.h
  ${SHARED_DIR}/Realtime.h
)

if(WIN32)
  add_executable(OpenGL_CUDAConvolution WIN32 ${SRC_FOR_EXCUTABLE} )
  if((NOT USE_GLUT) OR (NOT GLUT_FOUND))
    # strange: not working... but WIN32 above solve the issue
    #set_target_properties(OpenGL_CUDAConvolution PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    #set_target_properties(OpenGL_CUDAConvolution PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    add_definitions(-DNOGLUT)
  endif()
else()
  add_executable(OpenGL_CUDAConvolution ${SRC_FOR_EXCUTABLE} )
endif()
target_link_libraries(OpenGL_CUDAConvolution
    ${PLATFORM_LIBRARIES}
)

# if we wanted to have the effect close to the exe
# but let's use the effect from the resource directory instead : 
# the sample will pull the effect from RESOURCE_DIR
#_copy_files_to_target(OpenGL_CUDAConvolution ${PROJECT_SOURCE_DIR}/effect.glslfx)

if(USE_SVCUI AND SVCMFCUI_FOUND)
  # Copy the resources where the executable file will be created
  _copy_files_to_target( OpenGL_CUDAConvolution "${SVCMFCUI_DLL}")
endif()
if(ZLIB_FOUND)
  _copy_files_to_target( OpenGL_CUDAConvolution "${ZLIB_BIN}")
endif()
if(NSIGHT_DLL)
  _copy_files_to_target( OpenGL_CUDAConvolution "${NSIGHT_DLL}")
endif()
#_copy_files_to_target( simpleOpenGL "${RESOURCE_DIR}/NV_Shaderball_v133.bk3d.gz")
add_definitions(-DRESOURCE_DIR="${RESOURCE_DIR}" -DSOURCE_DIR="${OpenGL_CUDAConvolution_SOURCE_DIR}")
