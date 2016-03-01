#####################################################################################
# Macro for adding a (potentially cuda) executable.
#
if(USE_GLUT OR NOT WIN32)
    macro(_add_glut_executable target)
        if(WIN32)
            # Windows needs some of its dependency dll's copied into the same
            # directory as the executable.
            set( LIBRARIES ${GLUT_LIBRARIES})
            foreach (LIB ${LIBRARIES} )
                string(REPLACE ".lib" ".dll" DLL ${LIB})
                string(REPLACE ".LIB" ".DLL" DLL ${DLL})
                add_custom_command(
                    TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        ${DLL}
                        $<TARGET_FILE_DIR:${target}>
                )
            endforeach()
        endif()

    endmacro()
endif()
#####################################################################################
# Optional UI
#
if(USE_SVCUI)
    find_package(SvcMFCUI)
    if(SVCMFCUI_FOUND)
        add_definitions(-DUSESVCUI)
    endif()
endif()
#####################################################################################
# ZLIB
#
if( ARCH STREQUAL "x64" )
  #set( ZLIB_ROOT "$ENV{ProgramFiles}") # strange... still returning x86 folder... grrr
  # I add ZLIB_ROOT because findZLIB uses $ENV{ProgramFiles} and fails, too
  set( ZLIB_ROOT "C:/Program Files/zlib")
else ()
  # I add ZLIB_ROOT because findZLIB uses $ENV{ProgramFiles} and fails, too
  set( ZLIB_ROOT "C:/Program Files (x86)/zlib")
endif()
find_package(ZLib)
if(NOT ZLIB_FOUND)
  add_definitions(-DNOGZLIB)
  message(WARNING "
    ZLIB not found : the project will be created without the capability to read compressed models.
    But you can still unzip the models themselves, before using them through the samples."
  )
endif()
MARK_AS_ADVANCED(CLEAR ZLIB_LIBRARY ZLIB_INCLUDE_DIR)

##################################################################
# Shared folder
set(SHARED_DIR "${PROJECT_SOURCE_DIR}/samples/shared")
#####################################################################################
# Keep track of math files
#
#find_path(NV_MATH_DIR nv_math.h "${SHARED_DIR}/nv_math")
set(NV_MATH_DIR "${SHARED_DIR}/nv_math")
set(NV_MATH_HEADERS "${NV_MATH_DIR}/nv_math.h" "${NV_MATH_DIR}/nv_algebra.h" "${NV_MATH_DIR}/nv_mathdecl.h")
set(NV_MATH_SOURCE "${NV_MATH_DIR}/nv_math.cpp")

#####################################################################################
# Keep track of nvImage files
#
#find_path(NV_IMAGE_DIR nvImage.h "${SHARED_DIR}/nvImage")
set(NV_IMAGE_DIR "${SHARED_DIR}/nvImage")
set(NV_IMAGE_HEADERS "${NV_IMAGE_DIR}/nvImage.h" "${NV_IMAGE_DIR}/rgbe.h")
set(NV_IMAGE_SOURCES 
    "${NV_IMAGE_DIR}/rgbe.c"
    "${NV_IMAGE_DIR}/nvImage.cpp"
    "${NV_IMAGE_DIR}/nvImageDDS.cpp"
    "${NV_IMAGE_DIR}/nvImageHdr.cpp"
    #"${NV_IMAGE_DIR}/nvImagePng.cpp" # canceled it because we would then need png library... :-(
)

#####################################################################################
# model include
#find_path(BK3D_DIR bk3dBase.h "${SHARED_DIR}")
set(BK3D_DIR "${SHARED_DIR}/bk3d")
set(BK3D_HEADERS "${BK3D_DIR}/bk3dBase.h" "${BK3D_DIR}/bk3dDefs.h" "${BK3D_DIR}/bk3dEx.h")
#####################################################################################
# curve helper
#find_path(CV_DIR curveEngine.h "${SHARED_DIR}")
set(CV_DIR "${SHARED_DIR}/curve")
set(CV_HEADERS "${CV_DIR}/curveEngine.h" "${CV_DIR}/curvereader.h")
set(CV_SOURCES "${CV_DIR}/curveEngine.cpp" "${CV_DIR}/curvereader.cpp")
#####################################################################################
# folder for various models and textures
find_path(RESOURCE_DIR gargoyle_v134.bk3d.gz "resources")

#####################################################################################
# glut sample
#
if( OPENGL_FOUND AND GLEW_FOUND OR (APPLE AND GLUT_FOUND))
    add_subdirectory(OpenGLSimpleEffect)
    # sample to fix and check before activating it
    #add_subdirectory(OpenGLSimpleEffect_Subroutine)
    #add_subdirectory(OpenGLSimpleEffect_skinning) # some obsolete code to fix... commented-out
    add_subdirectory(OpenGLSimpleEffect_PNTriangles)
    add_subdirectory(OpenGLSimpleSceneEffect)
    add_subdirectory(OpenGL_HDR_IBL_Effect)
    add_subdirectory(OpenGL_GS_Sparkles)
    add_subdirectory(OpenGL_GLSLCompute)
    add_subdirectory(OpenGLLigthingEffect)
    add_subdirectory(ShaderToyViewer)
    # Broken...to fix and check before activating it
    #add_subdirectory(OpenGLFluitSimEffect)
    # Not ready yet
    #add_subdirectory(OpenGLUniformUpdateExample)
    if(USE_CUDA AND CUDA_FOUND)
        add_subdirectory(OpenGL_CUDAConvolution)
    endif()
else()
    set(MISSING "")

    if (NOT OPENGL_FOUND)
        list(APPEND MISSING OpenGL)
    endif()

    if (NOT GLEW_FOUND)
        list(APPEND MISSING glew)
    endif()

    if(USE_GLUT)
        if (NOT GLUT_FOUND)
            list(APPEND MISSING glut)
        endif()
    endif()

    message(WARNING
        "The following libraries could not be found : ${MISSING}.  "
    )
endif()

if(DXSDK_FOUND)
    add_subdirectory(D3DSimple)
endif()

