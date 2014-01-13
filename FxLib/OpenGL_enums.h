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
// Declaration of OpenGL enums for compatibility purpose.
// the idea is to creation D3D states from OpenGL states when used from the API (or from nvFX code)
#define GL_DEPTH_TEST 0xB71
#define GL_DEPTH_CLAMP 0x864F
#define GL_DEPTH_BOUNDS_TEST_EXT 0x8890
#define GL_STENCIL_TEST 0x0B90
#define GL_DEPTH_FUNC 0x0B74
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DEPTH_BOUNDS_EXT 0x8891
//#define GL_STENCIL_FRONT_REF 
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_VALUE_MASK 0x0B93
//#define GL_STENCIL_FRONT_FUNC 
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
//#define GL_STENCIL_FRONT_FAIL 
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_STENCIL_BACK_FAIL 0x8801
//#define GL_STENCIL_FRONT_PASS_DEPTH_FAIL 
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
//#define GL_STENCIL_FRONT_PASS_DEPTH_PASS 
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5
#define GL_STENCIL_WRITEMASK 0x0B98
//#define GL_STENCIL_FRONT_VALUE_MASK 
//#define GL_STENCIL_FRONT_WRITEMASK 
//#define GL_PER_DRAW_BUFFER_BLEND_FUNCTION_NV 
//#define GL_PER_DRAW_BUFFER_BLEND_EQUATION_NV 
//#define GL_PER_DRAW_BUFFER_BLEND_ENABLE_NV 
//#define GL_PER_DRAW_BUFFER_COLOR_WRITEMASK_NV 
#define GL_FRAMEBUFFER_SRGB 0x8DB9
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_ALPHA_TEST 0x0BC0
#define GL_MULTISAMPLE 0x809D
#define GL_SAMPLE_MASK 0x8E51
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_DITHER 0x0BD0
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_BLEND 0x0BE2
#define GL_ALPHA_TEST_REF 0x0BC2
#define GL_ALPHA_TEST_FUNC 0x0BC1
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC_RGB  0x80C9
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_DST_ALPHA 0x80CA
//#define GL_BLEND_EQUATION_RGBA_NV 
#define GL_BLEND_EQUATION 0x8009
#define GL_BLEND_EQUATION_RGB GL_BLEND_EQUATION
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_SAMPLE_MASK_VALUE 0x8E52
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_BLEND_COLOR 0x8005
#define GL_POINT_SMOOTH 0x0B10
#define GL_POINT_SPRITE 0x8861
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_STIPPLE 0x0B24
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_LINE 0x2A02
#define GL_POLYGON_STIPPLE 0x0B42
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_CULL_FACE 0x0B44
#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
#define GL_POLYGON_MODE 0x0B40
//#define GL_COMBINED_LINE_STIPPLE_NV 
#define GL_LINE_STIPPLE_PATTERN 0x0B25
#define GL_LINE_STIPPLE_REPEAT 0x0B26
#define GL_POINT_SIZE 0x0B11
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_LINE_WIDTH 0x0B21
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_POLYGON_OFFSET_UNITS 0x2A00

#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207

#define GL_KEEP 0x1E00
#define GL_ZERO 0
#define GL_ONE 1
#define GL_INVERT 0x150A
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03

#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_SRC1_COLOR 0x88F9
#define GL_ONE_MINUS_SRC1_COLOR 0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA 0x88FB
#define GL_SOURCE1_ALPHA_ARB 0x8589
#define GL_SOURCE1_ALPHA 0x8589
#define GL_SRC1_ALPHA GL_SOURCE1_ALPHA
#define GL_SUBTRACT 0x84E7
#define GL_ADD 0x0104
#define GL_FUNC_SUBTRACT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_FUNC_ADD 0x8006
#define GL_MIN 0x8007
#define GL_MAX 0x8008
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_LEFT 0x0406
#define GL_RIGHT 0x0407
#define GL_FRONT_AND_BACK 0x0408

#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_CLAMP 0x2900
#define GL_REPEAT 0x2901
#define GL_MIRRORED_REPEAT 0x8370
#define GL_CLAMP_TO_BORDER 0x812D

// OpenGL NV_path_rendering
//
#define GL_CLOSE_PATH_NV                                   0x00
#define GL_MOVE_TO_NV                                      0x02
#define GL_RELATIVE_MOVE_TO_NV                             0x03
#define GL_LINE_TO_NV                                      0x04
#define GL_RELATIVE_LINE_TO_NV                             0x05
#define GL_HORIZONTAL_LINE_TO_NV                           0x06
#define GL_RELATIVE_HORIZONTAL_LINE_TO_NV                  0x07
#define GL_VERTICAL_LINE_TO_NV                             0x08
#define GL_RELATIVE_VERTICAL_LINE_TO_NV                    0x09
#define GL_QUADRATIC_CURVE_TO_NV                           0x0A
#define GL_RELATIVE_QUADRATIC_CURVE_TO_NV                  0x0B
#define GL_CUBIC_CURVE_TO_NV                               0x0C
#define GL_RELATIVE_CUBIC_CURVE_TO_NV                      0x0D
#define GL_SMOOTH_QUADRATIC_CURVE_TO_NV                    0x0E
#define GL_RELATIVE_SMOOTH_QUADRATIC_CURVE_TO_NV           0x0F
#define GL_SMOOTH_CUBIC_CURVE_TO_NV                        0x10
#define GL_RELATIVE_SMOOTH_CUBIC_CURVE_TO_NV               0x11
#define GL_SMALL_CCW_ARC_TO_NV                             0x12
#define GL_RELATIVE_SMALL_CCW_ARC_TO_NV                    0x13
#define GL_SMALL_CW_ARC_TO_NV                              0x14
#define GL_RELATIVE_SMALL_CW_ARC_TO_NV                     0x15
#define GL_LARGE_CCW_ARC_TO_NV                             0x16
#define GL_RELATIVE_LARGE_CCW_ARC_TO_NV                    0x17
#define GL_LARGE_CW_ARC_TO_NV                              0x18
#define GL_RELATIVE_LARGE_CW_ARC_TO_NV                     0x19
#define GL_RESTART_PATH_NV                                 0xF0
#define GL_DUP_FIRST_CUBIC_CURVE_TO_NV                     0xF2
#define GL_DUP_LAST_CUBIC_CURVE_TO_NV                      0xF4
#define GL_RECT_NV                                         0xF6
#define GL_CIRCULAR_CCW_ARC_TO_NV                          0xF8
#define GL_CIRCULAR_CW_ARC_TO_NV                           0xFA
#define GL_CIRCULAR_TANGENT_ARC_TO_NV                      0xFC
#define GL_ARC_TO_NV                                       0xFE
#define GL_RELATIVE_ARC_TO_NV                              0xFF

#define GL_PATH_FORMAT_SVG_NV                              0x9070
#define GL_PATH_FORMAT_PS_NV                               0x9071

#define GL_STANDARD_FONT_NAME_NV                           0x9072
#define GL_SYSTEM_FONT_NAME_NV                             0x9073
#define GL_FILE_NAME_NV                                    0x9074

#define GL_SKIP_MISSING_GLYPH_NV                           0x90A9
#define GL_USE_MISSING_GLYPH_NV                            0x90AA

#define GL_PATH_STROKE_WIDTH_NV                            0x9075
#define GL_PATH_INITIAL_END_CAP_NV                         0x9077
#define GL_PATH_TERMINAL_END_CAP_NV                        0x9078
#define GL_PATH_JOIN_STYLE_NV                              0x9079
#define GL_PATH_MITER_LIMIT_NV                             0x907A
#define GL_PATH_INITIAL_DASH_CAP_NV                        0x907C
#define GL_PATH_TERMINAL_DASH_CAP_NV                       0x907D
#define GL_PATH_DASH_OFFSET_NV                             0x907E
#define GL_PATH_CLIENT_LENGTH_NV                           0x907F
#define GL_PATH_DASH_OFFSET_RESET_NV                       0x90B4

#define GL_PATH_FILL_MODE_NV                               0x9080
#define GL_PATH_FILL_MASK_NV                               0x9081
#define GL_PATH_FILL_COVER_MODE_NV                         0x9082
#define GL_PATH_STROKE_COVER_MODE_NV                       0x9083
#define GL_PATH_STROKE_MASK_NV                             0x9084

#define GL_PATH_END_CAPS_NV                                0x9076
#define GL_PATH_DASH_CAPS_NV                               0x907B

#define GL_COUNT_UP_NV                                     0x9088
#define GL_COUNT_DOWN_NV                                   0x9089

#define GL_PRIMARY_COLOR                                   0x8577  // from OpenGL 1.3
#define GL_PRIMARY_COLOR_NV                                0x852C  // from NV_register_combiners
#define GL_SECONDARY_COLOR_NV                              0x852D  // from NV_register_combiners

#define GL_EYE_LINEAR
#define GL_OBJECT_LINEAR
#define GL_PATH_OBJECT_BOUNDING_BOX_NV                     0x908A

#define GL_CONVEX_HULL_NV                                  0x908B
#define GL_BOUNDING_BOX_NV                                 0x908D

#define GL_TRANSLATE_X_NV                                  0x908E
#define GL_TRANSLATE_Y_NV                                  0x908F
#define GL_TRANSLATE_2D_NV                                 0x9090
#define GL_TRANSLATE_3D_NV                                 0x9091
#define GL_AFFINE_2D_NV                                    0x9092
#define GL_AFFINE_3D_NV                                    0x9094
#define GL_TRANSPOSE_AFFINE_2D_NV                          0x9096
#define GL_TRANSPOSE_AFFINE_3D_NV                          0x9098

#define GL_UTF8_NV                                         0x909A
#define GL_UTF16_NV                                        0x909B

#define GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV               0x909C

#define GL_PATH_COMMAND_COUNT_NV                           0x909D
#define GL_PATH_COORD_COUNT_NV                             0x909E
#define GL_PATH_DASH_ARRAY_COUNT_NV                        0x909F

#define GL_PATH_COMPUTED_LENGTH_NV                         0x90A0

#define GL_PATH_FILL_BOUNDING_BOX_NV                       0x90A1
#define GL_PATH_STROKE_BOUNDING_BOX_NV                     0x90A2

#define GL_SQUARE_NV                                       0x90A3
#define GL_ROUND_NV                                        0x90A4
#define GL_TRIANGULAR_NV                                   0x90A5

#define GL_BEVEL_NV                                        0x90A6
#define GL_MITER_REVERT_NV                                 0x90A7
#define GL_MITER_TRUNCATE_NV                               0x90A8

#define GL_MOVE_TO_RESETS_NV                               0x90B5
#define GL_MOVE_TO_CONTINUES_NV                            0x90B6

#define GL_BOLD_BIT_NV                                     0x01
#define GL_ITALIC_BIT_NV                                   0x02

#define GL_PATH_ERROR_POSITION_NV                          0x90AB

#define GL_PATH_FOG_GEN_MODE_NV                            0x90AC

#define GL_PATH_STENCIL_FUNC_NV                            0x90B7
#define GL_PATH_STENCIL_REF_NV                             0x90B8
#define GL_PATH_STENCIL_VALUE_MASK_NV                      0x90B9

#define GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV             0x90BD
#define GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV              0x90BE

#define GL_PATH_COVER_DEPTH_FUNC_NV                        0x90BF

#define GL_GLYPH_WIDTH_BIT_NV                              0x01
#define GL_GLYPH_HEIGHT_BIT_NV                             0x02
#define GL_GLYPH_HORIZONTAL_BEARING_X_BIT_NV               0x04
#define GL_GLYPH_HORIZONTAL_BEARING_Y_BIT_NV               0x08
#define GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV         0x10
#define GL_GLYPH_VERTICAL_BEARING_X_BIT_NV                 0x20
#define GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV                 0x40
#define GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV           0x80
#define GL_GLYPH_HAS_KERNING_BIT_NV                        0x100

#define GL_FONT_X_MIN_BOUNDS_BIT_NV                        0x00010000
#define GL_FONT_Y_MIN_BOUNDS_BIT_NV                        0x00020000
#define GL_FONT_X_MAX_BOUNDS_BIT_NV                        0x00040000
#define GL_FONT_Y_MAX_BOUNDS_BIT_NV                        0x00080000
#define GL_FONT_UNITS_PER_EM_BIT_NV                        0x00100000
#define GL_FONT_ASCENDER_BIT_NV                            0x00200000
#define GL_FONT_DESCENDER_BIT_NV                           0x00400000
#define GL_FONT_HEIGHT_BIT_NV                              0x00800000
#define GL_FONT_MAX_ADVANCE_WIDTH_BIT_NV                   0x01000000
#define GL_FONT_MAX_ADVANCE_HEIGHT_BIT_NV                  0x02000000
#define GL_FONT_UNDERLINE_POSITION_BIT_NV                  0x04000000
#define GL_FONT_UNDERLINE_THICKNESS_BIT_NV                 0x08000000
#define GL_FONT_HAS_KERNING_BIT_NV                         0x10000000

#define GL_ACCUM_ADJACENT_PAIRS_NV                         0x90AD
#define GL_ADJACENT_PAIRS_NV                               0x90AE
#define GL_FIRST_TO_REST_NV                                0x90AF

#define GL_PATH_GEN_MODE_NV                                0x90B0
#define GL_PATH_GEN_COEFF_NV                               0x90B1
#define GL_PATH_GEN_COLOR_FORMAT_NV                        0x90B2
#define GL_PATH_GEN_COMPONENTS_NV                          0x90B3

