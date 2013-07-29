project(FxLibCUDA)

include_directories( 
"../include"
"../FxLib"
"../FxLibGL"
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
  FxCUDA.h
)

#let cmake determine linker language
set(SOURCES
CUDAProgram.cpp
CUDAShader.cpp
FxLibCUDA.cpp
resourcesCUDA.cpp
UniformCUDA.cpp
)

source_group(sources FILES ${SOURCES})
source_group("" FILES ${PUBLIC_HEADERS})

# build a static library
# CUDA is built as a static library and then linked to the exe files OR to the GL/D3D Fx dll/so
# TODO: in the case where none of GL or D3D are used, the effect is considered as CUDA only
# This is an improbalble way to use nvFx but we should make it possible...
add_library(FxLibCUDA STATIC
  ${PUBLIC_HEADERS}
  ${HEADERS}
  ${SOURCES}
)
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
set_target_properties( FxLibCUDA PROPERTIES FOLDER "FxLibCUDA" )

##################################################################
# INSTALL
if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL )
  if(NOT FOR_DS)
    install(TARGETS FxLibCUDA
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
  endif()
endif()
