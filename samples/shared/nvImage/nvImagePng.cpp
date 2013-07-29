//
// nvImagePng.cpp - Image support class
//
// The nvImage class implements an interface for a multipurpose image
// object. This class is useful for loading and formating images
// for use as textures. The class supports dds, png, and hdr formats.
//
// This file implements the PNG specific functionality.
//
// Author: Evan Hart
// Email: sdkfeedback@nvidia.com
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#include <png.h>

#include "nvImage.h"

using std::vector;

namespace nv {


//
//
////////////////////////////////////////////////////////////
void pngReadFn( png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE* fp = (FILE*)png_get_io_ptr(png_ptr);

    if (!data)
        png_error( png_ptr, "Attempt to read from null file pointer");

    fread( data, length, 1, fp);
}

//
//
////////////////////////////////////////////////////////////
void pngWriteFn( png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE* fp = (FILE*)png_get_io_ptr(png_ptr);

    if (!data)
        png_error( png_ptr, "Attempt to write to null file pointer");

    fwrite( data, length, 1, fp);
}

//
//
////////////////////////////////////////////////////////////
void pngFlushFn( png_structp png_ptr) {
    FILE* fp = (FILE*)png_get_io_ptr(png_ptr);

    fflush(fp);
}

//
//  readPng
//
//    Image loader function for png files. The code is heavily
//  based on the example png loader code distributed with libPNG
////////////////////////////////////////////////////////////
bool Image::readPng( const char *file, Image& i) {
    FILE *fp = fopen( file, "rb");

    if ( !fp)
        return false;

    GLubyte signature[8];
    fread( signature, 8, 1, fp);

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    if (!png_check_sig( signature, 8)) {
        fclose(fp);
        return false;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return false;   /* out of memory */
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return false;   /* out of memory */
    }

    // setjmp() is used for error handling with libPNG, if something goes wrong it is coming back here

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return false;
    }

    // Need to override the standard I/O methods since libPNG may be linked against a different run-time
    png_set_read_fn( png_ptr, fp, pngReadFn);

    png_set_sig_bytes(png_ptr, 8);  // skip the sig bytes

    png_read_info(png_ptr, info_ptr);  // automagically read everything to the image data

    i._width = png_get_image_width(png_ptr, info_ptr);
    i._height = png_get_image_height(png_ptr, info_ptr);
    i._depth = 0; // using the convention of depth == 0 for 2D images
    int colorType = png_get_color_type( png_ptr, info_ptr);
    int bitDepth = png_get_bit_depth( png_ptr, info_ptr);

    if (setjmp(png_jmpbuf(png_ptr))) { 
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return false;
    }

    //Setup the read transforms
    // expand palette images to RGB and low-bit-depth grayscale images to 8 bits
    // convert transparency chunks to full alpha channel
    if (colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
        png_set_gray_1_2_4_to_8(png_ptr);
    }
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }

    // now configure for reading, and allocate the memory
    png_read_update_info(png_ptr, info_ptr);

    int rowBytes = png_get_rowbytes(png_ptr, info_ptr);

    GLubyte *data = new GLubyte[rowBytes * i._height];

    GLubyte **rowPointers = new GLubyte*[i._height]; 

    // set up the row pointers
    for ( int ii = 0;  ii < i._height; ii++) {
        rowPointers[ii] = data + ii*rowBytes;
    }

    // read the image
    png_read_image(png_ptr, rowPointers);


    // reading is complete, configure other parameters

    delete []rowPointers;
    bool use16 = bitDepth > 8;

    i._type = (use16) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE;

    switch ((int)png_get_channels(png_ptr, info_ptr)) {
        case 1:
            i._format = GL_LUMINANCE;
            i._internalFormat = (use16) ? GL_LUMINANCE16 : GL_LUMINANCE8;
            i._elementSize = (use16) ? 2 : 1;
            break;
        case 2:
            i._format = GL_LUMINANCE_ALPHA;
            i._internalFormat = (use16) ? GL_LUMINANCE16_ALPHA16 : GL_LUMINANCE8_ALPHA8;
            i._elementSize = (use16) ? 4 : 2;
            break;
        case 3:
            i._format = GL_RGB;
            i._internalFormat = (use16) ? GL_RGB16 : GL_RGB8;
            i._elementSize = (use16) ? 6 : 3;
            break;
        case 4:
            i._format = GL_RGBA;
            i._internalFormat = (use16) ? GL_RGBA16 : GL_RGBA8;
            i._elementSize = (use16) ? 8 : 4;
            break;
    }

    png_read_end(png_ptr, NULL);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    //finalize parameters
    i._data.push_back( data);
    i._levelCount = 1;
    i._layers = 1;
    i._depth = 0;
    i._cubeMap = false;
    if (Image::upperLeftOrigin)
        i.flipSurface( data, i._width, i._height, i._depth);
    
    return true;

}

//
//  writePng
//
//    Image saver function for png files. The code is heavily
//  based on the example png save code distributed with libPNG
////////////////////////////////////////////////////////////
bool Image::writePng( const char *file, Image& i) {

    //check preconditions, not cubemap, not array, not mipmapped, not a volume
    if (i._levelCount != 1 || i._layers != 1 || i._depth != 0 || i._cubeMap)
        return false;


    FILE *fp = fopen( file, "wb");

    if ( !fp)
        return false;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if (!png_ptr) {
        fclose(fp);
        return false; /* out of memory */
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return false; /* out of memory */
    }


    // setjmp() is used for error handling with libPNG, if something goes wrong it is coming back here

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }

    // Need to override the standard I/O methods since libPNG may be linked against a different run-time
    png_set_write_fn( png_ptr, fp, pngWriteFn, pngFlushFn);

    //TODO, set transforms and row pointers
    int bit_depth = 1;
    int color_type = PNG_COLOR_TYPE_GRAY;
    int row_bytes = 0;

    switch (i._type) {
        case GL_UNSIGNED_BYTE:
            bit_depth = 8;
            break;
        case GL_UNSIGNED_SHORT:
            bit_depth = 16;
            break;
        default:
            //throw error due to unsupported png type here
            break;
    };

    switch (i._format) {
        case GL_LUMINANCE:
            color_type = PNG_COLOR_TYPE_GRAY;
            row_bytes = i._width * (bit_depth >> 3);
            break;
        case GL_LUMINANCE_ALPHA:
            color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
            row_bytes = i._width * 2 * (bit_depth >> 3);
            break;
        case GL_RGB:
            color_type = PNG_COLOR_TYPE_RGB;
            row_bytes = i._width * 3 * (bit_depth >> 3);
            break;
        case GL_RGBA:
            color_type = PNG_COLOR_TYPE_RGB_ALPHA;
            row_bytes = i._width * 4 * (bit_depth >> 3);
            break;
        default:
            // throw error due to unsupported format
            break;
    };

    png_set_IHDR(png_ptr, info_ptr, i._width, i._height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png_ptr, info_ptr);

    GLubyte **row_pointers = new GLubyte*[i._height]; 

    // set up the row pointers
    GLubyte *data = i._data[0];
    for ( int ii = 0;  ii < i._height; ii++) {
        row_pointers[ii] = data + (i._height - 1 - ii)*row_bytes;
    }

    png_write_image( png_ptr, row_pointers);

    delete []row_pointers;

    png_write_end(png_ptr, NULL /*info_ptr*/);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return true;
}

};