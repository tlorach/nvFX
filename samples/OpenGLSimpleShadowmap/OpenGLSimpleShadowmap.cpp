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

#define SCENE_EFFECT "OpenGLSimpleShadowmap.glslfx"

//-------------- Optional User interface
#ifdef USESVCUI
#include "ISvcUI.h"
#endif
//-------------- Effect system
#include "FxParser.h"
//
// stuff effect, tech and pass interfaces
//
nvFX::IContainer*   fx_EffectScene  = NULL;

nvFX::ITechnique*   fx_TechScene    = NULL;
nvFX::IPass*        fx_pass         = NULL;
nvFX::ICstBuffer*   fx_transfBlock1 = NULL;
nvFX::ICstBuffer*   fx_transfBlock2 = NULL;
nvFX::IUniform*     fx_scaleBias  = NULL;

// data structure reflecting buffers in shaders
struct transfBlock1
{
    mat4 m4_ViewProj;
    mat4 m4_Proj;
    mat4 m4_View;
    mat4 m4_ViewIT;
    mat4 m4_shadowMatrix;
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
transfBlock2 g_transfBlock2;
vec4 scaleBias;

// Light info
vec3 up(0,1,0);
vec3 g_lightTarget(0,0,0);
vec3 g_lightPos(1,0.5,-0.21);
mat4 g_lightProjection;
mat4 g_lightView;
mat4 g_lightViewProj;
mat4 g_lightViewProjScaleBias;

//-------------- Basic UI defined in main.cpp
#ifdef USESVCUI
#include "ISvcUI.h"
extern IWindowHandler*      g_pWinHandler;
extern IWindowConsole*      g_pConsole;
extern IWindowLog*          g_pLog;
extern IProgressBar*        g_pProgress;

IWindowContainer*           g_pControls     = NULL;
IControlCombo*              g_pComboTech    = NULL;

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
mat4        g_cameraProj;

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

//---------------- bk3d Mesh
#include <zlib.h>
#include "bk3dEx.h"
bk3d::FileHeader * meshFile;

//---------------------- other params
float g_posOffset[3]= {0,0,0};
float g_fSpinX = 0.0;
float g_scale = 1.0;

//-------------------- VAO (to be Core OpenGL compliant :-( )
GLuint g_vao;
//-------------------- Timer Query
GLuint  timerQueries[4];
int     tqOffset = 0;
bool    tqStart = true;
//-------------------- forward decalarations
bool loadSceneEffect();
//------------------------------------------------------------------------------
void reshape(int w, int h)
{
    g_winSz[0] = w; 
    g_winSz[1] = h;

    //
    // Let's validate again the base of resource management to make sure things keep consistent
    //
    int W = g_winSz[0];
    int H = g_winSz[1];

    glViewport(0, 0, W, H);

    perspective(g_cameraProj, 45.0f, (float)g_winSz[0] / (float)g_winSz[1], 0.01f, 10.0f);

    nvFX::getResourceRepositorySingleton()->setParams(0,0,W,H,1,0,NULL );
    bool failed = nvFX::getResourceRepositorySingleton()->validateAll() ? false : true;
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
#else
            shutdownBase();
            shutdown();
            exit(0);
#endif
            break;
        case 'a':
            g_scale *= 0.5f;
            UPDATEUI(g_scale)
            break;
        case 'q':
            g_scale *= 2.0f;
            UPDATEUI(g_scale)
            break;
        case ' ':
            g_realtime.bNonStopRendering = g_realtime.bNonStopRendering ? false : true;
            UPDATEUI(g_realtime.bNonStopRendering);
            break;
        case '1':
            fx_TechScene    = fx_EffectScene->findTechnique(0);
            LOGI("Using scene technique %s", fx_TechScene->getName());
            #ifdef USESVCUI
            if(g_pComboTech) g_pComboTech->SetSelectedByIndex(0);
            #endif
            break;
        //case '2':
        //    fx_TechScene    = fx_EffectScene->findTechnique(1);
        //    LOGI("Using scene technique %s", fx_TechScene->getName());
        //    #ifdef USESVCUI
        //    if(g_pComboTech) g_pComboTech->SetSelectedByIndex(1);
        //    #endif
        //    break;
        //case '3':
        //    fx_TechScene    = fx_EffectScene->findTechnique(2);
        //    LOGI("Using scene technique %s", fx_TechScene->getName());
        //    #ifdef USESVCUI
        //    if(g_pComboTech) g_pComboTech->SetSelectedByIndex(2);
        //    #endif
        //    break;
        case 's':
            loadSceneEffect();
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
    fp = fopen(incName, "r");
    if(fp) return;

    sprintf(fullpath, SOURCE_DIR "/%s", incName);
    fp = fopen(fullpath, "r");
    if(fp) return;

    sprintf(fullpath, RESOURCE_DIR "/%s", incName);
    fp = fopen(fullpath, "r");
    if(fp) return;

    OutputDebugString("Error: Include file not found : ");
    OutputDebugString(incName);
    OutputDebugString("\n");
}

//-----------------------------------------------------------------------------
// Load scene effect
//-----------------------------------------------------------------------------
bool loadSceneEffect()
{
    bool failed = false;
    if(fx_EffectScene)
    {
        LOGI("=========> Destroying effect\n");
        nvFX::IContainer::destroy(fx_EffectScene);
        fx_EffectScene = NULL;
    }
    //
    // effect for scene-leve
    //
    LOGI("=========> Create the effect\n");
    fx_EffectScene = nvFX::IContainer::create();
    if(!fx_EffectScene)
        return false;
    bool bRes = nvFX::loadEffectFromFile(fx_EffectScene, SCENE_EFFECT);
    if(!bRes)
        bRes = nvFX::loadEffectFromFile(fx_EffectScene, SOURCE_DIR "/" SCENE_EFFECT);
    if(!bRes)
        bRes = nvFX::loadEffectFromFile(fx_EffectScene, RESOURCE_DIR "/" SCENE_EFFECT);

#ifdef USESVCUI
    if(g_pComboTech) g_pComboTech->RemoveAllItems();
#endif
    for(int t=0; fx_TechScene = fx_EffectScene->findTechnique(t); t++)
    {
        fx_TechScene->validate();
#ifdef USESVCUI
        if(g_pComboTech) g_pComboTech->AddItem(fx_TechScene->getName(), t);
#endif
    }
#ifdef USESVCUI
        if(g_pComboTech) g_pComboTech->SetSelectedByIndex(0);
#endif
    fx_TechScene    = fx_EffectScene->findTechnique(0); // take the one right after the floor technique
    fx_pass         = NULL;

    LOGI("=========> Resource setup\n");
    //
    // Some resources could have been created from the effect and added to the resource repository
    //
    int fboId = 0;
    int W = g_winSz[0];
    int H = g_winSz[1];
    nvFX::getResourceRepositorySingleton()->setParams(0,0,W,H,1,0,(void*)(fboId) );
    if(!nvFX::getResourceRepositorySingleton()->validateAll())
        return false;

    fx_transfBlock1 = fx_EffectScene->findCstBuffer("transfBlock1");
    fx_transfBlock1->buildGLBuffer(); // ask nvFx to take care of the UBO creation
    fx_transfBlock2 = fx_EffectScene->findCstBuffer("transfBlock2");
    fx_transfBlock2->buildGLBuffer();
    fx_scaleBias = fx_EffectScene->findUniform("scaleBias");
    if(fx_scaleBias)
        fx_scaleBias->setValue4f(0, 0, 0, g_scale);

    return failed ? false : true;
}
//-----------------------------------------------------------------------------
// Load scene effect
//-----------------------------------------------------------------------------
void loadModel()
{
    // Load a model
    const char* p = RESOURCE_DIR;
    //---------------
    #define MODEL "gargoyle_v134.bk3d.gz"
    //---------------
    LOGI("Loading Mesh... " MODEL "\n");
    meshFile = bk3d::load(MODEL);
    if(!meshFile)
    {
        LOGI("Trying " RESOURCE_DIR "/" MODEL "\n");
        //if(!(meshFile = bk3d::load(RESOURCE_DIR MODEL)))
        meshFile = bk3d::load(RESOURCE_DIR "/" MODEL);
    }
    if(!meshFile)
    {
        LOGE("error in loading mesh\n");
        return;
    }
    // To be Core OpenGL, we must create some buffer objects
    // Check the bounding boxes and adjust scaling
    float min[3] = {10000.0, 10000.0, 10000.0};
    float max[3] = {-10000.0, -10000.0, -10000.0};
    for(int i=0; i<meshFile->pMeshes->n; i++)
    {
        bk3d::Mesh *pMesh = meshFile->pMeshes->p[i];
        // walk through the slots (== buffers of attributes) and create a VBO for it
        // bk3d already has all needed for a quick setup
        for(int j=0; j<pMesh->pSlots->n; j++)
        {
            bk3d::Slot* pSlot = pMesh->pSlots->p[j];
            glGenBuffers(1, (unsigned int*)&pSlot->userData); // store directly in the structure... easier
            glBindBuffer(GL_ARRAY_BUFFER, pSlot->userData);
            glBufferData(GL_ARRAY_BUFFER, pSlot->vtxBufferSizeBytes, pSlot->pVtxBufferData, GL_STATIC_DRAW);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        for(int j=0; j<pMesh->pPrimGroups->n; j++)
        {
            bk3d::PrimGroup* pPG = pMesh->pPrimGroups->p[j];
            // the element buffer can be shared between many primitive groups
            // the main owner is specified by "pOwnerOfIB"
            // if no pOwnerOfIB or if pOwnerOfIB is itself, then we create the buffer
            // later, the indexArrayByteOffset will tell where to fetch indices
            if((pPG->pOwnerOfIB == pPG) || (pPG->pOwnerOfIB == NULL))
            {
                glGenBuffers(1, (unsigned int*)&pPG->userPtr); // store directly in the structure... easier
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)pPG->userPtr);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, pPG->indexArrayByteSize, pPG->pIndexBufferData, GL_STATIC_DRAW);
            } else {
                // here the primitive group uses a subset of an existing buffer
                // we always assume the owner was first in the list of primgroups
                pPG->userPtr = pPG->pOwnerOfIB->userPtr;
            }
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        // NOTE: to be accurate, we should go through primitive groups
        // see if this prim group used a transformation. If so, we should
        // take its bounding box into account...
        // For now, let's only take the Mesh's bbox
        mat4 matModel;
        vec3 meshMin(pMesh->aabbox.min);
        vec3 meshMax(pMesh->aabbox.max);
        if(pMesh->pTransforms && pMesh->pTransforms->n == 1)
            matModel = mat4(pMesh->pTransforms->p[0]->MatrixAbs());
        else
            matModel.identity();
        meshMin = matModel * meshMin;
        meshMax = matModel * meshMax;
        if(meshMin[0] < min[0]) min[0] = meshMin[0];
        if(meshMin[1] < min[1]) min[1] = meshMin[1];
        if(meshMin[2] < min[2]) min[2] = meshMin[2];
        if(meshMax[0] > max[0]) max[0] = meshMax[0];
        if(meshMax[1] > max[1]) max[1] = meshMax[1];
        if(meshMax[2] > max[2]) max[2] = meshMax[2];
    }
    g_posOffset[0] = (max[0] + min[0])*0.5f;
    g_posOffset[1] = (max[1] + min[1])*0.5f;
    g_posOffset[2] = (max[2] + min[2])*0.5f;
    float bigger = 0;
    if((max[0]-min[0]) > bigger) bigger = (max[0]-min[0]);
    if((max[1]-min[1]) > bigger) bigger = (max[1]-min[1]);
    if((max[2]-min[2]) > bigger) bigger = (max[2]-min[2]);
    if((bigger) > 0.001)
    {
        g_scale = 1.0f / bigger;
        LOGI("Scaling the model by %f...\n", g_scale);
    }
    if(fx_scaleBias)
        fx_scaleBias->setValue4f(0, 0, 0, g_scale);
        //fx_scaleBias->setValue4f(g_posOffset[0], g_posOffset[1], g_posOffset[2], g_scale);
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
            g_renderCnt++;
        };
        void Button(IWindow *pWin, int pressed) 
        {
            size_t tag;
            pWin->GetUserData(&tag);
            keyboard((unsigned char)tag, 0,0);
        };
        void ToolbarChanged(IControlToolbar *pWin, int selecteditem, int prev) 
        {
            int states;
            size_t tag;
            int ddsidx;
            pWin->GetItemInfos(selecteditem, states, tag, NULL, 0, NULL, 0, ddsidx);
            keyboard((unsigned char)tag, 0,0);
        }
        void CheckBoxChanged(IControlScalar *pWin, bool &value, bool prev)
        {
            g_renderCnt++;
        }
	    void ComboSelectionChanged(IControlCombo *pWin, unsigned int selectedidx)
        {
            if(!strcmp(pWin->GetID(), "SCTECH"))
            {
                fx_TechScene    = fx_EffectScene->findTechnique(selectedidx);
            }
            else if(!strcmp(pWin->GetID(), "MTECH"))
            {
                // ...
            }
            g_renderCnt++;
        };
    };
    static ControlsEvents controlsEvents;
    //---------------------------------------------------------------------------
    if(g_pWinHandler)
    {
        g_pControls =  g_pWinHandler->CreateWindowFolding("CTRLS", "Controls");

        g_pComboTech = g_pWinHandler->CreateCtrlCombo("SCTECH", "Scene Tech", g_pControls);
        g_pWinHandler->CreateCtrlButton("LM", "Reload Materials", g_pControls)->SetUserData(NULL, 'm');
        g_pWinHandler->CreateCtrlButton("LS", "Reload Scene Effect", g_pControls)->SetUserData(NULL, 's');
        g_pWinHandler->VariableBind(g_pWinHandler->CreateCtrlCheck("RT", "Render Realtime", g_pControls), &g_realtime.bNonStopRendering);

        g_pControls->SetVisible(1)->SetLocation(g_winSz[0]+16,0)->SetSize(250,g_winSz[1]);
        // simplest solution: register the whole to one event manager
        g_pWinHandler->Register(&controlsEvents);
    }
#endif
    //---------------------------------------------------------------------------
    glewInit();
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // Bind only one vao
    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    //
    // Effects
    //
    nvFX::setErrorCallback(errorCallbackFunc);
    nvFX::setMessageCallback(errorCallbackFunc);
    nvFX::setIncludeCallback(includeCallbackFunc);
    loadSceneEffect();
    loadModel();
    LOGI("Pg Up/Down : zoom\n");
    LOGI("Arrows: rotate the camera\n");
#ifdef NOGLUT
    LOGI("Ctrl + Arrows: pan the camera taget\n");
#endif
    LOGI("Mouse + left button: rotate the camera\n");
    LOGI("Mouse + middle button: Pan the camera target\n");
    LOGI("Mouse + right button: rotate Horizontally and change camera distance\n");
    LOGI("space: toggle redraw on every frame\n");
    LOGI("1,2,3 : switch between scene techniques\n");
    LOGI("m/s : reload material/scene effect\n");

    //
    // Timer Query
    //
    glGenQueries(4, timerQueries);
    //
    // Light... nothing special: it's a static light for this sample
    //
    perspective(g_lightProjection, 35.0f, 1.0f, 0.01f, 10.0f);
    look_at(g_lightView, g_lightPos, g_lightTarget, up);
    g_lightViewProj = g_lightProjection * g_lightView;
 	mat4 offsetMat = mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);
   g_lightViewProjScaleBias = offsetMat * g_lightViewProj;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void displayScene(nvFX::PassInfo &pr)
{
    g_transfBlock2.m4_World.identity();
    g_transfBlock2.m4_WorldView = g_transfBlock1.m4_View * g_transfBlock2.m4_World;
    g_transfBlock2.m4_WorldViewProj = g_transfBlock1.m4_Proj * g_transfBlock2.m4_WorldView;

    if(!meshFile)
    {
#ifdef NOGLUT
    SwapBuffers( g_hDC );
#else
    glutSwapBuffers();
#endif
        return;
    }
    //g_transfBlock2.m4_WorldIT = ... todo;
    if(fx_transfBlock2)
    {
        void* p;
        fx_transfBlock2->mapBuffer(&p);
        memcpy(p, &g_transfBlock2, sizeof(transfBlock2));
        fx_transfBlock2->unmapBuffer();
    }
    for(int i=0; i< meshFile->pMeshes->n; i++)
    {
        bk3d::Mesh *pMesh = meshFile->pMeshes->p[i];
        // case where the mesh references a transformation
        // the absolute value must be available by default
        // if more than one transf, skip it : this might be a list of skeleton transformations
        if(pMesh->pTransforms && pMesh->pTransforms->n == 1)
        {
            g_transfBlock2.m4_World = mat4(pMesh->pTransforms->p[0]->MatrixAbs());
            g_transfBlock2.m4_WorldView = g_transfBlock1.m4_View * g_transfBlock2.m4_World;
            g_transfBlock2.m4_WorldViewProj = g_transfBlock1.m4_Proj * g_transfBlock2.m4_WorldView;
            //g_transfBlock2.m4_WorldIT = ... todo;
            if(fx_transfBlock2)
            {
                void* p;
                fx_transfBlock2->mapBuffer(&p);
                memcpy(p, &g_transfBlock2, sizeof(transfBlock2));
                fx_transfBlock2->unmapBuffer();
            }
        }
        //
        // let's make it very simple : each mesh attribute is *supposed* to match
        // the attribute Id of the effect. In real, meshes might not always match
        // but this is totally arbitrary...
        // we assume the mesh got baked as follow:
        // 0: would be pos; 1: normal; 2: TC; 3 and 4: Tan Binorm
        //
        int j = 0;
        for(int k=0; k<pMesh->pSlots->n;k++)
        {
            bk3d::Slot* pSlot = pMesh->pSlots->p[k];
            glBindBuffer(GL_ARRAY_BUFFER, pSlot->userData);
            // assign buffers
            for(int l=0; l<pSlot->pAttributes->n; l++)
            {
                glEnableVertexAttribArray(j);
                glVertexAttribPointer(j,
                    pSlot->pAttributes->p[l].p->numComp, 
                    pSlot->pAttributes->p[l].p->formatGL, GL_FALSE,
                    pSlot->pAttributes->p[l].p->strideBytes,
                    (void*)pSlot->pAttributes->p[l].p->dataOffsetBytes);
                j++;
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        int MaxAttr = 16; //glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MaxAttr);
        for(; j<MaxAttr;j++)
            glDisableVertexAttribArray(j);

        for(int pg=0; pg<pMesh->pPrimGroups->n; pg++)
        {
            bk3d::PrimGroup* pPG = pMesh->pPrimGroups->p[pg];
            //if( ( (pPG->topologyGL == GL_LINES)
            //   ||(pPG->topologyGL == GL_LINE_LOOP)
            //   ||(pPG->topologyGL == GL_LINE_STRIP)))
            //{            }
            // case where the Primitive group references a transformation
            // the absolute value must be available by default
            if(pPG->pTransforms && pPG->pTransforms->n > 0)
            {
                g_transfBlock2.m4_World = mat4(pPG->pTransforms->p[0]->MatrixAbs());
                g_transfBlock2.m4_WorldView = g_transfBlock1.m4_View * g_transfBlock2.m4_World;
                g_transfBlock2.m4_WorldViewProj = g_transfBlock1.m4_Proj * g_transfBlock2.m4_WorldView;
                //g_transfBlock2.m4_WorldIT = ... todo;
                if(fx_transfBlock2)
                {
                    void* p;
                    fx_transfBlock2->mapBuffer(&p);
                    memcpy(p, &g_transfBlock2, sizeof(transfBlock2));
                    fx_transfBlock2->unmapBuffer();
                }
            }

            bk3d::Material *pMat = pPG->pMaterial;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)pPG->userPtr);
            glDrawElements(
                pPG->topologyGL,
                pPG->indexCount,
                pPG->indexFormatGL,
                (void*)pPG->indexArrayByteOffset);// pIndexBufferData);
        }

    }
    if(fx_pass)
        fx_pass->unbindProgram();

    fx_pass = NULL;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    // SCENE-LEVEL TEST
    //
    nvFX::PassInfo pr;
    int np = fx_TechScene->getNumPasses();
    for(int i=0; i<np; i++)
    {
        nvFX::IPass* scenePass = fx_TechScene->getPass(i);

// FIXME: viewport size depends on the render-target that was defined in nvFX
// but I don't remember how to setup the size from nvFX when the target depends on the window of the sample
// so, setting it by default to this window size is a temporary fix
glViewport(0, 0, g_winSz[0], g_winSz[1]);

        scenePass->execute(&pr);
        switch(pr.renderingMode)
        {
        case nvFX::RENDER_SCENEGRAPH_SHADED:
            look_at(g_transfBlock1.m4_View, g_camera.curEyePos, g_camera.curFocusPos, up);
            g_transfBlock1.m4_Proj = g_cameraProj;
            g_transfBlock1.m4_ViewProj = g_transfBlock1.m4_Proj * g_transfBlock1.m4_View;
            g_transfBlock1.eyePos = g_camera.curEyePos;
            g_transfBlock1.m4_shadowMatrix = g_lightViewProjScaleBias; // needed here to find where we are in the shadowmap
            // copy the block to OGL
            if(fx_transfBlock1)
            {
                void* p;
                fx_transfBlock1->mapBuffer(&p);
                memcpy(p, &g_transfBlock1, sizeof(transfBlock1));
                fx_transfBlock1->unmapBuffer();
            }
            displayScene(pr);
            break;
        // NOTE: Let's assume for now that we do use RENDER_SCENEGRAPH_NOSHADING for the shadowmap view
        case nvFX::RENDER_SHADOWMAP:
            g_transfBlock1.m4_Proj = g_lightProjection;
            g_transfBlock1.m4_View = g_lightView;
            g_transfBlock1.m4_ViewProj = g_lightViewProj;
            g_transfBlock1.eyePos = g_lightPos; // useless for lightview rendering
            // copy the block to OGL
            if(fx_transfBlock1)
            {
                void* p;
                fx_transfBlock1->mapBuffer(&p);
                memcpy(p, &g_transfBlock1, sizeof(transfBlock1));
                fx_transfBlock1->unmapBuffer();
            }
            displayScene(pr);
            break;
        default:
            break;
        }
    }
#ifdef NOGLUT
    SwapBuffers( g_hDC );
#else
    glutSwapBuffers();
#endif
}
//------------------------------------------------------------------------------
