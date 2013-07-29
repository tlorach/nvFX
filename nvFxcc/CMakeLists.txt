project(nvFxcc)

set(PLATFORM_LIBRARIES
    ${NVFX_LIBS}
    ${OPENGL_LIBRARY}
    ${CUDA_LIBRARIES}
)


include_directories(
  "${nvFx_SOURCE_DIR}/include"
    ${GLEW_INCLUDE_DIR}
)

set(PUBLIC_HEADERS
  ${nvFx_SOURCE_DIR}/include/FxLib.h
  ${nvFx_SOURCE_DIR}/include/FxLibEx.h
  ${nvFx_SOURCE_DIR}/include/FxParser.h
)

set(HEADERS
  stdafx.h
)

#let cmake determine linker language
set(SOURCE_FILES
  nvFXcc.cpp
)
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

source_group(sources FILES ${SOURCE_FILES})
source_group("" FILES ${PUBLIC_HEADERS})

# build a static library
add_executable(nvFxcc
  ${PUBLIC_HEADERS}
  ${HEADERS}
  ${SOURCE_FILES}
)
#linkage dependencies would only be for a SHARED...
target_link_libraries( nvFxcc ${PLATFORM_LIBRARIES} )

# http://cmake.org/cmake/help/v2.8.8/cmake.html#command:set_property
# http://cmake.org/cmake/help/v2.8.8/cmake.html#prop_tgt:IMPORTED_LINK_INTERFACE_LIBRARIES
#set_property(TARGET nvFx PROPERTY LINK_INTERFACE_LIBRARIES "")

# http://cmake.org/cmake/help/v2.8.8/cmake.html#command:set_target_properties
# http://cmake.org/cmake/help/v2.8.8/cmake.html#prop_tgt:FOLDER
set_target_properties( nvFxcc PROPERTIES FOLDER "nvFxcc" )
