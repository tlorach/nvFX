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

#include "stdafx.h"

#include "FxParser.h"
#include "FxLibEx.h"
#ifdef OGLES2
#if (defined(_M_AMD64) || defined(_M_IA64))
#error "ES2 emulation cannot work in x64... don't have the libraries..."
#endif
#include <EGL/eglplatform.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2platform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include "gl\glew.h"
#endif

char      g_path[200];

HDC       g_hDC       = NULL;
HGLRC     g_hRC       = NULL;
HWND      g_hWnd      = NULL;
HINSTANCE g_hInstance = NULL;

int         g_nErrors = 0;

FILE *fileOut = NULL;

#ifdef OGLES2
EGLDisplay g_display;
EGLSurface g_surface;
#endif
//-----------------------------------------------------------------------------
// Missing OpenGL extensions from GLEW
//-----------------------------------------------------------------------------
PFNWGLCREATECONTEXTATTRIBSARB   wglCreateContextAttribsARB;
//PFNGLDEBUGMESSAGECONTROLARB     glDebugMessageControlARB;
//PFNGLDEBUGMESSAGEINSERTARB      glDebugMessageInsertARB;
//PFNGLDEBUGMESSAGECALLBACKARB    glDebugMessageCallbackARB;
//PFNGLGETDEBUGMESSAGELOGARB      glGetDebugMessageLogARB;
//PFNWGLGETPOINTERV               glGetPointerv;

void PrintMessage(LPCSTR fmt, ...)
{
    static char dest[4096];
    LPCSTR *ppc = (LPCSTR*)_ADDRESSOF(fmt);
    vsprintf_s(dest, 4095, fmt, (va_list)&(ppc[1]));
    OutputDebugString(dest);
    printf(dest);
    if(fileOut)
        fprintf(fileOut, dest);
}
void myErrorCb(const char *errMsg)
{
    g_nErrors++;
    printf(errMsg);
    if(fileOut)
        fprintf(fileOut, errMsg);
}
void myMsgCb(const char *msg)
{
    printf(msg);
    if(fileOut)
        fprintf(fileOut, msg);
}
void myIncludeCb(const char *incName, FILE *&fp, const char *&buf)
{
    nvFX::printf("(Including file %s...)\n", incName);
    fp = fopen(incName, "r");
    if(fp)
        return;
    char s[200];
    sprintf(s, "%s/%s", g_path, incName);
    fp = fopen(s, "r");
    if(fp)
        return;
    nvFX::printf("Failed to load %s !\n", incName);
    buf = "\n";
}

#ifndef OGLES2
//-----------------------------------------------------------------------------
// Name: myOpenGLCallback
// Desc: 
//-----------------------------------------------------------------------------
void myOpenGLCallback(  GLenum source,
                        GLenum type,
                        GLuint id,
                        GLenum severity,
                        GLsizei length,
                        const char* message,
                        GLvoid* userParam)
{
    //static std::map<GLuint, bool> ignoreMap;
    //if(ignoreMap[id] == true)
    //    return;
    char *strSource = "0";
    char *strType = strSource;
    char *strSeverity = strSource;
    switch(source)
    {
    case GL_DEBUG_SOURCE_API_ARB:
        strSource = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
        strSource = "WINDOWS";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
        strSource = "SHADER COMP.";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
        strSource = "3RD PARTY";
        break;
    case GL_DEBUG_SOURCE_APPLICATION_ARB:
        strSource = "APP";
        break;
    case GL_DEBUG_SOURCE_OTHER_ARB:
        strSource = "OTHER";
        break;
    }
    switch(type)
    {
    case GL_DEBUG_TYPE_ERROR_ARB:
        strType = "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        strType = "Deprecated";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        strType = "Undefined";
        break;
    case GL_DEBUG_TYPE_PORTABILITY_ARB:
        strType = "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        strType = "Performance";
        break;
    case GL_DEBUG_TYPE_OTHER_ARB:
        strType = "Other";
        break;
    }
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_ARB:
        strSeverity = "High";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB:
        strSeverity = "Medium";
        break;
    case GL_DEBUG_SEVERITY_LOW_ARB:
        strSeverity = "Low";
        break;
    }
    nvFX::printf("%s - %s - %s : %s\n",strSeverity, strSource, strType, message);
}
#endif
//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void initGL()
{
#ifdef OGLES2
    //
    // OpenGL ES 2 : See http://www.khronos.org/opengles/documentation/opengles1_0/html/eglIntro.html
    // For Android, an example here : http://jiggawatt.org/badc0de/android/index.html
    //
    g_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(g_display, 0, 0);
    //CHECKEGLERRORS();

    typedef struct
    {
        EGLint redSize;
        EGLint greenSize;
        EGLint blueSize;
        EGLint alphaSize;
        EGLint depthSize;
        EGLint stencilSize;
    } NvTglConfigRequest;
    NvTglConfigRequest request;

    memset(&request, 0, sizeof(request));
    request.redSize = 5;
    request.greenSize = 6;
    request.blueSize = 5;
    request.alphaSize = 0;
    //if (hasDepth)
        request.depthSize = 1;   // accept any size
    //if (hasStencil)
    //    request.stencilSize = 1;   // accept any size

    #define MAX_CONFIG_ATTRS_SIZE 32
    #define MAX_MATCHING_CONFIGS 64
    EGLint Attributes[] = {
        //EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,

        //EGL_RED_SIZE, 8,
        //EGL_GREEN_SIZE, 8,
        //EGL_BLUE_SIZE, 8,
        //EGL_NONE

        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_BUFFER_SIZE, request.redSize + request.greenSize + request.blueSize + request.alphaSize,
        EGL_RED_SIZE, request.redSize,
        EGL_GREEN_SIZE, request.greenSize,
        EGL_BLUE_SIZE, request.blueSize,
        EGL_ALPHA_SIZE, request.alphaSize,
        EGL_DEPTH_SIZE, request.depthSize,
        EGL_STENCIL_SIZE, request.stencilSize,
        EGL_NONE
    };
    // choose configs
    EGLConfig matchingConfigs[MAX_MATCHING_CONFIGS];
    EGLint numMatchingConfigs = 0;
    eglChooseConfig(g_display, Attributes, matchingConfigs, 1, &numMatchingConfigs);
    //CHECKEGLERRORS();
    EGLConfig config;
    // look for exact match
    int i;
    for (i = 0; i < numMatchingConfigs; i++)
    {
        EGLConfig testConfig = matchingConfigs[i];

        // query attributes from config
        NvTglConfigRequest cfg;
        EGLBoolean ok = EGL_TRUE;
        ok &= eglGetConfigAttrib(g_display, testConfig, EGL_RED_SIZE, &cfg.redSize);
        ok &= eglGetConfigAttrib(g_display, testConfig, EGL_GREEN_SIZE, &cfg.greenSize);
        ok &= eglGetConfigAttrib(g_display, testConfig, EGL_BLUE_SIZE, &cfg.blueSize);
        ok &= eglGetConfigAttrib(g_display, testConfig, EGL_ALPHA_SIZE, &cfg.alphaSize);
        ok &= eglGetConfigAttrib(g_display, testConfig, EGL_DEPTH_SIZE, &cfg.depthSize);
        ok &= eglGetConfigAttrib(g_display, testConfig, EGL_STENCIL_SIZE, &cfg.stencilSize);
        if (!ok)
        {
            printf("eglGetConfigAttrib failed!\n");
            break;
        }
        // depthSize == 1 indicates to accept any non-zero depth
        EGLBoolean depthMatches = ((request.depthSize == 1) && (cfg.depthSize > 0)) ||
                       (cfg.depthSize == request.depthSize);

        // stencilSize == 1 indicates to accept any non-zero stencil
        EGLBoolean stencilMatches = ((request.stencilSize == 1) && (cfg.stencilSize > 0)) ||
                         (cfg.stencilSize == request.stencilSize);
        
        // compare to request
        if (cfg.redSize == request.redSize &&
                cfg.greenSize == request.greenSize &&
                cfg.blueSize == request.blueSize &&
                cfg.alphaSize == request.alphaSize &&
                depthMatches && stencilMatches)
        {
            config = testConfig;
            break;
        }
    }
    if (i >= numMatchingConfigs)
    {
        // no exact match was found
        //if (exactMatchOnly)
        //{
        //    printf("No exactly matching EGL configs found!\n");
        //    return false;
        //}

        // just return any matching config (ie, the first one)
        config = matchingConfigs[0];
    }



    EGLint ContextAttributes[] = {	EGL_CONTEXT_CLIENT_VERSION, 2,	EGL_NONE
    };
    EGLContext Context = eglCreateContext(g_display, config, EGL_NO_CONTEXT, ContextAttributes);
    //CHECKEGLERRORS();
    // In Windows System, EGLNativeWindowType == HWND
    g_surface = eglCreateWindowSurface(g_display, config, g_hWnd, NULL);
    //CHECKEGLERRORS();
    eglMakeCurrent(g_display, g_surface, g_surface, Context);
    //CHECKEGLERRORS();
#else
    GLuint PixelFormat;
    
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    
    g_hDC = GetDC( g_hWnd );
    if(!g_hDC)
    {
        nvFX::printf("Failed to get the Windows device context!\n");
        exit(1);
    }
    PixelFormat = ChoosePixelFormat( g_hDC, &pfd );
    SetPixelFormat( g_hDC, PixelFormat, &pfd);
    g_hRC = wglCreateContext( g_hDC );
    if(!g_hRC)
    {
        nvFX::printf("Failed to create OpenGL context!\n");
        exit(1);
    }
    wglMakeCurrent( g_hDC, g_hRC );
    //wglSwapInterval(0);

    GLenum res = glewInit();
    if(res)
    {
        nvFX::printf("Failed to initialize Glew\n");
        exit(1);
    }
    GLboolean b = glewIsSupported("GL_ARB_texture_compression");

    GETPROCADDRESS(PFNWGLCREATECONTEXTATTRIBSARB,wglCreateContextAttribsARB)
    if(wglCreateContextAttribsARB)
    {
        HGLRC hRC = NULL;
        int attribList[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 0,
            WGL_CONTEXT_FLAGS_ARB, //WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB|
#ifdef _DEBUG
            WGL_CONTEXT_DEBUG_BIT_ARB
#else
            0
#endif
            ,0, 0
        };
        // First try creating an OpenGL 3.1 context.
        if (!(hRC = wglCreateContextAttribsARB(g_hDC, 0, attribList)))
        {
            // Fall back to an OpenGL 3.0 context.
            attribList[3] = 0;
            if (!(hRC = wglCreateContextAttribsARB(g_hDC, 0, attribList)))
                nvFX::printf("wglCreateContextAttribsARB() failed for OpenGL 4.2 context.\n");
        }
        else if (!wglMakeCurrent(g_hDC, hRC))
        {
            nvFX::printf("wglMakeCurrent() failed\n");
        }
        else
        {
            g_hRC = hRC;
#ifdef _DEBUG
        //GETPROCADDRESS(PFNGLDEBUGMESSAGECONTROLARB,     glDebugMessageControlARB);
        //GETPROCADDRESS(PFNGLDEBUGMESSAGEINSERTARB,      glDebugMessageInsertARB);
        //GETPROCADDRESS(PFNGLDEBUGMESSAGECALLBACKARB,    glDebugMessageCallbackARB);
        //GETPROCADDRESS(PFNGLGETDEBUGMESSAGELOGARB,      glGetDebugMessageLogARB);
        //GETPROCADDRESS(PFNWGLGETPOINTERV,               glGetPointerv);
        if(glDebugMessageCallbackARB)
        {
            glDebugMessageCallbackARB((GLDEBUGPROCARB)myOpenGLCallback, NULL);
        }
#endif
        }
    }
#endif
}

nvFX::IContainer* parseEffect(const char * pathName, bool dumpASM)
{
    nvFX::IContainer*    pGLSLFx;
    nvFX::ITechnique*    fxTech;
    nvFX::IPass*         fxPass;

    nvFX::printf("\n----------------- Processing %s -----------------\n", pathName);
    pGLSLFx = nvFX::IContainer::create("nvFXcc");
    if(!pGLSLFx)
    {
        nvFX::printf("Failed to create the effect container!\n");
        return NULL;
    }
    //
    // extract the path of the file
    //
    strncpy(g_path, pathName, 200);
    for(int i=(int)strlen(g_path); i>0; i--)
    {
        if((g_path[i] == '/')||(g_path[i] == '\\'))
        {
            g_path[i] = '\0';
            break;
        }
    }
    //
    // Load the effect
    //
    bool failed = false;
    if(nvFX::loadEffectFromFile(pGLSLFx, pathName))
    {
        nvFX::getResourceRepositorySingleton()->setParams(0,0,100,100,1,0,(void*)NULL);
        if(!nvFX::getResourceRepositorySingleton()->validateAll() )
            failed = true;
        nvFX::printf("Frame buffer objects validation Done\n");
        for(int i=0; fxTech = pGLSLFx->findTechnique(i); i++)
        {
            nvFX::printf("TECHNIQUE %s :\n", fxTech->getName());
            for(int j=0; fxPass = fxTech->getPass(j); j++)
            {
                nvFX::printf("  PASS %s\n", fxPass->getName());
                bool bRes = fxPass->validate();
                if(!bRes)
                {
                    //nvFX::printf("\nErrors with Pass %s\n", fxPass->getName());
                    //return 1;
                    failed  = true;
                }
                if(dumpASM)
                {
                    nvFX::IProgram* pProg = fxPass->getExInterface()->getProgram(0);
                    nvFX::IProgramEx* pProgEx = NULL;
                    if(pProg) {
                        pProgEx = pProg->getExInterface();
                        if(pProgEx)
                        {
                            int l = pProgEx->getASMCode(NULL, 0);
                            char *buf = (char *)malloc(l);
                            pProgEx->getASMCode(buf, l);
                            nvFX::printf("----------------------------------------------\n");
                            nvFX::printf("%s\n", buf);
                            nvFX::printf("----------------------------------------------\n");
                        }
                    } else {
                        nvFX::IProgramPipeline *pPP = fxPass->getExInterface()->getProgramPipeline(0);
                        if(pPP)
                            for(int j=0; pProg = pPP->getShaderProgram(j); j++)
                            {
                                nvFX::printf("-------------------- %d ----------------------\n", j);
                                int sf = pProg->getProgramShaderFlags();
                                if(sf & FX_VERTEX_SHADER_BIT)
                                    nvFX::printf("FX_VERTEX_SHADER\n");
                                if(sf & FX_FRAGMENT_SHADER_BIT)
                                    nvFX::printf("FX_FRAGMENT_SHADER\n");
                                if(sf & FX_GEOMETRY_SHADER_BIT)
                                    nvFX::printf("FX_GEOMETRY_SHADER\n");
                                if(sf & FX_TESS_CONTROL_SHADER_BIT)
                                    nvFX::printf("FX_TESS_CONTROL_SHADER\n");
                                if(sf & FX_TESS_EVALUATION_SHADER_BIT)
                                    nvFX::printf("FX_TESS_EVALUATION_SHADER\n");
                                if(sf & FX_COMPUTE_SHADER_BIT)
                                    nvFX::printf("FX_COMPUTE_SHADER\n");
                                pProgEx = pProg->getExInterface();
                                int l = pProgEx->getASMCode(NULL, 0);
                                char *buf = (char *)malloc(l);
                                pProgEx->getASMCode(buf, l);
                                nvFX::printf("%s\n", buf);
                                nvFX::printf("----------------------------------------------\n");
                            }
                    }
                }
            }
        }
    } else {
        nvFX::printf("Error>> failed to load the effect\n");
        failed = true;
    }
    if(failed)
    {
        nvFX::IContainer::destroy(pGLSLFx);
        return NULL;
    }
    nvFX::printf("Effect Validation Done\n");
#if 0
    // Now create the serialization file if requested
    char fn[400];
    sprintf(fn, "%s.cpp", pathName);
    pGLSLFx->getExInterface()->serializeToCpp(fn);
    sprintf(fn, "%s.nvfx", pathName);
    pGLSLFx->getExInterface()->serializeToNvFX(fn);
#endif
    return pGLSLFx;
}

int _tmain(int argc, _TCHAR* argv[])
{
    if(argc <= 1)
    {
        nvFX::printf("%s [-o<logfile>] <nvFX file> [nvFX file2 [...]]\n", argv[0]);
        return 1;
    }
    //
    // fake window for OpenGL...
    //
    WNDCLASSEX winClass;
    MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));

    HINSTANCE g_hInstance = GetModuleHandle(NULL);

    winClass.lpszClassName = "NVFX_WINDOWS_CLASS";
    winClass.cbSize        = sizeof(WNDCLASSEX);
    winClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    winClass.lpfnWndProc   = DefWindowProc;
    winClass.hInstance     = g_hInstance;
    winClass.hIcon           = 0;
    winClass.hIconSm       = 0;
    winClass.hCursor       = 0;
    winClass.hbrBackground = 0;
    winClass.lpszMenuName  = NULL;
    winClass.cbClsExtra    = 0;
    winClass.cbWndExtra    = 0;
    
    if(!RegisterClassEx(&winClass) )
        return E_FAIL;

    g_hWnd = CreateWindowEx( NULL, "NVFX_WINDOWS_CLASS",
                             "nvFX Compilation",
                             WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, 
                             g_hInstance, NULL );
    if( g_hWnd == NULL )
        return E_FAIL;
    //
    // Init GL for GLSL compilation part
    //
    initGL();

    unsigned int v = nvFX::getVersion();
    nvFX::printf("\nnvFX v%.2d.%.4d\n", v>>16, v&0xFFFF);
    //
    // take care of an effect
    //
    nvFX::setErrorCallback(myErrorCb);
    nvFX::setMessageCallback(myMsgCb);
    nvFX::setIncludeCallback(myIncludeCb);
    int failed = 0;
    bool dumpASM = false;
    for(int c=1; c<argc; c++)
    {
        if(*argv[c] == '-') switch(argv[c][1])
        {
            case 'o':
                fileOut = fopen(argv[c]+2,"w");
                if(!fileOut)
                    fprintf(stderr, "couldn't open for writing '%s'\n", argv[c][2]);
                break;
            case 'a':
                dumpASM = true;
                break;
            case 'h':
            case '?':
                fprintf(stdout, "%s [-a] -oOutFile <effect [effect2 [...]]>\n", argv[0]);
                break;
            default:
                fprintf(stderr, "Unknown option %c\n", argv[c][1]);
                break;
        }
        else if(parseEffect(argv[c], dumpASM) == NULL)
        {
            failed = 1;
        }
    }
    DestroyWindow(g_hWnd);
    UnregisterClass( "NVFX_WINDOWS_CLASS", g_hInstance );
    if(failed)
    {
        nvFX::printf("---------------\n");
        nvFX::printf("-- %d Errors --\n", g_nErrors);
        nvFX::printf("---------------\n");
        return 1;
    }
    nvFX::printf("----------------\n");
    nvFX::printf("-- All passed --\n");
    nvFX::printf("----------------\n");
    if(fileOut)
        fclose(fileOut);
    return 0;
}

