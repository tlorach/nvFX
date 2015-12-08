#

# Try to find OPTIX library and include path.
# Once done this will define
#
# OPTIX_FOUND
# OPTIX_INCLUDE_DIR
# OPTIX_LIBRARY
# OPTIX_SOURCE
#

include(FindPackageHandleStandardArgs)
if (WIN32)
    find_path( OPTIX_INCLUDE_DIR
        NAMES
            optix.h
        PATHS
            ${OPTIX_PATH}/include
            $ENV{OPTIX_PATH}/include
            ${OptiX_INSTALL_DIR}/include
            $ENV{OptiX_INSTALL_DIR}/include
            ${PROJECT_SOURCE_DIR}/extern/optix/include
            ${OPTIX_PATH}
            $ENV{OPTIX_PATH}
            DOC "The directory where optix.h resides" )
    if(ARCH STREQUAL "x86")
      find_library( OPTIX_LIBRARY
          NAMES
              optix.1
          PATHS
              ${OPTIX_PATH}/lib
              ${OPTIX_PATH}/lib/x86
              ${OPTIX_PATH}/lib/win32
              $ENV{OPTIX_PATH}/lib
              ${PROJECT_SOURCE_DIR}/extern/optix/lib
              ${PROJECT_SOURCE_DIR}/extern/optix/lib/x86
              ${PROJECT_SOURCE_DIR}/extern/optix/lib/win32
              ${OPTIX_PATH}
              $ENV{OPTIX_PATH}
              DOC "The OPTIX library")
    else()
      find_library( OPTIX_LIBRARY
          NAMES
              optix.1
          PATHS
              ${OPTIX_PATH}/lib64
              $ENV{OPTIX_PATH}/lib64
              ${OptiX_INSTALL_DIR}/lib64
              $ENV{OptiX_INSTALL_DIR}/lib64
              ${PROJECT_SOURCE_DIR}/extern/optix/lib64
              ${PROJECT_SOURCE_DIR}/extern/optix/lib/x64
              DOC "The OPTIX library")
    endif()
endif ()

if (${CMAKE_HOST_UNIX})
    find_path( OPTIX_INCLUDE_DIR
        NAMES
            optix.h
        PATHS
            ${OPTIX_PATH}/include
            $ENV{OPTIX_PATH}/include
            ${OptiX_INSTALL_DIR}/include
            $ENV{OptiX_INSTALL_DIR}/include
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
            NO_DEFAULT_PATH
            DOC "The directory where optix.h resides"
    )
    find_library( OPTIX_LIBRARY
        NAMES
            optix.1
        PATHS
            ${OPTIX_PATH}/lib
            $ENV{OPTIX_PATH}/lib
            ${OptiX_INSTALL_DIR}/lib
            $ENV{OptiX_INSTALL_DIR}/lib
            /usr/lib64
            /usr/lib
            /usr/local/lib64
            /usr/local/lib
            /sw/lib
            /opt/local/lib
            NO_DEFAULT_PATH
            DOC "The optix library")
endif ()

find_package_handle_standard_args(OPTIX DEFAULT_MSG
    OPTIX_INCLUDE_DIR
    OPTIX_LIBRARY
)

mark_as_advanced( OPTIX_FOUND )
