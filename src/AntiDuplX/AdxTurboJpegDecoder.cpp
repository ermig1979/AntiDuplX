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

#include "AntiDuplX/AdxTurboJpegDecoder.h"

#ifdef __linux__
#include <turbojpeg.h>

namespace Adx
{
    TurboJpegDecoder::TurboJpegDecoder()
        : _decoder(0)
    {
        _decoder = tjInitDecompress();
    }

    TurboJpegDecoder::~TurboJpegDecoder()
    {
        if (_decoder)
        {
            int error = tjDestroy(_decoder);
            _decoder = NULL;
        }
    }

    bool TurboJpegDecoder::Decode(const Buffer& buffer, View& view)
    {
        int width, height, subSample, colorSpace;
        int error = tjDecompressHeader3(_decoder, buffer.data(), buffer.size(), &width, &height, &subSample, &colorSpace);
        if (error)
            return false;
        view.Recreate(width, height, View::Gray8);
        error = tjDecompress2(_decoder, buffer.data(), buffer.size(), view.data, view.width, view.stride, view.height, TJPF_GRAY, TJFLAG_FASTDCT);
        if (error)
            return false;
        return true;
    }
}

#else
namespace Adx
{
    TurboJpegDecoder::TurboJpegDecoder()
        : _decoder(0)
    {
    }

    TurboJpegDecoder::~TurboJpegDecoder()
    {
    }

    bool TurboJpegDecoder::Decode(const Buffer& buffer, View& view)
    {
        return false;
    }
}
#endif

