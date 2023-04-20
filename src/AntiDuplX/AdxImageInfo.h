/*
* AntiDuplX utility (http://github.com/ermig1979/AntiDuplX).
*
* Copyright (c) 2023-2023 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma once

#include "AntiDuplX/AdxCommon.h"

namespace Adx
{
    enum ImageErorr
    {
        ImageOk = 0,
        ImageLoadError,
        ImageDecodeError,
        ImageRemoveError,
    };

    typedef Simd::ImageMatcher<struct ImageInfo*, Simd::Allocator> Matcher;

    struct ImageInfo
    {
        String path;
        size_t size;
        FileTime time;
        bool remove;
        size_t width;
        size_t height;
        SimdImageFileType format;
        Matcher::HashPtr hash;
        ImageErorr error;

        ImageInfo()
            : size(0)
            , remove(false)
            , width(0)
            , height(0)
            , format(SimdImageFileUndefined)
            , error(ImageOk)
        {
        }
    };

    typedef std::vector<ImageInfo*> ImageInfos;

    inline String ToStr(SimdImageFileType format)
    {
        switch (format)
        {
        case SimdImageFileUndefined: return "NONE";
        case SimdImageFilePgmTxt: return "PGM";
        case SimdImageFilePgmBin: return "PGM";
        case SimdImageFilePpmTxt: return "PPM";
        case SimdImageFilePpmBin: return "PPM";
        case SimdImageFilePng: return "PNG";
        case SimdImageFileJpeg: return "JPEG";
        default:
            return "";
        }
    }

    inline int Compare(const ImageInfo& a, const ImageInfo& b)
    {
        size_t aArea = a.width * a.height;
        size_t bArea = b.width * b.height;
        if (aArea > bArea)
            return 1;
        if (aArea < bArea)
            return -1;
        if (a.format == b.format)
        {
            if (a.size > b.size)
                return 1;
            if (a.size < b.size)
                return -1;
        }
        else if (a.format == SimdImageFilePng)
            return 1;
        else if (b.format == SimdImageFilePng)
            return -1;
        return 0;
    }

    CPL_INLINE bool Lesser(const ImageInfo* a, const ImageInfo* b)
    {
        return Compare(*a, *b) < 0;
    }

    CPL_INLINE bool Greater(const ImageInfo* a, const ImageInfo* b)
    {
        return Compare(*a, *b) > 0;
    }
}

