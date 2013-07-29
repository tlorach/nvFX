project(FxLib)

include_directories( "../include" )

set(PUBLIC_HEADERS
  ../include/FxLib.h
  ../include/FxLibEx.h
  ../include/FxParser.h
)

set(HEADERS
  Fx.h
  OpenGL_enums.h
)

#let cmake determine linker language
set(SOURCES
  Annotation.cpp
  Container.cpp
  CstBuffer.cpp
  FxLib.cpp
  Pass.cpp
  resources.cpp
  SamplerState.cpp
  Serialize.cpp
  StateGroup.cpp
  Technique.cpp
  Uniform.cpp
)

source_group(sources FILES ${SOURCES})
source_group("" FILES ${PUBLIC_HEADERS})

# build a static library
add_library(FxLib STATIC
  ${PUBLIC_HEADERS}
  ${HEADERS}
  ${SOURCES}
)
#linkage dependencies would only be for a SHARED...
#target_link_libraries( nvFx )

# http://cmake.org/cmake/help/v2.8.8/cmake.html#command:set_property
# http://cmake.org/cmake/help/v2.8.8/cmake.html#prop_tgt:IMPORTED_LINK_INTERFACE_LIBRARIES
#set_property(TARGET nvFx PROPERTY LINK_INTERFACE_LIBRARIES "")

# http://cmake.org/cmake/help/v2.8.8/cmake.html#command:set_target_properties
# http://cmake.org/cmake/help/v2.8.8/cmake.html#prop_tgt:FOLDER
set_target_properties( FxLib PROPERTIES FOLDER "FxLib" )

##################################################################
# INSTALL
if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL )
  if(NOT FOR_DS)
    install(TARGETS FxLib
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
  endif()
endif()
