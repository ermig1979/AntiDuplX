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

    void ImageLoader::SetProgress(size_t index)
    {
        double progress = double(std::min(index, _imageInfos.size())) / double(_imageInfos.size());
        if (progress >= _progress + 0.001 || progress == 1.0)
        {
            _progress = progress;
            std::cout << "\rLoad progress: " << std::fixed << std::setprecision(1) << _progress * 100.0 << "%" << std::flush;
            if (index == -1)
                std::cout << std::endl;
        }
    }

    bool ImageLoader::LoadImage(size_t index)
    {
        CPL_PERF_FUNC();

        if (!LoadFile(index))
            return false;

        if (!DecodeImage(index))
        {
            CPL_LOG_SS(Verbose, "Can't decode '" << _imageInfos[index].path << "' image!");
            return true;
        }

        if (!CreateHash(index))
            return false;

        return true;
    }

    bool ImageLoader::LoadFile(size_t index)
    {
        CPL_PERF_FUNC();

        const ImageInfo& info = _imageInfos[index];
        return Cpl::LoadBinaryData(info.path, _buffer);
    }

    bool ImageLoader::DecodeImage(size_t index)
    {
        CPL_PERF_FUNC();

        const ImageInfo& info = _imageInfos[index];
        if (_turboJpegDecoder.Enable() && info.format == SimdImageFileJpeg)
            return _turboJpegDecoder.Decode(_buffer, _image);
        else
            return _image.Load(_buffer.data(), _buffer.size());
    }

    bool ImageLoader::CreateHash(size_t index)
    {
        CPL_PERF_FUNC();

        ImageInfo& info = _imageInfos[index];
        info.width = _image.width;
        info.height = _image.height;
        info.hash = _matcher.Create(_image, index);

        return true;
    }
}

