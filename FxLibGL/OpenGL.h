// TODO: this part is dirty. The fact we include OGL here is maybe a bad idea...
#ifdef OGLES2
    // OGLES2 : no Cg at all and no GL include here
#   undef USE_CG
#   undef INCLUDEGL
#else
    // case for OpenGL : include GL things and expose Cg as a possible choice
//#   define USE_CG
//# define USE_CG_COMBINE_PROG
#   define INCLUDEGL
#endif

//#ifdef INCLUDEGL
#ifdef ES2EMU
#   pragma MESSAGE("-----------------------------------------------")
#   pragma MESSAGE("---- Windows ES2 Emulation --------------------")
#   pragma MESSAGE("-----------------------------------------------")
#   include <GLES2/gl2platform.h>
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#elif defined(ANDROID)
//#   pragma MESSAGE("-----------------------------------------------")
//#   pragma MESSAGE("---- ANDROID ----------------------------------")
//#   pragma MESSAGE("-----------------------------------------------")
#   define OGLES2
#   include <GLES2/gl2platform.h>
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#elif defined(IOS)
//#   pragma MESSAGE("-----------------------------------------------")
//#   pragma MESSAGE("---- IOS --------------------------------------")
//#   pragma MESSAGE("-----------------------------------------------")
//  This one is for iOS
#   include <OpenGLES/ES2/gl.h>
#   include <OpenGLES/ES2/glext.h>
#else // Non-ES : Windows
#   ifdef NOGLEW
#       pragma MESSAGE("-----------------------------------------------")
#       pragma MESSAGE("---- Windows + regular gl.h  ------------------")
#       pragma MESSAGE("-----------------------------------------------")
#       include "gl.h"
#       include "glext.h"
#       include "wgl.h"
#       include "wglext.h"
#   else
#       pragma MESSAGE("-----------------------------------------------")
#       pragma MESSAGE("---- Windows + Glew.h  ------------------------")
#       pragma MESSAGE("-----------------------------------------------")
#       include <GL/glew.h>
#      ifndef WIN32
#       include <GL/glxew.h>
#      endif
#   endif
#endif
//#endif
