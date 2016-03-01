Project(D3DSimple)

SET(SHADER_DIR_LOCAL "${D3DSimple_SOURCE_DIR}/shared")

## Remove d3dx11 and dxguid libraries
set(PLATFORM_LIBRARIES
    ${NVFX_BASE_LIBS}
    ${NVFX_D3D_LIB}
    ${NSIGHT_LIB}
    ${CUDA_LIB}
	${DXSDK_d3d11_LIBRARY}
	${DXSDK_d3dcompiler_LIBRARY}
	${DXSDK_d3dx11_LIBRARY}
	${DXSDK_dxguid_LIBRARY}
)
include_directories(
    ${DXSDK_INCLUDE_DIR}
    ${nvFx_SOURCE_DIR}/include
    ${GLEW_INCLUDE_DIR}
    ${SHADER_DIR_LOCAL}
)
set(SOURCE_FILES 
  D3DSimple.cpp
  Process.cpp
  Process.h
  Simple.d3dfx
)
#####################################################################################
# UI
if(SVCMFCUI_FOUND AND USE_SVCUI)
  include_directories(${SVCMFCUI_INCLUDE_DIR})
  LIST(APPEND SOURCE_FILES ${SVCMFCUI_HEADERS})
endif()

if(NVFXCC_ON_EFFECTS)
  add_custom_command(
    OUTPUT D3DSimple_Log.txt
    COMMAND ${NVFX_BINARY_PATH}/nvFxcc
            -oD3DSimple_Log.txt
            ${D3DSimple_SOURCE_DIR}/Simple.d3dfx
    DEPENDS ${NVFX_BINARY_PATH}/nvFxcc Simple.d3dfx
    WORKING_DIRECTORY ${D3DSimple_SOURCE_DIR}
    )
  add_custom_target(EFFECTS_D3DSimple ALL DEPENDS D3DSimple_Log.txt)
endif()

set(SRC_FOR_EXCUTABLE
  ${SOURCE_FILES}
  ${PUBLIC_HEADERS}
  ${SOURCES}
  ${WIN_SRC}
)

if(WIN32)
  add_executable(D3DSimple WIN32 ${SRC_FOR_EXCUTABLE} )
  if((NOT USE_GLUT) OR (NOT GLUT_FOUND))
    # strange: not working... but WIN32 above solve the issue
    #set_target_properties(D3DSimple PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    #set_target_properties(D3DSimple PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    add_definitions(-DNOGLUT)
  endif()
else()
  add_executable(D3DSimple ${SRC_FOR_EXCUTABLE} )
endif()
#list(APPEND PLATFORM_LIBRARIES ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libFxLib.a)
target_link_libraries(D3DSimple
    ${PLATFORM_LIBRARIES}
)

# if we wanted to have the effect close to the exe
# but let's use the effect from the resource directory instead : 
# the sample will pull the effect from RESOURCE_DIR
#_copy_files_to_target(D3DSimple ${PROJECT_SOURCE_DIR}/effect.glslfx)

if(USE_SVCUI AND SVCMFCUI_FOUND)
  # Copy the resources where the executable file will be created
  _copy_files_to_target( D3DSimple "${SVCMFCUI_DLL}")
endif()
if(NSIGHT_DLL)
  _copy_files_to_target( D3DSimple "${NSIGHT_DLL}")
endif()
#_copy_files_to_target( simpleOpenGL "${RESOURCE_DIR}/NV_Shaderball_v133.bk3d.gz")
add_definitions(-DRESOURCE_DIR="${RESOURCE_DIR}" -DSOURCE_DIR="${D3DSimple_SOURCE_DIR}")
