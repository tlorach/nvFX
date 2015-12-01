Project(OpenGLUniformUpdateExample)

set(PLATFORM_LIBRARIES
    ${NVFX_LIBS}
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
    ${CUDA_LIB}
    ${NSIGHT_LIB}
)
include_directories(
    ${nvFx_SOURCE_DIR}/include
    ${GLEW_INCLUDE_DIR}
    ${SHARED_DIR}
    ${NV_MATH_DIR}
)
set(SOURCE_FILES 
  OpenGLUniformUpdateExample.cpp
  ${SHARED_DIR}/main.cpp
  OpenGLUniformUpdateExample.glslfx
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
else()
  set(WIN_SRC
    ${SHARED_DIR}/resources.h
    ${SHARED_DIR}/resources.rc
  )
endif()

#####################################################################################
# PUBLIC HEADERS
set(PUBLIC_HEADERS
  ${nvFx_SOURCE_DIR}/include/FxLib.h
  ${nvFx_SOURCE_DIR}/include/FxLibEx.h
  ${nvFx_SOURCE_DIR}/include/FxParser.h
)
#####################################################################################
# GROUPS
source_group(nvFx FILES
  ${PUBLIC_HEADERS}
)
source_group(nv_math FILES
  ${NV_MATH_SOURCE}
  ${NV_MATH_HEADERS}
)
if(SVCMFCUI_FOUND AND USE_SVCUI)
  source_group(SvcUI FILES 
    ${SVCMFCUI_HEADERS}
  )
endif()

#if(APPLE)
#    _add_glut_executable(OpenGLUniformUpdateExample OpenGLUniformUpdateExample.cpp)
#    add_definitions(-DAPPLE)
#else()
#_add_glut_executable(OpenGLUniformUpdateExample
#    OpenGLUniformUpdateExample.cpp
#    ${INC_FILES}
#)
#endif()

#####################################################################################
# If we want to compile effect prior to running the sample
if(NVFXCC_ON_EFFECTS)
  add_custom_command(
    OUTPUT OpenGLUniformUpdateExample_Log.txt
    COMMAND ${NVFX_BINARY_PATH}/nvFxcc
            -oOpenGLUniformUpdateExample_Log.txt
            ${OpenGLUniformUpdateExample_SOURCE_DIR}/OpenGLUniformUpdateExample.glslfx
    DEPENDS ${NVFX_BINARY_PATH}/nvFxcc OpenGLUniformUpdateExample.glslfx
    WORKING_DIRECTORY ${OpenGLUniformUpdateExample_SOURCE_DIR}
    )
  add_custom_target(EFFECTS_OpenGLUniformUpdateExample ALL DEPENDS OpenGLUniformUpdateExample_Log.txt)
endif()

#####################################################################################
# all the souce code for executable to be made
set(SRC_FOR_EXCUTABLE
  ${SOURCE_FILES}
  ${PUBLIC_HEADERS}
  ${SOURCES}
  ${NV_MATH_SOURCE}
  ${NV_MATH_HEADERS}
  ${WIN_SRC}
  ${SHARED_DIR}/Camera.h
  ${SHARED_DIR}/Realtime.h
)

if(WIN32)
  add_executable(OpenGLUniformUpdateExample WIN32 ${SRC_FOR_EXCUTABLE} )
  if((NOT USE_GLUT) OR (NOT GLUT_FOUND))
    # strange: not working... but WIN32 above solve the issue
    #set_target_properties(OpenGLUniformUpdateExample PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    #set_target_properties(OpenGLUniformUpdateExample PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    add_definitions(-DNOGLUT)
  endif()
else()
  add_executable(OpenGLUniformUpdateExample ${SRC_FOR_EXCUTABLE} )
endif()
target_link_libraries(OpenGLUniformUpdateExample
    ${PLATFORM_LIBRARIES}
)

#####################################################################################
# if we wanted to have the effect close to the exe
# but let's use the effect from the resource directory instead : 
# the sample will pull the effect from RESOURCE_DIR
#_copy_files_to_target(OpenGLUniformUpdateExample ${PROJECT_SOURCE_DIR}/effect.glslfx)

if(USE_SVCUI AND SVCMFCUI_FOUND)
  # Copy the resources where the executable file will be created
  _copy_files_to_target( OpenGLUniformUpdateExample "${SVCMFCUI_DLL}")
endif()
if(NSIGHT_DLL)
  _copy_files_to_target( OpenGLUniformUpdateExample "${NSIGHT_DLL}")
endif()
add_definitions(-DSOURCE_DIR="${OpenGLUniformUpdateExample_SOURCE_DIR}")
