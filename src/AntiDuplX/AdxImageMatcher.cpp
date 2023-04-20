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
    }

    bool ImageMatcher::Run()
    {
        CPL_PERF_FUNC();

        CPL_LOG_SS(Info, "Match images: ");

        SetProgress();        
        
        std::sort(_imageInfos.begin(), _imageInfos.end(), Lesser);

        if (_options.matchThreads > 1)
        {
            _context.resize(ValidThreadNumber(_options.matchThreads, _imageInfos.size()));
            for (size_t t = 0; t < _context.size(); ++t)
                _context[t].thread = std::thread(&ImageMatcher::MatchThread, this, t);

            do
            {
                SetProgress();
                Sleep(40);
            } while (Processed() < _imageInfos.size());

            for (size_t t = 0; t < _context.size(); ++t)
                if (_context[t].thread.joinable())
                    _context[t].thread.join();
        }
        else
        {
            _context.resize(1);
            MatchThread(0);
        }
        SetProgress();

        return true;
    }

    void ImageMatcher::MatchThread(size_t thread)
    {
        Context& context = _context[thread];
        context.matcher.Init(_options.compareThreshold, Matcher::Hash16x16, std::max<size_t>(_imageInfos.size() / _context.size(), 1), true);
        for (context.index = 0; context.index < _imageInfos.size(); context.index++)
        {
            if (_context.size() == 1)
                SetProgress();
            ImageInfo& info = *_imageInfos[context.index];
            if (!info.hash)
                continue;
            Matcher::Results results;
            if (context.matcher.Find(info.hash, results))
            {
				for (size_t r = 0; r < results.size(); ++r)
				    results[r].hash->tag->remove = true;
            }
            if(context.index % _context.size() == thread)
                context.matcher.Add(info.hash);
        }
    }

    void ImageMatcher::SetProgress()
    {
        size_t processed = Processed();
        double progress = double(std::min(processed, _imageInfos.size())) / double(_imageInfos.size());
        if (progress >= _progress + 0.001 || progress == 1.0)
        {
            _progress = progress;
            std::cout << "\rMatch progress: " << std::fixed << std::setprecision(1) << _progress * 100.0 << "%" << std::flush;
            if (processed == _imageInfos.size())
                std::cout << std::endl;
        }
    }

    size_t ImageMatcher::Processed() const
    {
        size_t processed = 0;
        for (size_t t = 0; t < _context.size(); ++t)
            processed += _context[t].index;
        return processed / std::max<size_t>(1, _context.size());
    }
}

