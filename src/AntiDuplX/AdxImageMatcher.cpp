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

    ImageMatcher::ImageMatcher(const Options& options, ImageInfos& imageInfos)
        : _options(options)
        , _imageInfos(imageInfos)
        , _progress(-1.0)
    {
        _pmLoad = Cpl::PerformanceStorage::Global().Get("Image Load");
        _pmMatch = Cpl::PerformanceStorage::Global().Get("Image Match");
    }

    bool ImageMatcher::Run()
    {
        CPL_PERF_FUNC();

        CPL_LOG_SS(Info, "Match images: ");

        _matcher.Init(_options.threshold, Matcher::Hash16x16, _imageInfos.size(), true);

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
        if (progress >= _progress + 0.001 || progress == 1.0)
        {
            _progress = progress;
            std::cout << "\rMatch progress: " << std::fixed << std::setprecision(1) << _progress * 100.0 << "%" << std::flush;
            if (index == -1)
                std::cout << std::endl;
        }
    }

    bool ImageMatcher::LoadImage(size_t index)
    {
        ImageInfo & info = _imageInfos[index];
        const String & path = info.path;
        View image;
        {
            Cpl::PerformanceHolder pmhLoad(_pmLoad);
            if (!image.Load(path, View::Gray8))
            {
                //CPL_LOG_SS(Warning, "Can't load '" << path << "' image!");
                return true;
            }
        }
        info.width = image.width;
        info.height = image.height;

        Cpl::PerformanceHolder pmhMatch(_pmMatch);
        Matcher::HashPtr hash = _matcher.Create(image, index);
        Matcher::Results results;
        if (_matcher.Find(hash, results))
        {
            size_t best = 0;
            for (size_t i = 1; i < results.size(); ++i)
            {
                if (results[i].difference < results[best].difference)
                    best = i;
            }
            int compare = Compare(info, _imageInfos[results[best].hash->tag]);
            if (compare <= 0)
            {
                info.remove = true;
                return true;
            }
            else
            {
                _matcher.Skip(results[best].hash);
                _imageInfos[results[best].hash->tag].remove = true;
            }
        }
        _matcher.Add(hash);

        return true;
    }

    int ImageMatcher::Compare(const ImageInfo& a, const ImageInfo& b) const
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
}

