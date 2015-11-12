/*-----------------------------------------------------------------------
    Copyright (c) 2013, Tristan Lorach. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Neither the name of its contributors may be used to endorse 
       or promote products derived from this software without specific
       prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
    OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    feedback to lorachnroll@gmail.com (Tristan Lorach)
*/ //--------------------------------------------------------------------
#if defined(__APPLE__)
    #include <GLUT/glut.h>
#else
    #include <assert.h>
	#define ASSERT(a) assert(a);
    #include <stdio.h>
    #include <stdlib.h>
    #include <vector>
    #include <GL/glew.h>
    #ifdef MEMORY_LEAKS_CHECK
    #   pragma message("build will Check for Memory Leaks!")
    #   define _CRTDBG_MAP_ALLOC
    #   include <stdlib.h>
    #   include <crtdbg.h>
    inline void* operator new(size_t size, const char *file, int line)
    {
       return ::operator new(size, 1, file, line);
    }

    inline void __cdecl operator delete(void *ptr, const char *file, int line) 
    {
       ::operator delete(ptr, _NORMAL_BLOCK, file, line);
    }

    #define DEBUG_NEW new( __FILE__, __LINE__)
    #define MALLOC_DBG(x) _malloc_dbg(x, 1, __FILE__, __LINE__);
    #define malloc(x) MALLOC_DBG(x)
    #define new DEBUG_NEW
    #endif
    #include <windows.h>
    #include "resources.h"
#endif
#ifdef USESVCSPACEMOUSE
#include "ISvcSpaceMouse.h"
extern ISpaceMouse *       g_pISpaceMouse;
#endif

#include "nv_math.h"
using namespace nv_math;

#include "FxLibEx.h"
#ifdef USE_NSIGHT
#   include "NSightEvents.h"
#else
#   define NX_RANGE int
#   define NX_MARK(name)
#   define NX_RANGESTART(name) 0
#   define NX_RANGEEND(id)
#   define NX_RANGEPUSH(name)
#   define NX_RANGEPUSHCOL(name, c)
#   define NX_RANGEPOP()
#   define NXPROFILEFUNC(name)
#   define NXPROFILEFUNCCOL(name, c)
#   define NXPROFILEFUNCCOL2(name, c, a)
#endif

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
#ifndef WIN32
struct POINT {
    int x;
    int y;
};
#endif
// to avoid using struct/class when singleton

#define WINDOWINERTIACAMERA_EXTERN
#include "WindowInertiaCamera.h"
//-----------------------------------------------------------------------------
// Derive the Window for this sample
//-----------------------------------------------------------------------------
class MyWindow: public WindowInertiaCamera
{
public:
    MyWindow();
    virtual bool init();
    virtual void shutdown();
    virtual void reshape(int w, int h);
    virtual void motion(int x, int y);
    virtual void mousewheel(short delta);
    virtual void mouse(NVPWindow::MouseButton button, ButtonAction action, int mods, int x, int y);
    virtual void menu(int m);
    virtual void keyboard(MyWindow::KeyCode key, ButtonAction action, int mods, int x, int y);
    virtual void keyboardchar(unsigned char key, int mods, int x, int y);
    virtual void display();

    int argc;
    const char ** argv;

    int m_W, m_H;
    float m_mouse[4];
};
extern MyWindow g_myWindow;

// information for simple highlighting
namespace ViewerStates
{
    extern bool        bCameraMode;
    extern float       moveStep;
    extern bool        bAnimate;
};

