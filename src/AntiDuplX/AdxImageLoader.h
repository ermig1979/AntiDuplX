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
#include "AntiDuplX/AdxOptions.h"
#include "AntiDuplX/AdxImageInfo.h"
#include "AntiDuplX/AdxTurboJpegDecoder.h"

namespace Adx
{
    class ImageLoader
    {
    public:
        ImageLoader(const Options& options, ImageInfos& imageInfos);

        bool Run();

    private:
        const Options & _options;
        ImageInfos & _imageInfos;
        Matcher _matcher;
        double _progress;
        struct Context
        {
            std::thread thread;
            volatile size_t index;
            Buffer buffer;
            TurboJpegDecoder decoder;
            View image;
            Context()
                : index(0)
            {}
        };
        std::vector<Context> _context;

        void SetProgress();
        void LoadThread(size_t thread, size_t begin, size_t end);
        bool LoadFile(Context& context, ImageInfo& info);
        bool DecodeImage(Context& context, ImageInfo& info);
        void CreateHash(Context& context, ImageInfo& info);
        size_t Processed() const;
    };
}

