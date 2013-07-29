project(FxLibD3D)

include_directories( 
"../include"
"../FxLib"
"${DXSDK_INCLUDE_DIR}")

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
  FxD3D.h
)

#let cmake determine linker language
set(SOURCES
  CstBufferD3D.cpp
  D3DShader.cpp
  FxLibD3D.cpp
  Program.cpp
  resourcesD3D.cpp
  SamplerStateD3D.cpp
  StateGroupD3D.cpp
  UniformD3D.cpp
)

source_group(sources FILES ${SOURCES})
source_group("" FILES ${PUBLIC_HEADERS})

if(USE_DLLSO)
  #####################################################################################
  # build as a so/dll
  set(PLATFORM_LIBRARIES
      FxLib
      ${OPENGL_LIBRARY}
      ${GLEW_LIBRARY}
      ${ZLIB_LIBRARIES}
      ${CUDA_LIBRARIES}
  )
  # build as a so/dll
  add_library(FxLibD3D SHARED
    ${PUBLIC_HEADERS}
    ${HEADERS}
    ${SOURCES}
  )
  target_link_libraries(FxLibD3D
      ${PLATFORM_LIBRARIES}
  )
else()
  # build a static library
  add_library(FxLibD3D STATIC
    ${PUBLIC_HEADERS}
    ${HEADERS}
    ${SOURCES}
  )
endif()
#if (WIN32)
#    add_definitions(    )
#endif()


#linkage dependencies would only be for a SHARED...
#target_link_libraries( nvFx )

# http://cmake.org/cmake/help/v2.8.8/cmake.html#command:set_property
# http://cmake.org/cmake/help/v2.8.8/cmake.html#prop_tgt:IMPORTED_LINK_INTERFACE_LIBRARIES
#set_property(TARGET nvFx PROPERTY LINK_INTERFACE_LIBRARIES "")

# http://cmake.org/cmake/help/v2.8.8/cmake.html#command:set_target_properties
# http://cmake.org/cmake/help/v2.8.8/cmake.html#prop_tgt:FOLDER
set_target_properties( FxLibD3D PROPERTIES FOLDER "FxLibD3D" )

##################################################################
# INSTALL
if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL )
  if(NOT FOR_DS)
    install(TARGETS FxLibD3D
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
  endif()
endif()
