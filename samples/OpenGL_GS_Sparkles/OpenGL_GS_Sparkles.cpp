/*
    Copyright (c) 2013, NVIDIA CORPORATION. All rights reserved.
    Copyright (c) 2013, Tristan Lorach. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Neither the name of NVIDIA CORPORATION nor the names of its
       contributors may be used to endorse or promote products derived
       from this software without specific prior written permission.

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

    Please direct any questions to tlorach@nvidia.com (Tristan Lorach)
*/
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
#include "nvImage.h"

#define TECH_DEFAULT 2 // we assume 0 and 1 are for init and grid floor
#define MATERIAL_EFFECT "materialCarPaint.glslfx"
#define SCENE_EFFECT "OpenGL_GS_Sparkles.glslfx"
#define NMODELS 2
#define MODEL1 "gargoyle_v134.bk3d.gz"
#define MODEL2 "NVShaderBall_134.bk3d.gz"
#define PROJ_NEAR   0.01f
#define PROJ_FAR    10.0f

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
nvFX::IContainer*   fx_EffectMaterial = NULL;

nvFX::ITechnique*   fx_TechScene    = NULL;
nvFX::ITechnique*   fx_TechMaterial = NULL;
nvFX::ITechnique*   fx_TechFloor    = NULL;
nvFX::IPass*        fx_pass         = NULL;

// uniforms taken from material
nvFX::ICstBuffer*   fx_transfBlock1 = NULL;
nvFX::ICstBuffer*   fx_transfBlock2 = NULL;
nvFX::ICstBuffer*   fx_materialBlock= NULL;
nvFX::ICstBuffer*   fx_almostNeverChange   = NULL;

//#define USECSTBUFUNIFORMS
#ifdef USECSTBUFUNIFORMS
// here we show that it is also possible to update uniforms inside a constant-buffer
// and rely on the effect system to update the UBO for you
// let's define interfaces for each component of the UBO structure
// from fx_transfBlock1:
nvFX::IUniform*     fx_m4_ViewProj      = NULL;
nvFX::IUniform*     fx_m4_ViewProjI     = NULL;
nvFX::IUniform*     fx_m4_Proj          = NULL;
nvFX::IUniform*     fx_m4_View          = NULL;
nvFX::IUniform*     fx_m4_ViewIT        = NULL;
nvFX::IUniform*     fx_eyePos           = NULL;
// from fx_transfBlock2:
nvFX::IUniform*     fx_m4_World         = NULL;
nvFX::IUniform*     fx_m4_WorldView     = NULL;
nvFX::IUniform*     fx_m4_WorldViewProj = NULL;
nvFX::IUniform*     fx_m4_WorldIT       = NULL;

// material information
nvFX::IUniform*     fx_diffuse          = NULL;    ///< RGB diffuse color
nvFX::IUniform*     fx_specexp          = NULL;    ///< exponent of the specular lighting
nvFX::IUniform*     fx_ambient          = NULL;    ///< RGB ambient color
nvFX::IUniform*     fx_reflectivity     = NULL;    ///< intensity of the reflection
nvFX::IUniform*     fx_transparency     = NULL;    ///< RGB transparency
nvFX::IUniform*     fx_translucency     = NULL;    ///< translucency : 0, no transparency at all; 1 : object 100% transparent (transparency could be used)
nvFX::IUniform*     fx_specular         = NULL;    ///< specular RGB color


#endif

// uniforms from material
nvFX::IUniform*     fx_scaleBias  = NULL;

// uniforms taken from scene effect
nvFX::IUniform*     fx_gViewportSizeI = NULL;

std::vector<nvFX::IResource*> fx_resources;
// data structure reflecting buffers in shaders
struct transfBlock1
{
    mat4 m4_ViewProj;
    mat4 m4_ViewProjI;
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
transfBlock2 g_transfBlock2;

struct MaterialBlock
{
    // we map it the same as in bk3d::Material for the sake of simplicity of the demo
    vec3     diffuse;         ///< RGB diffuse color
    float    specexp;            ///< exponent of the specular lighting
    vec3     ambient;         ///< RGB ambient color
    float    reflectivity;       ///< intensity of the reflection
    vec3     transparency;    ///< RGB transparency
    float    translucency;       ///< translucency : 0, no transparency at all; 1 : object 100% transparent (transparency could be used)
    vec3     specular;        ///< specular RGB color
};

int     g_maxSprites = 5;
vec4    scaleBias;
#ifndef USECSTBUFUNIFORMS
// case where we do it by hand
GLuint transfBlockUBO[2] = {0,0};
GLuint materialBlockUBO = 0;
#endif

//-------------- Basic UI defined in main.cpp
#ifdef USESVCUI
#include "ISvcUI.h"
extern IWindowHandler*      g_pWinHandler;
extern IWindowConsole*      g_pConsole;
extern IWindowLog*          g_pLog;
extern IProgressBar*        g_pProgress;

IWindowContainer*           g_pControls     = NULL;
IWindowContainer*           g_pUniforms     = NULL;
IControlCombo*              g_pComboTech    = NULL;
IControlCombo*              g_pComboTechMat = NULL;
IControlCombo*              g_pComboMesh    = NULL;

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

//---------------- bk3d Mesh
#include <zlib.h>
#include "bk3dEx.h"
int                 g_curMesh = 0;
bk3d::FileHeader *  g_meshFile[NMODELS];
vec3                g_posOffset[NMODELS];
float               g_scale[NMODELS] = {1.0,1.0};

//---------------------- other params
float g_fSpinX = 0.0;
bool  g_bUseMaterial = true;

//-------------------- VAO (to be Core OpenGL compliant :-( )
GLuint g_vao;
//-------------------- Timer Query
GLuint  timerQueries[4];
int     tqOffset = 0;
bool    tqStart = true;
//-------------------- forward decalarations
bool loadMaterialEffect();
bool loadSceneEffect();
#ifdef USESVCUI
void removeEffectParamsUI();
void gatherEffectParamsUI();
#endif
//------------------------------------------------------------------------------
void reshape(int w, int h)
{
    g_winSz[0] = w; 
    g_winSz[1] = h;

    perspective(g_transfBlock1.m4_Proj, 45.0f, (float)g_winSz[0] / (float)g_winSz[1], PROJ_NEAR, PROJ_FAR);
    //
    // Let's validate again the base of resource management to make sure things keep consistent
    //
    int W = g_winSz[0];
    int H = g_winSz[1];

    glViewport(0, 0, W, H);

    perspective(g_transfBlock1.m4_Proj, 45.0f, (float)g_winSz[0] / (float)g_winSz[1], PROJ_NEAR, PROJ_FAR);

    nvFX::getResourceRepositorySingleton()->setParams(0,0,W,H,1,0,NULL );
    bool failed = nvFX::getResourceRepositorySingleton()->validateAll() ? false : true;
    if(failed)
        assert(!"Oops");
    if(fx_gViewportSizeI)
        fx_gViewportSizeI->setValue2f(1.0/(float)g_winSz[0], 1.0/(float)g_winSz[1]);
    if(fx_almostNeverChange)
    {
        nvFX::IUniform* pU = fx_almostNeverChange->findUniform("near_far_scrratio");
        if(pU) pU->setValue4f(PROJ_NEAR, PROJ_FAR, (float)W/(float)H, 0.0);
        pU = fx_almostNeverChange->findUniform("DepthRTSz");
        if(pU) pU->setValue2f((float)W, (float)H);
    }
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
    nvFX::IContainer::destroy(fx_EffectMaterial);
    fx_EffectMaterial = NULL;
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
            g_scale[g_curMesh] *= 0.5f;
            UPDATEUI(g_scale[g_curMesh])
            break;
        case 'q':
            g_scale[g_curMesh] *= 2.0f;
            UPDATEUI(g_scale[g_curMesh])
            break;
        case ' ':
            g_realtime.bNonStopRendering = g_realtime.bNonStopRendering ? false : true;
            UPDATEUI(g_realtime.bNonStopRendering);
            break;
        case '1':
        case '2':
        case '3':
            if(fx_TechScene    = fx_EffectScene->findTechnique(key-'1'))
            {
                LOGI("Using scene technique %s", fx_TechScene->getName());
                #ifdef USESVCUI
                	if(g_pComboTech) g_pComboTech->SetSelectedByIndex(key-'1');
                #endif
            } else fx_TechScene = fx_EffectScene->findTechnique(0); // defaulting
            break;
        case '4':
        case '5':
            if(fx_TechMaterial = fx_EffectMaterial->findTechnique(TECH_DEFAULT+key-'4'))
            {
                LOGI("Using Material technique %s", fx_TechMaterial->getName());
                #ifdef USESVCUI
                if(g_pComboTechMat) g_pComboTechMat->SetSelectedByIndex(key-'4');
                #endif
            } else fx_TechMaterial = fx_EffectMaterial->findTechnique(TECH_DEFAULT);
            break;
        case '6':
        case '7':
            g_curMesh = key-'6';
            if(fx_scaleBias)
                fx_scaleBias->setValue4f(0, 0, 0, g_scale[g_curMesh]);
            #ifdef USESVCUI
            if(g_pComboMesh) g_pComboMesh->SetSelectedByIndex(g_curMesh);
            #endif
            break;
        case 'm':
#ifdef USESVCUI
            removeEffectParamsUI();
#endif
            loadMaterialEffect();
#ifdef USESVCUI
            gatherEffectParamsUI();
#endif
            break;
        case 's':
#ifdef USESVCUI
            removeEffectParamsUI();
#endif
            loadSceneEffect();
#ifdef USESVCUI
            gatherEffectParamsUI();
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
void messageCallbackFunc(const char *msg)
{
#ifdef WIN32
    OutputDebugString(msg);
#endif
    printf(msg);
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
}

//-----------------------------------------------------------------------------
// Load texture image to a resource
//-----------------------------------------------------------------------------
bool loadTexture(const char* fname, nvFX::IResourceEx* pRes)
{
    nv::Image image; // for hdr... and later for dds
    char fullpath[200];
    // we take advantage of the resource repository of nvFx: the place where nvFX
    // stores his resources such as render-targets declared from
    //nvFX::IResource* pR = nvFX::getResourceRepositorySingleton()->find("fname")
    if(pRes->getGLTextureID() != 0)
    {
        LOGI("Texture %s already uploaded!\n", fname);
        return true;
    }

    bool bRes = image.loadImageFromFile(fname);
    if(!bRes)
    {
        sprintf(fullpath, SOURCE_DIR "/%s", fname);
        bRes = image.loadImageFromFile(fullpath);
    }
    if(!bRes)
    {
        sprintf(fullpath, RESOURCE_DIR "/%s", fname);
        bRes = image.loadImageFromFile(fullpath);
    }
    if(!bRes)
        return false;

    GLuint texID;
    switch(pRes->getType())
    {
    case nvFX::RESTEX_CUBE_MAP:
        {
            if (image.getFaces()==0)
                if (!image.convertCrossToCubemap()) 
                {
                    LOGE("Error>> converting image %s to cubemap\n", fullpath);
                    return false;
                }
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            //if(image.getMipLevels()<2)
            //{
            //    LOGW("no mipmaps in %s. activating GL_GENERATE_MIPMAP_SGIS\n", fullpath);
            //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
            //} else
            //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP_SGIS, GL_FALSE);
            // load face data
            GLenum internalFormat = image.getInternalFormat();
            for(int i=0; i<6; i++) 
            {
                int w = image.getWidth();
                int h = image.getHeight();
                int d = image.getDepth();
                for(int l=0; l<image.getMipLevels(); l++)
                {
                    GLenum imgfmt  = image.getFormat();
                    GLenum imgtype = image.getType();
                    void *imgdata = image.getLevel(l, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
                    if(image.isCompressed())
                        glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, l,
                        internalFormat, w, h, 0, image.getImageSize(l), imgdata);
                    else
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, l,
                                 internalFormat, w, h, 0, 
                                 imgfmt, imgtype, 
                                 imgdata);
                    w /= 2; h /= 2;
                }
            }
            // the important part: the ID refering to the allocated resource in the Driver
            pRes->setGLTexture(texID);
            // The rest can just help for consistency with the fact that this resource is in the repository
            // and could be queried...
            pRes->setDimensions(image.getWidth(), image.getHeight(), image.getDepth());
            // TODO: set the format from OpenGL enums...
            // note that this is not so important: nvFX won't use this information. But cleaner to give it
            //pRes->setFormatFromGL(image.getInternalFormat());
        }
        break;
    case nvFX::RESTEX_1D:
        {
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_1D, texID);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            //if(image.getMipLevels()<2)
            //{
            //    PRINTF(("no mipmaps in %s. activating GL_GENERATE_MIPMAP_SGIS\n", fullpath));
            //    glTexParameteri(GL_TEXTURE_1D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
            //} else
            //    glTexParameteri(GL_TEXTURE_1D, GL_GENERATE_MIPMAP_SGIS, GL_FALSE);
            GLenum internalFormat = image.getInternalFormat();
            int w = image.getWidth();
            for(int l=0; l<image.getMipLevels(); l++)
            {
                if(image.isCompressed())
                    glCompressedTexImage1DARB(GL_TEXTURE_1D, l,
                    internalFormat, image.getWidth(), 0, image.getImageSize(l), image.getLevel(l));
                else
                    glTexImage1D(GL_TEXTURE_1D, l,
                             internalFormat, image.getWidth(), 0, 
                             image.getFormat(), image.getType(), 
                             image.getLevel(l));
                w /= 2;
            }
            pRes->setGLTexture(texID);
            pRes->setDimensions(image.getWidth(), image.getHeight(), image.getDepth());
        }
        break;
    case nvFX::RESTEX_2D:
        {
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_2D, texID);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            //if(image.getMipLevels()<2)
            //{
            //    PRINTF(("no mipmaps in %s. activating GL_GENERATE_MIPMAP_SGIS\n", fullpath));
            //    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
            //} else
            //    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_FALSE);
            GLenum internalFormat = image.getInternalFormat();
            int w = image.getWidth();
            int h = image.getHeight();
            for(int l=0; l<image.getMipLevels(); l++)
            {
                if(image.isCompressed())
                    glCompressedTexImage2DARB(GL_TEXTURE_2D, l,
                    internalFormat, w,h, 0, image.getImageSize(l), image.getLevel(l));
                else
                    glTexImage2D(GL_TEXTURE_2D, l,
                             internalFormat, w, h, 0, 
                             image.getFormat(), image.getType(), 
                             image.getLevel(l));
                w /= 2; h /= 2;
            }
            pRes->setGLTexture(texID);
            pRes->setDimensions(image.getWidth(), image.getHeight(), image.getDepth());
        }
        break;
    case nvFX::RESTEX_3D:
        {
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_3D, texID);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            //if(image.getMipLevels()<2)
            //{
            //    PRINTF(("no mipmaps in %s. activating GL_GENERATE_MIPMAP_SGIS\n", fullpath));
            //    glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
            //} else
            //    glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP_SGIS, GL_FALSE);
            GLenum internalFormat = image.getInternalFormat();
            int w = image.getWidth();
            int h = image.getHeight();
            int d = image.getDepth();
            for(int l=0; l<image.getMipLevels(); l++)
            {
                if(image.isCompressed())
                    glCompressedTexImage3DARB(GL_TEXTURE_3D, l,
                    internalFormat, image.getWidth(), image.getHeight(), image.getDepth()
                    , 0, image.getImageSize(l), image.getLevel(l));
                else
                    glTexImage3D(GL_TEXTURE_3D, l,
                             internalFormat, image.getWidth(), image.getHeight(), image.getDepth(), 0, 
                             image.getFormat(), image.getType(), 
                             image.getLevel(l));
                w /= 2; h /= 2; d /= 2;
            }
            pRes->setGLTexture(texID);
            pRes->setDimensions(image.getWidth(), image.getHeight(), image.getDepth());
        }
        break;
    case nvFX::RESTEX_2DRECT:
        {
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texID);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            //if(image.getMipLevels()<2)
            //{
            //    PRINTF(("no mipmaps in %s. activating GL_GENERATE_MIPMAP_SGIS\n", fullpath));
            //    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
            //} else
            //    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_GENERATE_MIPMAP_SGIS, GL_FALSE);
            GLenum internalFormat = image.getInternalFormat();
            int w = image.getWidth();
            int h = image.getHeight();
            for(int l=0; l<image.getMipLevels(); l++)
            {
                glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, l,
                             internalFormat, w, h, 0, 
                             image.getFormat(), image.getType(), 
                             image.getLevel(l));
                w /= 2; h /= 2;
            }
            pRes->setGLTexture(texID);
            pRes->setDimensions(image.getWidth(), image.getHeight(), image.getDepth());
        }
        break;
    default:
        LOGE("Error: unknown type of resource. Or type not handled!");
        return false;
    }
    LOGI("Yes>> Loaded image %s\n", fullpath);
    return true;
}

//-----------------------------------------------------------------------------
// scene instances, depending on the scene level needs
//-----------------------------------------------------------------------------
bool validateAndCreateSceneInstances()
{
    bool failed = false;
    //
    // gather techniques of materials that we want to instanciate
    // depending on scene effects
    //
    int Sz = fx_EffectMaterial->getNumTechniques();
    nvFX::ITechnique** techs = new nvFX::ITechnique*[Sz];
    for(int t=0; t<Sz ; t++) {
        techs[t] = fx_EffectMaterial->findTechnique(t);
        LOGI("Setup overrides for technique %s\n", techs[t]->getName());
    }
    //
    // validate scene-level techniques
    //
    nvFX::ITechnique* scTech;
    for(int t=0; ; t++)
    {
        scTech = fx_EffectScene->findTechnique(t);
        if(scTech == NULL)
            break;
        LOGI("Checking overrides from technique %s\n", scTech->getName());
        bool bRes = scTech->validate();
        if(!bRes)
        {
            PRINTF(("Error>> couldn't validate the scene technique %d\n", t));
            failed  = true;
        }
        int np = scTech->getNumPasses();
        for(int i=0; i<np; i++)
        {
            // in this special example, only one 'material' technique is used...
            nvFX::IPass* p  = scTech->getPass(i);
            //
            // instanciate what is inside the array
            //
            bool bRes       = p->setupOverrides(techs, Sz);
        }
    }
    //
    // Let's find all the techniques and validates them
    // subsequent passes will get validated and related shaders/programs
    // will be created
    //
    for(int t=0; t<Sz ; t++)
    {
        bool bRes = techs[t]->validate();
        if(!bRes)
        {
            PRINTF(("Error>> couldn't validate the material technique %s\n", techs[t]->getName()));
            failed  = true;
        }
    }
    delete [] techs;
    return failed;
}


//-----------------------------------------------------------------------------
// setup the constant buffers - shared among effect... ("global" keyword)
//-----------------------------------------------------------------------------
void setup_common_uniforms(nvFX::IContainer* pEffect)
{
#ifdef USECSTBUFUNIFORMS
    // we assume all the effects share the same block definitions
    // so we can find them back from whatever effect : those are shared ("global" token)
    if(!fx_transfBlock1)
    {
        fx_transfBlock1 = pEffect->findCstBuffer("transfBlock1");
        // case where we ask for help from nvFX to create the proper buffer.
        // but in this sample, we decided to create the UBOs from here and assign
        // them to nvFX. This allows us to share UBOs among many effects
        if(fx_transfBlock1) fx_transfBlock1->buildGLBuffer(); // ask nvFx to take care of the UBO creation
    }
    if(!fx_transfBlock2)
    {
        fx_transfBlock2 = pEffect->findCstBuffer("transfBlock2");
        if(fx_transfBlock2) fx_transfBlock2->buildGLBuffer();
    }
    if(!fx_materialBlock)
    {
        fx_materialBlock = pEffect->findCstBuffer("materialBlock");
        if(fx_materialBlock) fx_materialBlock->buildGLBuffer();
    }
    // example of other way to update: let's track the uniforms inside this buffer.
    if(fx_transfBlock1)
    {
        fx_m4_ViewProj      = fx_transfBlock1->findUniform("m4_ViewProj");
        fx_m4_ViewProjI     = fx_transfBlock1->findUniform("m4_ViewProjI");
        fx_m4_Proj          = fx_transfBlock1->findUniform("m4_Proj");
        fx_m4_View          = fx_transfBlock1->findUniform("m4_View");
        fx_m4_ViewIT        = fx_transfBlock1->findUniform("m4_ViewIT");
        fx_eyePos           = fx_transfBlock1->findUniform("eyePos");
    }
    if(fx_transfBlock2)
    {
        fx_m4_World         = fx_transfBlock2->findUniform("m4_World");
        fx_m4_WorldView     = fx_transfBlock2->findUniform("m4_WorldView");
        fx_m4_WorldViewProj = fx_transfBlock2->findUniform("m4_WorldViewProj");
        fx_m4_WorldIT       = fx_transfBlock2->findUniform("m4_WorldIT");
    }
    if(fx_materialBlock)
    {
        fx_diffuse          = fx_materialBlock->findUniform("diffuse");
        fx_specexp          = fx_materialBlock->findUniform("specexp");
        fx_ambient          = fx_materialBlock->findUniform("ambient");
        fx_reflectivity     = fx_materialBlock->findUniform("reflectivity");
        fx_transparency     = fx_materialBlock->findUniform("transparency");
        fx_translucency     = fx_materialBlock->findUniform("translucency");
        fx_specular         = fx_materialBlock->findUniform("specular");
    }

#else
    // we assume all the effects share the same block definitions
    // so we can find them back from whatever effect : those are shared ("global" token)
    if(!fx_transfBlock1)
    {
        fx_transfBlock1 = pEffect->findCstBuffer("transfBlock1");
        if(fx_transfBlock1) fx_transfBlock1->setGLBuffer(transfBlockUBO[0]);
    }
    if(!fx_transfBlock2)
    {
        fx_transfBlock2 = pEffect->findCstBuffer("transfBlock2");
        if(fx_transfBlock2) fx_transfBlock2->setGLBuffer(transfBlockUBO[1]);
    }
    if(!fx_materialBlock)
    {
        fx_materialBlock = pEffect->findCstBuffer("materialBlock");
        if(fx_materialBlock) fx_materialBlock->setGLBuffer(materialBlockUBO);
    }
#endif
    if(!fx_almostNeverChange)
    {
        fx_almostNeverChange = pEffect->findCstBuffer("almostNeverChange");
        fx_almostNeverChange->buildGLBuffer(nvFX::ICstBuffer::STREAM_DRAW);
        nvFX::IUniform* pU = fx_almostNeverChange->findUniform("near_far_scrratio");
        if(pU) pU->setValue4f(PROJ_NEAR, PROJ_FAR, (float)g_winSz[0]/(float)g_winSz[1], 0.0);
        pU = fx_almostNeverChange->findUniform("DepthRTSz");
        if(pU) pU->setValue2f((float)g_winSz[0], (float)g_winSz[1]);
    }

    fx_gViewportSizeI    = fx_EffectScene->findUniform("gViewportSizeI");
    if(fx_gViewportSizeI) fx_gViewportSizeI->setValue2f(1.0/(float)g_winSz[0], 1.0/(float)g_winSz[1]);
}
//-----------------------------------------------------------------------------
// Load scene effect
//-----------------------------------------------------------------------------
bool loadSceneEffect()
{
    bool failed = false;
    if(fx_EffectScene)
    {
        //
        // We first should get rid of some instances in material that don't have anymore any meaning
        // those instances were created to fit to the scene effects
        // For the demo. let's only work with "MaterialTechnique", but we could create a table
        // of techniques to send to "releaseOverrides"
        //
        if(fx_EffectMaterial)
        {
            LOGI("=========> Releasing overrides\n");
            nvFX::ITechnique* tech;
            for(int mt=0; tech = fx_EffectMaterial->findTechnique(mt); mt++)
            {
                nvFX::ITechnique* scTech;
                for(int t=0; ; t++)
                {
                    scTech = fx_EffectScene->findTechnique(t);
                    if(scTech == NULL)
                        break;
                    int np = scTech->getNumPasses();
                    for(int i=0; i<np; i++)
                    {
                        nvFX::IPass* p = scTech->getPass(i);
                        // this will remove the pass instances in 'tech' that were dedicated to the pass p
                        bool bRes = p->releaseOverrides(&tech, 1);
                    }
                }
            }
            // optional but useful to optimize the programs/shaders being used
            // remove useless shaders from the effect, after Overrides got trimmed down
            LOGI("=========> Consolidating effect\n");
            fx_EffectMaterial->consolidateShaders();
        }
        //fx_EffectMaterial->destroy();
        //or
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
    //
    // gather few uniforms in common between effects
    //
    setup_common_uniforms(fx_EffectScene);
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
    //
    // Load possible textures
    //
    nvFX::IResource* pRes = NULL;
    for(int i=0; pRes=fx_EffectScene->findResource(i); i++)
    {
        const char* name = pRes->annotations()->getAnnotationString("defaultFile");
        if(!name)
            continue;
        loadTexture(name, pRes->getExInterface());
    }
    //
    // Now we need to re-build the instances of materials for the scenes
    //
    LOGI("=========> Effect instance override setup\n");
    if(fx_EffectMaterial)
        if(validateAndCreateSceneInstances())
            failed = true;

    return failed ? false : true;
}
//-----------------------------------------------------------------------------
// Load scene effect
//-----------------------------------------------------------------------------
bool loadMaterialEffect()
{
    if(fx_EffectMaterial)
    {
        LOGI("Desroying previous material Effect\n");
        //fx_EffectMaterial->destroy();
        //or
        LOGI("=========> Destroying effect\n");
        nvFX::IContainer::destroy(fx_EffectMaterial);
        fx_EffectMaterial = NULL;
    }
    LOGI("Creating Effect (material)\n");
    fx_EffectMaterial = nvFX::IContainer::create("material");
    bool bRes = nvFX::loadEffectFromFile(fx_EffectMaterial, MATERIAL_EFFECT);
    if(!bRes)
        bRes = nvFX::loadEffectFromFile(fx_EffectMaterial, SOURCE_DIR "/" MATERIAL_EFFECT);
    if(!bRes)
        bRes = nvFX::loadEffectFromFile(fx_EffectMaterial, RESOURCE_DIR "/" MATERIAL_EFFECT);
    if(!bRes)
    {
        LOGE("Failed\n");
        return false;
    }
    LOGOK("Loaded\n");
    //
    // Let's keep track in interface pointers everything, for purpose of clarity
    //
    fx_TechMaterial = fx_EffectMaterial->findTechnique("Init");
    if(fx_TechMaterial && (!fx_TechMaterial->validate()))
        exit(1);
    // execute the pass as a way to initialize some basic GL states
    fx_TechMaterial->getPass(0)->execute();
    fx_TechFloor = fx_EffectMaterial->findTechnique("Floor");
    if(fx_TechFloor && (!fx_TechFloor->validate()))
        exit(1);
    // loop in the rest and validate them
#ifdef USESVCUI
    if(g_pComboTechMat) g_pComboTechMat->RemoveAllItems();
#endif
    // we assume 0 and 1 are the previous techniques... arbitrarily...
    for(int t=TECH_DEFAULT; fx_TechMaterial = fx_EffectMaterial->findTechnique(t); t++)
    {
        fx_TechMaterial->validate();
#ifdef USESVCUI
        if(g_pComboTechMat) g_pComboTechMat->AddItem(fx_TechMaterial->getName(), t);
#endif
    }
#ifdef USESVCUI
    if(g_pComboTechMat) g_pComboTechMat->SetSelectedByIndex(0);
#endif
    fx_TechMaterial = fx_EffectMaterial->findTechnique(TECH_DEFAULT);
    //
    // gather few uniforms in common between effects
    //
    setup_common_uniforms(fx_EffectMaterial);

    fx_scaleBias = fx_EffectMaterial->findUniform("scaleBias");
    if(fx_scaleBias)
        fx_scaleBias->setValue4f(0, 0, 0, g_scale[g_curMesh]);
        //fx_scaleBias->setValue4f(g_posOffset.x, g_posOffset.y, g_posOffset.z, g_scale[]);
    //
    // Now let's load the default resources that the effect might need
    //
    nvFX::IResource* pRes = NULL;
    for(int i=0; pRes=fx_EffectMaterial->findResource(i); i++)
    {
        const char* name = pRes->annotations()->getAnnotationString("defaultFile");
        if(!name)
            continue;
        loadTexture(name, pRes->getExInterface());
    }
    //
    // This operation is necessary to *instanciate* material passes depending on the override of
    // upper effect techniques (scene)
    //
    if(validateAndCreateSceneInstances())
        return false;
    return true;
}

//-----------------------------------------------------------------------------
// Load few models.
// This is using a Binary format that has lots of things ready to use
// we will even use it to grab the possible textures needed by the model
//-----------------------------------------------------------------------------
void loadModel()
{
    // Load a model
    const char* p = RESOURCE_DIR;
    const char* modelNames[NMODELS] = {MODEL1, MODEL2};
    //---------------
    for(int m=0; m<NMODELS; m++)
    {
        LOGI("Loading Meshes... %s\n", modelNames[m]);
#ifdef USESVCUI
        if(g_pComboMesh) {
			g_pComboMesh->AddItem(modelNames[m], m);
			if(m==0) g_pComboMesh->SetSelectedByIndex(0);
		}
#endif
        g_meshFile[m] = bk3d::load(MODEL1);
        if(!g_meshFile[m])
        {
            LOGI("Trying " RESOURCE_DIR "/%s\n", modelNames[m]);
            char fullname[200];
            sprintf(fullname, RESOURCE_DIR "/%s", modelNames[m]);
            g_meshFile[m] = bk3d::load(fullname);
        }
        //// use tech #2
        //fx_TechMaterial = fx_EffectMaterial->findTechnique(TECH_DEFAULT+1);
        //#ifdef USESVCUI
        //g_pComboTech->SetSelectedByIndex(1);
        //#endif
        if(!g_meshFile[m])
        {
            LOGE("error in loading mesh\n");
            continue;
        }
        // To be Core OpenGL, we must create some buffer objects
        // Check the bounding boxes and adjust scaling
        float min[3] = {10000.0, 10000.0, 10000.0};
        float max[3] = {-10000.0, -10000.0, -10000.0};
        for(int i=0; i<g_meshFile[m]->pMeshes->n; i++)
        {
            bk3d::Mesh *pMesh = g_meshFile[m]->pMeshes->p[i];
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
        //
        // Scale and offset for the mesh
        //
        g_posOffset[m].x = (max[0] + min[0])*0.5f;
        g_posOffset[m].y = (max[1] + min[1])*0.5f;
        g_posOffset[m].z = (max[2] + min[2])*0.5f;
        float bigger = 0;
        if((max[0]-min[0]) > bigger) bigger = (max[0]-min[0]);
        if((max[1]-min[1]) > bigger) bigger = (max[1]-min[1]);
        if((max[2]-min[2]) > bigger) bigger = (max[2]-min[2]);
        if((bigger) > 0.001)
        {
            g_scale[m] = 1.0f / bigger;
            LOGI("Scaling the model by %f...\n", g_scale[m]);
        }
        //
        // Now, let's check the texture resources
        // TODO... the idea: load texture resources needed by the mesh. But this sample doesn't suffer from not doing it
        if(g_meshFile[m]->pMaterials)
        {
            for(int i=0; i<g_meshFile[m]->pMaterials->nMaterials; i++)
            {
                bk3d::Material* pMat = g_meshFile[m]->pMaterials->pMaterials[i];
                if(pMat->diffuseTexture.filename)
                {
                    LOGI("%s would need diffuse texture %s...\n", modelNames[m], pMat->diffuseTexture.filename);
                    // TODO
                }
            }
        }
        //if(!nvFX::getResourceRepositorySingleton()->validate(0,0,W,H,1,0,(void*)(fboId) ))

    }
    if(fx_scaleBias)
        fx_scaleBias->setValue4f(0, 0, 0, g_scale[0]);
        //fx_scaleBias->setValue4f(g_posOffset[0], g_posOffset[1], g_posOffset[2], g_scale);
}
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
#ifdef USESVCUI
void createUniformUIControl(nvFX::IUniform* pU, IWindowFolding* pCont)
{
    //-----------------------------------------------------------------------------
    // event catching for those controls
    class ControlsEvents : public IEventsWnd
    {
        void ScalarChanged(IControlScalar *pWin, float &v, float prev)
        {
            nvFX::IUniform* pU = (nvFX::IUniform*)pWin->GetUserData();
            // special case in our demo:
            if(!strcmp(pU->getName(), "maxSprites"))
                g_maxSprites = (int)v;
            // for now nvFx doesn't cast types... todo
            switch(pU->getType()) {
                case nvFX::IUniform::TFloat:
                    pU->setValue1f(v);
                    break;
                case nvFX::IUniform::TInt:
                    pU->setValue1i(v);
                    break;
            }
            g_renderCnt++;
        };
        void VectorChanged(IControlVector *pWin, float *v, float *prev, int dim)
        {
            nvFX::IUniform* pU = (nvFX::IUniform*)pWin->GetUserData();
            // special case in our demo:
            if(!strcmp(pU->getName(), "maxSprites"))
                g_maxSprites = (int)v;
            // for now nvFx doesn't cast types... todo
            switch(pU->getType()) {
                case nvFX::IUniform::TVec2:
                    pU->setValue2fv(v);
                    break;
                case nvFX::IUniform::TVec3:
                    pU->setValue3fv(v);
                    break;
                case nvFX::IUniform::TVec4:
                    pU->setValue4fv(v);
                    break;
            }
            g_renderCnt++;
        }
        void ColorPicker(IWindow *pWin, float *color, float *prev)
        {
            nvFX::IUniform* pU = (nvFX::IUniform*)pWin->GetUserData();
            pU->setValue4fv(color);
            g_renderCnt++;
        }
    };
    static ControlsEvents tweaksEvents;
    //-----------------------------------------------------------------------------
    // create the control
    const char* uname = pU->getName();
    float uimin = pU->annotations()->getAnnotationFloat("uimin");
    float uimax = pU->annotations()->getAnnotationFloat("uimax");
    if(uimax == uimin)
        uimax = uimin+1.0;
    switch(pU->getType())
    {
    case nvFX::IUniform::TFloat:
        {
        float v; pU->getValuefv(&v, 1);
        g_pWinHandler->CreateCtrlScalar(uname, uname, pCont)
            ->SetBounds(uimin,uimax)
            ->SetValue(v)
            ->SetUserData(pU)
            ->Register(&tweaksEvents);
        break;
        }
    case nvFX::IUniform::TInt:
        {
        int v; pU->getValueiv(&v, 1);
        g_pWinHandler->CreateCtrlScalar(uname, uname, pCont)
            ->SetIntMode()
            ->SetBounds(uimin,uimax)
            ->SetValue(v)
            ->SetUserData(pU)
            ->Register(&tweaksEvents);
        break;
        }
    case nvFX::IUniform::TVec3:
        {
        float v[3]; pU->getValuefv(v, 3);
        g_pWinHandler->CreateCtrlColor(uname, uname, pCont)
            ->SetValue(v[0], v[1], v[2], 1.0)
            ->SetUserData(pU)
            ->Register(&tweaksEvents);
        break;
        }
    case nvFX::IUniform::TVec4:
        {
        float v[4]; pU->getValuefv(v, 4);
        g_pWinHandler->CreateCtrlColor(uname, uname, pCont)
            ->SetValue(v[0], v[1], v[2], v[3])
            ->SetUserData(pU)
            ->Register(&tweaksEvents);
        break;
        }
    }
}
#endif
//------------------------------------------------------------------------------
// gather parameters we want to see in the UI
//------------------------------------------------------------------------------
#ifdef USESVCUI
void gatherEffectParamsUI()
{
	if(!g_pWinHandler)
		return;
    nvFX::IUniform* pU;
    nvFX::ICstBuffer* pCst;
    nvFX::IContainer*   effects[2] = {fx_EffectScene, fx_EffectMaterial};
    for(int e=0; e<2; e++)
    {
        for(int i=0; pCst = effects[e]->findCstBuffer(i); i++)
        {
            const char* groupName;
            if(!(groupName = pCst->annotations()->getAnnotationString("uigroup")))
                continue;
            IWindowFolding* pCont = g_pWinHandler->GetFoldingContainer(groupName);
            if(!pCont)
                pCont = g_pWinHandler->CreateWindowFolding(groupName, groupName, g_pUniforms);
            for(int j=0; pU=pCst->findUniform(j); j++)
            {
                createUniformUIControl(pU, pCont);
            }
        }
        for(int i=0; pU = effects[e]->findUniform(i); i++)
        {
            const char* groupName;
            if(!(groupName = pU->annotations()->getAnnotationString("uigroup")))
                continue;
            IWindowFolding* pCont = g_pWinHandler->GetFoldingContainer(groupName);
            if(!pCont)
                pCont = g_pWinHandler->CreateWindowFolding(groupName, groupName, g_pUniforms);
            createUniformUIControl(pU, pCont);
        }
    }
}
//------------------------------------------------------------------------------
// Destroy effect params in the UI
//------------------------------------------------------------------------------
void removeEffectParamsUI()
{
	if(!g_pWinHandler)
		return;
    int n = g_pUniforms->GetNumItems();
    for(int i=0; i<n; i++)
    {
        //IWindow* pWin = g_pUniforms->GetItemNum(0);
        //pWin->Destroy();
        //g_pWinHandler->Destroy(pWin->GetID());
    }
}
#endif
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void initGL()
{
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
                fx_TechMaterial = fx_EffectMaterial->findTechnique(TECH_DEFAULT+selectedidx);
            }
            else if(!strcmp(pWin->GetID(), "MESH"))
            {
                g_curMesh = selectedidx;
                if(fx_scaleBias)
                    fx_scaleBias->setValue4f(0, 0, 0, g_scale[g_curMesh]);
            }
            g_renderCnt++;
        };
    };
    static ControlsEvents controlsEvents;
    //---------------------------------------------------------------------------
    if(g_pWinHandler)
    {
        g_pControls =  g_pWinHandler->CreateWindowFolding("CTRLS", "Controls");

        g_pComboTech    = g_pWinHandler->CreateCtrlCombo("SCTECH", "Scene Tech", g_pControls);
        g_pComboTechMat = g_pWinHandler->CreateCtrlCombo("MTECH", "Material Tech", g_pControls);
        g_pComboMesh    = g_pWinHandler->CreateCtrlCombo("MESH", "Mesh", g_pControls);

        g_pWinHandler->CreateCtrlButton("LM", "Reload Materials", g_pControls)->SetUserData(NULL, 'm');
        g_pWinHandler->CreateCtrlButton("LS", "Reload Scene Effect", g_pControls)->SetUserData(NULL, 's');
        g_pWinHandler->VariableBind(g_pWinHandler->CreateCtrlCheck("RT", "Render Realtime", g_pControls), &g_realtime.bNonStopRendering);

        //g_pWinHandler->VariableBind(
        //    g_pWinHandler->CreateCtrlScalar("SCALE", "Mesh scale", g_pControls)->SetBounds(0.0, 2.0)
        //    , g_scale);
        //g_pWinHandler->VariableBind(
        //    g_pWinHandler->CreateCtrlScalar("SCALE2", "Mesh scale", g_pControls)->SetBounds(0.0, 2.0)
        //    , g_scale+1);
        g_pUniforms = g_pWinHandler->CreateWindowFolding("UNIF", "Uniforms from effects", g_pControls);
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
    // Create UBOs for our uniform blocks
    //
#ifndef USECSTBUFUNIFORMS
    glGenBuffers(2, transfBlockUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, transfBlockUBO[0]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(transfBlock1), NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, transfBlockUBO[1]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(transfBlock2), NULL, GL_STREAM_DRAW);
    glGenBuffers(1, &materialBlockUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, materialBlockUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialBlock), NULL, GL_STREAM_DRAW);
#endif
    //
    // Effects
    //
    nvFX::setErrorCallback(errorCallbackFunc);
    nvFX::setMessageCallback(messageCallbackFunc);
    nvFX::setIncludeCallback(includeCallbackFunc);
    loadSceneEffect();
    loadMaterialEffect();
    loadModel();
#ifdef USESVCUI
    gatherEffectParamsUI();
#endif
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
    LOGI("4,5 : switch between material techniques\n");
    LOGI("6,7 : switch between models\n");
    LOGI("m/s : reload material/scene effect\n");

    //
    // Timer Query
    //
    glGenQueries(4, timerQueries);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void displayScene(nvFX::PassInfo &pr, bool useeffect=true, int instancing=0)
{
    //-----------------------------------------------------------------
    //
    // Render a basic floor
    //
    //glBeginQuery(GL_TIME_ELAPSED, timerQueries[tqOffset]);
#if 1
#define U 1.0f
#define DU 0.1f
	if(instancing == 0)
	{
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
		if(useeffect && fx_TechFloor)
		{
			fx_TechFloor->setActiveProgramLayer(pr.overrideID);
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
	}
#endif
    //glEndQuery(GL_TIME_ELAPSED);
    //
    // Mesh rendering
    //
    //glBeginQuery(GL_TIME_ELAPSED, timerQueries[tqOffset+1]);

    if(!g_meshFile[g_curMesh])
    {
        return;
    }
    //
    // default values for second block made of World...
    //
    mat4 m4_world1;
    g_transfBlock2.m4_World.identity();
    g_transfBlock2.m4_WorldView = g_transfBlock1.m4_View * g_transfBlock2.m4_World;
    g_transfBlock2.m4_WorldViewProj = g_transfBlock1.m4_Proj * g_transfBlock2.m4_WorldView;
    mat4 WI;
    invert(WI, g_transfBlock2.m4_World);
    transpose(g_transfBlock2.m4_WorldIT, WI);
    if(fx_transfBlock2)
    {
#ifdef USECSTBUFUNIFORMS
        fx_m4_World->setMatrix4f(g_transfBlock2.m4_World.mat_array);
        fx_m4_WorldView->setMatrix4f(g_transfBlock2.m4_WorldView.mat_array);
        fx_m4_WorldViewProj->setMatrix4f(g_transfBlock2.m4_WorldViewProj.mat_array);
        fx_m4_WorldIT->setMatrix4f(g_transfBlock2.m4_WorldIT.mat_array);
        fx_transfBlock2->update();
#else
        void* p;
        // mapBuffer() also updates the buffer with possible changes made through ICstBuffer uniforms directly
        fx_transfBlock2->mapBuffer(&p);
        memcpy(p, &g_transfBlock2, sizeof(transfBlock2));
        fx_transfBlock2->unmapBuffer();
#endif
    }
    if(useeffect && fx_TechMaterial)
    {
        fx_TechMaterial->setActiveProgramLayer(pr.overrideID);
        fx_pass = fx_TechMaterial->getPass(0);
        fx_pass->execute();
    }
    for(int i=0; i< g_meshFile[g_curMesh]->pMeshes->n; i++)
    {
        bk3d::Mesh *pMesh = g_meshFile[g_curMesh]->pMeshes->p[i];
        // case where the mesh references a transformation
        // the absolute value must be available by default
        // if more than one transf, skip it : this might be a list of skeleton transformations
        if(pMesh->pTransforms && pMesh->pTransforms->n == 1)
        {
            g_transfBlock2.m4_World = mat4(pMesh->pTransforms->p[0]->MatrixAbs());
            g_transfBlock2.m4_WorldView = g_transfBlock1.m4_View * g_transfBlock2.m4_World;
            g_transfBlock2.m4_WorldViewProj = g_transfBlock1.m4_Proj * g_transfBlock2.m4_WorldView;
            mat4 WI;
            invert(WI, g_transfBlock2.m4_World);
            transpose(g_transfBlock2.m4_WorldIT, WI);
            mat4 matMV = g_transfBlock1.m4_View * g_transfBlock2.m4_World;
            if(fx_transfBlock2)
            {
#ifdef USECSTBUFUNIFORMS
                fx_m4_World->setMatrix4f(g_transfBlock2.m4_World.mat_array);
                fx_m4_WorldView->setMatrix4f(g_transfBlock2.m4_WorldView.mat_array);
                fx_m4_WorldViewProj->setMatrix4f(g_transfBlock2.m4_WorldViewProj.mat_array);
                fx_m4_WorldIT->setMatrix4f(g_transfBlock2.m4_WorldIT.mat_array);
                fx_transfBlock2->update();
#else
                void* p;
                fx_transfBlock2->mapBuffer(&p);
                memcpy(p, &g_transfBlock2, sizeof(transfBlock2));
                fx_transfBlock2->unmapBuffer();
#endif
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
            if(pPG->pTransforms && pPG->pTransforms->n > 0)
            {
                g_transfBlock2.m4_World = mat4(pPG->pTransforms->p[0]->MatrixAbs());
                g_transfBlock2.m4_WorldView = g_transfBlock1.m4_View * g_transfBlock2.m4_World;
                g_transfBlock2.m4_WorldViewProj = g_transfBlock1.m4_Proj * g_transfBlock2.m4_WorldView;
                mat4 WI;
                invert(WI, g_transfBlock2.m4_World);
                transpose(g_transfBlock2.m4_WorldIT, WI);
                if(fx_transfBlock2)
                {
#ifdef USECSTBUFUNIFORMS
                    fx_m4_World->setMatrix4f(g_transfBlock2.m4_World.mat_array);
                    fx_m4_WorldView->setMatrix4f(g_transfBlock2.m4_WorldView.mat_array);
                    fx_m4_WorldViewProj->setMatrix4f(g_transfBlock2.m4_WorldViewProj.mat_array);
                    fx_m4_WorldIT->setMatrix4f(g_transfBlock2.m4_WorldIT.mat_array);
                    fx_transfBlock2->update();
#else
                    void* p;
                    fx_transfBlock2->mapBuffer(&p);
                    memcpy(p, &g_transfBlock2, sizeof(transfBlock2));
                    fx_transfBlock2->unmapBuffer();
#endif
                }
            }

            bk3d::Material *pMat = pPG->pMaterial;
            if(pMat && g_bUseMaterial && fx_materialBlock)
            {
                // small issue with original modell (has a black material by default...)
                if((pMat->Diffuse()[0]==0.0f)
                 &&(pMat->Diffuse()[1]==0.0f)
                 &&(pMat->Diffuse()[2]==0.0f))
                    pMat->Diffuse()[0]=pMat->Diffuse()[1]=pMat->Diffuse()[2]= 0.7f;
#ifdef USECSTBUFUNIFORMS
                fx_diffuse->setValue3fv(pMat->diffuse);
                fx_specexp->setValue1f(pMat->specexp);
                fx_ambient->setValue3fv(pMat->ambient);
                fx_reflectivity->setValue1f(pMat->reflectivity);
                fx_transparency->setValue3fv(pMat->transparency);
                fx_translucency->setValue1f(pMat->translucency);
                fx_specular->setValue3fv(pMat->specular);
                fx_materialBlock->update();
                // NOTE: it is still possible to simply copy things directly
                // however you'll have to accept that nvFX won't be able to maintain
                // dirty status on various uniforms...
                //MaterialBlock* p;
                //fx_materialBlock->mapBuffer((void**)&p);
                //// simply copy of the data as they are in the original model memory
                //memcpy(p, pMat->diffuse, sizeof(MaterialBlock));
                //fx_materialBlock->unmapBuffer();
#else
                MaterialBlock* p;
                fx_materialBlock->mapBuffer((void**)&p);
                // simply copy of the data as they are in the original model memory
                memcpy(p, pMat->Diffuse(), sizeof(MaterialBlock));
                fx_materialBlock->unmapBuffer();
#endif
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)pPG->userPtr);
            if(instancing == 0)
              glDrawElements(
                pPG->topologyGL,
                pPG->indexCount,
                pPG->indexFormatGL,
                (void*)pPG->indexArrayByteOffset);// pIndexBufferData);
            else
              glDrawElementsInstanced(
                pPG->topologyGL,
                pPG->indexCount,
                pPG->indexFormatGL,
                (void*)pPG->indexArrayByteOffset,
                g_maxSprites);
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
    invert( g_transfBlock1.m4_ViewProjI, g_transfBlock1.m4_ViewProj);
    g_transfBlock1.eyePos = g_camera.curEyePos;
    if(fx_transfBlock1)
    {
#ifdef USECSTBUFUNIFORMS
        // this won't trigger any upload to the UBO, yet... later if needed
        fx_m4_View->setMatrix4f(g_transfBlock1.m4_View.mat_array);
        //fx_m4_ViewIT->setMatrix4f(g_transfBlock1.m4_ViewIT.mat_array);
        fx_m4_ViewProj->setMatrix4f(g_transfBlock1.m4_ViewProj.mat_array);
        fx_m4_ViewProjI->setMatrix4f(g_transfBlock1.m4_ViewProjI.mat_array);
        fx_eyePos->setValue4fv(g_transfBlock1.eyePos.vec_array);
#else
        // Here let's do it the simple way: update our UBO from here.
        // this means that nvFX doesn't maintain or check uniforms in this UBO
        // This is ok when we assume like here that we don't use nvFX to set/update parts of what is in UBOs
        void* p;
        glBindBuffer(GL_UNIFORM_BUFFER, transfBlockUBO[0]);
        p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
        memcpy(p, &g_transfBlock1, sizeof(transfBlock1));
        glUnmapBuffer(GL_UNIFORM_BUFFER);
#endif
    }

    //
    // SCENE-LEVEL TEST
    //
    nvFX::PassInfo pr;
    int np = fx_TechScene->getNumPasses();
    for(int i=0; i<np; i++)
    {
        nvFX::IPass* scenePass = fx_TechScene->getPass(i);
        scenePass->execute(&pr);
        switch(pr.renderingMode)
        {
        case nvFX::RENDER_SCENEGRAPH_SHADED:
            displayScene(pr, true, pr.renderingGroup);
            break;
        case nvFX::RENDER_SCENEGRAPH_NOSHADING:
            displayScene(pr, false, pr.renderingGroup);
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
    //glEndQuery(GL_TIME_ELAPSED);
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
