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
#include "shared/WindowInertiaCamera.h"
#include "ShaderToyViewer.h"
#include "FxParser.h"
#ifdef USESVCUI
#include "ISvcUI.h"
#endif
#ifdef USEANTTWEAKBAR
#include "AntTweakBar.h"
TwBar *bar; // Pointer to the tweak bar
#endif
#ifdef USESVCSPACEMOUSE
nvSvc::ISvcFactory* g_pFactMouse = NULL;
ISpaceMouse *       g_pISpaceMouse = NULL;
#endif

nvFX::IContainer*	fx_EffectScene      = NULL;
nvFX::ITechnique*   fx_TechScene        = NULL;

nvFX::IUniform*     iResolution         = NULL;
nvFX::IUniform*     iGlobalTime         = NULL;
nvFX::IUniform*     iMouse              = NULL;
nvFX::IUniform*     iDate               = NULL;
nvFX::IUniform*     iSampleRate         = NULL;
nvFX::IUniform*     iChannelTime        = NULL;
nvFX::IUniform*     iChannelResolution  = NULL;


bool        verbose = true; // Needed by MMD.cpp (that we share with bk3dutil)

namespace ViewerStates
{
    bool        bCameraMode = false;
    float       fTime = 0.0;
};

//-------------------- 
MyWindow::MyWindow()
{
    m_W = 0;
    m_H = 0;
    m_mouse[0] = m_mouse[1] = m_mouse[2] = m_mouse[3] = 0.0f;
}

/////////////////////////////////////////////////////////////////////////
// called by main logMessage()
//
void printMessage(int level, const char * txt)
{
    OutputDebugString(txt);
}
void nvFXErrorMsg(const char * txt)
{
    OutputDebugString(txt);
}
void nvFXMsg(const char * txt)
{
    OutputDebugString(txt);
}

//-------------------- VAO (to be Core OpenGL compliant :-( )
GLuint g_vao;
//-------------------- Timer Query
GLuint  timerQueries[4];
int     tqOffset = 0;
bool    tqStart = true;

//------------------------------------------------------------------------------
void MyWindow::reshape(int w, int h)
{
    WindowInertiaCamera::reshape(w,h);
    //
    // Let's validate again the base of resource management to make sure things keep consistent
    //
    m_W = getWidth();
    m_H = getHeight();

    glViewport(0, 0, m_W, m_H);
#ifdef USEANTTWEAKBAR
    TwWindowSize(m_W, m_H);
#endif

    float r = (float)m_W / (float)m_H;
    mat4f proj;
    perspective(proj, 30.0f, r, 0.01f, 10.0f);
    mat4f shift(array16_id);

    //ui::reshape(w,h);

}

#if _MSC_VER
    #define snprintf _snprintf
#endif

#define drawString(x, y, ...)               \
    { char line[1024]; \
      snprintf(line, 1024, __VA_ARGS__); \
      char *p = line; \
      glWindowPos2i(x, y); \
      while(*p) { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p++); } }


//------------------------------------------------------------------------------
#define CAMERATAU 0.03f
void MyWindow::motion(int x, int y)
{
    WindowInertiaCamera::motion(x, y);
#ifdef USEANTTWEAKBAR
    if(!(ViewerStates::mousingMode || ViewerStates::mousingModeM || ViewerStates::mousingModeR) )
    {
        int ret = TwMouseMotion(x, y);
        if(ret) {
			postRedisplay();
            return;
		}
    }
#endif
    //---------------------------- LEFT
    if(m_bMousing)
    {
        m_mouse[0] = (float)m_ptCurrentMousePosit.x;
        m_mouse[1] = (float)m_ptCurrentMousePosit.y;
    }
}

//------------------------------------------------------------------------------
void MyWindow::mousewheel(short delta)
{
    WindowInertiaCamera::mousewheel(delta);
}
//------------------------------------------------------------------------------
void MyWindow::mouse(NVPWindow::MouseButton button, ButtonAction action, int mods, int x, int y)
{
    WindowInertiaCamera::mouse(button, action, mods, x, y);
#ifdef USEANTTWEAKBAR
    int res = TwMouseButton(action==BUTTON_PRESS? TW_MOUSE_PRESSED: TW_MOUSE_RELEASED, (TwMouseButtonID)(button+1));
    if(res) {
		postRedisplay();
        return;
	}
#endif
    WindowInertiaCamera::mouse(button, action, mods, x, y);
    switch(button)
    {
    case MOUSE_BUTTON_LEFT:
        if(action==BUTTON_PRESS) // Down
        {
            m_mouse[2] = (float)m_ptOriginalMousePosit.x;
            m_mouse[3] = (float)m_ptOriginalMousePosit.y;
            postRedisplay();
        }
        break;
    case MOUSE_BUTTON_RIGHT:
        if(action==BUTTON_PRESS)
        {
        }
        else if(action==BUTTON_REPEAT)
        {
        }
        else {
        }
        break;
    case MOUSE_BUTTON_MIDDLE:
        break;
    }
  }

  //------------------------------------------------------------------------------
  void MyWindow::shutdown()
  {
    WindowInertiaCamera::shutdown();
    //ui::terminate();

#ifndef NOGLUT
    exit(0);
#endif
  }

  //------------------------------------------------------------------------------
  void MyWindow::menu(int m)
  {

  }

  //------------------------------------------------------------------------------
  #define KEYTAU 0.10f
  void MyWindow::keyboard(MyWindow::KeyCode key, ButtonAction action, int mods, int x, int y)
  {
#ifdef USEANTTWEAKBAR
    // to fix some mapping issues with special keys
    // TODO: find the real Fix...
    if(action == MyWindow::BUTTON_PRESS)
    {
        switch(key)
        {
        case NVPWindow::KEY_HOME:
            if(TwEventSpecialGLUT(106/*GLUT_KEY_HOME*/, x,y) ) { postRedisplay(); return; }
            break;
        case NVPWindow::KEY_END:
            if(TwEventSpecialGLUT(107/*GLUT_KEY_END*/, x,y) ) { postRedisplay(); return; }
            break;
        case NVPWindow::KEY_LEFT:
            if(TwEventSpecialGLUT(100/*GLUT_KEY_LEFT*/, x,y) ) { postRedisplay(); return; }
            break;
        case NVPWindow::KEY_RIGHT:
            if(TwEventSpecialGLUT(102/*GLUT_KEY_RIGHT*/, x,y) ) { postRedisplay(); return; }
            break;
        case NVPWindow::KEY_BACKSPACE:
            if(TwEventKeyboardGLUT(TW_KEY_BACKSPACE, x,y) ) { postRedisplay(); return; }
            break;
        case NVPWindow::KEY_DELETE:
            if(TwEventKeyboardGLUT(TW_KEY_DELETE, x,y) ) { postRedisplay(); return; }
            break;
        case NVPWindow::KEY_ENTER:
        case NVPWindow::KEY_KP_ENTER:
            if(TwEventKeyboardGLUT(TW_KEY_RETURN, x,y)) { postRedisplay(); return; }
            break;
        case NVPWindow::KEY_ESCAPE:
            //postQuit();
            break;
        }
    }
    TwEventMousePosGLFW(x,y);
    if(TwEventKeyGLFW(key, action) ) {
    //if(TwEventSpecialGLUT(key, x,y) ) {
		postRedisplay();
        return;
	}
#endif
    WindowInertiaCamera::keyboard(key, action, mods, x, y);
  }
  //------------------------------------------------------------------------------
  void MyWindow::keyboardchar(unsigned char key, int mods, int x, int y)
  {
      WindowInertiaCamera::keyboardchar(key, mods, x, y);
#ifdef USEANTTWEAKBAR
    //int res = TwEventKeyboardGLUT(key, x,y);
    TwEventMousePosGLFW(x,y);
    int res = TwEventCharGLFW(key, BUTTON_PRESS|mods);
    if(res) {
		postRedisplay();
        return;
	}
#endif
  }

  //------------------------------------------------------------------------------
  void idle()
  {
    //bool bContinue = ViewerStates::camera.update((float)ViewerStates::realtime.frameDT);
    //
    // Now work on the matrices
    //
#ifdef USESVCSPACEMOUSE
    if(g_pISpaceMouse && g_pISpaceMouse->isValid())
    {
        static float v[] = {0,0,0,0,0,0,0,0,0};
        float newv[9];
        g_pISpaceMouse->pullData(0.001f/(/*ViewerStates::g_bCameraMode ? 1.0f : g_scale*/1.0f), 0.02f, newv, newv+3, newv+6);
        if((v[0]!=newv[0])||(v[1]!=newv[1])||(v[2]!=newv[2])||
           (v[3]!=newv[3])||(v[4]!=newv[4])||(v[5]!=newv[5])||
           (v[6]!=newv[6])||(v[7]!=newv[7])||(v[8]!=newv[8]))
           bContinue = true;
        memcpy(v, newv, sizeof(float)*9);
        /*if(ViewerStates::g_bCameraMode)*/
            g_pISpaceMouse->updateCameraTransform();
    }
#endif
    //if (bContinue || ViewerStates::realtime.bNonStopRendering)
        g_myWindow.postRedisplay();

  }

  //------------------------------------------------------------------------------
  //
  //------------------------------------------------------------------------------
  bool MyWindow::init()
  {
    WindowInertiaCamera::init();
    glewInit();


    // Initialize AntTweakBar
    // http://anttweakbar.sourceforge.net/doc
#ifdef USEANTTWEAKBAR
    TwInit(TW_OPENGL_CORE, NULL);
#endif

	// check the minimum requirement
	if(!glewIsSupported("GL_ARB_separate_shader_objects"))
	{
		LOGE("GL_ARB_separate_shader_objects needed");
		MessageBox(NULL, "GL_ARB_separate_shader_objects needed", "Problem", MB_OK);
	}

	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // Bind only one vao
    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    //
    // Timer Query
    //
    glGenQueries(4, timerQueries);
    //
    // Space Mouse
    //
#ifdef USESVCSPACEMOUSE
    FACTORY_LOAD(g_pFactMouse, "SvcSpaceMouse_1.0.dll");
    if(!g_pFactMouse)
    {
        FACTORY_LOAD(g_pFactMouse, "SvcSpaceMouse64_1.0.dll");
    }
    if(g_pFactMouse)
    {
        g_pISpaceMouse = (ISpaceMouse*)g_pFactMouse->CreateInstance("SpaceMouse");
    }
#endif
    reshape(getWidth(), getHeight());

    nvFX::setErrorCallback(nvFXErrorMsg);
    nvFX::setMessageCallback(nvFXMsg);
    fx_EffectScene = nvFX::IContainer::create();
    bool bRes = nvFX::loadEffectFromFile(fx_EffectScene, "ShaderToyEffect.glslfx");
    if(!bRes)
        bRes = nvFX::loadEffectFromFile(fx_EffectScene, SOURCE_DIR "/ShaderToyEffect.glslfx");
    if(!bRes)
    {
        OutputDebugStringA("Effect file not found. Make sure it is close to the exe file...");
        return false;
    }
    fx_TechScene = fx_EffectScene->findTechnique(0);
    fx_TechScene->validate();
    iResolution = fx_EffectScene->findUniform("iResolution");
    iGlobalTime = fx_EffectScene->findUniform("iGlobalTime");
    iMouse      = fx_EffectScene->findUniform("iMouse");
    iDate       = fx_EffectScene->findUniform("iDate");
    iSampleRate = fx_EffectScene->findUniform("iSampleRate");
    //iChannelTime = fx_EffectScene->findUniform("");
    //iChannelResolution = fx_EffectScene->findUniform("");


    return true;
}

void MyWindow::display()
{
    WindowInertiaCamera::display();
    glClearColor(0.4f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //
    // timing for frames
    //
    //ViewerStates::realtime.update(true);
    ViewerStates::fTime += (float)m_realtime.frameDT;
    //
    // SCENE-LEVEL TEST
    //
    nvFX::PassInfo pr;
    memset(&pr, 0, sizeof(pr));
    if(fx_TechScene)
    {
        int np = fx_TechScene->getNumPasses();
        for(int i=0; i<np; i++)
        {
            nvFX::IPass* scenePass = fx_TechScene->getPass(i, &pr);
            if(!pr.bActive)
                continue;
            if(!scenePass->isValidated())
            {
                continue;
            }
            pr.renderingGroup = 0; // set back to 0 before each pass. So no persistent value across passes
            scenePass->execute(&pr);
            iResolution->updateValue3f((float)m_W, (float)m_H, 0, scenePass);
            iGlobalTime->updateValue1f(ViewerStates::fTime, scenePass);
            iMouse->updateValue4fv(m_mouse, scenePass);
            //iDate       = fx_EffectScene->findUniform("iDate");
            //iSampleRate = fx_EffectScene->findUniform("iSampleRate");
            switch(pr.renderingMode)
            {
            default:
                break;
            }
    glUseProgram(0);
        }
    }
    // Draw tweak bars
#ifdef USEANTTWEAKBAR
    //TwDraw();
#endif
    swapBuffers();
}

/////////////////////////////////////////////////////////////////////////
// Window
//
MyWindow g_myWindow;
/////////////////////////////////////////////////////////////////////////
// Main initialization point
//
int sample_main(int argc, const char** argv)
{
    NVPWindow::ContextFlags context(
    3,      //major;
    3,      //minor;
    false,   //core;
    1,      //MSAA;
    true,   //debug;
    false,  //robust;
    false,  //forward;
    NULL   //share;
    );

    g_myWindow.argc = argc;
    g_myWindow.argv = argv;

    if(!g_myWindow.create("bk3d viewer", &context))
        return false;

    g_myWindow.makeContextCurrent();
    g_myWindow.swapInterval(0);

    while(MyWindow::sysPollEvents(false) )
    {
        idle();
    }
    return true;
}
