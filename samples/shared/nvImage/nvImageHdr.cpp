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
// nvImageHdr.cpp - Image support class
//
// The nvImage class implements an interface for a multipurpose image
// object. This class is useful for loading and formating images
// for use as textures. The class supports dds, png, and hdr formats.
//
// This file implements the HDR specific functionality.
//
// Author: Evan Hart
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#include "rgbe.h"

#include "nvImage.h"

using std::vector;

namespace nv {

//
//  readHdr
//
//    Image loader function for hdr files. 
////////////////////////////////////////////////////////////
bool Image::readHdr( const char *file, Image& i) {
    int width, height;
    FILE *fp = fopen(file, "rb");
    if (!fp) {
        return false;
    }

    rgbe_header_info header;

    if (RGBE_ReadHeader( fp, &width, &height, &header)) {
        fclose(fp);
        return false;
    }

    GLubyte *data = (GLubyte*)new float[width*height*3];
    
    if (!data) {
        fclose(fp);
        return false;
    }

    if (RGBE_ReadPixels_RLE( fp, (float*)data, width, height)) {
        delete []data;
        fclose(fp);
        return false;
    }

    //set all the parameters
    i._width = width;
    i._height = height;
    i._depth = 0;
    i._levelCount = 1;
    i._type = GL_FLOAT;
    i._format = GL_RGB;
    i._internalFormat = GL_RGB32F_ARB;
    i._layers = 1;
    i._elementSize = 12;
    i._cubeMap = false;
    i._data.push_back( data);

    //hdr images come in upside down
    if (Image::upperLeftOrigin)
        i.flipSurface( data, i._width, i._height, i._depth);

    fclose(fp);

    return true;
}

};
