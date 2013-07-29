#if defined(__APPLE__)
    #include <GLUT/glut.h>
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <vector>
    #include <GL/glew.h>
#ifndef NOGLUT
    #include <GL/glut.h>
#else
    #include <windows.h>
    #include "resources.h"
#endif
#endif

#include "nv_math.h"
//-------------- Generated mesh
#define VSTEPS 20
#define HSTEPS 60
GLuint g_vbo, g_ebo;
struct VtxLayout
{
    vec3 Pos;
    vec3 N;
};
#define MAXMESHES 10000
int g_numMeshes = 1000;
int g_eltsLevel = VSTEPS-2;
//-------------- Optional User interface
#ifdef USESVCUI
#include "ISvcUI.h"
#endif
//-------------- Effect system
#include "FxParser.h"
//
// offset specific to the effect for this demo
//
#define TECH_DEFAULT 2
#define TECH_WIREFLOOR 1
#define TECH_INIT 0
//
// stuff effect, tech and pass interfaces
//
nvFX::IContainer*   fx_Effect       = NULL;
nvFX::ITechnique*   fx_Tech         = NULL;
nvFX::ITechnique*   fx_TechBgnd     = NULL;
nvFX::ITechnique*   fx_TechFloor    = NULL;
nvFX::IPass*        fx_pass         = NULL;
nvFX::ICstBuffer*   fx_transfBlock1 = NULL;

nvFX::ICstBuffer*   fx_transfBlock2 = NULL;

enum UpdateMode {
    USECSTBUFUNIFORMS,
    USEBUFFERSUBDATA,
    USEMAPBUFFER,
    USEUNIFORMS,
};
UpdateMode g_uniformUpdateMode = USEMAPBUFFER;
// here we show that it is also possible to update uniforms inside a constant-buffer
// and rely on the effect system to update the UBO for you
// let's define interfaces for each component of the UBO structure
nvFX::IUniform*     fx_m4_World         = NULL;
nvFX::IUniform*     fx_m4_WorldView     = NULL;
nvFX::IUniform*     fx_m4_WorldViewProj = NULL;
nvFX::IUniform*     fx_m4_WorldIT       = NULL;

// binding points for the uniforms used without uniform blocks
nvFX::IUniform*     fx_u_m4_World         = NULL;
nvFX::IUniform*     fx_u_m4_WorldView     = NULL;
nvFX::IUniform*     fx_u_m4_WorldViewProj = NULL;
nvFX::IUniform*     fx_u_m4_WorldIT       = NULL;

nvFX::ICstBuffer*   fx_materialBlock= NULL;
std::vector<nvFX::IResource*> fx_resources;
// data structure reflecting buffers in shaders
struct transfBlock1
{
    mat4 m4_ViewProj;
    mat4 m4_Proj;
    mat4 m4_View;
    mat4 m4_ViewIT;
    vec3 eyePos;
};
transfBlock1 g_transfBlock1;
struct transfBlock2
{
    mat4 m4_World;
    mat4 m4_WorldView;
    mat4 m4_WorldViewProj;
    mat4 m4_WorldIT;
};
transfBlock2 g_transfBlock2[MAXMESHES];
struct MaterialBlock
{
    // we map it the same as in bk3d::Material for the sake of simplicity of the demo
    vec3     diffuse[3];         ///< RGB diffuse color
    float    specexp;            ///< exponent of the specular lighting
    vec3     ambient[3];         ///< RGB ambient color
    float    reflectivity;       ///< intensity of the reflection
    vec3     transparency[3];    ///< RGB transparency
    float    translucency;       ///< translucency : 0, no transparency at all; 1 : object 100% transparent (transparency could be used)
    vec3     specular[3];        ///< specular RGB color
};

//-------------- Basic UI defined in main.cpp
#ifdef USESVCUI
#include "ISvcUI.h"
extern IWindowHandler*      g_pWinHandler;
extern IWindowConsole*      g_pConsole;
extern IWindowLog*          g_pLog;
extern IProgressBar*        g_pProgress;

IWindowContainer*           g_pControls     = NULL;
//IControlCombo*              g_pComboTech    = NULL;
IControlCombo*              g_pComboUniformUpdate  = NULL;

#   define  LOGI(...)  { if(g_pLog) g_pLog->AddMessage(0, __VA_ARGS__); }
#   define  LOGW(...)  { if(g_pLog) g_pLog->AddMessage(1, __VA_ARGS__); }
#   define  LOGE(...)  { if(g_pLog) g_pLog->AddMessage(2, __VA_ARGS__); }
#   define  LOGOK(...)  { if(g_pLog) g_pLog->AddMessage(7, __VA_ARGS__); }
#   define  UPDATEUI(v) if(g_pWinHandler) g_pWinHandler->VariableFlush(&v);
#else
#   define  LOGI(...)  printf(__VA_ARGS__)
#   define  LOGW(...)  printf(__VA_ARGS__)
#   define  LOGE(...)  printf(__VA_ARGS__)
#   define  LOGOK(...)  printf(__VA_ARGS__)
#   define  UPDATEUI(v)
#endif
//-------------- value from Main.cpp
extern void shutdownBase(); // to shutdown basic things
#ifdef NOGLUT
extern HDC      g_hDC;
#endif
extern int      g_renderCnt; // increment it to force a refresh
extern bool     g_bCtrl;
extern bool     g_bShift;
//-------------- Space mouse
#ifdef SVCSPACEMOUSE
nvSvc::ISvcFactory* g_pFactMouse = NULL;
ISpaceMouse *       g_pISpaceMouse = NULL;
#endif

bool        g_freeze = false;
int         g_frame = 0;

//----------------------- Helpers
#include "Camera.h"
#include "Realtime.h"
Realtime    g_realtime;
Camera      g_camera;

bool        g_bCameraMode = false;
float       g_moveStep = 0.2f;
int         g_forceRender = true;

//---------------------- window interactions
#ifndef WIN32
struct POINT {
  int x;
  int y;
};
#endif
static  POINT ptLastMousePosit;
static  POINT ptCurrentMousePosit;
static  bool bMousing;
static  bool bRMousing;
static  bool bMMousing;

int     g_winSz[2] = {800, 600};

//---------------------- other params
float g_posOffset[3]= {0,0,0};
float g_fSpinX = 0.0;
bool  g_bUseMaterial = true;

//-------------------- VAO (to be Core OpenGL compliant :-( )
GLuint g_vao;
//-------------------- Timer Query
GLuint  timerQueries[4];
int     tqOffset = 0;
bool    tqStart = true;
//-------------------- forward declarations
bool loadMaterialEffect();
void updateTransforms();
//------------------------------------------------------------------------------
void reshape(int w, int h)
{
    g_winSz[0] = w; 
    g_winSz[1] = h;

    perspective(g_transfBlock1.m4_Proj, 45.0f, (float)g_winSz[0] / (float)g_winSz[1], 0.01f, 10.0f);
    //
    // Let's validate again the base of resource management to make sure things keep consistent
    //
    int W = g_winSz[0];
    int H = g_winSz[1];

    glViewport(0, 0, W, H);

    perspective(g_transfBlock1.m4_Proj, 45.0f, (float)g_winSz[0] / (float)g_winSz[1], 0.01f, 10.0f);

    bool failed = nvFX::getResourceRepositorySingleton()->validate(0,0,W,H,1,0,NULL ) ? false : true;
    if(failed)
        assert(!"Oops");
    failed = nvFX::getFrameBufferObjectsRepositorySingleton()->validate(0,0,W,H,1,0,NULL ) ? false : true;
    if(failed)
        assert(!"Oops");
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
void motion(int x, int y)
{
    ptCurrentMousePosit.x = x;
    ptCurrentMousePosit.y = y;

    if( bMousing )
    {
        float hval = 2.0f*(float)(ptCurrentMousePosit.x - ptLastMousePosit.x)/(float)g_winSz[0];
        float vval = 2.0f*(float)(ptCurrentMousePosit.y - ptLastMousePosit.y)/(float)g_winSz[1];
        g_camera.tau = CAMERATAU;
        g_camera.rotateH(hval);
        g_camera.rotateV(vval);
        g_renderCnt++;
    }
    if( bRMousing )
    {
        float hval = 2.0f*(float)(ptCurrentMousePosit.x - ptLastMousePosit.x)/(float)g_winSz[0];
        float vval = -2.0f*(float)(ptCurrentMousePosit.y - ptLastMousePosit.y)/(float)g_winSz[1];
        g_camera.tau = CAMERATAU;
        g_camera.rotateH(hval, g_bCtrl);
        g_camera.move(vval, g_bCtrl);
        g_renderCnt++;
    }
    if( bMMousing )
    {
        float hval = 2.0f*(float)(ptCurrentMousePosit.x - ptLastMousePosit.x)/(float)g_winSz[0];
        float vval = 2.0f*(float)(ptCurrentMousePosit.y - ptLastMousePosit.y)/(float)g_winSz[1];
        g_camera.tau = CAMERATAU;
        g_camera.rotateH(hval, true);
        g_camera.rotateV(vval, true);
        g_renderCnt++;
    }
    
    ptLastMousePosit.x = ptCurrentMousePosit.x;
    ptLastMousePosit.y = ptCurrentMousePosit.y;
}

//------------------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
{
    switch(button)
    {
    case 0:// LEFT
        if(state==0) // Down
        {
            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
            bMousing = true;
            g_renderCnt++;
        }
        else {
            bMousing = false;
            g_renderCnt++;
        }
        break;
    case 2: // Right
        if(state==0)
        {
            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
            bRMousing = true;
            g_renderCnt++;
        }
        else {
            bRMousing = false;
            g_renderCnt++;
        }
        break;
    case 1: // Middle
        if(state==0)
        {
            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
            bMMousing = true;
            g_renderCnt++;
        }
        else {
            bMMousing = false;
            g_renderCnt++;
        }
        break;
    }
}

//------------------------------------------------------------------------------
void shutdown()
{
    nvFX::IContainer::destroy(fx_Effect);
    fx_Effect = NULL;
#ifndef NOGLUT
    exit(0);
#endif
}

//------------------------------------------------------------------------------
void menu(int m)
{

}

//------------------------------------------------------------------------------
#define KEYTAU 0.10f
void specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case /*GLUT_KEY_F1                        */0x0001:
#ifdef SVCSPACEMOUSE
        if(g_pISpaceMouse)
        {
            g_pISpaceMouse->clearObjectTransform();
            g_pISpaceMouse->clearCameraTransform();
        }
#endif
        break;
    case /*GLUT_KEY_F2                        */0x0002:
    case /*GLUT_KEY_F3                        */0x0003:
    case /*GLUT_KEY_F4                        */0x0004:
    case /*GLUT_KEY_F5                        */0x0005:
    case /*GLUT_KEY_F6                        */0x0006:
    case /*GLUT_KEY_F7                        */0x0007:
    case /*GLUT_KEY_F8                        */0x0008:
    case /*GLUT_KEY_F9                        */0x0009:
    case /*GLUT_KEY_F10                       */0x000A:
    case /*GLUT_KEY_F11                       */0x000B:
    case /*GLUT_KEY_F12                       */0x000C:
        g_bCameraMode ^= 1;
        UPDATEUI(g_bCameraMode)
        break;
    case /*GLUT_KEY_LEFT                      */0x0064:
        g_camera.tau = KEYTAU;
        g_camera.rotateH(g_moveStep, g_bCtrl);
        break;
    case /*GLUT_KEY_UP                        */0x0065:
        g_camera.tau = KEYTAU;
        g_camera.rotateV(g_moveStep, g_bCtrl);
        break;
    case /*GLUT_KEY_RIGHT                     */0x0066:
        g_camera.tau = KEYTAU;
        g_camera.rotateH(-g_moveStep, g_bCtrl);
        break;
    case /*GLUT_KEY_DOWN                      */0x0067:
        g_camera.tau = KEYTAU;
        g_camera.rotateV(-g_moveStep, g_bCtrl);
        break;
    case /*GLUT_KEY_PAGE_UP                   */0x0068:
        g_camera.tau = KEYTAU;
        g_camera.move(g_moveStep, g_bCtrl);
        break;
    case /*GLUT_KEY_PAGE_DOWN                 */0x0069:
        g_camera.tau = KEYTAU;
        g_camera.move(-g_moveStep, g_bCtrl);
        break;
    case /*GLUT_KEY_HOME                      */0x006A:
        break;
    case /*GLUT_KEY_END                       */0x006B:
        break;
    case /*GLUT_KEY_INSERT                    */0x006C:
        break;
    }
}
//------------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    switch( key )
    {
        case 0x1b:
#ifdef NOGLUT
            PostQuitMessage(0);
#endif
            shutdownBase();
            shutdown();
            exit(0);
            break;
        case ' ':
            g_realtime.bNonStopRendering = g_realtime.bNonStopRendering ? false : true;
            UPDATEUI(g_realtime.bNonStopRendering);
            break;
        case 'm':
            loadMaterialEffect();
            break;
        //case '1':
        //case '2':
        //case '3':
        //    fx_Tech    = fx_Effect->findTechnique(TECH_DEFAULT+key-'1');
        //    LOGI("Using scene technique %s", fx_Tech->getName());
        //    #ifdef USESVCUI
        //    if(g_pComboTech) g_pComboTech->SetSelectedByIndex(key-'1');
        //    #endif
        //    break;
        case '1':
        case '2':
        case '3':
            g_uniformUpdateMode = (UpdateMode)(key-'1');
            fx_Tech = fx_Effect->findTechnique("uniformBlock");
            #ifdef USESVCUI
            if(g_pComboUniformUpdate) g_pComboUniformUpdate->SetSelectedByIndex(key-'1');
            #endif
            break;
        case '4':
            g_uniformUpdateMode = (UpdateMode)(key-'1');
            fx_Tech = fx_Effect->findTechnique("Uniforms"); // use another approach
            #ifdef USESVCUI
            if(g_pComboUniformUpdate) g_pComboUniformUpdate->SetSelectedByIndex(key-'1');
            #endif
            break;
    }
}

//------------------------------------------------------------------------------
void idle()
{
    //
    // update the motion of the camera
    //
    bool bContinue = g_camera.update((float)g_realtime.frameDT);
    //
    // timing for frames
    //
    g_realtime.update(bContinue);

    if (g_forceRender || bContinue)
        g_renderCnt++;
#ifndef NOGLUT
    if(g_renderCnt > 0)
    {
        g_renderCnt--;
        glutPostRedisplay();
    }
#endif

}

//------------------------------------------------------------------------------
void errorCallbackFunc(const char *errMsg)
{
#ifdef WIN32
    OutputDebugString(errMsg);
#endif
    printf(errMsg);
}
void includeCallbackFunc(const char *incName, FILE *&fp, const char *&buf)
{
    char fullpath[200];
    fp = fopen("incName", "r");
    if(fp) return;

    sprintf(fullpath, SOURCE_DIR "/%s", incName);
    fp = fopen(fullpath, "r");
    if(fp) return;

}

//------------------------------------------------------------------------------
void initSimpleMesh()
{
    int bufSz = sizeof(VtxLayout)*((VSTEPS-2)*HSTEPS + 2);
    int eltsSz = sizeof(int)*((VSTEPS-2)*(HSTEPS)*6 + (HSTEPS*3*2));
    int *elts = (int *)malloc(eltsSz);
    int *pelts = elts;
    VtxLayout *vertices = (VtxLayout *)malloc(bufSz);
    VtxLayout *pvtx = vertices;

    pvtx->Pos = vec3(0,1,0);
    pvtx->N = pvtx->Pos;
    pvtx++;
    for(int iy=1; iy<(VSTEPS-1); iy++)
    {
        nv_scalar y = nv_pi * ((nv_scalar)iy/(nv_scalar)VSTEPS);
        float r = sin(y);
        y = cos(y);
        for(int ix=0; ix<HSTEPS; ix++)
        {
            nv_scalar a = nv_two_pi * (nv_scalar)ix/(nv_scalar)HSTEPS;
            pvtx->Pos = vec3(r*cos(a), y, r*sin(a));
            pvtx->N = pvtx->Pos;
            pvtx++;
            if(iy==1)
            {
                *pelts++ = 0;
                *pelts++ = ((iy-1)*HSTEPS)+ix+1;
                *pelts++ = ((iy-1)*HSTEPS)+((ix+1)%HSTEPS)+1;
            } else {
                *pelts++ = ((iy-1)*HSTEPS)+ix+1;
                *pelts++ = ((iy-1)*HSTEPS)+((ix+1)%HSTEPS)+1;
                *pelts++ = ((iy-2)*HSTEPS)+ix+1;

                *pelts++ = ((iy-2)*HSTEPS)+((ix+1)%HSTEPS)+1;
                *pelts++ = ((iy-2)*HSTEPS)+ix+1;
                *pelts++ = ((iy-1)*HSTEPS)+((ix+1)%HSTEPS)+1;
            }
        }
        if(iy == VSTEPS-2)
            for(int ix=0; ix<HSTEPS; ix++)
            {
                *pelts++ = (HSTEPS*(VSTEPS-2)+1);
                *pelts++ = ((iy-1)*HSTEPS)+((ix+1)%HSTEPS)+1;
                *pelts++ = ((iy-1)*HSTEPS)+ix+1;
            }
    }
    pvtx->Pos = vec3(0,-1,0);
    pvtx->N = pvtx->Pos;
    pvtx++;
    glGenBuffers(1, &g_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glBufferData(GL_ARRAY_BUFFER, bufSz, vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &g_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, eltsSz, elts, GL_STATIC_DRAW);
    free(elts);
    free(vertices);
}
//------------------------------------------------------------------------------
void initGL()
{
    //--------------------------------------------------------------------------
#ifdef USESVCUI
    LOGI("adding more UI controls...\n");
    class ControlsEvents : public IEventsWnd
    {
        void ScalarChanged(IControlScalar *pWin, float &v, float prev)
        {
            if(!strcmp(pWin->GetID(), "NMESH"))
                updateTransforms();
            g_renderCnt++;
        };
        void Button(IWindow *pWin, int pressed) 
        {
            size_t tag;
            pWin->GetUserData(&tag);
            keyboard(tag, 0,0);
        };
        void ToolbarChanged(IControlToolbar *pWin, int selecteditem, int prev) 
        {
            int states;
            size_t tag;
            int ddsidx;
            pWin->GetItemInfos(selecteditem, states, tag, NULL, 0, NULL, 0, ddsidx);
            keyboard(tag, 0,0);
        }
        void CheckBoxChanged(IControlScalar *pWin, bool &value, bool prev)
        {
            g_renderCnt++;
        }
        //void ComboSelectionChanged(IControlCombo *pWin, unsigned int selectedidx)
        //{
        //    fx_Tech    = fx_Effect->findTechnique(selectedidx+TECH_DEFAULT);
        //    if(fx_Tech == NULL)
        //        fx_Tech    = fx_Effect->findTechnique(TECH_DEFAULT);
        //    g_renderCnt++;
        //};
    };
    static ControlsEvents controlsEvents;
    //---------------------------------------------------------------------------
    if(g_pWinHandler)
    {
        g_pControls =  g_pWinHandler->CreateWindowFolding("CTRLS", "Controls");
        g_pWinHandler->CreateCtrlLabel("FPS", "famerate", g_pControls);

        g_pComboUniformUpdate = g_pWinHandler->CreateCtrlCombo("UUPDATE", "Uniforms", g_pControls);
        g_pComboUniformUpdate->AddItem("nvFx funcs", USECSTBUFUNIFORMS);
        g_pComboUniformUpdate->AddItem("Buffer Subdata", USEBUFFERSUBDATA);
        g_pComboUniformUpdate->AddItem("Map Buffer", USEMAPBUFFER);
        g_pWinHandler->VariableBind(g_pComboUniformUpdate, (unsigned int*)&g_uniformUpdateMode);

        //g_pComboTech = g_pWinHandler->CreateCtrlCombo("MTECH", "Techniques", g_pControls);
        g_pWinHandler->CreateCtrlButton("LM", "Reload Materials", g_pControls)->SetUserData(NULL, 'm');
        g_pWinHandler->VariableBind(g_pWinHandler->CreateCtrlCheck("RT", "Render Realtime", g_pControls), &g_realtime.bNonStopRendering);

        g_pWinHandler->VariableBind(
            g_pWinHandler->CreateCtrlScalar("Elts", "Elts", g_pControls)->SetBounds(0.0, VSTEPS-2)->SetIntMode()
            , &g_eltsLevel);
        g_pWinHandler->VariableBind(
            g_pWinHandler->CreateCtrlScalar("NMESH", "Num Meshes", g_pControls)->SetBounds(1, MAXMESHES-1)->SetIntMode()
            , &g_numMeshes);
        g_pControls->SetVisible(1)->SetLocation(g_winSz[0]+16,0)->SetSize(250,g_winSz[1]);
        g_pWinHandler->Register(&controlsEvents);
    }
#endif
    //---------------------------------------------------------------------------
    glewInit();
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // Bind only one vao
    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    nvFX::setErrorCallback(errorCallbackFunc);
    nvFX::setIncludeCallback(includeCallbackFunc);
    loadMaterialEffect();
    initSimpleMesh();
    updateTransforms();

    LOGI("Pg Up/Down : zoom\n");
    LOGI("Arrows: rotate the camera\n");
#ifdef NOGLUT
    LOGI("Ctrl + Arrows: pan the camera taget\n");
#endif
    LOGI("Mouse + left button: rotate the camera\n");
    LOGI("Mouse + middle button: Pan the camera target\n");
    LOGI("Mouse + right button: rotate Horizontally and change camera distance\n");
    LOGI("space: toggle redraw on every frame\n");
    LOGI("1,2,3 : switch between techniques\n");
    LOGI("m : reload material effect\n");

    //
    // Timer Query
    //
    glGenQueries(4, timerQueries);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool loadMaterialEffect()
{
    LOGI("Creating Effect (OpenGLUniformUpdateExample)\n");
    fx_Effect = nvFX::IContainer::create("OpenGLUniformUpdateExample");
    bool bRes = nvFX::loadEffectFromFile(fx_Effect, "OpenGLUniformUpdateExample.glslfx");
    if(!bRes)
        bRes = nvFX::loadEffectFromFile(fx_Effect, SOURCE_DIR "/OpenGLUniformUpdateExample.glslfx");
    if(!bRes)
    {
        LOGE("Failed\n");
    }
    else
    {
        LOGOK("Loaded\n");
        //
        // Let's keep track in interface pointers everything, for purpose of clarity
        //
        #ifdef USESVCUI
        //if(g_pComboTech) g_pComboTech->RemoveAllItems();
        #endif
        for(int t=0; fx_Tech = fx_Effect->findTechnique(t); t++)
        {
            fx_Tech->validate();
            #ifdef USESVCUI
            // avoid the tech #0 and #1: only some init for states...
            //if(t>=TECH_DEFAULT) if(g_pComboTech) g_pComboTech->AddItem(fx_Tech->getName(), t);
            #endif
        }
        #ifdef USESVCUI
        //if(g_pComboTech) g_pComboTech->SetSelectedByIndex(1);
        #endif
        // set the material for the model
        fx_Tech = fx_Effect->findTechnique("uniformBlock");
        fx_TechFloor = fx_Effect->findTechnique(TECH_WIREFLOOR);

        // execute the pass as a way to initialize some basic GL states
        nvFX::ITechnique* fx_TechInit = fx_Effect->findTechnique(TECH_INIT);
        fx_TechInit->getPass(0)->execute();

        fx_transfBlock1 = fx_Effect->findCstBuffer("transfBlock1");
        fx_transfBlock1->buildGLBuffer(nvFX::ICstBuffer::STATIC_DRAW); // ask nvFx to take care of the UBO creation
        fx_transfBlock2 = fx_Effect->findCstBuffer("transfBlock2");
        fx_transfBlock2->buildGLBuffer(nvFX::ICstBuffer::STATIC_DRAW); // ask nvFx to take care of the UBO creation
        // example of other way to update: let's track the uniforms inside this buffer.
        {
            fx_m4_World         = fx_transfBlock2->findUniform("m4_World");
            fx_m4_WorldView     = fx_transfBlock2->findUniform("m4_WorldView");
            fx_m4_WorldViewProj = fx_transfBlock2->findUniform("m4_WorldViewProj");
            fx_m4_WorldIT       = fx_transfBlock2->findUniform("m4_WorldIT");
        }
        // gathering uniforms
        fx_u_m4_World         = fx_Effect->findUniform("u_m4_World");
        fx_u_m4_WorldView     = fx_Effect->findUniform("u_m4_WorldView");
        fx_u_m4_WorldViewProj = fx_Effect->findUniform("u_m4_WorldViewProj");
        fx_u_m4_WorldIT       = fx_Effect->findUniform("u_m4_WorldIT");

        fx_materialBlock = fx_Effect->findCstBuffer("materialBlock");
        fx_materialBlock->buildGLBuffer(nvFX::ICstBuffer::STREAM_DRAW); // ask nvFx to take care of the UBO creation
    }
    return true;
}

//------------------------------------------------------------------------------
// update Transformations
//------------------------------------------------------------------------------
void updateTransforms()
{
    float wf = powf(g_numMeshes, 1.0/3.0)+0.5;
    int w = (int)wf;
    wf = 1.0f/wf;
    int cnt = 0;
    for(int x = 0; x<w; x++)
    {
        for(int y = 0; y<w; y++)
        {
          for(int z = 0; z<w; z++)
          {
            if(cnt >= g_numMeshes)
                break;
            mat4 m4_world1;
            g_transfBlock2[cnt].m4_World.identity();
            g_transfBlock2[cnt].m4_World.scale(wf * 0.2f);
            g_transfBlock2[cnt].m4_World.translate(vec3((float)(2*x-w), (float)(2*y-w), (float)(2*z-w)));
            g_transfBlock2[cnt].m4_WorldView = g_transfBlock1.m4_View * g_transfBlock2[cnt].m4_World;
            g_transfBlock2[cnt].m4_WorldViewProj = g_transfBlock1.m4_Proj * g_transfBlock2[cnt].m4_WorldView;
            mat4 WI;
            invert(WI, g_transfBlock2[cnt].m4_World);
            transpose(g_transfBlock2[cnt].m4_WorldIT, WI);
            cnt++;
          }
        }
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void draw1()
{
    //NXPROFILEFUNCCOL2("draw1", COLOR_ORANGE, 11);
    float wf = powf(g_numMeshes, 1.0/3.0)+0.5;
    int w = (int)wf;
    wf = 1.0f/wf;

    updateTransforms();

    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VtxLayout), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VtxLayout), (void*)sizeof(vec3));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
    int nElts = HSTEPS*3;//((VSTEPS-2)*(HSTEPS)*6) + (HSTEPS*3*2);
    nElts += (g_eltsLevel)*((HSTEPS)*6);
    if(g_eltsLevel == VSTEPS-2)
        nElts += HSTEPS*3;
    int cnt = 0;
    for(int x = 0; x<w; x++)
    {
        for(int y = 0; y<w; y++)
        {
          for(int z = 0; z<w; z++)
          {
            if(cnt >= g_numMeshes)
                break;
            switch(g_uniformUpdateMode)
            {
            case USEUNIFORMS:
                fx_u_m4_World->updateMatrix4f(g_transfBlock2[cnt].m4_World.mat_array, fx_pass);
                fx_u_m4_WorldView->updateMatrix4f(g_transfBlock2[cnt].m4_WorldView.mat_array, fx_pass);
                fx_u_m4_WorldViewProj->updateMatrix4f(g_transfBlock2[cnt].m4_WorldViewProj.mat_array, fx_pass);
                break;
            case USECSTBUFUNIFORMS:
                if(fx_transfBlock2)
                {
                    fx_m4_World->setMatrix4f(g_transfBlock2[cnt].m4_World.mat_array);
                    fx_m4_WorldView->setMatrix4f(g_transfBlock2[cnt].m4_WorldView.mat_array);
                    fx_m4_WorldViewProj->setMatrix4f(g_transfBlock2[cnt].m4_WorldViewProj.mat_array);
                    fx_transfBlock2->update();
                }
                break;
            case USEBUFFERSUBDATA:
                if(fx_transfBlock2)
                {
                    glBindBuffer(GL_UNIFORM_BUFFER, fx_transfBlock2->getGLBuffer());
                    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(transfBlock2), &g_transfBlock2[cnt]);
                    glBindBuffer(GL_UNIFORM_BUFFER, 0);
                }
                break;
            case USEMAPBUFFER:
                if(fx_transfBlock2)
                {
                    void* p;
                    fx_transfBlock2->mapBuffer(&p);
                    memcpy(p, &g_transfBlock2[cnt], sizeof(transfBlock2));
                    fx_transfBlock2->unmapBuffer();
                }
                break;
            } // switch
            glDrawElements(GL_TRIANGLES, nElts, GL_UNSIGNED_INT, 0);
            cnt++;
          }
        }
    }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, g_winSz[0], g_winSz[1]);
//SwapBuffers( g_hDC );
//return;
    //
    // setup the block1 with base matrices
    //
    // g_transfBlock1.m4_Proj already done
    vec3 up(0,1,0);
    look_at(g_transfBlock1.m4_View, g_camera.curEyePos, g_camera.curFocusPos, up);
    //g_transfBlock1.m4_ViewIT = ...todo
    g_transfBlock1.m4_ViewProj = g_transfBlock1.m4_Proj * g_transfBlock1.m4_View;
    g_transfBlock1.eyePos = g_camera.curEyePos;
    // copy the block to OGL
    if(fx_transfBlock1)
    {
        void* p;
        fx_transfBlock1->mapBuffer(&p);
        memcpy(p, &g_transfBlock1, sizeof(transfBlock1));
        fx_transfBlock1->unmapBuffer();
    }
    //-----------------------------------------------------------------
    //
    // Render a basic floor
    //
    //glBeginQuery(GL_TIME_ELAPSED, timerQueries[tqOffset]);
#define U 1.0f
#define DU 0.1f
    struct Grid
    {
        Grid() {
            Elts = 0;
            for(float i=-U; i<=(U+DU); i+=DU)
            {
                vtx[Elts++] = vec3(-U, 0, i);
                vtx[Elts++] = vec3( U, 0, i);
                vtx[Elts++] = vec3(i, 0,-U);
                vtx[Elts++] = vec3(i, 0, U);
            }
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*(10*10+2), vtx[0].vec_array, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        int     Elts;
        GLuint  vbo;
        vec3    vtx[10*10+2];
    };
    static Grid sgrid;
    if(fx_TechFloor)
    {
        fx_pass = fx_TechFloor->getPass(0);
        fx_pass->execute();
    }
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, sgrid.vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glDrawArrays(GL_LINES, 0, sgrid.Elts);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(fx_pass)
        fx_pass->unbindProgram();
    fx_pass = NULL;

    //glEndQuery(GL_TIME_ELAPSED);
    //
    // Mesh rendering
    //
    //glBeginQuery(GL_TIME_ELAPSED, timerQueries[tqOffset+1]);

    if(fx_Tech)
    {
        fx_pass = fx_Tech->getPass(0);
        fx_pass->execute();
    }
    //===========> Draw the generated mesh
    draw1();
    //====================================

    if(fx_pass)
        fx_pass->unbindProgram();

    fx_pass = NULL;

    //glEndQuery(GL_TIME_ELAPSED);

#ifdef NOGLUT
    SwapBuffers( g_hDC );
#else
    glutSwapBuffers();
#endif

    //
    // Timer Query results
    //
    //tqOffset = tqOffset ? 0 : 2; // alternate between 2 groups
    //if(tqStart) // special case of the first render
    //    tqStart = false;
    //else
    //{
    //    int available = 0;
    //    while (!available)
    //    {
    //        glGetQueryObjectiv(timerQueries[tqOffset+1], GL_QUERY_RESULT_AVAILABLE, &available);
    //    }
    //    GLuint64 timeElapsed;
    //    for (int i = 0; i < 2; i++)
    //    {
    //        // See how much time the rendering of object i took in nanoseconds.
    //        glGetQueryObjectui64v(timerQueries[tqOffset+i], GL_QUERY_RESULT, &timeElapsed);
    //    }
    //}
}
//------------------------------------------------------------------------------
