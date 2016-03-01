cmake_minimum_required(VERSION 2.8)

PROJECT( nvFx )

set(VERSION "1.2.0")

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

if(WIN32)
  SET( USE_NSIGHT OFF CACHE BOOL "Use NSight" )
  SET( MEMORY_LEAKS_CHECK OFF CACHE BOOL "Build the project for Memory leaks check" )
endif(WIN32)
SET( NVFXVERBOSE OFF CACHE BOOL "allow the runtime to output lots of debug log messages" )
SET( BUILD_SAMPLES ON CACHE BOOL "Build the test samples" )
SET( USE_CUDA OFF CACHE BOOL "Use CUDA" )
SET( USE_OPTIX OFF CACHE BOOL "Use OPTIX" )
if(WIN32)
  SET( USE_D3D OFF CACHE BOOL "Use D3D" )
endif()
#SET( USE_DLLSO OFF CACHE BOOL "nvFx as .dll or .so" )
SET( USE_OPENGL ON CACHE BOOL "Use OpenGL" )
SET( USE_SVCUI ON CACHE BOOL "Use the optional UI" )
SET( USE_GLUT OFF CACHE BOOL "Use Glut or not(in the case of Windows)" )
SET( NVFXCC_ON_EFFECTS OFF CACHE BOOL "compile effects of samples with nvFxcc cmd-line during project builds" )
#SET( GLUT_LOCATION "" CACHE PATH "Path for Glut, if cmake didn't find it" )
#SET( GLEW_LOCATION "" CACHE PATH "Path for Glew, if cmake didn't find it" )

# Specify the list of directories to search for cmake modules.
set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

set( CMAKE_PROGRAM_PATH "$ENV{PATH}")

# Macro for adding files close to the executable
macro(_copy_files_to_target target thefiles)
    if(WIN32)
        foreach (FFF ${thefiles} )
      add_custom_command(
          TARGET ${target} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "${FFF}"
              $<TARGET_FILE_DIR:${target}>
              VERBATIM
      )
        endforeach()
    endif()

endmacro()

if ( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "armv7l")
  set(ARCH "${CMAKE_SYSTEM_PROCESSOR}" CACHE STRING "CPU Architecture" )
elseif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set (ARCH "x64" CACHE STRING "CPU Architecture")
  set (ARCHSUFFIX "64")
else ()
  set (ARCH "x86" CACHE STRING "CPU Architecture")
endif()

if(MSVC90)
  set (CRT "crt90")
  set ( COMPILER_ARCH "win32-msvc2008-${ARCH}" )
  #enable multiprocessor build on visual studio 2008
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP ${MSVC_CXX_FLAGS}")
elseif(MSVC10)
  set (CRT "crt100")
  set ( COMPILER_ARCH "win32-msvc2010-${ARCH}" )
  #enable multiprocessor build on visual studio 2010
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP ${MSVC_CXX_FLAGS}")
endif(MSVC90)
  
if(UNIX) 
  set(OS "linux")
  add_definitions(-DLINUX)
else(UNIX)
  if(APPLE)
  else(APPLE)
    if(WIN32)
      set(OS "win")
      add_definitions(-DNOMINMAX)
    endif(WIN32)
  endif(APPLE)
endif(UNIX)

# ===============> bison
find_package(BISON)
find_package(FLEX)
if((NOT BISON_FOUND) OR (NOT FLEX_FOUND))
    message(WARNING "
      No Bison and/or Flex available. FxParser will only use the already generated code.
      You can try to set BISON_EXECUTABLE & FLEX_EXECUTABLE"
    )
  set(BISON_EXECUTABLE "" CACHE PATHFILE "bison")
  set(FLEX_EXECUTABLE "" CACHE PATHFILE "flex")
  MARK_AS_ADVANCED(CLEAR BISON_EXECUTABLE)
  MARK_AS_ADVANCED(CLEAR FLEX_EXECUTABLE)
endif()
# ===============> D3D
if(USE_D3D AND (WIN32))
    find_package(DXSDK MODULE)
endif()
#=============> Only in OpenGL Case
if(USE_OPENGL)
  find_package(OpenGL)
  message(STATUS "OpenGL Libraries: ${OPENGL_LIBRARIES}")
  # ===============> GLUT
  if(USE_GLUT)
    find_package(GLUT)
    if(NOT GLUT_FOUND)
        message(WARNING "Try to set GLUT_LOCATION")
    endif()
  endif()
  # ===============> GLEW
  if (NOT APPLE)
      find_package(GLEW REQUIRED)
  endif()
  # ===============> OPTIX
  if(USE_OPTIX)
    find_package(OPTIX)
    if(OPTIX_FOUND)
        add_definitions(
            -DUSEOPTIX
        )
    else()
        message(WARNING "OPTIX was not found")
    endif()
  endif()
  # ===============> CUDA
  if(USE_CUDA)
    find_package(CUDA)
    MARK_AS_ADVANCED(CUDA_SDK_ROOT_DIR)
    if(CUDA_FOUND)
        add_definitions(
            -DUSECUDA
        )
    else()
        message(WARNING "CUDA was not found")
    endif()
  endif()
endif() # OpenGL
# ===============> Memory leak check: define
if( MEMORY_LEAKS_CHECK)
  add_definitions(
      -DMEMORY_LEAKS_CHECK
  )
endif()
# ===============> Other general options
if( NVFXVERBOSE)
  add_definitions(
      -DNVFXVERBOSE
  )
endif()

# ===============
if ("${CMAKE_GENERATOR}" MATCHES "^(Visual Studio).*")
  set (NVFX_CONFIG "$(ConfigurationName)")
else()
  if( "${CMAKE_BUILD_TYPE}" STREQUAL "" )
    set (NVFX_CONFIG "release")
  endif()
endif()

#set binary directory
if(WIN32 AND "${CMAKE_GENERATOR}" MATCHES "^(Visual Studio).*")
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" )
  else()
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/${NVFX_CONFIG}" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/${NVFX_CONFIG}" )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/${NVFX_CONFIG}" )
endif()

set(NVFX_BINARY_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${NVFX_CONFIG}" CACHE STRING "Devtech Platform binary path")

#####################################################################################
# NSIGHT
#
if(USE_NSIGHT)
  add_definitions(-DUSE_NSIGHT)
  include_directories(NSight)
  if( ARCH STREQUAL "x64" )
    set(NSIGHT_LIB "${PROJECT_SOURCE_DIR}/NSight/nvToolsExt64_1.lib")
    set(NSIGHT_DLL "${PROJECT_SOURCE_DIR}/NSight/nvToolsExt64_1.dll")
  else()
    set(NSIGHT_LIB "${PROJECT_SOURCE_DIR}/NSight/nvToolsExt32_1.lib")
    set(NSIGHT_DLL "${PROJECT_SOURCE_DIR}/NSight/nvToolsExt32_1.dll")
  endif()
  #MARK_AS_ADVANCED(NSIGHT_LIB)
else()
  set(NSIGHT_LIB, "")
endif()

#include_directories(${NVFX_HOME})
set(NVFX_LIBS FxLib)
set(NVFX_BASE_LIBS FxLib)

add_subdirectory( FxLib )
# add this library to the list
if(USE_OPENGL)
  add_subdirectory( FxLibGL )
  list(APPEND NVFX_LIBS FxLibGL)
  # For now, CUDA makes sense only for OpenGL...
  # later, we shall expose it to D3D
  # or even allow to use CUDA without any Gfx API...
  if(USE_CUDA AND CUDA_FOUND)
    add_subdirectory( FxLibCUDA )
    list(APPEND NVFX_LIBS FxLibCUDA) # FxLib refers to it when cmake allowed CUDA... so we need it for all :-(
  endif()
  # for now, Optix makes sense only for OpenGL, too
  if(USE_OPTIX AND OPTIX_FOUND)
    add_subdirectory( FxLibOPTIX )
    list(APPEND NVFX_LIBS FxLibOPTIX)
  endif()
endif()
if(USE_D3D AND WIN32)
  add_subdirectory( FxLibD3D )
  set(NVFX_D3D_LIB FxLibD3D)
  list(APPEND NVFX_LIBS FxLibD3D)
endif()
add_subdirectory( FxParser )
list(APPEND NVFX_LIBS FxParser)
list(APPEND NVFX_BASE_LIBS FxParser)
# for now, only available in Windows
if(WIN32)
  add_subdirectory( nvFxcc )
else()
  message(STATUS "cmd line compiler only available for Windows")
endif()

if(BUILD_SAMPLES)
  add_subdirectory( samples )
endif()

##################################################################
# INSTALL
if(NOT SKIP_INSTALL_HEADERS AND NOT SKIP_INSTALL_ALL )
  file(GLOB PUBLIC_HDRS "." "include/*.h")
  install(FILES ${PUBLIC_HDRS} DESTINATION "${INSTALL_INC_DIR}")
endif()

