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
// nvImage.cpp - Image support class
//
// The nvImage class implements an interface for a multipurpose image
// object. This class is useful for loading and formating images
// for use as textures. The class supports dds, png, and hdr formats.
//
// This file implements the format independent interface.
//
// Author: Evan Hart
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <algorithm>

#include "nvImage.h"

using std::vector;
using std::max;

#ifdef WIN32
#define strcasecmp _stricmp
#endif

namespace nv {

Image::FormatInfo Image::formatTable[] = {
    { "png", Image::readPng, Image::writePng},
    { "dds", Image::readDDS, 0},
    { "hdr", Image::readHdr, 0}
};

bool Image::upperLeftOrigin = true;

//
//
////////////////////////////////////////////////////////////
void Image::UpperLeftOrigin( bool ul) {
    upperLeftOrigin = ul;
}

//
//
////////////////////////////////////////////////////////////
Image::Image() : _width(0), _height(0), _depth(0), _levelCount(0), _layers(0), _format(GL_RGBA),
    _internalFormat(GL_RGBA8), _type(GL_UNSIGNED_BYTE), _elementSize(0), _cubeMap(false) {
}

//
//
////////////////////////////////////////////////////////////
Image::~Image() {
    freeData();
}

//
//
////////////////////////////////////////////////////////////
void Image::freeData() {
    for (vector<GLubyte*>::iterator it = _data.begin(); it != _data.end(); it++) {
        delete []*it;
    }
    _data.clear();
}

//
//
////////////////////////////////////////////////////////////
int Image::getImageSize( int level) const {
    bool compressed = isCompressed();
    int w = _width >> level;
    int h = _height >> level;
    int d = _depth >> level;
    w = (w) ? w : 1;
    h = (h) ? h : 1;
    d = (d) ? d : 1;
    int bw = (compressed) ? ( w + 3 ) / 4 : w;
    int bh = (compressed) ? ( h + 3 ) / 4 : h;
    int elementSize = _elementSize;

    return bw*bh*d*elementSize;
}


//
//
////////////////////////////////////////////////////////////
const void* Image::getLevel( int level, GLenum face) const {
    assert( level < _levelCount);
    assert( face >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    assert( face == GL_TEXTURE_CUBE_MAP_POSITIVE_X || _cubeMap);

    face = face - GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    assert( (face*_levelCount + level) < (int)_data.size());

    // make sure we don't hand back a garbage pointer
    if (level >=_levelCount || (int)face >= _layers)
        return NULL;

    return _data[ face*_levelCount + level];
}

//
//
////////////////////////////////////////////////////////////
void* Image::getLevel( int level, GLenum face) {
    assert( level < _levelCount);
    assert( face >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    assert( face == GL_TEXTURE_CUBE_MAP_POSITIVE_X || _cubeMap);

    face = face - GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    assert( (face*_levelCount + level) < (int)_data.size());

    // make sure we don't hand back a garbage pointer
    if (level >=_levelCount || (int)face >= _layers)
        return NULL;

    return _data[ face*_levelCount + level];
}

//
//
////////////////////////////////////////////////////////////
const void* Image::getLayerLevel( int level, int layer) const {
    assert( level < _levelCount);
    assert( layer < _layers);


    assert( (layer*_levelCount + level) < (int)_data.size());

    // make sure we don't hand back a garbage pointer
    if (level >=_levelCount || layer >= _layers)
        return NULL;

    return _data[ layer*_levelCount + level];
}

//
//
////////////////////////////////////////////////////////////
void* Image::getLayerLevel( int level, int layer) {
    assert( level < _levelCount);
    assert( layer < _layers);


    assert( (layer*_levelCount + level) < (int)_data.size());

    // make sure we don't hand back a garbage pointer
    if (level >=_levelCount || layer >= _layers)
        return NULL;

    return _data[ layer*_levelCount + level];
}

//
//
////////////////////////////////////////////////////////////
bool Image::loadImageFromFile( const char* file) {
    const char* extension;
    extension = strrchr( file, '.');

    if (extension)
        extension++; //start looking after the .
    else
        return false;

    int formatCount = sizeof(Image::formatTable) / sizeof(Image::FormatInfo);

    //try to match by format first
    for ( int ii = 0; ii < formatCount; ii++) {
        if ( ! strcasecmp( formatTable[ii].extension, extension)) {
            //extension matches, load it
            return formatTable[ii].reader( file, *this);
        }
    }


    return false;
}

//
//
////////////////////////////////////////////////////////////
void Image::flipSurface(GLubyte *surf, int width, int height, int depth)
{
    unsigned int lineSize;

    depth = (depth) ? depth : 1;

    if (!isCompressed()) {
        lineSize = _elementSize * width;
        unsigned int sliceSize = lineSize * height;

        GLubyte *tempBuf = new GLubyte[lineSize];

        for ( int ii = 0; ii < depth; ii++) {
            GLubyte *top = surf + ii*sliceSize;
            GLubyte *bottom = top + (sliceSize - lineSize);
    
            for ( int jj = 0; jj < (height >> 1); jj++) {
                memcpy( tempBuf, top, lineSize);
                memcpy( top, bottom, lineSize);
                memcpy( bottom, tempBuf, lineSize);

                top += lineSize;
                bottom -= lineSize;
            }
        }

        delete []tempBuf;
    }
    else
    {
        void (*flipblocks)(GLubyte*, unsigned int);
        width = (width + 3) / 4;
        height = (height + 3) / 4;
        unsigned int blockSize = 0;

        switch (_format)
        {
            case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: 
                blockSize = 8;
                flipblocks = &Image::flip_blocks_dxtc1; 
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT: 
                blockSize = 16;
                flipblocks = &Image::flip_blocks_dxtc3; 
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: 
                blockSize = 16;
                flipblocks = &Image::flip_blocks_dxtc5; 
                break;
            case GL_COMPRESSED_LUMINANCE_LATC1_EXT:
            case GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT:
            case GL_COMPRESSED_RED_RGTC1:
            case GL_COMPRESSED_SIGNED_RED_RGTC1:
                blockSize = 8;
                flipblocks = &Image::flip_blocks_bc4;
                break;
            case GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT:
            case GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT:
            case GL_COMPRESSED_RG_RGTC2:
            case GL_COMPRESSED_SIGNED_RG_RGTC2:
                blockSize = 16;
                flipblocks = &Image::flip_blocks_bc5;
                break;
            default:
                return;
        }

        lineSize = width * blockSize;
        GLubyte *tempBuf = new GLubyte[lineSize];

        GLubyte *top = surf;
        GLubyte *bottom = surf + (height-1) * lineSize;

        for (unsigned int j = 0; j < max( (unsigned int)height >> 1, (unsigned int)1); j++)
        {
            if (top == bottom)
            {
                flipblocks(top, width);
                break;
            }

            flipblocks(top, width);
            flipblocks(bottom, width);

            memcpy( tempBuf, top, lineSize);
            memcpy( top, bottom, lineSize);
            memcpy( bottom, tempBuf, lineSize);

            top += lineSize;
            bottom -= lineSize;
        }
        delete []tempBuf;
    }
}    

//
//
////////////////////////////////////////////////////////////
bool Image::convertCrossToCubemap() {
    //can't already be a cubemap
    if (isCubeMap())
        return false;

    //mipmaps are not supported
    if (_levelCount != 1)
        return false;

    //compressed textures are not supported
    if (isCompressed())
        return false;

    //this function only supports vertical cross format for now (3 wide by 4 high)
    if (  (_width / 3 != _height / 4) || (_width % 3 != 0) || (_height % 4 != 0) || (_depth != 0))
        return false;

    //get the source data
    GLubyte *data = _data[0];

    int fWidth = _width / 3;
    int fHeight = _height / 4;

    //remove the old pointer from the vector
    _data.pop_back();
    
    GLubyte *face = new GLubyte[ fWidth * fHeight * _elementSize];
    GLubyte *ptr;

    //extract the faces

    // positive X
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + 2 * fWidth) * _elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // negative X
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[(_height - (fHeight + j + 1))*_width*_elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // positive Y
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((4 * fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // negative Y
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((2*fHeight - j - 1)*_width + fWidth)*_elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // positive Z
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        memcpy( ptr, &data[((_height - (fHeight + j + 1))*_width + fWidth) * _elementSize], fWidth*_elementSize);
        ptr += fWidth*_elementSize;
    }
    _data.push_back(face);

    // negative Z
    face = new GLubyte[ fWidth * fHeight * _elementSize];
    ptr = face;
    for (int j=0; j<fHeight; j++) {
        for (int i=0; i<fWidth; i++) {
            memcpy( ptr, &data[(j*_width + 2 * fWidth - (i + 1))*_elementSize], _elementSize);
            ptr += _elementSize;
        }
    }
    _data.push_back(face);

    //set the new # of faces, width and height
    _layers = 6;
    _width = fWidth;
    _height = fHeight;
    _cubeMap = true;

    //delete the old pointer
    delete []data;

    return true;
}

//
//
////////////////////////////////////////////////////////////
bool Image::setImage( int width, int height, GLenum format, GLenum type, const void* data){
    //check parameters before destroying the old image
    int elementSize;
    GLenum internalFormat;

    switch (format) {
        case GL_ALPHA:
            switch (type) {
                case GL_UNSIGNED_BYTE:
                    internalFormat = GL_ALPHA8;
                    elementSize = 1;
                    break;
                case GL_UNSIGNED_SHORT:
                    internalFormat = GL_ALPHA16;
                    elementSize = 2;
                    break;
                case GL_FLOAT:
                    internalFormat = GL_ALPHA32F_ARB;
                    elementSize = 4;
                    break;
                case GL_HALF_FLOAT_ARB:
                    internalFormat = GL_ALPHA16F_ARB;
                    elementSize = 2;
                    break;
                default:
                    return false; //format/type combo not supported
            }
            break;
        case GL_LUMINANCE:
            switch (type) {
                case GL_UNSIGNED_BYTE:
                    internalFormat = GL_R8;// GL_LUMINANCE8;
                    elementSize = 1;
                    break;
                case GL_UNSIGNED_SHORT:
                    internalFormat = GL_LUMINANCE16;
                    elementSize = 2;
                    break;
                case GL_FLOAT:
                    internalFormat = GL_LUMINANCE32F_ARB;
                    elementSize = 4;
                    break;
                case GL_HALF_FLOAT_ARB:
                    internalFormat = GL_LUMINANCE16F_ARB;
                    elementSize = 2;
                    break;
                default:
                    return false; //format/type combo not supported
            }
            break;
        case GL_LUMINANCE_ALPHA:
            switch (type) {
                case GL_UNSIGNED_BYTE:
                    internalFormat = GL_RG8;// GL_LUMINANCE8_ALPHA8;
                    elementSize = 2;
                    break;
                case GL_UNSIGNED_SHORT:
                    internalFormat = GL_LUMINANCE16_ALPHA16;
                    elementSize = 4;
                    break;
                case GL_FLOAT:
                    internalFormat = GL_LUMINANCE_ALPHA32F_ARB;
                    elementSize = 8;
                    break;
                case GL_HALF_FLOAT_ARB:
                    internalFormat = GL_LUMINANCE_ALPHA16F_ARB;
                    elementSize = 4;
                    break;
                default:
                    return false; //format/type combo not supported
            }
            break;
        case GL_RGB:
            switch (type) {
                case GL_UNSIGNED_BYTE:
                    internalFormat = GL_RGB8;
                    elementSize = 3;
                    break;
                case GL_UNSIGNED_SHORT:
                    internalFormat = GL_RGB16;
                    elementSize = 6;
                    break;
                case GL_FLOAT:
                    internalFormat = GL_RGB32F_ARB;
                    elementSize = 12;
                    break;
                case GL_HALF_FLOAT_ARB:
                    internalFormat = GL_RGB16F_ARB;
                    elementSize = 6;
                    break;
                default:
                    return false; //format/type combo not supported
            }
            break;
        case GL_RGBA:
            switch (type) {
                case GL_UNSIGNED_BYTE:
                    internalFormat = GL_RGBA8;
                    elementSize = 4;
                    break;
                case GL_UNSIGNED_SHORT:
                    internalFormat = GL_RGBA16;
                    elementSize = 8;
                    break;
                case GL_FLOAT:
                    internalFormat = GL_RGBA32F_ARB;
                    elementSize = 16;
                    break;
                case GL_HALF_FLOAT_ARB:
                    internalFormat = GL_RGBA16F_ARB;
                    elementSize = 8;
                    break;
                default:
                    return false; //format/type combo not supported
            }
            break;
        default:
            //bad format
            return false;
            break;
    }


    //clear old data
    freeData();

    GLubyte *newImage = new GLubyte[width*height*elementSize];
    memcpy( newImage, data, width*height*elementSize);

    _data.push_back(newImage);

    _width = width;
    _height = height;
    _elementSize = elementSize;
    _internalFormat = internalFormat;
    _levelCount = 1;
    _layers = 1;
    _depth = 0;
    _format = format;
    _type = type;
    _cubeMap = false;

    return true;
}

//
//
////////////////////////////////////////////////////////////
bool Image::saveImageToFile( const char* file) {
    const char* extension;
    extension = strrchr( file, '.');

    if (extension)
        extension++; //start looking after the .
    else
        return false;

    int formatCount = sizeof(Image::formatTable) / sizeof(Image::FormatInfo);

    //try to match by format first
    for ( int ii = 0; ii < formatCount; ii++) {
        if ( ! strcasecmp( formatTable[ii].extension, extension)) {
            //extension matches, load it
            if (formatTable[ii].writer) {
                return formatTable[ii].writer( file, *this);
            }
        }
    }


    return false;
}

};