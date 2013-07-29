project(FxLibGL)

include_directories( 
"../include"
"../FxLib"
"${GLEW_INCLUDE_DIR}")

if(CUDA_INCLUDE_DIRS)
  include_directories(
    "${CUDA_INCLUDE_DIRS}"
    "../FxLibCUDA")
endif()

set(PUBLIC_HEADERS
  ../include/FxLib.h
  ../include/FxLibEx.h
  ../include/FxParser.h
)

set(HEADERS
  ../FxLib/Fx.h
  FxGL.h
  OpenGL.h
)

#let cmake determine linker language
set(SOURCE_FILES
  CstBufferGL.cpp
  FxLibGL.cpp
  GLSLProgram.cpp
  GLSLShader.cpp
  PathRendering.cpp
  resourcesGL.cpp
  SamplerStateGL.cpp
  StateGroupGL.cpp
  UniformGL.cpp
)
#####################################################################################
# Glew : source or lib
message(STATUS "using GLEW library")
LIST(APPEND PLATFORM_LIBRARIES ${GLEW_LIBRARY})
#add_definitions(-DGLEW_STATIC)
add_definitions(-DGLEW_NO_GLU)

source_group(sources FILES ${SOURCE_FILES})
source_group("" FILES ${PUBLIC_HEADERS})

if(USE_DLLSO)
  #####################################################################################
  # build as a so/dll
  set(NVFX_LIBS_GL FxLib)
  # cuda ?
  if(USE_CUDA AND CUDA_FOUND)
    add_subdirectory( FxLibCUDA )
    list(APPEND NVFX_LIBS_GL FxLibCUDA)
  endif()
  # for now, Optix makes sense only for OpenGL, too
  if(USE_OPTIX AND OPTIX_FOUND)
    add_subdirectory( FxLibOPTIX )
    list(APPEND NVFX_LIBS_GL FxLibOPTIX)
  endif()
  set(PLATFORM_LIBRARIES
      ${NVFX_LIBS_GL}
      ${OPENGL_LIBRARY}
      ${GLEW_LIBRARY}
      ${ZLIB_LIBRARIES}
      ${CUDA_LIBRARIES}
  )
  # Glew : source or lib
  if(GLEW_SOURCE)
    message(STATUS "found Glew source code. Using it instead of library")
    LIST(APPEND SOURCE_FILES ${GLEW_SOURCE})
    add_definitions(-DGLEW_STATIC)
  else()
      message(STATUS "using GLEW library")
      LIST(APPEND PLATFORM_LIBRARIES ${GLEW_LIBRARY})
  endif()
  # GLUT
  if(USE_GLUT AND GLUT_FOUND)
    LIST(APPEND PLATFORM_LIBRARIES ${GLUT_LIBRARIES} )
    include_directories(${GLUT_INCLUDE_DIR})
  endif()
  add_definitions(-DGLEW_NO_GLU)
  add_library(FxLibGL SHARED
  ${PUBLIC_HEADERS}
  ${HEADERS}
  ${SOURCE_FILES}
  )
  target_link_libraries(FxLibGL
      ${PLATFORM_LIBRARIES}
  )
else()
  #####################################################################################
  # build a static library
  add_library(FxLibGL STATIC
    ${PUBLIC_HEADERS}
    ${HEADERS}
    ${SOURCE_FILES}
  )
  #####################################################################################
endif()

if (WIN32)
    add_definitions(
        # Link against the static version of GLEW.
        -DGLEW_STATIC
    )
endif()


#linkage dependencies would only be for a SHARED...
#target_link_libraries( nvFx )

# http://cmake.org/cmake/help/v2.8.8/cmake.html#command:set_property
# http://cmake.org/cmake/help/v2.8.8/cmake.html#prop_tgt:IMPORTED_LINK_INTERFACE_LIBRARIES
#set_property(TARGET nvFx PROPERTY LINK_INTERFACE_LIBRARIES "")

# http://cmake.org/cmake/help/v2.8.8/cmake.html#command:set_target_properties
# http://cmake.org/cmake/help/v2.8.8/cmake.html#prop_tgt:FOLDER
set_target_properties( FxLibGL PROPERTIES FOLDER "FxLibGL" )

##################################################################
# INSTALL
if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL )
  if(NOT FOR_DS)
    install(TARGETS FxLibGL
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
  endif()
endif()
