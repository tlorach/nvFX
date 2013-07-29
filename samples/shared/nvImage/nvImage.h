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
// nvImage.h - Image support class
//
// The nvImage class implements an interface for a multipurpose image
// object. This class is useful for loading and formating images
// for use as textures. The class supports dds, png, and hdr formats.
//
// Author: Evan Hart
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#ifndef NV_IMAGE_H
#define NV_IMAGE_H

#if 0//def WIN32
#ifdef NVIMAGE_EXPORTS
#define NVSDKENTRY __declspec(dllexport)
#else
#define NVSDKENTRY __declspec(dllimport)
#endif
#if _MSC_VER >= 1400  
# pragma warning(disable:4996) 
#endif 
#else
#define NVSDKENTRY
#endif

#include <vector>
#include <assert.h>

#include <GL/glew.h>

namespace nv {

    class Image {
    public:

        // By default, the image library places the origin of images at the
        // lower-left corner, to make it map exactly to OpenGL screen coords.
        // This flips the image, and it might make it incompatible with
        // the texture coordinate conventions of an imported model.
        NVSDKENTRY static void UpperLeftOrigin( bool ul);

        NVSDKENTRY Image();
        NVSDKENTRY virtual ~Image();

        // return the width of the image
        NVSDKENTRY int getWidth() const { return _width; }

        //return the height of the image
        NVSDKENTRY int getHeight() const { return _height; }

        //return the dpeth of the image (0 for images with no depth)
        NVSDKENTRY int getDepth() const { return _depth; }

        //return the number of mipmap levels available for the image
        NVSDKENTRY int getMipLevels() const { return _levelCount; }

        //return the number of cubemap faces available for the image (0 for non-cubemap images)
        NVSDKENTRY int getFaces() const { return _cubeMap ? _layers : 0; }

        //return the number of layers for use in texture arrays
        NVSDKENTRY int getLayers() const { return _layers; }

        //return the format of the image data (GL_RGB, GL_BGR, etc)
        NVSDKENTRY GLenum getFormat() const { return _format; }

        //return the suggested internal format for the data
        NVSDKENTRY GLenum getInternalFormat() const { return _internalFormat; }

        //return the type of the image data
        NVSDKENTRY GLenum getType() const { return _type; }

        //return the Size in bytes of a level of the image 
        NVSDKENTRY int getImageSize(int level = 0) const;

        //return whether the data is a crompressed format
        NVSDKENTRY bool isCompressed() const {
            switch(_format) {
                case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
                case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
                case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
                case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
				case GL_COMPRESSED_LUMINANCE_LATC1_EXT:
				case GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT:
				case GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT:
				case GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT:
                case GL_COMPRESSED_RG_RGTC2:
                case GL_COMPRESSED_RED_RGTC1:
                case GL_COMPRESSED_SIGNED_RG_RGTC2:
                case GL_COMPRESSED_SIGNED_RED_RGTC1:
                    return true;
            }
            return false;
        }

        //return whether the image represents a cubemap
        NVSDKENTRY bool isCubeMap() const { return _cubeMap; }

        //return whether the image represents a texture array
        NVSDKENTRY bool isArray() const { return _layers > 1; }

        //return whether the image represents a volume
        NVSDKENTRY bool isVolume() const { return _depth > 0; }

        //get a pointer to level data
        NVSDKENTRY const void* getLevel( int level, GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X) const;
        NVSDKENTRY void* getLevel( int level, GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        NVSDKENTRY const void* getLayerLevel( int level, int slice) const;
        NVSDKENTRY void* getLayerLevel( int level, int slice);

        //initialize an image from a file
        NVSDKENTRY bool loadImageFromFile( const char* file);

        //convert a suitable image from a cubemap cross to a cubemap (returns false for unsuitable images)
        NVSDKENTRY bool convertCrossToCubemap();

        //load an image from memory, for the purposes of saving
        NVSDKENTRY bool setImage( int width, int height, GLenum format, GLenum type, const void* data);

        //save an image to a file
        NVSDKENTRY bool saveImageToFile( const char* file);

    protected:
        int _width;
        int _height;
        int _depth;
        int _levelCount;
        int _layers;
        GLenum _format;
        GLenum _internalFormat;
        GLenum _type;
        int _elementSize;
        bool _cubeMap;

        //pointers to the levels
        std::vector<GLubyte*> _data;

        NVSDKENTRY void freeData();
        NVSDKENTRY void flipSurface(GLubyte *surf, int width, int height, int depth);


        //
        // Static elements used to dispatch to proper sub-readers
        //
        //////////////////////////////////////////////////////////////
        struct FormatInfo {
            const char* extension;
            bool (*reader)( const char* file, Image& i);
            bool (*writer)( const char* file, Image& i);
        };

        static FormatInfo formatTable[]; 
        static bool upperLeftOrigin;

        NVSDKENTRY static bool readPng( const char *file, Image& i)
        {
            return false;
        }
        NVSDKENTRY static bool readDDS( const char *file, Image& i);
        NVSDKENTRY static bool readHdr( const char *file, Image& i);

        NVSDKENTRY static bool writePng( const char *file, Image& i)
        {
            return false;
        }
        //NVSDKENTRY static bool writeDDS( const char *file, Image& i);
        //NVSDKENTRY static bool writeHdr( const char *file, Image& i);

        NVSDKENTRY static void flip_blocks_dxtc1(GLubyte *ptr, unsigned int numBlocks);
        NVSDKENTRY static void flip_blocks_dxtc3(GLubyte *ptr, unsigned int numBlocks);
        NVSDKENTRY static void flip_blocks_dxtc5(GLubyte *ptr, unsigned int numBlocks);
        NVSDKENTRY static void flip_blocks_bc4(GLubyte *ptr, unsigned int numBlocks);
        NVSDKENTRY static void flip_blocks_bc5(GLubyte *ptr, unsigned int numBlocks);

        friend bool TranslateDX10Format( const void *ptr, Image &i, int &bytesPerElement, bool &btcCompressed);
    };
};

#endif //NV_IMAGE_H