/*
    Copyright (c) 2013, NVIDIA CORPORATION. All rights reserved.

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
//
// nvImageDDS.cpp - Image support class
//
// The nvImage class implements an interface for a multipurpose image
// object. This class is useful for loading and formating images
// for use as textures. The class supports dds, png, and hdr formats.
//
// This file implements the DDS specific functionality.
//
// Author: Evan Hart
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "nvImage.h"


using std::vector;

namespace nv {

//
//  Structure defines and constants from nvdds
//
//////////////////////////////////////////////////////////////////////

// surface description flags
const unsigned long DDSF_CAPS           = 0x00000001l;
const unsigned long DDSF_HEIGHT         = 0x00000002l;
const unsigned long DDSF_WIDTH          = 0x00000004l;
const unsigned long DDSF_PITCH          = 0x00000008l;
const unsigned long DDSF_PIXELFORMAT    = 0x00001000l;
const unsigned long DDSF_MIPMAPCOUNT    = 0x00020000l;
const unsigned long DDSF_LINEARSIZE     = 0x00080000l;
const unsigned long DDSF_DEPTH          = 0x00800000l;

// pixel format flags
const unsigned long DDSF_ALPHAPIXELS    = 0x00000001l;
const unsigned long DDSF_FOURCC         = 0x00000004l;
const unsigned long DDSF_RGB            = 0x00000040l;
const unsigned long DDSF_RGBA           = 0x00000041l;

// dwCaps1 flags
const unsigned long DDSF_COMPLEX         = 0x00000008l;
const unsigned long DDSF_TEXTURE         = 0x00001000l;
const unsigned long DDSF_MIPMAP          = 0x00400000l;

// dwCaps2 flags
const unsigned long DDSF_CUBEMAP         = 0x00000200l;
const unsigned long DDSF_CUBEMAP_POSITIVEX  = 0x00000400l;
const unsigned long DDSF_CUBEMAP_NEGATIVEX  = 0x00000800l;
const unsigned long DDSF_CUBEMAP_POSITIVEY  = 0x00001000l;
const unsigned long DDSF_CUBEMAP_NEGATIVEY  = 0x00002000l;
const unsigned long DDSF_CUBEMAP_POSITIVEZ  = 0x00004000l;
const unsigned long DDSF_CUBEMAP_NEGATIVEZ  = 0x00008000l;
const unsigned long DDSF_CUBEMAP_ALL_FACES  = 0x0000FC00l;
const unsigned long DDSF_VOLUME          = 0x00200000l;

// compressed texture types
const unsigned long FOURCC_UNKNOWN       = 0;

#ifndef MAKEFOURCC
#define MAKEFOURCC(c0,c1,c2,c3) \
	((unsigned long)(unsigned char)(c0)| \
	((unsigned long)(unsigned char)(c1) << 8)| \
	((unsigned long)(unsigned char)(c2) << 16)| \
	((unsigned long)(unsigned char)(c3) << 24))
#endif

const unsigned long FOURCC_R8G8B8        = 20;
const unsigned long FOURCC_A8R8G8B8      = 21;
const unsigned long FOURCC_X8R8G8B8      = 22;
const unsigned long FOURCC_R5G6B5        = 23;
const unsigned long FOURCC_X1R5G5B5      = 24;
const unsigned long FOURCC_A1R5G5B5      = 25;
const unsigned long FOURCC_A4R4G4B4      = 26;
const unsigned long FOURCC_R3G3B2        = 27;
const unsigned long FOURCC_A8            = 28;
const unsigned long FOURCC_A8R3G3B2      = 29;
const unsigned long FOURCC_X4R4G4B4      = 30;
const unsigned long FOURCC_A2B10G10R10   = 31;
const unsigned long FOURCC_A8B8G8R8      = 32;
const unsigned long FOURCC_X8B8G8R8      = 33;
const unsigned long FOURCC_G16R16        = 34;
const unsigned long FOURCC_A2R10G10B10   = 35;
const unsigned long FOURCC_A16B16G16R16  = 36;

const unsigned long FOURCC_L8            = 50;
const unsigned long FOURCC_A8L8          = 51;
const unsigned long FOURCC_A4L4          = 52;
const unsigned long FOURCC_DXT1          = 0x31545844l; //(MAKEFOURCC('D','X','T','1'))
const unsigned long FOURCC_DXT2          = 0x32545844l; //(MAKEFOURCC('D','X','T','1'))
const unsigned long FOURCC_DXT3          = 0x33545844l; //(MAKEFOURCC('D','X','T','3'))
const unsigned long FOURCC_DXT4          = 0x34545844l; //(MAKEFOURCC('D','X','T','3'))
const unsigned long FOURCC_DXT5          = 0x35545844l; //(MAKEFOURCC('D','X','T','5'))
const unsigned long FOURCC_ATI1          = MAKEFOURCC('A','T','I','1');
const unsigned long FOURCC_ATI2          = MAKEFOURCC('A','T','I','2');
const unsigned long FOURCC_BC4U          = MAKEFOURCC('B','C','4','U');
const unsigned long FOURCC_BC4S          = MAKEFOURCC('B','C','4','S');
const unsigned long FOURCC_BC5S          = MAKEFOURCC('B','C','5','S');

const unsigned long FOURCC_D16_LOCKABLE  = 70;
const unsigned long FOURCC_D32           = 71;
const unsigned long FOURCC_D24X8         = 77;
const unsigned long FOURCC_D16           = 80;

const unsigned long FOURCC_D32F_LOCKABLE = 82;

const unsigned long FOURCC_L16           = 81;

const unsigned long FOURCC_DX10          = MAKEFOURCC('D','X','1','0');

// signed normalized formats
const unsigned long FOURCC_Q16W16V16U16  = 110;

// Floating point surface formats

// s10e5 formats (16-bits per channel)
const unsigned long FOURCC_R16F          = 111;
const unsigned long FOURCC_G16R16F       = 112;
const unsigned long FOURCC_A16B16G16R16F = 113;

// IEEE s23e8 formats (32-bits per channel)
const unsigned long FOURCC_R32F          = 114;
const unsigned long FOURCC_G32R32F       = 115;
const unsigned long FOURCC_A32B32G32R32F = 116;

//DXGI enums
const unsigned long DDS10_FORMAT_UNKNOWN = 0;
const unsigned long DDS10_FORMAT_R32G32B32A32_TYPELESS = 1;
const unsigned long DDS10_FORMAT_R32G32B32A32_FLOAT = 2;
const unsigned long DDS10_FORMAT_R32G32B32A32_UINT = 3;
const unsigned long DDS10_FORMAT_R32G32B32A32_SINT = 4;
const unsigned long DDS10_FORMAT_R32G32B32_TYPELESS = 5;
const unsigned long DDS10_FORMAT_R32G32B32_FLOAT = 6;
const unsigned long DDS10_FORMAT_R32G32B32_UINT = 7;
const unsigned long DDS10_FORMAT_R32G32B32_SINT = 8;
const unsigned long DDS10_FORMAT_R16G16B16A16_TYPELESS = 9;
const unsigned long DDS10_FORMAT_R16G16B16A16_FLOAT = 10;
const unsigned long DDS10_FORMAT_R16G16B16A16_UNORM = 11;
const unsigned long DDS10_FORMAT_R16G16B16A16_UINT = 12;
const unsigned long DDS10_FORMAT_R16G16B16A16_SNORM = 13;
const unsigned long DDS10_FORMAT_R16G16B16A16_SINT = 14;
const unsigned long DDS10_FORMAT_R32G32_TYPELESS = 15;
const unsigned long DDS10_FORMAT_R32G32_FLOAT = 16;
const unsigned long DDS10_FORMAT_R32G32_UINT = 17;
const unsigned long DDS10_FORMAT_R32G32_SINT = 18;
const unsigned long DDS10_FORMAT_R32G8X24_TYPELESS = 19;
const unsigned long DDS10_FORMAT_D32_FLOAT_S8X24_UINT = 20;
const unsigned long DDS10_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21;
const unsigned long DDS10_FORMAT_X32_TYPELESS_G8X24_UINT = 22;
const unsigned long DDS10_FORMAT_R10G10B10A2_TYPELESS = 23;
const unsigned long DDS10_FORMAT_R10G10B10A2_UNORM = 24;
const unsigned long DDS10_FORMAT_R10G10B10A2_UINT = 25;
const unsigned long DDS10_FORMAT_R11G11B10_FLOAT = 26;
const unsigned long DDS10_FORMAT_R8G8B8A8_TYPELESS = 27;
const unsigned long DDS10_FORMAT_R8G8B8A8_UNORM = 28;
const unsigned long DDS10_FORMAT_R8G8B8A8_UNORM_SRGB = 29;
const unsigned long DDS10_FORMAT_R8G8B8A8_UINT = 30;
const unsigned long DDS10_FORMAT_R8G8B8A8_SNORM = 31;
const unsigned long DDS10_FORMAT_R8G8B8A8_SINT = 32;
const unsigned long DDS10_FORMAT_R16G16_TYPELESS = 33;
const unsigned long DDS10_FORMAT_R16G16_FLOAT = 34;
const unsigned long DDS10_FORMAT_R16G16_UNORM = 35;
const unsigned long DDS10_FORMAT_R16G16_UINT = 36;
const unsigned long DDS10_FORMAT_R16G16_SNORM = 37;
const unsigned long DDS10_FORMAT_R16G16_SINT = 38;
const unsigned long DDS10_FORMAT_R32_TYPELESS = 39;
const unsigned long DDS10_FORMAT_D32_FLOAT = 40;
const unsigned long DDS10_FORMAT_R32_FLOAT = 41;
const unsigned long DDS10_FORMAT_R32_UINT = 42;
const unsigned long DDS10_FORMAT_R32_SINT = 43;
const unsigned long DDS10_FORMAT_R24G8_TYPELESS = 44;
const unsigned long DDS10_FORMAT_D24_UNORM_S8_UINT = 45;
const unsigned long DDS10_FORMAT_R24_UNORM_X8_TYPELESS = 46;
const unsigned long DDS10_FORMAT_X24_TYPELESS_G8_UINT = 47;
const unsigned long DDS10_FORMAT_R8G8_TYPELESS = 48;
const unsigned long DDS10_FORMAT_R8G8_UNORM = 49;
const unsigned long DDS10_FORMAT_R8G8_UINT = 50;
const unsigned long DDS10_FORMAT_R8G8_SNORM = 51;
const unsigned long DDS10_FORMAT_R8G8_SINT = 52;
const unsigned long DDS10_FORMAT_R16_TYPELESS = 53;
const unsigned long DDS10_FORMAT_R16_FLOAT = 54;
const unsigned long DDS10_FORMAT_D16_UNORM = 55;
const unsigned long DDS10_FORMAT_R16_UNORM = 56;
const unsigned long DDS10_FORMAT_R16_UINT = 57;
const unsigned long DDS10_FORMAT_R16_SNORM = 58;
const unsigned long DDS10_FORMAT_R16_SINT = 59;
const unsigned long DDS10_FORMAT_R8_TYPELESS = 60;
const unsigned long DDS10_FORMAT_R8_UNORM = 61;
const unsigned long DDS10_FORMAT_R8_UINT = 62;
const unsigned long DDS10_FORMAT_R8_SNORM = 63;
const unsigned long DDS10_FORMAT_R8_SINT = 64;
const unsigned long DDS10_FORMAT_A8_UNORM = 65;
const unsigned long DDS10_FORMAT_R1_UNORM = 66;
const unsigned long DDS10_FORMAT_R9G9B9E5_SHAREDEXP = 67;
const unsigned long DDS10_FORMAT_R8G8_B8G8_UNORM = 68;
const unsigned long DDS10_FORMAT_G8R8_G8B8_UNORM = 69;
const unsigned long DDS10_FORMAT_BC1_TYPELESS = 70;
const unsigned long DDS10_FORMAT_BC1_UNORM = 71;
const unsigned long DDS10_FORMAT_BC1_UNORM_SRGB = 72;
const unsigned long DDS10_FORMAT_BC2_TYPELESS = 73;
const unsigned long DDS10_FORMAT_BC2_UNORM = 74;
const unsigned long DDS10_FORMAT_BC2_UNORM_SRGB = 75;
const unsigned long DDS10_FORMAT_BC3_TYPELESS = 76;
const unsigned long DDS10_FORMAT_BC3_UNORM = 77;
const unsigned long DDS10_FORMAT_BC3_UNORM_SRGB = 78;
const unsigned long DDS10_FORMAT_BC4_TYPELESS = 79;
const unsigned long DDS10_FORMAT_BC4_UNORM = 80;
const unsigned long DDS10_FORMAT_BC4_SNORM = 81;
const unsigned long DDS10_FORMAT_BC5_TYPELESS = 82;
const unsigned long DDS10_FORMAT_BC5_UNORM = 83;
const unsigned long DDS10_FORMAT_BC5_SNORM = 84;
const unsigned long DDS10_FORMAT_B5G6R5_UNORM = 85;
const unsigned long DDS10_FORMAT_B5G5R5A1_UNORM = 86;
const unsigned long DDS10_FORMAT_B8G8R8A8_UNORM = 87;
const unsigned long DDS10_FORMAT_B8G8R8X8_UNORM = 88;
const unsigned long DDS10_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89;
const unsigned long DDS10_FORMAT_B8G8R8A8_TYPELESS = 90;
const unsigned long DDS10_FORMAT_B8G8R8A8_UNORM_SRGB = 91;
const unsigned long DDS10_FORMAT_B8G8R8X8_TYPELESS = 92;
const unsigned long DDS10_FORMAT_B8G8R8X8_UNORM_SRGB = 93;
const unsigned long DDS10_FORMAT_BC6H_TYPELESS = 94;
const unsigned long DDS10_FORMAT_BC6H_UF16 = 95;
const unsigned long DDS10_FORMAT_BC6H_SF16 = 96;
const unsigned long DDS10_FORMAT_BC7_TYPELESS = 97;
const unsigned long DDS10_FORMAT_BC7_UNORM = 98;
const unsigned long DDS10_FORMAT_BC7_UNORM_SRGB = 99;
const unsigned long DDS10_FORMAT_FORCE_UINT = 0xffffffffUL;


//DDS 10 resource dimension enums
const unsigned long DDS10_RESOURCE_DIMENSION_UNKNOWN = 0;
const unsigned long DDS10_RESOURCE_DIMENSION_BUFFER = 1;
const unsigned long DDS10_RESOURCE_DIMENSION_TEXTURE1D = 2;
const unsigned long DDS10_RESOURCE_DIMENSION_TEXTURE2D = 3;
const unsigned long DDS10_RESOURCE_DIMENSION_TEXTURE3D = 4;


struct DXTColBlock
{
    GLushort col0;
    GLushort col1;

    GLubyte row[4];
};

struct DXT3AlphaBlock
{
    GLushort row[4];
};

struct DXT5AlphaBlock
{
    GLubyte alpha0;
    GLubyte alpha1;
    
    GLubyte row[6];
};

struct DDS_PIXELFORMAT
{
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwFourCC;
    unsigned long dwRGBBitCount;
    unsigned long dwRBitMask;
    unsigned long dwGBitMask;
    unsigned long dwBBitMask;
    unsigned long dwABitMask;
};

struct DDS_HEADER
{
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwHeight;
    unsigned long dwWidth;
    unsigned long dwPitchOrLinearSize;
    unsigned long dwDepth;
    unsigned long dwMipMapCount;
    unsigned long dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    unsigned long dwCaps1;
    unsigned long dwCaps2;
    unsigned long dwReserved2[3];
};

struct DDS_HEADER_10
{
    unsigned long dxgiFormat;  // check type
    unsigned long resourceDimension; //check type
    unsigned long miscFlag;
    unsigned long arraySize;
    unsigned long reserved;
};

//
//
////////////////////////////////////////////////////////////
bool nv::TranslateDX10Format( const void *ptr, Image &i, int &bytesPerElement, bool &btcCompressed) {
    const DDS_HEADER_10 &header = *(const DDS_HEADER_10*)ptr;

    printf( "translating DX10 Format\n");
    printf( "  header.dxgiFormat = %x\n", header.dxgiFormat);
    printf( "  header.resourceDimension = %x\n", header.resourceDimension);
    printf( "  header.arraySize = %x\n", header.arraySize);
    printf( "  header.miscFlag = %x\n", header.miscFlag);

    switch (header.resourceDimension) {
        case DDS10_RESOURCE_DIMENSION_TEXTURE1D:
        case DDS10_RESOURCE_DIMENSION_TEXTURE2D:
        case DDS10_RESOURCE_DIMENSION_TEXTURE3D:
            //do I really need to do anything here ?
            break;
        case DDS10_RESOURCE_DIMENSION_UNKNOWN:
        case DDS10_RESOURCE_DIMENSION_BUFFER:
        default:
            // these are presently unsupported formats
            printf( "Bad resource dimension\n");
            return false;
    };


#define SET_TYPE_INFO( intf, f, t, size) \
    i._internalFormat = intf; \
    i._format = f; \
    i._type = t; \
    bytesPerElement = size; \
    btcCompressed = false;

#define SET_COMPRESSED_TYPE_INFO( intf, f, t, size) \
    i._internalFormat = intf; \
    i._format = f; \
    i._type = t; \
    bytesPerElement = size; \
    btcCompressed = true;

    switch (header.dxgiFormat) {
        case DDS10_FORMAT_R32G32B32A32_FLOAT:
            SET_TYPE_INFO( GL_RGBA32F, GL_RGBA, GL_FLOAT, 16);
            break;

        case DDS10_FORMAT_R32G32B32A32_UINT:
            SET_TYPE_INFO( GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16);
            break;

        case DDS10_FORMAT_R32G32B32A32_SINT:
            SET_TYPE_INFO( GL_RGBA32I, GL_RGBA_INTEGER, GL_INT, 16);
            break;

        case DDS10_FORMAT_R32G32B32_FLOAT:
            SET_TYPE_INFO( GL_RGBA32F, GL_RGB, GL_FLOAT, 12);
            break;

        case DDS10_FORMAT_R32G32B32_UINT:
            SET_TYPE_INFO( GL_RGB32UI, GL_RGB_INTEGER, GL_UNSIGNED_INT, 12);
            break;

        case DDS10_FORMAT_R32G32B32_SINT:
            SET_TYPE_INFO( GL_RGB32I, GL_RGB_INTEGER, GL_INT, 12);
            break;

        case DDS10_FORMAT_R16G16B16A16_FLOAT:
            SET_TYPE_INFO( GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT, 8);
            break;

        case DDS10_FORMAT_R16G16B16A16_UNORM:
            SET_TYPE_INFO( GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT, 8);
            break;

        case DDS10_FORMAT_R16G16B16A16_UINT:
            SET_TYPE_INFO( GL_RGBA16UI, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, 8);
            break;

        case DDS10_FORMAT_R16G16B16A16_SNORM:
            SET_TYPE_INFO( GL_RGBA16_SNORM, GL_RGBA, GL_SHORT, 8);
            break;

        case DDS10_FORMAT_R16G16B16A16_SINT:
            SET_TYPE_INFO( GL_RGBA16I, GL_RGBA_INTEGER, GL_SHORT, 8);
            break;
        
        case DDS10_FORMAT_R32G32_FLOAT:
            SET_TYPE_INFO( GL_RG32F, GL_RG, GL_FLOAT, 8);
            break;

        case DDS10_FORMAT_R32G32_UINT:
            SET_TYPE_INFO( GL_RG32UI, GL_RG_INTEGER, GL_UNSIGNED_INT, 8);
            break;

        case DDS10_FORMAT_R32G32_SINT:
            SET_TYPE_INFO( GL_RG32I, GL_RG_INTEGER, GL_INT, 8);
            break;

        case DDS10_FORMAT_R32G8X24_TYPELESS:
        case DDS10_FORMAT_D32_FLOAT_S8X24_UINT:
        case DDS10_FORMAT_R32_FLOAT_X8X24_TYPELESS:
        case DDS10_FORMAT_X32_TYPELESS_G8X24_UINT:
            //these formats have no real direct mapping to OpenGL
            // fail creation
            return false;
        
        case DDS10_FORMAT_R10G10B10A2_UNORM:
            SET_TYPE_INFO( GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV, 4); // is the rev version needed?
            break;

        case DDS10_FORMAT_R10G10B10A2_UINT:
            //doesn't exist in OpenGL
            return false;

        case DDS10_FORMAT_R11G11B10_FLOAT:
            SET_TYPE_INFO( GL_R11F_G11F_B10F_EXT, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV_EXT, 4);
            break;

        case DDS10_FORMAT_R8G8B8A8_UNORM:
            SET_TYPE_INFO( GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4);
            break;

        case DDS10_FORMAT_R8G8B8A8_UNORM_SRGB:
            SET_TYPE_INFO( GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE, 4);
            break;

        case DDS10_FORMAT_R8G8B8A8_UINT:
            SET_TYPE_INFO( GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, 4);
            break;

        case DDS10_FORMAT_R8G8B8A8_SNORM:
            SET_TYPE_INFO( GL_RGBA8_SNORM, GL_RGBA, GL_BYTE, 4);
            break;

        case DDS10_FORMAT_R8G8B8A8_SINT:
            SET_TYPE_INFO( GL_RGBA8UI, GL_RGBA_INTEGER, GL_BYTE, 4);
            break;

        case DDS10_FORMAT_R16G16_FLOAT:
            SET_TYPE_INFO( GL_RG16F, GL_RG, GL_HALF_FLOAT, 4);
            break;

        case DDS10_FORMAT_R16G16_UNORM:
            SET_TYPE_INFO( GL_RG16, GL_RG, GL_UNSIGNED_SHORT, 4);
            break;

        case DDS10_FORMAT_R16G16_UINT:
            SET_TYPE_INFO( GL_RG16UI, GL_RG_INTEGER, GL_UNSIGNED_SHORT, 4);
            break;

        case DDS10_FORMAT_R16G16_SNORM:
            SET_TYPE_INFO( GL_RG16_SNORM, GL_RG, GL_SHORT, 4);
            break;

        case DDS10_FORMAT_R16G16_SINT:
            SET_TYPE_INFO( GL_RG16I, GL_RG_INTEGER, GL_SHORT, 4);
            break;

        case DDS10_FORMAT_D32_FLOAT:
            SET_TYPE_INFO( GL_DEPTH_COMPONENT32F, GL_DEPTH, GL_FLOAT, 4);
            break;

        case DDS10_FORMAT_R32_FLOAT:
            SET_TYPE_INFO( GL_R32F, GL_RED, GL_FLOAT, 4);
            break;

        case DDS10_FORMAT_R32_UINT:
            SET_TYPE_INFO( GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, 4);
            break;

        case DDS10_FORMAT_R32_SINT:
            SET_TYPE_INFO( GL_R32I, GL_RED_INTEGER, GL_INT, 4);
            break;

        //these seem a little problematic to deal with
        case DDS10_FORMAT_R24G8_TYPELESS:
        case DDS10_FORMAT_D24_UNORM_S8_UINT:
        case DDS10_FORMAT_R24_UNORM_X8_TYPELESS:
        case DDS10_FORMAT_X24_TYPELESS_G8_UINT:
            //OpenGL doesn't really offer a packed depth stencil textures
            return false;

        case DDS10_FORMAT_R8G8_UNORM:
            SET_TYPE_INFO( GL_RG8, GL_RG, GL_UNSIGNED_BYTE, 2);
            break;

        case DDS10_FORMAT_R8G8_UINT:
            SET_TYPE_INFO( GL_RG8UI, GL_RG_INTEGER, GL_UNSIGNED_BYTE, 2);
            break;

        case DDS10_FORMAT_R8G8_SNORM:
            SET_TYPE_INFO( GL_RG8_SNORM, GL_RG, GL_BYTE, 2);
            break;

        case DDS10_FORMAT_R8G8_SINT:
            SET_TYPE_INFO( GL_RG8I, GL_RG_INTEGER, GL_BYTE, 2);
            break;

        case DDS10_FORMAT_R16_FLOAT:
            SET_TYPE_INFO( GL_R16F, GL_RED, GL_HALF_FLOAT, 2);
            break;

        case DDS10_FORMAT_D16_UNORM:
            SET_TYPE_INFO( GL_DEPTH_COMPONENT16, GL_DEPTH, GL_UNSIGNED_SHORT, 2);
            break;

        case DDS10_FORMAT_R16_UNORM:
            SET_TYPE_INFO( GL_R16, GL_RED, GL_UNSIGNED_SHORT, 2);
            break;

        case DDS10_FORMAT_R16_UINT:
            SET_TYPE_INFO( GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT, 2);
            break;

        case DDS10_FORMAT_R16_SNORM:
            SET_TYPE_INFO( GL_R16_SNORM, GL_RED, GL_SHORT, 2);
            return false;

        case DDS10_FORMAT_R16_SINT:
            SET_TYPE_INFO( GL_R16I, GL_RED_INTEGER, GL_SHORT, 2);
            break;

        case DDS10_FORMAT_R8_UNORM:
            SET_TYPE_INFO( GL_R8, GL_RED, GL_UNSIGNED_BYTE, 1);
            break;

        case DDS10_FORMAT_R8_UINT:
            SET_TYPE_INFO( GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 1);
            break;

        case DDS10_FORMAT_R8_SNORM:
            SET_TYPE_INFO( GL_R8_SNORM, GL_RED, GL_BYTE, 1);
            break;

        case DDS10_FORMAT_R8_SINT:
            SET_TYPE_INFO( GL_R8I, GL_RED_INTEGER, GL_BYTE, 1);
            break;

        case DDS10_FORMAT_A8_UNORM:
            SET_TYPE_INFO( GL_ALPHA8, GL_ALPHA, GL_UNSIGNED_BYTE, 1);
            break;

        case DDS10_FORMAT_R9G9B9E5_SHAREDEXP:
            SET_TYPE_INFO( GL_RGB9_E5, GL_RGB, GL_UNSIGNED_INT_5_9_9_9_REV, 4);
            break;

        case DDS10_FORMAT_R8G8_B8G8_UNORM:
        case DDS10_FORMAT_G8R8_G8B8_UNORM:
            // unsure how to interpret these formats
            return false;

        case DDS10_FORMAT_BC1_UNORM:
            SET_COMPRESSED_TYPE_INFO( GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_UNSIGNED_BYTE, 8);
            break;

        case DDS10_FORMAT_BC1_UNORM_SRGB:
            SET_COMPRESSED_TYPE_INFO( GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_UNSIGNED_BYTE, 8);
            break;

        case DDS10_FORMAT_BC2_UNORM:
            SET_COMPRESSED_TYPE_INFO( GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_UNSIGNED_BYTE, 16);
            break;

        case DDS10_FORMAT_BC2_UNORM_SRGB:
            SET_COMPRESSED_TYPE_INFO( GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_UNSIGNED_BYTE, 16);
            break;

        case DDS10_FORMAT_BC3_UNORM:
            SET_COMPRESSED_TYPE_INFO( GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_UNSIGNED_BYTE, 16);
            break;

        case DDS10_FORMAT_BC3_UNORM_SRGB:
            SET_COMPRESSED_TYPE_INFO( GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_UNSIGNED_BYTE, 16);
            break;

        case DDS10_FORMAT_BC4_UNORM:
            SET_COMPRESSED_TYPE_INFO( GL_COMPRESSED_RED_RGTC1, GL_COMPRESSED_RED_RGTC1, GL_UNSIGNED_BYTE, 8);
            break;

        case DDS10_FORMAT_BC4_SNORM:
            SET_COMPRESSED_TYPE_INFO( GL_COMPRESSED_SIGNED_RED_RGTC1, GL_COMPRESSED_SIGNED_RED_RGTC1, GL_UNSIGNED_BYTE, 8);
            break;

        case DDS10_FORMAT_BC5_UNORM:
            SET_COMPRESSED_TYPE_INFO( GL_COMPRESSED_RG_RGTC2, GL_COMPRESSED_RG_RGTC2, GL_UNSIGNED_BYTE, 16);
            break;

        case DDS10_FORMAT_BC5_SNORM:
            SET_COMPRESSED_TYPE_INFO( GL_COMPRESSED_SIGNED_RG_RGTC2, GL_COMPRESSED_SIGNED_RG_RGTC2, GL_UNSIGNED_BYTE, 16);
            break;

        case DDS10_FORMAT_B5G6R5_UNORM:
            SET_TYPE_INFO( GL_RGB5, GL_BGR, GL_UNSIGNED_SHORT_5_6_5, 2);
            break;

        case DDS10_FORMAT_B5G5R5A1_UNORM:
            SET_TYPE_INFO( GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_5_5_5_1, 2);
            break;

        case DDS10_FORMAT_B8G8R8A8_UNORM:
            SET_TYPE_INFO( GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE, 2);
            break;

        case DDS10_FORMAT_B8G8R8X8_UNORM:
            SET_TYPE_INFO( GL_RGB8, GL_BGRA, GL_UNSIGNED_BYTE, 4);
            break;

        case DDS10_FORMAT_B8G8R8A8_UNORM_SRGB:
            SET_TYPE_INFO( GL_SRGB8_ALPHA8, GL_BGRA, GL_UNSIGNED_BYTE, 4);
            break;

        case DDS10_FORMAT_B8G8R8X8_UNORM_SRGB:
            SET_TYPE_INFO( GL_SRGB8, GL_BGRA, GL_UNSIGNED_BYTE, 4);
            break;

        case DDS10_FORMAT_R32G32B32A32_TYPELESS:
        case DDS10_FORMAT_R32G32B32_TYPELESS:
        case DDS10_FORMAT_R16G16B16A16_TYPELESS:
        case DDS10_FORMAT_R32G32_TYPELESS:
        case DDS10_FORMAT_R10G10B10A2_TYPELESS:
        case DDS10_FORMAT_R8G8B8A8_TYPELESS:
        case DDS10_FORMAT_R16G16_TYPELESS:
        case DDS10_FORMAT_R32_TYPELESS:
        case DDS10_FORMAT_R8G8_TYPELESS:
        case DDS10_FORMAT_R16_TYPELESS:
        case DDS10_FORMAT_R8_TYPELESS:
        case DDS10_FORMAT_BC1_TYPELESS:
        case DDS10_FORMAT_BC3_TYPELESS:
        case DDS10_FORMAT_BC4_TYPELESS:
        case DDS10_FORMAT_BC2_TYPELESS:
        case DDS10_FORMAT_BC5_TYPELESS:
        case DDS10_FORMAT_B8G8R8A8_TYPELESS:
        case DDS10_FORMAT_B8G8R8X8_TYPELESS:
            //unclear what to do with typeless formats, leave them as unsupported for now
            // in the future it might make sense to use a default type, if these are common
            return false;

        case DDS10_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
        case DDS10_FORMAT_R1_UNORM:
        case DDS10_FORMAT_BC6H_TYPELESS:
        case DDS10_FORMAT_BC6H_UF16:
        case DDS10_FORMAT_BC6H_SF16:
        case DDS10_FORMAT_BC7_TYPELESS:
        case DDS10_FORMAT_BC7_UNORM:
        case DDS10_FORMAT_BC7_UNORM_SRGB:
            //these formats are unsupported presently
            return false;

        case DDS10_FORMAT_FORCE_UINT:
        case DDS10_FORMAT_UNKNOWN:
        default:
            //these are errors
            return false;
    };

    i._layers = header.arraySize;
    i._cubeMap = (header.miscFlag & 0x4) != 0;


    return true;
}

//
//
////////////////////////////////////////////////////////////
bool Image::readDDS( const char *file, Image& i) {

    // open file
    FILE *fp = fopen(file, "rb");
    if (fp == NULL)
        return false;

    // read in file marker, make sure its a DDS file
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0)
    {
        fclose(fp);
        return false;
    }

    // read in DDS header
    DDS_HEADER ddsh;
    DDS_HEADER_10 ddsh10;
    fread(&ddsh, sizeof(DDS_HEADER), 1, fp);

    // check if image is a volume texture
    if ((ddsh.dwCaps2 & DDSF_VOLUME) && (ddsh.dwDepth > 0))
        i._depth = ddsh.dwDepth;
    else
        i._depth = 0;

    if ((ddsh.ddspf.dwFlags & DDSF_FOURCC) && (ddsh.ddspf.dwFourCC == FOURCC_DX10)) {
        //This DDS file uses the DX10 header extension
        fread(&ddsh10, sizeof(DDS_HEADER_10), 1, fp);
    }

    // There are flags that are supposed to mark these fields as valid, but some dds files don't set them properly
    i._width = ddsh.dwWidth;
    i._height = ddsh.dwHeight;
    
    if (ddsh.dwFlags & DDSF_MIPMAPCOUNT) {
        i._levelCount = ddsh.dwMipMapCount;
    }
    else
        i._levelCount = 1;

    //check cube-map faces, the DX10 parser will override this
    if ( ddsh.dwCaps2 & DDSF_CUBEMAP && !(ddsh.ddspf.dwFlags & DDSF_FOURCC && ddsh.ddspf.dwFourCC == FOURCC_DX10)) {
        //this is a cubemap, count the faces
        i._layers = 0;
        i._layers += (ddsh.dwCaps2 & DDSF_CUBEMAP_POSITIVEX) ? 1 : 0;
        i._layers += (ddsh.dwCaps2 & DDSF_CUBEMAP_NEGATIVEX) ? 1 : 0;
        i._layers += (ddsh.dwCaps2 & DDSF_CUBEMAP_POSITIVEY) ? 1 : 0;
        i._layers += (ddsh.dwCaps2 & DDSF_CUBEMAP_NEGATIVEY) ? 1 : 0;
        i._layers += (ddsh.dwCaps2 & DDSF_CUBEMAP_POSITIVEZ) ? 1 : 0;
        i._layers += (ddsh.dwCaps2 & DDSF_CUBEMAP_NEGATIVEZ) ? 1 : 0;

        //check for a complete cubemap
        if ( (i._layers != 6) || (i._width != i._height) ) {
            fclose(fp);
            return false;
        }

        i._cubeMap = true;
    }
    else {
        //not a cubemap
        i._layers = 1;
        i._cubeMap = false;
    }

    bool btcCompressed = false;
    int bytesPerElement = 0;

    // figure out what the image format is
    if (ddsh.ddspf.dwFlags & DDSF_FOURCC) 
    {
        switch(ddsh.ddspf.dwFourCC)
        {
            case FOURCC_DXT1:
                i._format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                i._internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                i._type = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                bytesPerElement = 8;
                btcCompressed = true;
                break;

            case FOURCC_DXT2:
            case FOURCC_DXT3:
                i._format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                i._internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                i._type = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                bytesPerElement = 16;
                btcCompressed = true;
                break;

            case FOURCC_DXT4:
            case FOURCC_DXT5:
                i._format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                i._internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                i._type = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                bytesPerElement = 16;
                btcCompressed = true;
                break;

			case FOURCC_ATI1:
                i._format = GL_COMPRESSED_RED_RGTC1;
                i._internalFormat = GL_COMPRESSED_RED_RGTC1;
                i._type = GL_COMPRESSED_RED_RGTC1;
                bytesPerElement = 8;
                btcCompressed = true;
                break;

            case FOURCC_BC4U:
                i._format = GL_COMPRESSED_RED_RGTC1;
                i._internalFormat = GL_COMPRESSED_RED_RGTC1;
                i._type = GL_COMPRESSED_RED_RGTC1;
                bytesPerElement = 8;
                btcCompressed = true;
                break;

            case FOURCC_BC4S:
                i._format = GL_COMPRESSED_SIGNED_RED_RGTC1;
                i._internalFormat = GL_COMPRESSED_SIGNED_RED_RGTC1;
                i._type = GL_COMPRESSED_SIGNED_RED_RGTC1;
                bytesPerElement = 8;
                btcCompressed = true;
                break;

			case FOURCC_ATI2:
                i._format = GL_COMPRESSED_RG_RGTC2; //GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
                i._internalFormat = GL_COMPRESSED_RG_RGTC2; //GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
                i._type = GL_COMPRESSED_RG_RGTC2; //GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
                bytesPerElement = 16;
                btcCompressed = true;
                break;

            case FOURCC_BC5S:
                i._format = GL_COMPRESSED_SIGNED_RG_RGTC2; //GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
                i._internalFormat = GL_COMPRESSED_SIGNED_RG_RGTC2; //GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
                i._type = GL_COMPRESSED_SIGNED_RG_RGTC2; //GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;
                bytesPerElement = 16;
                btcCompressed = true;
                break;

            case FOURCC_R8G8B8:
                i._format = GL_BGR;
                i._internalFormat = GL_RGB8;
                i._type = GL_UNSIGNED_BYTE;
                bytesPerElement = 3;
                break;

            case FOURCC_A8R8G8B8:
                i._format = GL_BGRA;
                i._internalFormat = GL_RGBA8;
                i._type = GL_UNSIGNED_BYTE;
                bytesPerElement = 4;
                break;

            case FOURCC_X8R8G8B8:
                i._format = GL_BGRA;
                i._internalFormat = GL_RGB8;
                i._type = GL_UNSIGNED_INT_8_8_8_8;
                bytesPerElement = 4;
                break;

            case FOURCC_R5G6B5:
                i._format = GL_BGR;
                i._internalFormat = GL_RGB5;
                i._type = GL_UNSIGNED_SHORT_5_6_5;
                bytesPerElement = 2;
                break;

            case FOURCC_A8:
                i._format = GL_ALPHA;
                i._internalFormat = GL_ALPHA8;
                i._type = GL_UNSIGNED_BYTE;
                bytesPerElement = 1;
                break;

            case FOURCC_A2B10G10R10:
                i._format = GL_RGBA;
                i._internalFormat = GL_RGB10_A2;
                i._type = GL_UNSIGNED_INT_10_10_10_2;
                bytesPerElement = 4;
                break;

            case FOURCC_A8B8G8R8:
                i._format = GL_RGBA;
                i._internalFormat = GL_RGBA8;
                i._type = GL_UNSIGNED_BYTE;
                bytesPerElement = 4;
                break;

            case FOURCC_X8B8G8R8:
                i._format = GL_RGBA;
                i._internalFormat = GL_RGB8;
                i._type = GL_UNSIGNED_INT_8_8_8_8;
                bytesPerElement = 4;
                break;

            case FOURCC_A2R10G10B10:
                i._format = GL_BGRA;
                i._internalFormat = GL_RGB10_A2;
                i._type = GL_UNSIGNED_INT_10_10_10_2;
                bytesPerElement = 4;
                break;

            case FOURCC_G16R16:
                i._format = GL_RG;
                i._internalFormat = GL_RG16;
                i._type = GL_UNSIGNED_SHORT;
                bytesPerElement = 4;
                break;

            case FOURCC_A16B16G16R16:
                i._format = GL_RGBA;
                i._internalFormat = GL_RGBA16;
                i._type = GL_UNSIGNED_SHORT;
                bytesPerElement = 8;
                break;

            case FOURCC_L8:
                i._format = GL_LUMINANCE;
                i._internalFormat = GL_LUMINANCE;
                i._type = GL_UNSIGNED_BYTE;
                bytesPerElement = 1;
                break;

            case FOURCC_A8L8:
                i._format = GL_LUMINANCE_ALPHA;
                i._internalFormat = GL_LUMINANCE8_ALPHA8;
                i._type = GL_UNSIGNED_BYTE;
                bytesPerElement = 2;
                break;

            case FOURCC_L16:
                i._format = GL_LUMINANCE;
                i._internalFormat = GL_LUMINANCE16;
                i._type = GL_UNSIGNED_SHORT;
                bytesPerElement = 2;
                break;

            case FOURCC_Q16W16V16U16:
                i._format = GL_RGBA;
                i._internalFormat = GL_RGBA16_SNORM;
                i._type = GL_SHORT;
                bytesPerElement = 8;
                break;

            case FOURCC_R16F:
                i._format = GL_RED; 
                i._internalFormat = GL_R16F; 
                i._type = GL_HALF_FLOAT_ARB;
                bytesPerElement = 2;
                break;

            case FOURCC_G16R16F:
                i._format = GL_RG; 
                i._internalFormat = GL_RG16F; 
                i._type = GL_HALF_FLOAT_ARB;
                bytesPerElement = 4;
                break;

            case FOURCC_A16B16G16R16F:
                i._format = GL_RGBA;
                i._internalFormat = GL_RGBA16F_ARB;
                i._type = GL_HALF_FLOAT_ARB;
                bytesPerElement = 8;
                break;

            case FOURCC_R32F:
                i._format = GL_RED; 
                i._internalFormat = GL_R32F; 
                i._type = GL_FLOAT;
                bytesPerElement = 4;
                break;

            case FOURCC_G32R32F:
                i._format = GL_RG; 
                i._internalFormat = GL_RG32F; 
                i._type = GL_FLOAT;
                bytesPerElement = 8;
                break;

            case FOURCC_A32B32G32R32F:
                i._format = GL_RGBA;
                i._internalFormat = GL_RGBA32F_ARB;
                i._type = GL_FLOAT;
                bytesPerElement = 16;
                break;

            case FOURCC_DX10:
                if (!TranslateDX10Format( &ddsh10, i, bytesPerElement, btcCompressed)) {
                    fclose(fp);
                    return false; //translation from DX10 failed
                }
                break;

            case FOURCC_UNKNOWN:
            case FOURCC_X1R5G5B5:
            case FOURCC_A1R5G5B5:
            case FOURCC_A4R4G4B4:
            case FOURCC_R3G3B2:
            case FOURCC_A8R3G3B2:
            case FOURCC_X4R4G4B4:
            case FOURCC_A4L4:
            case FOURCC_D16_LOCKABLE:
            case FOURCC_D32:
            case FOURCC_D24X8:
            case FOURCC_D16:
            case FOURCC_D32F_LOCKABLE:
                //these are unsupported for now
            default:
                fclose(fp);
                return false;
        }
    }
    else if (ddsh.ddspf.dwFlags == DDSF_RGBA && ddsh.ddspf.dwRGBBitCount == 32)
    {
        if ( ddsh.ddspf.dwRBitMask == 0xff && ddsh.ddspf.dwGBitMask == 0xff00 && ddsh.ddspf.dwBBitMask == 0xff0000 && ddsh.ddspf.dwABitMask == 0xff000000 ) {
            //RGBA8 order
            i._format = GL_RGBA;
            i._internalFormat = GL_RGBA8;
            i._type = GL_UNSIGNED_BYTE;
        }
        else if ( ddsh.ddspf.dwRBitMask == 0xff0000 && ddsh.ddspf.dwGBitMask == 0xff00 && ddsh.ddspf.dwBBitMask == 0xff && ddsh.ddspf.dwABitMask == 0xff000000 ) {
            //BGRA8 order
            i._format = GL_BGRA;
            i._internalFormat = GL_RGBA8;
            i._type = GL_UNSIGNED_BYTE;
        } 
        else if ( ddsh.ddspf.dwRBitMask == 0x3ff00000 && ddsh.ddspf.dwGBitMask == 0xffc00 && ddsh.ddspf.dwBBitMask == 0x3ff && ddsh.ddspf.dwABitMask == 0xc0000000 ) {
            //BGR10_A2 order
            i._format = GL_RGBA;
            i._internalFormat = GL_RGB10_A2;
            i._type = GL_UNSIGNED_INT_2_10_10_10_REV; //GL_UNSIGNED_INT_10_10_10_2;
        }
        else if ( ddsh.ddspf.dwRBitMask == 0x3ff && ddsh.ddspf.dwGBitMask == 0xffc00 && ddsh.ddspf.dwBBitMask == 0x3ff00000 && ddsh.ddspf.dwABitMask == 0xc0000000 ) {
            //RGB10_A2 order
            i._format = GL_RGBA;
            i._internalFormat = GL_RGB10_A2;
            i._type = GL_UNSIGNED_INT_10_10_10_2;
        }
        else {
            //we'll just guess BGRA8, because that is the common legacy format for improperly labeled files
            i._format = GL_BGRA;
            i._internalFormat = GL_RGBA8;
            i._type = GL_UNSIGNED_BYTE;
        }
        bytesPerElement = 4;
    }
    else if (ddsh.ddspf.dwFlags == DDSF_RGB  && ddsh.ddspf.dwRGBBitCount == 32)
    {
        if ( ddsh.ddspf.dwRBitMask == 0xffff && ddsh.ddspf.dwGBitMask == 0xffff0000 && ddsh.ddspf.dwBBitMask == 0x00 && ddsh.ddspf.dwABitMask == 0x00 ) {
            i._format = GL_RG;
            i._internalFormat = GL_RG16;
            i._type = GL_UNSIGNED_SHORT;
        }
        else if ( ddsh.ddspf.dwRBitMask == 0xff && ddsh.ddspf.dwGBitMask == 0xff00 && ddsh.ddspf.dwBBitMask == 0xff0000 && ddsh.ddspf.dwABitMask == 0x00 ) {
            i._format = GL_RGB;
            i._internalFormat = GL_RGBA8;
            i._type = GL_UNSIGNED_INT_8_8_8_8;
        }
        else if ( ddsh.ddspf.dwRBitMask == 0xff0000 && ddsh.ddspf.dwGBitMask == 0xff00 && ddsh.ddspf.dwBBitMask == 0xff && ddsh.ddspf.dwABitMask == 0x00 ) {
            i._format = GL_BGR;
            i._internalFormat = GL_RGBA8;
            i._type = GL_UNSIGNED_INT_8_8_8_8;
        }
        else {
            // probably a poorly labeled file with BGRX semantics
            i._format = GL_BGR;
            i._internalFormat = GL_RGBA8;
            i._type = GL_UNSIGNED_INT_8_8_8_8;
        }
        bytesPerElement = 4;
    }
    else if (ddsh.ddspf.dwFlags == DDSF_RGB  && ddsh.ddspf.dwRGBBitCount == 24)
    {
        i._format = GL_BGR;
        i._internalFormat = GL_RGB8;
        i._type = GL_UNSIGNED_BYTE;
        bytesPerElement = 3;
    }
	else if (ddsh.ddspf.dwRGBBitCount == 8)
	{
		i._format = GL_RED;//GL_LUMINANCE; 
        i._internalFormat = GL_RED;//GL_LUMINANCE; 
        i._type = GL_UNSIGNED_BYTE;
		bytesPerElement = 1;
	}
    else 
    {
        fclose(fp);
        return false;
    }

    i._elementSize = bytesPerElement;

    for (int face = 0; face < i._layers; face++) {
        int w = i._width, h = i._height, d = (i._depth) ? i._depth : 1;
        for (int level = 0; level < i._levelCount; level++) {
            int bw = (btcCompressed) ? (w+3)/4 : w;
            int bh = (btcCompressed) ? (h+3)/4 : h;
            int size = bw*bh*d*bytesPerElement;

            GLubyte *data = new GLubyte[size];

            fread( data, size, 1, fp);

            i._data.push_back(data);

            if ( Image::upperLeftOrigin && !i._cubeMap)
                i.flipSurface( data, w, h, d);

            //reduce mip sizes
            w = ( w > 1) ? w >> 1 : 1;
            h = ( h > 1) ? h >> 1 : 1;
            d = ( d > 1) ? d >> 1 : 1;
        }
    }

    fclose(fp);
    return true;
}

//
// flip a DXT1 color block
////////////////////////////////////////////////////////////
void Image::flip_blocks_dxtc1(GLubyte *ptr, unsigned int numBlocks)
{
    DXTColBlock *curblock = (DXTColBlock*)ptr;
    GLubyte temp;

    for (unsigned int i = 0; i < numBlocks; i++) {
        temp = curblock->row[0];
        curblock->row[0] = curblock->row[3];
        curblock->row[3] = temp;
        temp = curblock->row[1];
        curblock->row[1] = curblock->row[2];
        curblock->row[2] = temp;

        curblock++;
    }
}

//
// flip a DXT3 color block
////////////////////////////////////////////////////////////
void Image::flip_blocks_dxtc3(GLubyte *ptr, unsigned int numBlocks)
{
    DXTColBlock *curblock = (DXTColBlock*)ptr;
    DXT3AlphaBlock *alphablock;
    GLushort tempS;
    GLubyte tempB;

    for (unsigned int i = 0; i < numBlocks; i++)
    {
        alphablock = (DXT3AlphaBlock*)curblock;

        tempS = alphablock->row[0];
        alphablock->row[0] = alphablock->row[3];
        alphablock->row[3] = tempS;
        tempS = alphablock->row[1];
        alphablock->row[1] = alphablock->row[2];
        alphablock->row[2] = tempS;

        curblock++;

        tempB = curblock->row[0];
        curblock->row[0] = curblock->row[3];
        curblock->row[3] = tempB;
        tempB = curblock->row[1];
        curblock->row[1] = curblock->row[2];
        curblock->row[2] = tempB;

        curblock++;
    }
}

//
// flip a DXT5 alpha block
////////////////////////////////////////////////////////////
void flip_dxt5_alpha(DXT5AlphaBlock *block)
{
    GLubyte gBits[4][4];
    
    const unsigned long mask = 0x00000007;          // bits = 00 00 01 11
    unsigned long bits = 0;
    memcpy(&bits, &block->row[0], sizeof(unsigned char) * 3);

    gBits[0][0] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[0][1] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[0][2] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[0][3] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[1][0] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[1][1] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[1][2] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[1][3] = (GLubyte)(bits & mask);

    bits = 0;
    memcpy(&bits, &block->row[3], sizeof(GLubyte) * 3);

    gBits[2][0] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[2][1] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[2][2] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[2][3] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[3][0] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[3][1] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[3][2] = (GLubyte)(bits & mask);
    bits >>= 3;
    gBits[3][3] = (GLubyte)(bits & mask);

    // clear existing alpha bits
    memset(block->row, 0, sizeof(GLubyte) * 6);

    unsigned long *pBits = ((unsigned long*) &(block->row[0]));

    *pBits = *pBits | (gBits[3][0] << 0);
    *pBits = *pBits | (gBits[3][1] << 3);
    *pBits = *pBits | (gBits[3][2] << 6);
    *pBits = *pBits | (gBits[3][3] << 9);

    *pBits = *pBits | (gBits[2][0] << 12);
    *pBits = *pBits | (gBits[2][1] << 15);
    *pBits = *pBits | (gBits[2][2] << 18);
    *pBits = *pBits | (gBits[2][3] << 21);

    pBits = ((unsigned long*) &(block->row[3]));

    *pBits = *pBits | (gBits[1][0] << 0);
    *pBits = *pBits | (gBits[1][1] << 3);
    *pBits = *pBits | (gBits[1][2] << 6);
    *pBits = *pBits | (gBits[1][3] << 9);

    *pBits = *pBits | (gBits[0][0] << 12);
    *pBits = *pBits | (gBits[0][1] << 15);
    *pBits = *pBits | (gBits[0][2] << 18);
    *pBits = *pBits | (gBits[0][3] << 21);
}

//
// flip a DXT5 color block
////////////////////////////////////////////////////////////
void Image::flip_blocks_dxtc5(GLubyte *ptr, unsigned int numBlocks)
{
    DXTColBlock *curblock = (DXTColBlock*)ptr;
    DXT5AlphaBlock *alphablock;
    GLubyte temp;
    
    for (unsigned int i = 0; i < numBlocks; i++)
    {
        alphablock = (DXT5AlphaBlock*)curblock;
        
        flip_dxt5_alpha(alphablock);

        curblock++;

        temp = curblock->row[0];
        curblock->row[0] = curblock->row[3];
        curblock->row[3] = temp;
        temp = curblock->row[1];
        curblock->row[1] = curblock->row[2];
        curblock->row[2] = temp;

        curblock++;
    }
}

//
// flip a BC4 block
////////////////////////////////////////////////////////////
void Image::flip_blocks_bc4(GLubyte *ptr, unsigned int numBlocks)
{
    DXT5AlphaBlock *alphablock = (DXT5AlphaBlock*)ptr;
    
    for (unsigned int i = 0; i < numBlocks; i++)
    {
        flip_dxt5_alpha(alphablock);

        alphablock++;
    }
}

//
// flip a BC5 block
////////////////////////////////////////////////////////////
void Image::flip_blocks_bc5(GLubyte *ptr, unsigned int numBlocks)
{
    DXT5AlphaBlock *alphablock = (DXT5AlphaBlock*)ptr;
    
    for (unsigned int i = 0; i < numBlocks; i++)
    {
        flip_dxt5_alpha(alphablock);

        alphablock++;
        flip_dxt5_alpha(alphablock);

        alphablock++;
    }
}


};