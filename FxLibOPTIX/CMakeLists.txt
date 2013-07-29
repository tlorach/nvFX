project(FxLibOPTIX)

include_directories( 
"../include"
"../FxLib"
"../FxLibGL"
"${GLEW_INCLUDE_DIR}")

if(OPTIX_INCLUDE_DIRS)
  include_directories(
    "${OPTIX_INCLUDE_DIRS}"
    "../FxLibOPTIX")
endif()

set(PUBLIC_HEADERS
  ../include/FxLib.h
  ../include/FxLibEx.h
  ../include/FxParser.h
)

set(HEADERS
  ../FxLib/Fx.h
  FxOPTIX.h
)

#let cmake determine linker language
set(SOURCES
OPTIXProgram.cpp
OPTIXContext.cpp
FxLibOPTIX.cpp
ResourcesOPTIX.cpp
UniformOPTIX.cpp
)

source_group(sources FILES ${SOURCES})
source_group("" FILES ${PUBLIC_HEADERS})

# build a static library
# OPTIX is built as a static library and then linked to the exe files OR to the GL/D3D Fx dll/so
# TODO: in the case where none of GL or D3D are used, the effect is considered as OPTIX only
# This is an way to use nvFx only for OptiX that we should make possible... later
add_library(FxLibOPTIX STATIC
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
set_target_properties( FxLibOPTIX PROPERTIES FOLDER "FxLibOPTIX" )

##################################################################
# INSTALL
if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL )
  if(NOT FOR_DS)
    install(TARGETS FxLibOPTIX
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
  endif()
endif()
