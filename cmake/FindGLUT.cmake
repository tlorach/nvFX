# Try to find GLUT library and include path.
# Once done this will define
#
# GLUT_FOUND
# GLUT_INCLUDE_DIR
# GLUT_LIBRARIES
#

if (WIN32)
    if(CYGWIN)
        find_path( GLUT_INCLUDE_DIR GL/glut.h
          ${GLUT_LOCATION}/include
          $ENV{GLUT_LOCATION}/include
          /usr/include
        )
        if(ARCH STREQUAL "x86")
          find_library( GLUT_glut_LIBRARY glut32 freeglut_static freeglut
            ${GLUT_LOCATION}/lib
            ${GLUT_LOCATION}/lib/win32
            ${GLUT_LOCATION}/lib/x86
            $ENV{GLUT_LOCATION}/lib
            ${GLUT_LOCATION}/lib/glut
            $ENV{GLUT_LOCATION}/lib/glut
            $ENV{GLUT_LOCATION}/lib/win32
            $ENV{GLUT_LOCATION}/lib/x86
            ${OPENGL_LIBRARY_DIR}
            /usr/lib
            /usr/lib/w32api
            /usr/local/lib
            /usr/X11R6/lib
          )
        else()
          find_library( GLUT_glut_LIBRARY glut32 freeglut_static freeglut
            ${GLUT_LOCATION}/lib/x64
            $ENV{GLUT_LOCATION}/lib/x64
            ${GLUT_LOCATION}/lib
            ${GLUT_LOCATION}/lib/glut
            $ENV{GLUT_LOCATION}/lib
            $ENV{GLUT_LOCATION}/lib/glut
            ${OPENGL_LIBRARY_DIR}
            /usr/lib
            /usr/lib/w32api
            /usr/local/lib
            /usr/X11R6/lib
          )
        endif()
    else()
        find_path( GLUT_INCLUDE_DIR GL/glut.h
            ${GLUT_LOCATION}/include
            $ENV{GLUT_LOCATION}/include
            ${PROJECT_SOURCE_DIR}/extern/glut/include
            $ENV{PROGRAMFILES}/GLUT/include
            ${OPENGL_INCLUDE_DIR}
            DOC "The directory where GL/glut.h resides")
        if(ARCH STREQUAL "x86")
          find_library( GLUT_glut_LIBRARY
            NAMES glut32 glut32s glut freeglut_static freeglut
            PATHS
            ${GLUT_LOCATION}/lib
            ${GLUT_LOCATION}/lib/win32
            ${GLUT_LOCATION}/lib/x86
            $ENV{GLUT_LOCATION}/lib
            $ENV{GLUT_LOCATION}/lib/win32
            $ENV{GLUT_LOCATION}/lib/x86
            ${GLUT_LOCATION}/lib/glut
            $ENV{GLUT_LOCATION}/lib/glut
            ${PROJECT_SOURCE_DIR}/extern/glut/bin
            ${PROJECT_SOURCE_DIR}/extern/glut/lib
            ${PROJECT_SOURCE_DIR}/extern/glut/lib/win32
            ${PROJECT_SOURCE_DIR}/extern/glut/lib/x86
            $ENV{PROGRAMFILES}/GLUT/lib
            $ENV{PROGRAMFILES}/GLUT/lib/win32
            $ENV{PROGRAMFILES}/GLUT/lib/x86
            ${OPENGL_LIBRARY_DIR}
            DOC "The GLUT library")
        else()
          find_library( GLUT_glut_LIBRARY
            NAMES glut32 glut32s glut freeglut_static freeglut
            PATHS
            ${GLUT_LOCATION}/lib/x64
            $ENV{GLUT_LOCATION}/lib/x64
            ${PROJECT_SOURCE_DIR}/extern/glut/lib/x64
            $ENV{PROGRAMFILES}/GLUT/lib/x64
            ${GLUT_LOCATION}/lib
            $ENV{GLUT_LOCATION}/lib
            ${GLUT_LOCATION}/lib/glut
            $ENV{GLUT_LOCATION}/lib/glut
            ${PROJECT_SOURCE_DIR}/extern/glut/bin
            ${PROJECT_SOURCE_DIR}/extern/glut/lib
            $ENV{PROGRAMFILES}/GLUT/lib
            ${OPENGL_LIBRARY_DIR}
            DOC "The GLUT library")
        endif()
    endif()
else ()
    if (APPLE)
        # These values for Apple could probably do with improvement.
        find_path( GLUT_INCLUDE_DIR glut.h
            /System/Library/Frameworks/GLUT.framework/Versions/A/Headers
            ${OPENGL_LIBRARY_DIR}
        )
        set(GLUT_glut_LIBRARY "-framework Glut" CACHE STRING "GLUT library for OSX")
        set(GLUT_cocoa_LIBRARY "-framework Cocoa" CACHE STRING "Cocoa framework for OSX")
    else ()
        find_path( GLUT_INCLUDE_DIR GL/glut.h
            ${GLUT_LOCATION}/include
            $ENV{GLUT_LOCATION}/include
            /usr/include
            /usr/include/GL
            /usr/local/include
            /usr/openwin/share/include
            /usr/openwin/include
            /usr/X11R6/include
            /usr/include/X11
            /opt/graphics/OpenGL/include
            /opt/graphics/OpenGL/contrib/libglut
        )
        find_library( GLUT_glut_LIBRARY glut libglut
            ${GLUT_LOCATION}/lib
            $ENV{GLUT_LOCATION}/lib
            ${GLUT_LOCATION}/lib/glut
            $ENV{GLUT_LOCATION}/lib/glut
            /usr/lib
            /usr/local/lib
            /usr/openwin/lib
            /usr/X11R6/lib
        )
        find_library( GLUT_X11_LIBRARY X11
            ${GLUT_LOCATION}/lib
            $ENV{GLUT_LOCATION}/lib
            /usr/lib
            /usr/local/lib
            /usr/openwin/lib
            /usr/X11R6/lib
        )
        find_library( GLUT_Xi_LIBRARY Xi
            ${GLUT_LOCATION}/lib
            $ENV{GLUT_LOCATION}/lib
            /usr/lib
            /usr/local/lib
            /usr/openwin/lib
            /usr/X11R6/lib
        )
        find_library( GLUT_Xmu_LIBRARY Xmu
            ${GLUT_LOCATION}/lib
            $ENV{GLUT_LOCATION}/lib
            /usr/lib
            /usr/local/lib
            /usr/openwin/lib
            /usr/X11R6/lib
        )
    endif (APPLE)
endif (WIN32)

set( GLUT_FOUND "NO" )

if(GLUT_INCLUDE_DIR)
  if(GLUT_glut_LIBRARY)
    # Is -lXi and -lXmu required on all platforms that have it?
    # If not, we need some way to figure out what platform we are on.
    set( GLUT_LIBRARIES
      ${GLUT_glut_LIBRARY}
      ${GLUT_Xmu_LIBRARY}
      ${GLUT_Xi_LIBRARY}
      ${GLUT_X11_LIBRARY}
      ${GLUT_cocoa_LIBRARY}
    )
    set( GLUT_FOUND "YES" )

    set (GLUT_LIBRARY ${GLUT_LIBRARIES})
    set (GLUT_INCLUDE_PATH ${GLUT_INCLUDE_DIR})

  endif(GLUT_glut_LIBRARY)
endif(GLUT_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GLUT DEFAULT_MSG
    GLUT_INCLUDE_DIR
    GLUT_LIBRARIES
)

# special case of freeglut requiring a define in static mode
string(FIND ${GLUT_glut_LIBRARY} "freeglut_static.lib" FREEGLUT) 
if(FREEGLUT GREATER 0)
  message(STATUS "GLUT: using ${GLUT_glut_LIBRARY}")
  add_definitions(-DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0)
endif()
mark_as_advanced(
  GLUT_INCLUDE_DIR
  GLUT_glut_LIBRARY
  GLUT_Xmu_LIBRARY
  GLUT_X11_LIBRARY
  GLUT_Xi_LIBRARY
)

