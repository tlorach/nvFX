#pragma once
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
#ifndef __BK3DDEFS__
#define __BK3DDEFS__

//-------------------------------------------------
// 
// D3D9 definitions
// ----------------
// This part is needed when OpenGL/DX9-10 is not used :
// some enums & defines are needed anyways.
// Instead of including OpenGL only for that,
// this section will define them
// Furthermore : this can be used by any exporter/converter
// 
//-------------------------------------------------
#ifndef _d3d9TYPES_H_
#ifdef BK3DVERBOSE
#pragma message("defining D3DPRIMITIVETYPE here...")
#endif
  enum D3DPRIMITIVETYPE
  {
    D3DPT_UNDEFINED             = 0,
    D3DPT_POINTLIST             = 1,
    D3DPT_LINELIST              = 2,
    D3DPT_LINESTRIP             = 3,
    D3DPT_TRIANGLELIST          = 4,
    D3DPT_TRIANGLESTRIP         = 5,
    D3DPT_TRIANGLEFAN           = 6,
	D3DPT_END = -1
};
#ifdef BK3DVERBOSE
#pragma message("defining D3DFORMAT here...")
#endif
  enum D3DFORMAT
  {
    D3DFMT_INDEX16              = 101,
    D3DFMT_INDEX32              = 102,
	D3DFMT_END = -1
  };
#ifdef BK3DVERBOSE
#pragma message("defining D3DDECLTYPE here...")
#endif
  enum D3DDECLTYPE
  {
    D3DDECLTYPE_FLOAT1 = 0,
    D3DDECLTYPE_FLOAT2 = 1,
    D3DDECLTYPE_FLOAT3 = 2,
    D3DDECLTYPE_FLOAT4 = 3,
    D3DDECLTYPE_D3DCOLOR = 4,
    D3DDECLTYPE_UBYTE4 = 5,
    D3DDECLTYPE_SHORT2 = 6,
    D3DDECLTYPE_SHORT4 = 7,
    D3DDECLTYPE_UBYTE4N = 8,
    D3DDECLTYPE_SHORT2N = 9,
    D3DDECLTYPE_SHORT4N = 10,
    D3DDECLTYPE_USHORT2N = 11,
    D3DDECLTYPE_USHORT4N = 12,
    D3DDECLTYPE_UDEC3 = 13,
    D3DDECLTYPE_DEC3N = 14,
    D3DDECLTYPE_FLOAT16_2 = 15,
    D3DDECLTYPE_FLOAT16_4 = 16,
    D3DDECLTYPE_UNUSED = 17,

    D3DDECLTYPE_UNDEF     =  -1,
  };
#endif

//-------------------------------------------------
// 
// D3D10 definitions
// ----------------
// This part is needed when OpenGL is not used :
// some enums & defines are needed anyways.
// Instead of including OpenGL only for that,
// this section will define them
// Furthermore : this can be used by any exporter/converter
// 
//-------------------------------------------------
#ifndef __d3d10_h__
#ifdef BK3DVERBOSE
#pragma message("defining D3D10_PRIMITIVE_TOPOLOGY enum...")
#endif
  enum D3D10_PRIMITIVE_TOPOLOGY
  {	
      D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED	        = 0,
	  D3D10_PRIMITIVE_TOPOLOGY_POINTLIST	        = 1,
	  D3D10_PRIMITIVE_TOPOLOGY_LINELIST	            = 2,
	  D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP	        = 3,
	  D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST	        = 4,
	  D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP	    = 5,
	  D3D10_PRIMITIVE_TOPOLOGY_LINELIST_ADJ	        = 10,
	  D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ	    = 11,
	  D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ	    = 12,
	  D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ	= 13,
	  //D3D10_PRIMITIVE_TOPOLOGY_FAN	            = 14                // Doesn't exist in DXGI...
	  D3D10_PT_END = -1
  };
#ifdef BK3DVERBOSE
#pragma message("defining DXGI_FORMAT enum...")
#endif
  enum DXGI_FORMAT // stick to DXGI values
  {
    DXGI_FORMAT_UNKNOWN	                    = 0,
    DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
    DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
    DXGI_FORMAT_R32G32B32A32_UINT           = 3,
    DXGI_FORMAT_R32G32B32A32_SINT           = 4,
    DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
    DXGI_FORMAT_R32G32B32_FLOAT             = 6,
    DXGI_FORMAT_R32G32B32_UINT              = 7,
    DXGI_FORMAT_R32G32B32_SINT              = 8,
    DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
    DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
    DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
    DXGI_FORMAT_R16G16B16A16_UINT           = 12,
    DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
    DXGI_FORMAT_R16G16B16A16_SINT           = 14,
    DXGI_FORMAT_R32G32_TYPELESS             = 15,
    DXGI_FORMAT_R32G32_FLOAT                = 16,
    DXGI_FORMAT_R32G32_UINT                 = 17,
    DXGI_FORMAT_R32G32_SINT                 = 18,
    DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
    DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
    DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
    DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
    DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
    DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
    DXGI_FORMAT_R10G10B10A2_UINT            = 25,
    DXGI_FORMAT_R11G11B10_FLOAT             = 26,
    DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
    DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
    DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
    DXGI_FORMAT_R8G8B8A8_UINT               = 30,
    DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
    DXGI_FORMAT_R8G8B8A8_SINT               = 32,
    DXGI_FORMAT_R16G16_TYPELESS             = 33,
    DXGI_FORMAT_R16G16_FLOAT                = 34,
    DXGI_FORMAT_R16G16_UNORM                = 35,
    DXGI_FORMAT_R16G16_UINT                 = 36,
    DXGI_FORMAT_R16G16_SNORM                = 37,
    DXGI_FORMAT_R16G16_SINT                 = 38,
    DXGI_FORMAT_R32_TYPELESS                = 39,
    DXGI_FORMAT_D32_FLOAT                   = 40,
    DXGI_FORMAT_R32_FLOAT                   = 41,
    DXGI_FORMAT_R32_UINT                    = 42,
    DXGI_FORMAT_R32_SINT                    = 43,
    DXGI_FORMAT_R24G8_TYPELESS              = 44,
    DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
    DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
    DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
    DXGI_FORMAT_R8G8_TYPELESS               = 48,
    DXGI_FORMAT_R8G8_UNORM                  = 49,
    DXGI_FORMAT_R8G8_UINT                   = 50,
    DXGI_FORMAT_R8G8_SNORM                  = 51,
    DXGI_FORMAT_R8G8_SINT                   = 52,
    DXGI_FORMAT_R16_TYPELESS                = 53,
    DXGI_FORMAT_R16_FLOAT                   = 54,
    DXGI_FORMAT_D16_UNORM                   = 55,
    DXGI_FORMAT_R16_UNORM                   = 56,
    DXGI_FORMAT_R16_UINT                    = 57,
    DXGI_FORMAT_R16_SNORM                   = 58,
    DXGI_FORMAT_R16_SINT                    = 59,
    DXGI_FORMAT_R8_TYPELESS                 = 60,
    DXGI_FORMAT_R8_UNORM                    = 61,
    DXGI_FORMAT_R8_UINT                     = 62,
    DXGI_FORMAT_R8_SNORM                    = 63,
    DXGI_FORMAT_R8_SINT                     = 64,
    DXGI_FORMAT_A8_UNORM                    = 65,
    DXGI_FORMAT_R1_UNORM                    = 66,
    DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
    DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
    DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
    DXGI_FORMAT_BC1_TYPELESS                = 70,
    DXGI_FORMAT_BC1_UNORM                   = 71,
    DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
    DXGI_FORMAT_BC2_TYPELESS                = 73,
    DXGI_FORMAT_BC2_UNORM                   = 74,
    DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
    DXGI_FORMAT_BC3_TYPELESS                = 76,
    DXGI_FORMAT_BC3_UNORM                   = 77,
    DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
    DXGI_FORMAT_BC4_TYPELESS                = 79,
    DXGI_FORMAT_BC4_UNORM                   = 80,
    DXGI_FORMAT_BC4_SNORM                   = 81,
    DXGI_FORMAT_BC5_TYPELESS                = 82,
    DXGI_FORMAT_BC5_UNORM                   = 83,
    DXGI_FORMAT_BC5_SNORM                   = 84,
    DXGI_FORMAT_B5G6R5_UNORM                = 85,
    DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
    DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
    DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
    DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
    DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
    DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
    DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
    DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
    DXGI_FORMAT_BC6H_TYPELESS               = 94,
    DXGI_FORMAT_BC6H_UF16                   = 95,
    DXGI_FORMAT_BC6H_SF16                   = 96,
    DXGI_FORMAT_BC7_TYPELESS                = 97,
    DXGI_FORMAT_BC7_UNORM                   = 98,
    DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
	DXGI_FORMAT_END = -1
  };
enum D3D10_INPUT_CLASSIFICATION
    {	D3D10_INPUT_PER_VERTEX_DATA	= 0,
	D3D10_INPUT_PER_INSTANCE_DATA	= 1
    };
#endif

#if !defined( __d3d11_h__ )
#if defined __d3dcommon_h__
#define D3D11_PRIMITIVE_TOPOLOGY D3D_PRIMITIVE_TOPOLOGY
#else
typedef 
enum D3D11_PRIMITIVE_TOPOLOGY
    {
    D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED	= 0,
    D3D11_PRIMITIVE_TOPOLOGY_POINTLIST	= 1,
    D3D11_PRIMITIVE_TOPOLOGY_LINELIST	= 2,
    D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP	= 3,
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST	= 4,
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP	= 5,
    D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ	= 10,
    D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ	= 11,
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ	= 12,
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ	= 13,
    D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST	= 33,
    D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST	= 34,
    D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST	= 35,
    D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST	= 36,
    D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST	= 37,
    D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST	= 38,
    D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST	= 39,
    D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST	= 40,
    D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST	= 41,
    D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST	= 42,
    D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST	= 43,
    D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST	= 44,
    D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST	= 45,
    D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST	= 46,
    D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST	= 47,
    D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST	= 48,
    D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST	= 49,
    D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST	= 50,
    D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST	= 51,
    D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST	= 52,
    D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST	= 53,
    D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST	= 54,
    D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST	= 55,
    D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST	= 56,
    D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST	= 57,
    D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST	= 58,
    D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST	= 59,
    D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST	= 60,
    D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST	= 61,
    D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST	= 62,
    D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST	= 63,
    D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST	= 64
    } 	D3D11_PRIMITIVE_TOPOLOGY;
#endif // common
#endif //d3d11

//-------------------------------------------------
// 
// OpenGL enums...
// This part is needed when OpenGL is not used :
// some enums & defines are needed anyways.
// Instead of including OpenGL only for that,
// this section will define them
// Furthermore : this can be used by any exporter/converter
// 
//-------------------------------------------------
#ifndef __gl_h_
  typedef unsigned int GLenum;
  typedef GLenum GLType;
  typedef GLenum GLTopology;
  //enum GLTopology // turn GL enums in real enums ?
  //{
#define    GL_POINTS                         0x0000
#define    GL_LINES                          0x0001
#define    GL_LINE_LOOP                      0x0002
#define    GL_LINE_STRIP                     0x0003
#define    GL_TRIANGLES                      0x0004
#define    GL_TRIANGLE_STRIP                 0x0005
#define    GL_TRIANGLE_FAN                   0x0006
#define    GL_QUADS                          0x0007
#define    GL_QUAD_STRIP                     0x0008
#define    GL_PATCHES                        0x000E
  //};
  //enum GLType
  //{
    // enums from OpenGL so that we are directly ready
#define    GL_BYTE                           0x1400
#define    GL_UNSIGNED_BYTE                  0x1401
#define    GL_SHORT                          0x1402
#define    GL_UNSIGNED_SHORT                 0x1403
#define    GL_INT                            0x1404
#define    GL_UNSIGNED_INT                   0x1405
#define    GL_FLOAT                          0x1406
#define    GL_2_BYTES                        0x1407
#define    GL_3_BYTES                        0x1408
#define    GL_4_BYTES                        0x1409
#define    GL_DOUBLE                         0x140A
  //};
#else
  typedef GLenum GLType;
  typedef GLenum GLTopology;
#endif
enum OGL_PATCH_VERTICES
{
	GL_PATCH_VERTICES_0	= 32,
	GL_PATCH_VERTICES_1	= 33,
	GL_PATCH_VERTICES_2	= 34,
	GL_PATCH_VERTICES_3	= 35,
	GL_PATCH_VERTICES_4	= 36,
	GL_PATCH_VERTICES_5	= 37,
	GL_PATCH_VERTICES_6	= 38,
	GL_PATCH_VERTICES_7	= 39,
	GL_PATCH_VERTICES_8	= 40,
	GL_PATCH_VERTICES_9	= 41,
	GL_PATCH_VERTICES_10	= 42,
	GL_PATCH_VERTICES_11	= 43,
	GL_PATCH_VERTICES_12	= 44,
	GL_PATCH_VERTICES_13	= 45,
	GL_PATCH_VERTICES_14	= 46,
	GL_PATCH_VERTICES_15	= 47,
	GL_PATCH_VERTICES_16	= 48,
	GL_PATCH_VERTICES_17	= 49,
	GL_PATCH_VERTICES_18	= 50,
	GL_PATCH_VERTICES_19	= 51,
	GL_PATCH_VERTICES_20	= 52,
	GL_PATCH_VERTICES_21	= 53,
	GL_PATCH_VERTICES_22	= 54,
	GL_PATCH_VERTICES_23	= 55,
	GL_PATCH_VERTICES_24	= 56,
	GL_PATCH_VERTICES_25	= 57,
	GL_PATCH_VERTICES_26	= 58,
	GL_PATCH_VERTICES_27	= 59,
	GL_PATCH_VERTICES_28	= 60,
	GL_PATCH_VERTICES_29	= 61,
	GL_PATCH_VERTICES_30	= 62,
	GL_PATCH_VERTICES_31	= 63,
	GL_PATCH_VERTICES_32	= 64
};


///
/// \brief These are the typical names of attributes that could be in the bk3d baked file
/// @{
#define MESH_POSITION        "position"
#define MESH_VERTEXID        "vertexid"
#define MESH_COLOR           "color"
#define MESH_FACENORMAL      "facenormal"
#define MESH_TANGENT         "tangent"
#define MESH_BINORMAL        "binormal"
#define MESH_NORMAL          "normal"
#define MESH_TEXCOORD0       "texcoord0"
#define MESH_TEXCOORD1       "texcoord1"
#define MESH_TEXCOORD2       "texcoord2"
#define MESH_TEXCOORD3       "texcoord3"
#define MESH_BLIND0          "blind0"
#define MESH_BLIND1          "blind1"
#define MESH_BLIND2          "blind2"
#define MESH_BLIND3          "blind3"
#define MESH_BONESOFFSETS    "bonesoffsets"
#define MESH_BONESWEIGHTS    "bonesweights"
#define MESH_2BONES2WEIGHTS  "2Bones2Weights"
/// @}

#endif //__BK3DDEFS__

