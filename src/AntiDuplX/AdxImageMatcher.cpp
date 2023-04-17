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

#include "AntiDuplX/AdxCommon.h"
#include "AntiDuplX/AdxImageMatcher.h"

#include <filesystem>

namespace Adx
{
    namespace fs = std::filesystem;

    bool ImageMatcher::Run()
    {
        CPL_LOG_SS(Info, "Match images: ");

        SetProgress(0);
        for (size_t i = 0; i < _imageInfos.size(); ++i)
        {
            if (!LoadImage(i))
                return false;
            SetProgress(i);
        }
        SetProgress(-1);

        return true;
    }

    void ImageMatcher::SetProgress(size_t index)
    {
        double progress = double(std::min(index, _imageInfos.size())) / double(_imageInfos.size());
        std::cout << "\rMatch progress: " << std::fixed << std::setprecision(1) << progress * 100.0 << "%" << std::flush;
        if(index == -1)
            std::cout << std::endl;
    }

    bool ImageMatcher::LoadImage(size_t index)
    {
        const String & path = _imageInfos[index].path;
        View image;
        if (!image.Load(path, View::Gray8))
        {
            CPL_LOG_SS(Warning, "Can't load '" << path << "' image!");
            return true;
        }

        return true;
    }
}

