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

    typedef std::vector<ImageInfo> ImageInfos;

    CPL_INLINE String ToStr(SimdImageFileType format)
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
}

