project(FxParser)

include_directories( 
"."
"../include"
"../FxLib")

set(PUBLIC_HEADERS
  ../include/FxLib.h
  ../include/FxLibEx.h
  ../include/FxParser.h
)

set(HEADERS
  ../FxLib/Fx.h
  Grammar.hpp
)

source_group(sources FILES ${SOURCES})
source_group("" FILES ${PUBLIC_HEADERS})

if(BISON_FOUND)
  BISON_TARGET(Grammar Grammar.ypp ${FxParser_SOURCE_DIR}/Grammar.cpp )
endif()
if(FLEX_FOUND)
  FLEX_TARGET(lex lex.yy ${FxParser_SOURCE_DIR}/lex.cpp)
endif()
if(FLEX_FOUND AND BISON_FOUND)
  ADD_FLEX_BISON_DEPENDENCY(lex Grammar)
endif()

#let cmake determine linker language
set(SOURCES
  FxParser.cpp
  Grammar.ypp
  lex.yy
  Grammar.cpp
  lex.cpp)

if(USE_DLLSO)
  #####################################################################################
  ############################## build as a so/dll
  #set(NVFX_LIBS_PARSER FxLib)
  # add this library to the list
  if(USE_OPENGL)
    message(STATUS "Parser .dll or .so prepared for OpenGL")
    list(APPEND NVFX_LIBS_PARSER FxLibGL)
    # For now, CUDA makes sense only for OpenGL...
    if(USE_CUDA AND CUDA_FOUND)
      message(STATUS "Parser .dll or .so prepared for CUDA")
      add_subdirectory( FxLibCUDA )
      list(APPEND NVFX_LIBS_PARSER FxLibCUDA)
    endif()
    # for now, Optix makes sense only for OpenGL, too
    if(USE_OPTIX AND OPTIX_FOUND)
      message(STATUS "Parser .dll or .so prepared for OptiX")
      add_subdirectory( FxLibOPTIX )
      list(APPEND NVFX_LIBS_PARSER FxLibOPTIX)
    endif()
  #endif()
  elseif(USE_D3D AND WIN32)
    message(STATUS "Parser .dll or .so prepared for D3D only")
    add_subdirectory( FxLibD3D )
    list(APPEND NVFX_LIBS_PARSER FxLibD3D)
  endif()
  set(PLATFORM_LIBRARIES
      ${NVFX_LIBS_PARSER}
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

  add_library(FxParser SHARED
    ${PUBLIC_HEADERS}
    ${HEADERS}
    ${SOURCES}
  )
  target_link_libraries(FxParser
      ${PLATFORM_LIBRARIES}
  )
else()
  #####################################################################################
  ################################# build a static library
  add_library(FxParser STATIC
    ${PUBLIC_HEADERS}
    ${HEADERS}
    ${SOURCES})
endif()

add_definitions(-DYY_NEVER_INTERACTIVE)
set_target_properties( FxParser PROPERTIES FOLDER "FxParser" )

##################################################################
# INSTALL
if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL )
  if(NOT FOR_DS)
    install(TARGETS FxParser
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
  endif()
endif()
