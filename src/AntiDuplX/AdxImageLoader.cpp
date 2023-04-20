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

#include "AntiDuplX/AdxImageLoader.h"

namespace Adx
{

    ImageLoader::ImageLoader(const Options& options, ImageInfos& imageInfos)
        : _options(options)
        , _imageInfos(imageInfos)
        , _progress(-1.0)
    {
    }

    bool ImageLoader::Run()
    {
        CPL_PERF_FUNC();

        CPL_LOG_SS(Info, "Load images: ");

        _matcher.Init(_options.compareThreshold, Matcher::Hash16x16, _imageInfos.size(), true);

        SetProgress();
        if (_options.loadThreads > 1)
        {
            _context.resize(ValidThreadNumber(_options.loadThreads, _imageInfos.size()));
            for (size_t t = 0, block = DivHi(_imageInfos.size(), _context.size()); t < _context.size(); ++t)
            {
                size_t begin = t * block;
                size_t end = std::min(_imageInfos.size(), begin + block);
                _context[t].thread = std::thread(&ImageLoader::LoadThread, this, t, begin, end);
            }

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
            LoadThread(0, 0, _imageInfos.size());
        }
        SetProgress();

        return true;
    }

    void ImageLoader::LoadThread(size_t thread, size_t begin, size_t end)
    {
        Context& context = _context[thread];
        for (context.index = 0; context.index + begin < end; context.index++)
        {
            if (_context.size() == 1)
                SetProgress();
            size_t index = context.index + begin;
            ImageInfo& info = *_imageInfos[index];
            if (!LoadFile(context, info))
                continue;
            if (!DecodeImage(context, info))
                continue;
            CreateHash(context, info);
        }
    }

    void ImageLoader::SetProgress()
    {
        size_t processed = Processed();
        double progress = double(std::min(processed, _imageInfos.size())) / double(_imageInfos.size());
        if (progress >= _progress + 0.001 || progress == 1.0)
        {
            _progress = progress;
            std::cout << "\rLoad progress: " << std::fixed << std::setprecision(1) << _progress * 100.0 << "%" << std::flush;
            if (processed == _imageInfos.size())
                std::cout << std::endl;
        }
    }

    bool ImageLoader::LoadFile(Context& context, ImageInfo& info)
    {
        //CPL_PERF_FUNC();
        bool result = Cpl::LoadBinaryData(info.path, context.buffer);
        if (!result)
        {
            info.error = ImageLoadError;
            CPL_LOG_SS(Verbose, "Can't load '" << info.path << "' image!");
        }
        return result;
    }

    bool ImageLoader::DecodeImage(Context& context, ImageInfo& info)
    {
        //CPL_PERF_FUNC();
        bool result = false;
        if (context.decoder.Enable() && info.format == SimdImageFileJpeg)
            result = context.decoder.Decode(context.buffer, context.image);
        else
            result = context.image.Load(context.buffer.data(), context.buffer.size());
        if (!result)
        {
            info.error = ImageDecodeError;
            CPL_LOG_SS(Verbose, "Can't decode '" << info.path << "' image!");
        }
        return result;
    }

    void ImageLoader::CreateHash(Context& context, ImageInfo& info)
    {
        //CPL_PERF_FUNC();
        info.width = context.image.width;
        info.height = context.image.height;
        info.hash = _matcher.Create(context.image, &info);
    }

    size_t ImageLoader::Processed() const
    {
        size_t processed = 0;
        for (size_t t = 0; t < _context.size(); ++t)
            processed += _context[t].index;
        return processed;
    }
}

