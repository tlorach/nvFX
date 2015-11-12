/*-----------------------------------------------------------------------
    Copyright (c) 2013, NVIDIA. All rights reserved.

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

    feedback to tlorach@nvidia.com (Tristan Lorach)
*/ //--------------------------------------------------------------------
#ifndef __WINDOWINERTIACAMERA__
#define __WINDOWINERTIACAMERA__
#include "main.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>

#include "TimeSampler.h"
#include "InertiaCamera.h"

#include <map>
using std::map;

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
#ifndef WIN32
struct POINT {
  int x;
  int y;
};
#endif
#ifdef WINDOWINERTIACAMERA_EXTERN
extern std::map<char, bool*>    g_toggleMap;
#else
std::map<char, bool*>    g_toggleMap;
#endif
inline void addToggleKey(char c, bool* target, const char* desc)
{
	LOGI(desc);
	g_toggleMap[c] = target;
}
//-----------------------------------------------------------------------------
// Derive the Window for this sample
//-----------------------------------------------------------------------------
class WindowInertiaCamera: public NVPWindow
{
public:
    WindowInertiaCamera()
	{
		m_bCameraMode = true;
		m_bContinue = true;
		m_moveStep = 0.2f;
		m_ptLastMousePosit.x = m_ptLastMousePosit.y  = 0;
		m_ptCurrentMousePosit.x = m_ptCurrentMousePosit.y = 0;
		m_ptOriginalMousePosit.x = m_ptOriginalMousePosit.y = 0;
		m_bMousing = false;
		m_bRMousing = false;
		m_bMMousing = false;
		m_bNewTiming = false;
		m_bAdjustTimeScale = true;
	}
protected:
	bool			m_bCameraMode;
    bool			m_bContinue;
    float			m_moveStep;
    POINT			m_ptLastMousePosit;
    POINT			m_ptCurrentMousePosit;
    POINT			m_ptOriginalMousePosit;
    bool			m_bMousing;
    bool			m_bRMousing;
    bool			m_bMMousing;
    bool			m_bNewTiming;
	bool			m_bAdjustTimeScale;

    TimeSampler		m_realtime;
    InertiaCamera	m_camera;
    mat4f			m_projection;
public:    
    inline mat4f&   projMat() { return m_projection; }
    inline mat4f&   viewMat() { return m_camera.m4_view; }
    inline bool&    nonStopRendering() { return m_realtime.bNonStopRendering; }

    virtual bool init();
    virtual void shutdown();
    virtual void reshape(int w, int h);
    virtual void motion(int x, int y);
    virtual void mousewheel(short delta);
    virtual void mouse(NVPWindow::MouseButton button, ButtonAction action, int mods, int x, int y);
    virtual void keyboard(WindowInertiaCamera::KeyCode key, ButtonAction action, int mods, int x, int y);
    virtual void keyboardchar(unsigned char key, int mods, int x, int y);
    virtual void idle();
    virtual void display();
};
#ifndef WINDOWINERTIACAMERA_EXTERN

bool WindowInertiaCamera::init()
{
    assert(glewInit() == GLEW_OK);
    m_realtime.bNonStopRendering = true;
    return true;
}

void WindowInertiaCamera::shutdown()
{
}


//------------------------------------------------------------------------------
#define CAMERATAU 0.03f
void WindowInertiaCamera::motion(int x, int y)
{
    m_ptCurrentMousePosit.x = x;
    m_ptCurrentMousePosit.y = y;
    //---------------------------- LEFT
    if(m_bMousing)
    {
        float hval = 2.0f*(float)(m_ptCurrentMousePosit.x - m_ptLastMousePosit.x)/(float)getWidth();
        float vval = 2.0f*(float)(m_ptCurrentMousePosit.y - m_ptLastMousePosit.y)/(float)getHeight();
        m_camera.tau = CAMERATAU;
        m_camera.rotateH(hval);
        m_camera.rotateV(vval);
        postRedisplay();
    }
    //---------------------------- MIDDLE
    if( m_bMMousing )
    {
        float hval = 2.0f*(float)(m_ptCurrentMousePosit.x - m_ptLastMousePosit.x)/(float)getWidth();
        float vval = 2.0f*(float)(m_ptCurrentMousePosit.y - m_ptLastMousePosit.y)/(float)getHeight();
        m_camera.tau = CAMERATAU;
        m_camera.rotateH(hval, true);
        m_camera.rotateV(vval, true);
        postRedisplay();
    }
    //---------------------------- RIGHT
    if( m_bRMousing )
    {
        float hval = 2.0f*(float)(m_ptCurrentMousePosit.x - m_ptLastMousePosit.x)/(float)getWidth();
        float vval = -2.0f*(float)(m_ptCurrentMousePosit.y - m_ptLastMousePosit.y)/(float)getHeight();
        m_camera.tau = CAMERATAU;
        m_camera.rotateH(hval, !!(getMods()&KMOD_CONTROL));
        m_camera.move(vval, !!(getMods()&KMOD_CONTROL));
        postRedisplay();
    }
    
    m_ptLastMousePosit.x = m_ptCurrentMousePosit.x;
    m_ptLastMousePosit.y = m_ptCurrentMousePosit.y;
}

//------------------------------------------------------------------------------
void WindowInertiaCamera::mousewheel(short delta)
{
    static int s_WheelPos = 0;
    delta /= WHEEL_DELTA;
    if(delta > 3)
        delta = 3;
    if(delta < -3)
        delta = -3;
    s_WheelPos += delta;
#ifdef USEANTTWEAKBAR
    int handled = TwMouseWheel(s_WheelPos);
    if(!handled)
#endif
    {
        //m_camera.tau = KEYTAU;
        //m_camera.moveToTarget(delta > 0 ? 0.2: -0.2, !!(getMods()&KMOD_CONTROL));
    }
	postRedisplay();
}
//------------------------------------------------------------------------------
void WindowInertiaCamera::mouse(NVPWindow::MouseButton button, NVPWindow::ButtonAction state, int mods, int x, int y)
{
    switch(button)
    {
    case NVPWindow::MOUSE_BUTTON_LEFT:
        if(state == NVPWindow::BUTTON_PRESS)
        {
            // TODO: equivalent of glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED/NORMAL);
            m_bMousing = true;
            postRedisplay();
            if(getMods()&KMOD_CONTROL)
            {
            }
            else if(getMods()&KMOD_SHIFT)
            {
            }
        }
        else {
            m_bMousing = false;
            postRedisplay();
        }
        break;
    case NVPWindow::MOUSE_BUTTON_RIGHT:
        if(state == NVPWindow::BUTTON_PRESS)
        {
            m_ptLastMousePosit.x = m_ptCurrentMousePosit.x = x;
            m_ptLastMousePosit.y = m_ptCurrentMousePosit.y = y;
            m_bRMousing = true;
            postRedisplay();
            if(getMods()&KMOD_CONTROL)
            {
            }
        }
        else {
            m_bRMousing = false;
            postRedisplay();
        }
        break;
    case NVPWindow::MOUSE_BUTTON_MIDDLE:
        if(state == NVPWindow::BUTTON_PRESS)
        {
            m_ptLastMousePosit.x = m_ptCurrentMousePosit.x = x;
            m_ptLastMousePosit.y = m_ptCurrentMousePosit.y = y;
            m_bMMousing = true;
            postRedisplay();
        }
        else {
            m_bMMousing = false;
            postRedisplay();
        }
        break;
    }
}
//------------------------------------------------------------------------------
#define KEYTAU 0.10f
void WindowInertiaCamera::keyboard(NVPWindow::KeyCode key, NVPWindow::ButtonAction action, int mods, int x, int y)
{
    if(action == NVPWindow::BUTTON_RELEASE)
        return;
    switch(key)
    {
    case NVPWindow::KEY_F1:
        break;
    case NVPWindow::KEY_F2:
        break;
    case NVPWindow::KEY_F3:
    case NVPWindow::KEY_F4:
    case NVPWindow::KEY_F5:
    case NVPWindow::KEY_F6:
    case NVPWindow::KEY_F7:
    case NVPWindow::KEY_F8:
    case NVPWindow::KEY_F9:
    case NVPWindow::KEY_F10:
    case NVPWindow::KEY_F11:
        break;
    case NVPWindow::KEY_F12:
        break;
    case NVPWindow::KEY_LEFT:
        m_camera.tau = KEYTAU;
        m_camera.rotateH(m_moveStep, !!(getMods()&KMOD_CONTROL));
        break;
    case NVPWindow::KEY_UP:
        m_camera.tau = KEYTAU;
        m_camera.rotateV(m_moveStep, !!(getMods()&KMOD_CONTROL));
        break;
    case NVPWindow::KEY_RIGHT:
        m_camera.tau = KEYTAU;
        m_camera.rotateH(-m_moveStep, !!(getMods()&KMOD_CONTROL));
        break;
    case NVPWindow::KEY_DOWN:
        m_camera.tau = KEYTAU;
        m_camera.rotateV(-m_moveStep, !!(getMods()&KMOD_CONTROL));
        break;
    case NVPWindow::KEY_PAGE_UP:
        m_camera.tau = KEYTAU;
        m_camera.move(m_moveStep, !!(getMods()&KMOD_CONTROL));
        break;
    case NVPWindow::KEY_PAGE_DOWN:
        m_camera.tau = KEYTAU;
        m_camera.move(-m_moveStep, !!(getMods()&KMOD_CONTROL));
        break;
    //case NVPWindow::KEY_ESCAPE:
    //    postQuit();
    //    break;
    }
}

void WindowInertiaCamera::keyboardchar( unsigned char key, int mods, int x, int y )
{
    // check registered toggles
    std::map<char, bool*>::iterator it = g_toggleMap.find(key);
    if(it != g_toggleMap.end())
    {
        *it->second = *it->second ? false : true;
    }
}

void WindowInertiaCamera::idle()
{
    //
    // if requested: trigger again the next frame for rendering
    //
    if (m_bContinue || m_realtime.bNonStopRendering)
        postRedisplay();
}
void WindowInertiaCamera::display()
{
    //
    // Camera motion
    //
    m_bContinue = m_camera.update((float)m_realtime.getTiming());
    //
    // time sampling
    //
    if(m_realtime.update(m_bContinue))
    {
    }
}
//------------------------------------------------------------------------------
void WindowInertiaCamera::reshape(int w, int h)
{
    //
    // Let's validate again the base of resource management to make sure things keep consistent
    //
    int W = getWidth();
    int H = getHeight();

    glViewport(0, 0, W, H);

    float r = (float)getWidth() / (float)getHeight();
    perspective(m_projection, 50.0f, r, 0.01f, 10.0f);
}
#endif //WINDOWINERTIACAMERA_EXTERN
#endif