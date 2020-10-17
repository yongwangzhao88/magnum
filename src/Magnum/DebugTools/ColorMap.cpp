/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
                2020 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "ColorMap.h"

#include <Corrade/Containers/ArrayView.h>

#include "Magnum/Math/Color.h"

namespace Magnum { namespace DebugTools { namespace ColorMap {

namespace {

/* https://gist.github.com/mikhailov-work/6a308c20e494d9e0ccc29036b28faa7a */
constexpr UnsignedByte Turbo[][3] = {
    { 48,  18,  59}, { 50,  21,  67}, { 51,  24,  74}, { 52,  27,  81},
    { 53,  30,  88}, { 54,  33,  95}, { 55,  36, 102}, { 56,  39, 109},
    { 57,  42, 115}, { 58,  45, 121}, { 59,  47, 128}, { 60,  50, 134},
    { 61,  53, 139}, { 62,  56, 145}, { 63,  59, 151}, { 63,  62, 156},
    { 64,  64, 162}, { 65,  67, 167}, { 65,  70, 172}, { 66,  73, 177},
    { 66,  75, 181}, { 67,  78, 186}, { 68,  81, 191}, { 68,  84, 195},
    { 68,  86, 199}, { 69,  89, 203}, { 69,  92, 207}, { 69,  94, 211},
    { 70,  97, 214}, { 70, 100, 218}, { 70, 102, 221}, { 70, 105, 224},
    { 70, 107, 227}, { 71, 110, 230}, { 71, 113, 233}, { 71, 115, 235},
    { 71, 118, 238}, { 71, 120, 240}, { 71, 123, 242}, { 70, 125, 244},
    { 70, 128, 246}, { 70, 130, 248}, { 70, 133, 250}, { 70, 135, 251},
    { 69, 138, 252}, { 69, 140, 253}, { 68, 143, 254}, { 67, 145, 254},
    { 66, 148, 255}, { 65, 150, 255}, { 64, 153, 255}, { 62, 155, 254},
    { 61, 158, 254}, { 59, 160, 253}, { 58, 163, 252}, { 56, 165, 251},
    { 55, 168, 250}, { 53, 171, 248}, { 51, 173, 247}, { 49, 175, 245},
    { 47, 178, 244}, { 46, 180, 242}, { 44, 183, 240}, { 42, 185, 238},
    { 40, 188, 235}, { 39, 190, 233}, { 37, 192, 231}, { 35, 195, 228},
    { 34, 197, 226}, { 32, 199, 223}, { 31, 201, 221}, { 30, 203, 218},
    { 28, 205, 216}, { 27, 208, 213}, { 26, 210, 210}, { 26, 212, 208},
    { 25, 213, 205}, { 24, 215, 202}, { 24, 217, 200}, { 24, 219, 197},
    { 24, 221, 194}, { 24, 222, 192}, { 24, 224, 189}, { 25, 226, 187},
    { 25, 227, 185}, { 26, 228, 182}, { 28, 230, 180}, { 29, 231, 178},
    { 31, 233, 175}, { 32, 234, 172}, { 34, 235, 170}, { 37, 236, 167},
    { 39, 238, 164}, { 42, 239, 161}, { 44, 240, 158}, { 47, 241, 155},
    { 50, 242, 152}, { 53, 243, 148}, { 56, 244, 145}, { 60, 245, 142},
    { 63, 246, 138}, { 67, 247, 135}, { 70, 248, 132}, { 74, 248, 128},
    { 78, 249, 125}, { 82, 250, 122}, { 85, 250, 118}, { 89, 251, 115},
    { 93, 252, 111}, { 97, 252, 108}, {101, 253, 105}, {105, 253, 102},
    {109, 254,  98}, {113, 254,  95}, {117, 254,  92}, {121, 254,  89},
    {125, 255,  86}, {128, 255,  83}, {132, 255,  81}, {136, 255,  78},
    {139, 255,  75}, {143, 255,  73}, {146, 255,  71}, {150, 254,  68},
    {153, 254,  66}, {156, 254,  64}, {159, 253,  63}, {161, 253,  61},
    {164, 252,  60}, {167, 252,  58}, {169, 251,  57}, {172, 251,  56},
    {175, 250,  55}, {177, 249,  54}, {180, 248,  54}, {183, 247,  53},
    {185, 246,  53}, {188, 245,  52}, {190, 244,  52}, {193, 243,  52},
    {195, 241,  52}, {198, 240,  52}, {200, 239,  52}, {203, 237,  52},
    {205, 236,  52}, {208, 234,  52}, {210, 233,  53}, {212, 231,  53},
    {215, 229,  53}, {217, 228,  54}, {219, 226,  54}, {221, 224,  55},
    {223, 223,  55}, {225, 221,  55}, {227, 219,  56}, {229, 217,  56},
    {231, 215,  57}, {233, 213,  57}, {235, 211,  57}, {236, 209,  58},
    {238, 207,  58}, {239, 205,  58}, {241, 203,  58}, {242, 201,  58},
    {244, 199,  58}, {245, 197,  58}, {246, 195,  58}, {247, 193,  58},
    {248, 190,  57}, {249, 188,  57}, {250, 186,  57}, {251, 184,  56},
    {251, 182,  55}, {252, 179,  54}, {252, 177,  54}, {253, 174,  53},
    {253, 172,  52}, {254, 169,  51}, {254, 167,  50}, {254, 164,  49},
    {254, 161,  48}, {254, 158,  47}, {254, 155,  45}, {254, 153,  44},
    {254, 150,  43}, {254, 147,  42}, {254, 144,  41}, {253, 141,  39},
    {253, 138,  38}, {252, 135,  37}, {252, 132,  35}, {251, 129,  34},
    {251, 126,  33}, {250, 123,  31}, {249, 120,  30}, {249, 117,  29},
    {248, 114,  28}, {247, 111,  26}, {246, 108,  25}, {245, 105,  24},
    {244, 102,  23}, {243,  99,  21}, {242,  96,  20}, {241,  93,  19},
    {240,  91,  18}, {239,  88,  17}, {237,  85,  16}, {236,  83,  15},
    {235,  80,  14}, {234,  78,  13}, {232,  75,  12}, {231,  73,  12},
    {229,  71,  11}, {228,  69,  10}, {226,  67,  10}, {225,  65,   9},
    {223,  63,   8}, {221,  61,   8}, {220,  59,   7}, {218,  57,   7},
    {216,  55,   6}, {214,  53,   6}, {212,  51,   5}, {210,  49,   5},
    {208,  47,   5}, {206,  45,   4}, {204,  43,   4}, {202,  42,   4},
    {200,  40,   3}, {197,  38,   3}, {195,  37,   3}, {193,  35,   2},
    {190,  33,   2}, {188,  32,   2}, {185,  30,   2}, {183,  29,   2},
    {180,  27,   1}, {178,  26,   1}, {175,  24,   1}, {172,  23,   1},
    {169,  22,   1}, {167,  20,   1}, {164,  19,   1}, {161,  18,   1},
    {158,  16,   1}, {155,  15,   1}, {152,  14,   1}, {149,  13,   1},
    {146,  11,   1}, {142,  10,   1}, {139,   9,   2}, {136,   8,   2},
    {133,   7,   2}, {129,   6,   2}, {126,   5,   2}, {122,   4,   3}
};

/* Generated with Implementation/colormap-to-srgb.py */
constexpr UnsignedByte Magma[][3] = {
    {  0,   0,   3}, {  0,   0,   4}, {  0,   0,   6}, {  1,   0,   7},
    {  1,   1,   9}, {  1,   1,  11}, {  2,   2,  13}, {  2,   2,  15},
    {  3,   3,  17}, {  4,   3,  19}, {  4,   4,  21}, {  5,   4,  23},
    {  6,   5,  25}, {  7,   5,  27}, {  8,   6,  29}, {  9,   7,  31},
    { 10,   7,  34}, { 11,   8,  36}, { 12,   9,  38}, { 13,  10,  40},
    { 14,  10,  42}, { 15,  11,  44}, { 16,  12,  47}, { 17,  12,  49},
    { 18,  13,  51}, { 20,  13,  53}, { 21,  14,  56}, { 22,  14,  58},
    { 23,  15,  60}, { 24,  15,  63}, { 26,  16,  65}, { 27,  16,  68},
    { 28,  16,  70}, { 30,  16,  73}, { 31,  17,  75}, { 32,  17,  77},
    { 34,  17,  80}, { 35,  17,  82}, { 37,  17,  85}, { 38,  17,  87},
    { 40,  17,  89}, { 42,  17,  92}, { 43,  17,  94}, { 45,  16,  96},
    { 47,  16,  98}, { 48,  16, 101}, { 50,  16, 103}, { 52,  16, 104},
    { 53,  15, 106}, { 55,  15, 108}, { 57,  15, 110}, { 59,  15, 111},
    { 60,  15, 113}, { 62,  15, 114}, { 64,  15, 115}, { 66,  15, 116},
    { 67,  15, 117}, { 69,  15, 118}, { 71,  15, 119}, { 72,  16, 120},
    { 74,  16, 121}, { 75,  16, 121}, { 77,  17, 122}, { 79,  17, 123},
    { 80,  18, 123}, { 82,  18, 124}, { 83,  19, 124}, { 85,  19, 125},
    { 87,  20, 125}, { 88,  21, 126}, { 90,  21, 126}, { 91,  22, 126},
    { 93,  23, 126}, { 94,  23, 127}, { 96,  24, 127}, { 97,  24, 127},
    { 99,  25, 127}, {101,  26, 128}, {102,  26, 128}, {104,  27, 128},
    {105,  28, 128}, {107,  28, 128}, {108,  29, 128}, {110,  30, 129},
    {111,  30, 129}, {113,  31, 129}, {115,  31, 129}, {116,  32, 129},
    {118,  33, 129}, {119,  33, 129}, {121,  34, 129}, {122,  34, 129},
    {124,  35, 129}, {126,  36, 129}, {127,  36, 129}, {129,  37, 129},
    {130,  37, 129}, {132,  38, 129}, {133,  38, 129}, {135,  39, 129},
    {137,  40, 129}, {138,  40, 129}, {140,  41, 128}, {141,  41, 128},
    {143,  42, 128}, {145,  42, 128}, {146,  43, 128}, {148,  43, 128},
    {149,  44, 128}, {151,  44, 127}, {153,  45, 127}, {154,  45, 127},
    {156,  46, 127}, {158,  46, 126}, {159,  47, 126}, {161,  47, 126},
    {163,  48, 126}, {164,  48, 125}, {166,  49, 125}, {167,  49, 125},
    {169,  50, 124}, {171,  51, 124}, {172,  51, 123}, {174,  52, 123},
    {176,  52, 123}, {177,  53, 122}, {179,  53, 122}, {181,  54, 121},
    {182,  54, 121}, {184,  55, 120}, {185,  55, 120}, {187,  56, 119},
    {189,  57, 119}, {190,  57, 118}, {192,  58, 117}, {194,  58, 117},
    {195,  59, 116}, {197,  60, 116}, {198,  60, 115}, {200,  61, 114},
    {202,  62, 114}, {203,  62, 113}, {205,  63, 112}, {206,  64, 112},
    {208,  65, 111}, {209,  66, 110}, {211,  66, 109}, {212,  67, 109},
    {214,  68, 108}, {215,  69, 107}, {217,  70, 106}, {218,  71, 105},
    {220,  72, 105}, {221,  73, 104}, {222,  74, 103}, {224,  75, 102},
    {225,  76, 102}, {226,  77, 101}, {228,  78, 100}, {229,  80,  99},
    {230,  81,  98}, {231,  82,  98}, {232,  84,  97}, {234,  85,  96},
    {235,  86,  96}, {236,  88,  95}, {237,  89,  95}, {238,  91,  94},
    {238,  93,  93}, {239,  94,  93}, {240,  96,  93}, {241,  97,  92},
    {242,  99,  92}, {243, 101,  92}, {243, 103,  91}, {244, 104,  91},
    {245, 106,  91}, {245, 108,  91}, {246, 110,  91}, {246, 112,  91},
    {247, 113,  91}, {247, 115,  92}, {248, 117,  92}, {248, 119,  92},
    {249, 121,  92}, {249, 123,  93}, {249, 125,  93}, {250, 127,  94},
    {250, 128,  94}, {250, 130,  95}, {251, 132,  96}, {251, 134,  96},
    {251, 136,  97}, {251, 138,  98}, {252, 140,  99}, {252, 142,  99},
    {252, 144, 100}, {252, 146, 101}, {252, 147, 102}, {253, 149, 103},
    {253, 151, 104}, {253, 153, 105}, {253, 155, 106}, {253, 157, 107},
    {253, 159, 108}, {253, 161, 110}, {253, 162, 111}, {253, 164, 112},
    {254, 166, 113}, {254, 168, 115}, {254, 170, 116}, {254, 172, 117},
    {254, 174, 118}, {254, 175, 120}, {254, 177, 121}, {254, 179, 123},
    {254, 181, 124}, {254, 183, 125}, {254, 185, 127}, {254, 187, 128},
    {254, 188, 130}, {254, 190, 131}, {254, 192, 133}, {254, 194, 134},
    {254, 196, 136}, {254, 198, 137}, {254, 199, 139}, {254, 201, 141},
    {254, 203, 142}, {253, 205, 144}, {253, 207, 146}, {253, 209, 147},
    {253, 210, 149}, {253, 212, 151}, {253, 214, 152}, {253, 216, 154},
    {253, 218, 156}, {253, 220, 157}, {253, 221, 159}, {253, 223, 161},
    {253, 225, 163}, {252, 227, 165}, {252, 229, 166}, {252, 230, 168},
    {252, 232, 170}, {252, 234, 172}, {252, 236, 174}, {252, 238, 176},
    {252, 240, 177}, {252, 241, 179}, {252, 243, 181}, {252, 245, 183},
    {251, 247, 185}, {251, 249, 187}, {251, 250, 189}, {251, 252, 191}
};

/* Generated with Implementation/colormap-to-srgb.py */
constexpr UnsignedByte Plasma[][3] = {
    { 12,   7, 134}, { 16,   7, 135}, { 19,   6, 137}, { 21,   6, 138},
    { 24,   6, 139}, { 27,   6, 140}, { 29,   6, 141}, { 31,   5, 142},
    { 33,   5, 143}, { 35,   5, 144}, { 37,   5, 145}, { 39,   5, 146},
    { 41,   5, 147}, { 43,   5, 148}, { 45,   4, 148}, { 47,   4, 149},
    { 49,   4, 150}, { 51,   4, 151}, { 52,   4, 152}, { 54,   4, 152},
    { 56,   4, 153}, { 58,   4, 154}, { 59,   3, 154}, { 61,   3, 155},
    { 63,   3, 156}, { 64,   3, 156}, { 66,   3, 157}, { 68,   3, 158},
    { 69,   3, 158}, { 71,   2, 159}, { 73,   2, 159}, { 74,   2, 160},
    { 76,   2, 161}, { 78,   2, 161}, { 79,   2, 162}, { 81,   1, 162},
    { 82,   1, 163}, { 84,   1, 163}, { 86,   1, 163}, { 87,   1, 164},
    { 89,   1, 164}, { 90,   0, 165}, { 92,   0, 165}, { 94,   0, 165},
    { 95,   0, 166}, { 97,   0, 166}, { 98,   0, 166}, {100,   0, 167},
    {101,   0, 167}, {103,   0, 167}, {104,   0, 167}, {106,   0, 167},
    {108,   0, 168}, {109,   0, 168}, {111,   0, 168}, {112,   0, 168},
    {114,   0, 168}, {115,   0, 168}, {117,   0, 168}, {118,   1, 168},
    {120,   1, 168}, {121,   1, 168}, {123,   2, 168}, {124,   2, 167},
    {126,   3, 167}, {127,   3, 167}, {129,   4, 167}, {130,   4, 167},
    {132,   5, 166}, {133,   6, 166}, {134,   7, 166}, {136,   7, 165},
    {137,   8, 165}, {139,   9, 164}, {140,  10, 164}, {142,  12, 164},
    {143,  13, 163}, {144,  14, 163}, {146,  15, 162}, {147,  16, 161},
    {149,  17, 161}, {150,  18, 160}, {151,  19, 160}, {153,  20, 159},
    {154,  21, 158}, {155,  23, 158}, {157,  24, 157}, {158,  25, 156},
    {159,  26, 155}, {160,  27, 155}, {162,  28, 154}, {163,  29, 153},
    {164,  30, 152}, {165,  31, 151}, {167,  33, 151}, {168,  34, 150},
    {169,  35, 149}, {170,  36, 148}, {172,  37, 147}, {173,  38, 146},
    {174,  39, 145}, {175,  40, 144}, {176,  42, 143}, {177,  43, 143},
    {178,  44, 142}, {180,  45, 141}, {181,  46, 140}, {182,  47, 139},
    {183,  48, 138}, {184,  50, 137}, {185,  51, 136}, {186,  52, 135},
    {187,  53, 134}, {188,  54, 133}, {189,  55, 132}, {190,  56, 131},
    {191,  57, 130}, {192,  59, 129}, {193,  60, 128}, {194,  61, 128},
    {195,  62, 127}, {196,  63, 126}, {197,  64, 125}, {198,  65, 124},
    {199,  66, 123}, {200,  68, 122}, {201,  69, 121}, {202,  70, 120},
    {203,  71, 119}, {204,  72, 118}, {205,  73, 117}, {206,  74, 117},
    {207,  75, 116}, {208,  77, 115}, {209,  78, 114}, {209,  79, 113},
    {210,  80, 112}, {211,  81, 111}, {212,  82, 110}, {213,  83, 109},
    {214,  85, 109}, {215,  86, 108}, {215,  87, 107}, {216,  88, 106},
    {217,  89, 105}, {218,  90, 104}, {219,  91, 103}, {220,  93, 102},
    {220,  94, 102}, {221,  95, 101}, {222,  96, 100}, {223,  97,  99},
    {223,  98,  98}, {224, 100,  97}, {225, 101,  96}, {226, 102,  96},
    {227, 103,  95}, {227, 104,  94}, {228, 106,  93}, {229, 107,  92},
    {229, 108,  91}, {230, 109,  90}, {231, 110,  90}, {232, 112,  89},
    {232, 113,  88}, {233, 114,  87}, {234, 115,  86}, {234, 116,  85},
    {235, 118,  84}, {236, 119,  84}, {236, 120,  83}, {237, 121,  82},
    {237, 123,  81}, {238, 124,  80}, {239, 125,  79}, {239, 126,  78},
    {240, 128,  77}, {240, 129,  77}, {241, 130,  76}, {242, 132,  75},
    {242, 133,  74}, {243, 134,  73}, {243, 135,  72}, {244, 137,  71},
    {244, 138,  71}, {245, 139,  70}, {245, 141,  69}, {246, 142,  68},
    {246, 143,  67}, {246, 145,  66}, {247, 146,  65}, {247, 147,  65},
    {248, 149,  64}, {248, 150,  63}, {248, 152,  62}, {249, 153,  61},
    {249, 154,  60}, {250, 156,  59}, {250, 157,  58}, {250, 159,  58},
    {250, 160,  57}, {251, 162,  56}, {251, 163,  55}, {251, 164,  54},
    {252, 166,  53}, {252, 167,  53}, {252, 169,  52}, {252, 170,  51},
    {252, 172,  50}, {252, 173,  49}, {253, 175,  49}, {253, 176,  48},
    {253, 178,  47}, {253, 179,  46}, {253, 181,  45}, {253, 182,  45},
    {253, 184,  44}, {253, 185,  43}, {253, 187,  43}, {253, 188,  42},
    {253, 190,  41}, {253, 192,  41}, {253, 193,  40}, {253, 195,  40},
    {253, 196,  39}, {253, 198,  38}, {252, 199,  38}, {252, 201,  38},
    {252, 203,  37}, {252, 204,  37}, {252, 206,  37}, {251, 208,  36},
    {251, 209,  36}, {251, 211,  36}, {250, 213,  36}, {250, 214,  36},
    {250, 216,  36}, {249, 217,  36}, {249, 219,  36}, {248, 221,  36},
    {248, 223,  36}, {247, 224,  36}, {247, 226,  37}, {246, 228,  37},
    {246, 229,  37}, {245, 231,  38}, {245, 233,  38}, {244, 234,  38},
    {243, 236,  38}, {243, 238,  38}, {242, 240,  38}, {242, 241,  38},
    {241, 243,  38}, {240, 245,  37}, {240, 246,  35}, {239, 248,  33}
};

/* Generated with Implementation/colormap-to-srgb.py */
constexpr UnsignedByte Inferno[][3] = {
    {  0,   0,   3}, {  0,   0,   4}, {  0,   0,   6}, {  1,   0,   7},
    {  1,   1,   9}, {  1,   1,  11}, {  2,   1,  14}, {  2,   2,  16},
    {  3,   2,  18}, {  4,   3,  20}, {  4,   3,  22}, {  5,   4,  24},
    {  6,   4,  27}, {  7,   5,  29}, {  8,   6,  31}, {  9,   6,  33},
    { 10,   7,  35}, { 11,   7,  38}, { 13,   8,  40}, { 14,   8,  42},
    { 15,   9,  45}, { 16,   9,  47}, { 18,  10,  50}, { 19,  10,  52},
    { 20,  11,  54}, { 22,  11,  57}, { 23,  11,  59}, { 25,  11,  62},
    { 26,  11,  64}, { 28,  12,  67}, { 29,  12,  69}, { 31,  12,  71},
    { 32,  12,  74}, { 34,  11,  76}, { 36,  11,  78}, { 38,  11,  80},
    { 39,  11,  82}, { 41,  11,  84}, { 43,  10,  86}, { 45,  10,  88},
    { 46,  10,  90}, { 48,  10,  92}, { 50,   9,  93}, { 52,   9,  95},
    { 53,   9,  96}, { 55,   9,  97}, { 57,   9,  98}, { 59,   9, 100},
    { 60,   9, 101}, { 62,   9, 102}, { 64,   9, 102}, { 65,   9, 103},
    { 67,  10, 104}, { 69,  10, 105}, { 70,  10, 105}, { 72,  11, 106},
    { 74,  11, 106}, { 75,  12, 107}, { 77,  12, 107}, { 79,  13, 108},
    { 80,  13, 108}, { 82,  14, 108}, { 83,  14, 109}, { 85,  15, 109},
    { 87,  15, 109}, { 88,  16, 109}, { 90,  17, 109}, { 91,  17, 110},
    { 93,  18, 110}, { 95,  18, 110}, { 96,  19, 110}, { 98,  20, 110},
    { 99,  20, 110}, {101,  21, 110}, {102,  21, 110}, {104,  22, 110},
    {106,  23, 110}, {107,  23, 110}, {109,  24, 110}, {110,  24, 110},
    {112,  25, 110}, {114,  25, 109}, {115,  26, 109}, {117,  27, 109},
    {118,  27, 109}, {120,  28, 109}, {122,  28, 109}, {123,  29, 108},
    {125,  29, 108}, {126,  30, 108}, {128,  31, 107}, {129,  31, 107},
    {131,  32, 107}, {133,  32, 106}, {134,  33, 106}, {136,  33, 106},
    {137,  34, 105}, {139,  34, 105}, {141,  35, 105}, {142,  36, 104},
    {144,  36, 104}, {145,  37, 103}, {147,  37, 103}, {149,  38, 102},
    {150,  38, 102}, {152,  39, 101}, {153,  40, 100}, {155,  40, 100},
    {156,  41,  99}, {158,  41,  99}, {160,  42,  98}, {161,  43,  97},
    {163,  43,  97}, {164,  44,  96}, {166,  44,  95}, {167,  45,  95},
    {169,  46,  94}, {171,  46,  93}, {172,  47,  92}, {174,  48,  91},
    {175,  49,  91}, {177,  49,  90}, {178,  50,  89}, {180,  51,  88},
    {181,  51,  87}, {183,  52,  86}, {184,  53,  86}, {186,  54,  85},
    {187,  55,  84}, {189,  55,  83}, {190,  56,  82}, {191,  57,  81},
    {193,  58,  80}, {194,  59,  79}, {196,  60,  78}, {197,  61,  77},
    {199,  62,  76}, {200,  62,  75}, {201,  63,  74}, {203,  64,  73},
    {204,  65,  72}, {205,  66,  71}, {207,  68,  70}, {208,  69,  68},
    {209,  70,  67}, {210,  71,  66}, {212,  72,  65}, {213,  73,  64},
    {214,  74,  63}, {215,  75,  62}, {217,  77,  61}, {218,  78,  59},
    {219,  79,  58}, {220,  80,  57}, {221,  82,  56}, {222,  83,  55},
    {223,  84,  54}, {224,  86,  52}, {226,  87,  51}, {227,  88,  50},
    {228,  90,  49}, {229,  91,  48}, {230,  92,  46}, {230,  94,  45},
    {231,  95,  44}, {232,  97,  43}, {233,  98,  42}, {234, 100,  40},
    {235, 101,  39}, {236, 103,  38}, {237, 104,  37}, {237, 106,  35},
    {238, 108,  34}, {239, 109,  33}, {240, 111,  31}, {240, 112,  30},
    {241, 114,  29}, {242, 116,  28}, {242, 117,  26}, {243, 119,  25},
    {243, 121,  24}, {244, 122,  22}, {245, 124,  21}, {245, 126,  20},
    {246, 128,  18}, {246, 129,  17}, {247, 131,  16}, {247, 133,  14},
    {248, 135,  13}, {248, 136,  12}, {248, 138,  11}, {249, 140,   9},
    {249, 142,   8}, {249, 144,   8}, {250, 145,   7}, {250, 147,   6},
    {250, 149,   6}, {250, 151,   6}, {251, 153,   6}, {251, 155,   6},
    {251, 157,   6}, {251, 158,   7}, {251, 160,   7}, {251, 162,   8},
    {251, 164,  10}, {251, 166,  11}, {251, 168,  13}, {251, 170,  14},
    {251, 172,  16}, {251, 174,  18}, {251, 176,  20}, {251, 177,  22},
    {251, 179,  24}, {251, 181,  26}, {251, 183,  28}, {251, 185,  30},
    {250, 187,  33}, {250, 189,  35}, {250, 191,  37}, {250, 193,  40},
    {249, 195,  42}, {249, 197,  44}, {249, 199,  47}, {248, 201,  49},
    {248, 203,  52}, {248, 205,  55}, {247, 207,  58}, {247, 209,  60},
    {246, 211,  63}, {246, 213,  66}, {245, 215,  69}, {245, 217,  72},
    {244, 219,  75}, {244, 220,  79}, {243, 222,  82}, {243, 224,  86},
    {243, 226,  89}, {242, 228,  93}, {242, 230,  96}, {241, 232, 100},
    {241, 233, 104}, {241, 235, 108}, {241, 237, 112}, {241, 238, 116},
    {241, 240, 121}, {241, 242, 125}, {242, 243, 129}, {242, 244, 133},
    {243, 246, 137}, {244, 247, 141}, {245, 248, 145}, {246, 250, 149},
    {247, 251, 153}, {249, 252, 157}, {250, 253, 160}, {252, 254, 164}
};

/* Generated with Implementation/colormap-to-srgb.py */
constexpr UnsignedByte Viridis[][3] = {
    { 68,   1,  84}, { 68,   2,  85}, { 68,   3,  87}, { 69,   5,  88},
    { 69,   6,  90}, { 69,   8,  91}, { 70,   9,  92}, { 70,  11,  94},
    { 70,  12,  95}, { 70,  14,  97}, { 71,  15,  98}, { 71,  17,  99},
    { 71,  18, 101}, { 71,  20, 102}, { 71,  21, 103}, { 71,  22, 105},
    { 71,  24, 106}, { 72,  25, 107}, { 72,  26, 108}, { 72,  28, 110},
    { 72,  29, 111}, { 72,  30, 112}, { 72,  32, 113}, { 72,  33, 114},
    { 72,  34, 115}, { 72,  35, 116}, { 71,  37, 117}, { 71,  38, 118},
    { 71,  39, 119}, { 71,  40, 120}, { 71,  42, 121}, { 71,  43, 122},
    { 71,  44, 123}, { 70,  45, 124}, { 70,  47, 124}, { 70,  48, 125},
    { 70,  49, 126}, { 69,  50, 127}, { 69,  52, 127}, { 69,  53, 128},
    { 69,  54, 129}, { 68,  55, 129}, { 68,  57, 130}, { 67,  58, 131},
    { 67,  59, 131}, { 67,  60, 132}, { 66,  61, 132}, { 66,  62, 133},
    { 66,  64, 133}, { 65,  65, 134}, { 65,  66, 134}, { 64,  67, 135},
    { 64,  68, 135}, { 63,  69, 135}, { 63,  71, 136}, { 62,  72, 136},
    { 62,  73, 137}, { 61,  74, 137}, { 61,  75, 137}, { 61,  76, 137},
    { 60,  77, 138}, { 60,  78, 138}, { 59,  80, 138}, { 59,  81, 138},
    { 58,  82, 139}, { 58,  83, 139}, { 57,  84, 139}, { 57,  85, 139},
    { 56,  86, 139}, { 56,  87, 140}, { 55,  88, 140}, { 55,  89, 140},
    { 54,  90, 140}, { 54,  91, 140}, { 53,  92, 140}, { 53,  93, 140},
    { 52,  94, 141}, { 52,  95, 141}, { 51,  96, 141}, { 51,  97, 141},
    { 50,  98, 141}, { 50,  99, 141}, { 49, 100, 141}, { 49, 101, 141},
    { 49, 102, 141}, { 48, 103, 141}, { 48, 104, 141}, { 47, 105, 141},
    { 47, 106, 141}, { 46, 107, 142}, { 46, 108, 142}, { 46, 109, 142},
    { 45, 110, 142}, { 45, 111, 142}, { 44, 112, 142}, { 44, 113, 142},
    { 44, 114, 142}, { 43, 115, 142}, { 43, 116, 142}, { 42, 117, 142},
    { 42, 118, 142}, { 42, 119, 142}, { 41, 120, 142}, { 41, 121, 142},
    { 40, 122, 142}, { 40, 122, 142}, { 40, 123, 142}, { 39, 124, 142},
    { 39, 125, 142}, { 39, 126, 142}, { 38, 127, 142}, { 38, 128, 142},
    { 38, 129, 142}, { 37, 130, 142}, { 37, 131, 141}, { 36, 132, 141},
    { 36, 133, 141}, { 36, 134, 141}, { 35, 135, 141}, { 35, 136, 141},
    { 35, 137, 141}, { 34, 137, 141}, { 34, 138, 141}, { 34, 139, 141},
    { 33, 140, 141}, { 33, 141, 140}, { 33, 142, 140}, { 32, 143, 140},
    { 32, 144, 140}, { 32, 145, 140}, { 31, 146, 140}, { 31, 147, 139},
    { 31, 148, 139}, { 31, 149, 139}, { 31, 150, 139}, { 30, 151, 138},
    { 30, 152, 138}, { 30, 153, 138}, { 30, 153, 138}, { 30, 154, 137},
    { 30, 155, 137}, { 30, 156, 137}, { 30, 157, 136}, { 30, 158, 136},
    { 30, 159, 136}, { 30, 160, 135}, { 31, 161, 135}, { 31, 162, 134},
    { 31, 163, 134}, { 32, 164, 133}, { 32, 165, 133}, { 33, 166, 133},
    { 33, 167, 132}, { 34, 167, 132}, { 35, 168, 131}, { 35, 169, 130},
    { 36, 170, 130}, { 37, 171, 129}, { 38, 172, 129}, { 39, 173, 128},
    { 40, 174, 127}, { 41, 175, 127}, { 42, 176, 126}, { 43, 177, 125},
    { 44, 177, 125}, { 46, 178, 124}, { 47, 179, 123}, { 48, 180, 122},
    { 50, 181, 122}, { 51, 182, 121}, { 53, 183, 120}, { 54, 184, 119},
    { 56, 185, 118}, { 57, 185, 118}, { 59, 186, 117}, { 61, 187, 116},
    { 62, 188, 115}, { 64, 189, 114}, { 66, 190, 113}, { 68, 190, 112},
    { 69, 191, 111}, { 71, 192, 110}, { 73, 193, 109}, { 75, 194, 108},
    { 77, 194, 107}, { 79, 195, 105}, { 81, 196, 104}, { 83, 197, 103},
    { 85, 198, 102}, { 87, 198, 101}, { 89, 199, 100}, { 91, 200,  98},
    { 94, 201,  97}, { 96, 201,  96}, { 98, 202,  95}, {100, 203,  93},
    {103, 204,  92}, {105, 204,  91}, {107, 205,  89}, {109, 206,  88},
    {112, 206,  86}, {114, 207,  85}, {116, 208,  84}, {119, 208,  82},
    {121, 209,  81}, {124, 210,  79}, {126, 210,  78}, {129, 211,  76},
    {131, 211,  75}, {134, 212,  73}, {136, 213,  71}, {139, 213,  70},
    {141, 214,  68}, {144, 214,  67}, {146, 215,  65}, {149, 215,  63},
    {151, 216,  62}, {154, 216,  60}, {157, 217,  58}, {159, 217,  56},
    {162, 218,  55}, {165, 218,  53}, {167, 219,  51}, {170, 219,  50},
    {173, 220,  48}, {175, 220,  46}, {178, 221,  44}, {181, 221,  43},
    {183, 221,  41}, {186, 222,  39}, {189, 222,  38}, {191, 223,  36},
    {194, 223,  34}, {197, 223,  33}, {199, 224,  31}, {202, 224,  30},
    {205, 224,  29}, {207, 225,  28}, {210, 225,  27}, {212, 225,  26},
    {215, 226,  25}, {218, 226,  24}, {220, 226,  24}, {223, 227,  24},
    {225, 227,  24}, {228, 227,  24}, {231, 228,  25}, {233, 228,  25},
    {236, 228,  26}, {238, 229,  27}, {241, 229,  28}, {243, 229,  30},
    {246, 230,  31}, {248, 230,  33}, {250, 230,  34}, {253, 231,  36}
};

/* Generated with Implementation/cool-warm.py */
constexpr UnsignedByte CoolWarmSmooth[][3] = {
    { 85,  72, 193}, { 86,  74, 194}, { 88,  76, 196}, { 89,  78, 198},
    { 90,  79, 199}, { 91,  81, 201}, { 92,  83, 202}, { 93,  85, 204},
    { 94,  87, 205}, { 95,  88, 207}, { 96,  90, 208}, { 97,  92, 209},
    { 98,  94, 211}, { 99,  95, 212}, {100,  97, 214}, {102,  99, 215},
    {103, 101, 216}, {104, 102, 218}, {105, 104, 219}, {106, 106, 220},
    {107, 107, 221}, {108, 109, 223}, {109, 111, 224}, {110, 113, 225},
    {111, 114, 226}, {112, 116, 227}, {113, 118, 228}, {115, 119, 230},
    {116, 121, 231}, {117, 123, 232}, {118, 124, 233}, {119, 126, 234},
    {120, 127, 235}, {121, 129, 236}, {122, 131, 237}, {123, 132, 238},
    {125, 134, 238}, {126, 135, 239}, {127, 137, 240}, {128, 139, 241},
    {129, 140, 242}, {130, 142, 243}, {131, 143, 243}, {132, 145, 244},
    {133, 146, 245}, {135, 148, 246}, {136, 149, 246}, {137, 151, 247},
    {138, 152, 248}, {139, 154, 248}, {140, 155, 249}, {141, 157, 249},
    {142, 158, 250}, {144, 160, 250}, {145, 161, 251}, {146, 162, 251},
    {147, 164, 252}, {148, 165, 252}, {149, 166, 253}, {150, 168, 253},
    {151, 169, 253}, {153, 170, 254}, {154, 172, 254}, {155, 173, 254},
    {156, 174, 254}, {157, 176, 255}, {158, 177, 255}, {159, 178, 255},
    {160, 179, 255}, {162, 181, 255}, {163, 182, 255}, {164, 183, 255},
    {165, 184, 255}, {166, 185, 255}, {167, 186, 255}, {168, 187, 255},
    {169, 189, 255}, {171, 190, 255}, {172, 191, 255}, {173, 192, 255},
    {174, 193, 255}, {175, 194, 255}, {176, 195, 255}, {177, 196, 254},
    {178, 197, 254}, {179, 198, 254}, {180, 199, 253}, {182, 200, 253},
    {183, 201, 253}, {184, 201, 252}, {185, 202, 252}, {186, 203, 252},
    {187, 204, 251}, {188, 205, 251}, {189, 206, 250}, {190, 206, 250},
    {191, 207, 249}, {192, 208, 249}, {193, 209, 248}, {194, 209, 247},
    {195, 210, 247}, {196, 211, 246}, {197, 211, 245}, {198, 212, 245},
    {199, 212, 244}, {200, 213, 243}, {201, 214, 242}, {202, 214, 242},
    {203, 215, 241}, {204, 215, 240}, {205, 216, 239}, {206, 216, 238},
    {207, 217, 237}, {208, 217, 236}, {209, 217, 235}, {210, 218, 234},
    {211, 218, 233}, {212, 218, 232}, {213, 219, 231}, {214, 219, 230},
    {214, 219, 229}, {215, 220, 228}, {216, 220, 227}, {217, 220, 226},
    {218, 220, 225}, {219, 220, 224}, {219, 220, 222}, {220, 221, 221},
    {221, 220, 220}, {222, 220, 219}, {223, 220, 217}, {224, 219, 216},
    {225, 219, 214}, {226, 218, 213}, {227, 217, 211}, {227, 217, 210},
    {228, 216, 209}, {229, 216, 207}, {230, 215, 206}, {231, 214, 204},
    {231, 214, 203}, {232, 213, 201}, {233, 212, 200}, {233, 211, 198},
    {234, 211, 197}, {235, 210, 195}, {235, 209, 194}, {236, 208, 192},
    {236, 207, 190}, {237, 207, 189}, {237, 206, 187}, {238, 205, 186},
    {238, 204, 184}, {239, 203, 183}, {239, 202, 181}, {240, 201, 180},
    {240, 200, 178}, {240, 199, 177}, {241, 198, 175}, {241, 197, 173},
    {241, 196, 172}, {242, 195, 170}, {242, 194, 169}, {242, 193, 167},
    {242, 191, 166}, {243, 190, 164}, {243, 189, 162}, {243, 188, 161},
    {243, 187, 159}, {243, 185, 158}, {243, 184, 156}, {243, 183, 154},
    {243, 182, 153}, {243, 180, 151}, {243, 179, 150}, {243, 178, 148},
    {243, 176, 147}, {243, 175, 145}, {243, 174, 143}, {243, 172, 142},
    {243, 171, 140}, {243, 169, 139}, {243, 168, 137}, {242, 167, 136},
    {242, 165, 134}, {242, 164, 133}, {242, 162, 131}, {241, 161, 129},
    {241, 159, 128}, {241, 158, 126}, {240, 156, 125}, {240, 154, 123},
    {240, 153, 122}, {239, 151, 120}, {239, 150, 119}, {238, 148, 117},
    {238, 146, 116}, {237, 145, 114}, {237, 143, 113}, {236, 141, 111},
    {236, 140, 110}, {235, 138, 108}, {235, 136, 107}, {234, 134, 105},
    {234, 133, 104}, {233, 131, 102}, {232, 129, 101}, {232, 127,  99},
    {231, 126,  98}, {230, 124,  97}, {229, 122,  95}, {229, 120,  94},
    {228, 118,  92}, {227, 116,  91}, {226, 114,  89}, {225, 112,  88},
    {224, 111,  87}, {224, 109,  85}, {223, 107,  84}, {222, 105,  83},
    {221, 103,  81}, {220, 101,  80}, {219,  99,  78}, {218,  97,  77},
    {217,  95,  76}, {216,  93,  75}, {215,  91,  73}, {214,  88,  72},
    {213,  86,  71}, {212,  84,  69}, {210,  82,  68}, {209,  80,  67},
    {208,  78,  66}, {207,  75,  64}, {206,  73,  63}, {205,  71,  62},
    {203,  68,  61}, {202,  66,  59}, {201,  64,  58}, {200,  61,  57},
    {198,  59,  56}, {197,  56,  55}, {196,  54,  54}, {194,  51,  52},
    {193,  48,  51}, {192,  45,  50}, {190,  42,  49}, {189,  39,  48},
    {187,  36,  47}, {186,  33,  46}, {185,  29,  45}, {183,  25,  44},
    {182,  20,  43}, {180,  15,  41}, {179,   8,  40}, {177,   1,  39}
};

/* Generated with Implementation/cool-warm.py */
constexpr UnsignedByte CoolWarmBent[][3] = {
    { 85,  72, 193}, { 86,  74, 194}, { 87,  75, 194}, { 88,  77, 195},
    { 88,  78, 196}, { 89,  79, 196}, { 90,  81, 197}, { 90,  82, 197},
    { 91,  84, 198}, { 92,  85, 199}, { 93,  87, 199}, { 94,  88, 200},
    { 94,  90, 200}, { 95,  91, 201}, { 96,  92, 202}, { 97,  94, 202},
    { 98,  95, 203}, { 98,  97, 203}, { 99,  98, 204}, {100,  99, 204},
    {101, 101, 205}, {102, 102, 206}, {103, 104, 206}, {104, 105, 207},
    {104, 106, 207}, {105, 108, 208}, {106, 109, 208}, {107, 110, 209},
    {108, 112, 209}, {109, 113, 210}, {110, 115, 210}, {111, 116, 211},
    {112, 117, 211}, {113, 119, 212}, {114, 120, 212}, {115, 121, 213},
    {116, 123, 213}, {117, 124, 214}, {118, 125, 214}, {119, 127, 215},
    {120, 128, 215}, {121, 129, 215}, {122, 131, 216}, {123, 132, 216},
    {124, 134, 217}, {125, 135, 217}, {126, 136, 218}, {127, 138, 218},
    {128, 139, 218}, {129, 140, 219}, {130, 142, 219}, {132, 143, 220},
    {133, 144, 220}, {134, 146, 221}, {135, 147, 221}, {136, 148, 221},
    {137, 150, 222}, {138, 151, 222}, {140, 152, 222}, {141, 154, 223},
    {142, 155, 223}, {143, 156, 224}, {144, 158, 224}, {146, 159, 224},
    {147, 160, 225}, {148, 162, 225}, {149, 163, 225}, {151, 164, 226},
    {152, 166, 226}, {153, 167, 226}, {154, 168, 227}, {156, 170, 227},
    {157, 171, 227}, {158, 172, 228}, {160, 174, 228}, {161, 175, 228},
    {162, 176, 229}, {164, 177, 229}, {165, 179, 229}, {166, 180, 230},
    {168, 181, 230}, {169, 183, 230}, {170, 184, 230}, {172, 185, 231},
    {173, 187, 231}, {175, 188, 231}, {176, 189, 232}, {177, 191, 232},
    {179, 192, 232}, {180, 193, 232}, {182, 194, 233}, {183, 196, 233},
    {185, 197, 233}, {186, 198, 234}, {188, 200, 234}, {189, 201, 234},
    {191, 202, 234}, {192, 204, 235}, {194, 205, 235}, {195, 206, 235},
    {197, 207, 235}, {198, 209, 236}, {200, 210, 236}, {201, 211, 236},
    {203, 213, 236}, {205, 214, 237}, {206, 215, 237}, {208, 216, 237},
    {209, 218, 237}, {211, 219, 238}, {213, 220, 238}, {214, 221, 238},
    {216, 223, 238}, {217, 224, 239}, {219, 225, 239}, {221, 227, 239},
    {222, 228, 239}, {224, 229, 240}, {226, 230, 240}, {227, 232, 240},
    {229, 233, 240}, {231, 234, 241}, {233, 235, 241}, {234, 237, 241},
    {236, 238, 241}, {238, 239, 242}, {240, 240, 242}, {241, 242, 242},
    {242, 242, 241}, {242, 240, 239}, {241, 239, 237}, {241, 237, 235},
    {240, 236, 232}, {240, 234, 230}, {239, 233, 228}, {239, 231, 226},
    {239, 230, 224}, {238, 228, 221}, {238, 226, 219}, {237, 225, 217},
    {237, 223, 215}, {237, 222, 213}, {236, 220, 211}, {236, 219, 209},
    {235, 217, 207}, {235, 216, 205}, {234, 214, 203}, {234, 213, 200},
    {234, 211, 198}, {233, 210, 196}, {233, 208, 194}, {232, 207, 192},
    {232, 205, 190}, {231, 204, 188}, {231, 202, 186}, {231, 201, 184},
    {230, 199, 182}, {230, 198, 180}, {229, 196, 178}, {229, 195, 176},
    {228, 193, 175}, {228, 192, 173}, {228, 190, 171}, {227, 188, 169},
    {227, 187, 167}, {226, 185, 165}, {226, 184, 163}, {225, 182, 161},
    {225, 181, 159}, {224, 179, 158}, {224, 178, 156}, {224, 176, 154},
    {223, 175, 152}, {223, 173, 150}, {222, 171, 148}, {222, 170, 147},
    {221, 168, 145}, {221, 167, 143}, {220, 165, 141}, {220, 164, 140},
    {219, 162, 138}, {219, 161, 136}, {219, 159, 134}, {218, 157, 133},
    {218, 156, 131}, {217, 154, 129}, {217, 153, 128}, {216, 151, 126},
    {216, 149, 124}, {215, 148, 123}, {215, 146, 121}, {214, 145, 119},
    {214, 143, 118}, {213, 141, 116}, {213, 140, 115}, {212, 138, 113},
    {212, 137, 111}, {211, 135, 110}, {211, 133, 108}, {210, 132, 107},
    {210, 130, 105}, {209, 129, 104}, {209, 127, 102}, {208, 125, 101},
    {208, 124,  99}, {207, 122,  98}, {207, 120,  96}, {206, 119,  95},
    {206, 117,  93}, {205, 115,  92}, {205, 114,  91}, {204, 112,  89},
    {203, 110,  88}, {203, 109,  86}, {202, 107,  85}, {202, 105,  84},
    {201, 103,  82}, {201, 102,  81}, {200, 100,  80}, {200,  98,  78},
    {199,  96,  77}, {198,  95,  76}, {198,  93,  74}, {197,  91,  73},
    {197,  89,  72}, {196,  87,  71}, {196,  86,  69}, {195,  84,  68},
    {194,  82,  67}, {194,  80,  66}, {193,  78,  65}, {193,  76,  63},
    {192,  74,  62}, {191,  72,  61}, {191,  70,  60}, {190,  68,  59},
    {189,  66,  58}, {189,  64,  57}, {188,  62,  56}, {188,  59,  55},
    {187,  57,  54}, {186,  55,  53}, {186,  52,  52}, {185,  50,  50},
    {184,  47,  49}, {184,  45,  49}, {183,  42,  48}, {182,  39,  47},
    {182,  36,  46}, {181,  33,  45}, {181,  29,  44}, {180,  25,  43},
    {179,  21,  42}, {178,  15,  41}, {178,   8,  40}, {177,   1,  39}
};

static_assert(sizeof(Turbo) == 256*3, "");
static_assert(sizeof(Magma) == 256*3, "");
static_assert(sizeof(Plasma) == 256*3, "");
static_assert(sizeof(Inferno) == 256*3, "");
static_assert(sizeof(Viridis) == 256*3, "");
static_assert(sizeof(CoolWarmSmooth) == 256*3, "");
static_assert(sizeof(CoolWarmBent) == 256*3, "");

}

Containers::StaticArrayView<256, const Vector3ub> turbo() {
    return Containers::StaticArrayView<256, const Vector3ub>{reinterpret_cast<const Vector3ub*>(&Turbo[0][0])};
}

Containers::StaticArrayView<256, const Vector3ub> magma() {
    return Containers::StaticArrayView<256, const Vector3ub>{reinterpret_cast<const Vector3ub*>(&Magma[0][0])};
}

Containers::StaticArrayView<256, const Vector3ub> plasma() {
    return Containers::StaticArrayView<256, const Vector3ub>{reinterpret_cast<const Vector3ub*>(&Plasma[0][0])};
}

Containers::StaticArrayView<256, const Vector3ub> inferno() {
    return Containers::StaticArrayView<256, const Vector3ub>{reinterpret_cast<const Vector3ub*>(&Inferno[0][0])};
}

Containers::StaticArrayView<256, const Vector3ub> viridis() {
    return Containers::StaticArrayView<256, const Vector3ub>{reinterpret_cast<const Vector3ub*>(&Viridis[0][0])};
}

Containers::StaticArrayView<256, const Vector3ub> coolWarmSmooth() {
    return Containers::StaticArrayView<256, const Vector3ub>{reinterpret_cast<const Vector3ub*>(&CoolWarmSmooth[0][0])};
}

Containers::StaticArrayView<256, const Vector3ub> coolWarmBent() {
    return Containers::StaticArrayView<256, const Vector3ub>{reinterpret_cast<const Vector3ub*>(&CoolWarmBent[0][0])};
}

}}}
