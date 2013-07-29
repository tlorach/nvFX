// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "windows.h"

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// MISSING OpenGL funcs that I don't have in Glew
//-----------------------------------------------------------------------------
#define GLAPIENTRY_ __stdcall
#define GETPROCADDRESS(t,n)\
	if(!n) n = (t)wglGetProcAddress(#n);\
    if(NULL == n) { PrintMessage("Error>> Failed to get pointer for %s\n", #n); };
typedef HGLRC (GLAPIENTRY_ * PFNWGLCREATECONTEXTATTRIBSARB) (HDC hDC, HGLRC hShareContext, const int *attribList);
extern PFNWGLCREATECONTEXTATTRIBSARB wglCreateContextAttribsARB;
#define WGL_CONTEXT_MAJOR_VERSION_ARB   0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB   0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB     0x2093
#define WGL_CONTEXT_FLAGS_ARB           0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB    0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB		0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	0x0002
#if 0
typedef GLvoid (GLAPIENTRY_ * PFNGLDEBUGMESSAGECONTROLARB) (enum source,
                            GLenum type,
                            GLenum severity,
                            GLsizei count,
                            const GLuint* ids,
                            GLboolean enabled);
extern PFNGLDEBUGMESSAGECONTROLARB glDebugMessageControlARB;

typedef GLvoid (GLAPIENTRY_ * PFNGLDEBUGMESSAGEINSERTARB) (enum source,
                           GLenum type,
                           GLuint id,
                           GLenum severity,
                           GLsizei length, 
                           const char* buf);
extern PFNGLDEBUGMESSAGEINSERTARB glDebugMessageInsertARB;

#define GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB                     0x8242

#define GL_MAX_DEBUG_MESSAGE_LENGTH_ARB                     0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_ARB                    0x9144
#define GL_DEBUG_LOGGED_MESSAGES_ARB                        0x9145
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB             0x8243

#define GL_DEBUG_CALLBACK_FUNCTION_ARB                      0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM_ARB                    0x8245

#define GL_DEBUG_SOURCE_API_ARB                             0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB                   0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER_ARB                 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY_ARB                     0x8249
#define GL_DEBUG_SOURCE_APPLICATION_ARB                     0x824A
#define GL_DEBUG_SOURCE_OTHER_ARB                           0x824B

#define GL_DEBUG_TYPE_ERROR_ARB                             0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB               0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB                0x824E
#define GL_DEBUG_TYPE_PORTABILITY_ARB                       0x824F
#define GL_DEBUG_TYPE_PERFORMANCE_ARB                       0x8250
#define GL_DEBUG_TYPE_OTHER_ARB                             0x8251
    
#define GL_DEBUG_SEVERITY_HIGH_ARB                          0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_ARB                        0x9147
#define GL_DEBUG_SEVERITY_LOW_ARB                           0x9148
typedef void (GLAPIENTRY *GLDEBUGPROCARB)(GLenum source,
                                                GLenum type,
                                                GLuint id,
                                                GLenum severity,
                                                GLsizei length,
                                                const GLchar* message,
                                                GLvoid* userParam);
typedef GLvoid (GLAPIENTRY_ * PFNGLDEBUGMESSAGECALLBACKARB) (GLDEBUGPROCARB callback,
                             void* userParam);
extern PFNGLDEBUGMESSAGECALLBACKARB glDebugMessageCallbackARB;

typedef GLuint (GLAPIENTRY_ * PFNGLGETDEBUGMESSAGELOGARB) (GLuint count,
                           GLsizei bufsize,
                           GLenum* sources,
                           GLenum* types,
                           GLuint* ids,
                           GLenum* severities,
                           GLsizei* lengths, 
                           GLchar* messageLog);
extern PFNGLGETDEBUGMESSAGELOGARB glGetDebugMessageLogARB;

#endif
