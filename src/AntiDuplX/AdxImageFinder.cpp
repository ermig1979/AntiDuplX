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
#include "AntiDuplX/AdxImageFinder.h"

#include <filesystem>

namespace Adx
{
    namespace fs = std::filesystem;

    bool ImageFinder::Run()
    {
        CPL_PERF_FUNC();

        _begin = 0, _previous = 0;
        for (size_t d = 0; d < _options.imageDirectories.size(); ++d)
        {
            if (!Find(Path(_options.imageDirectories[d])))
                return false;
        }
        return true;
    }

    bool ImageFinder::Find(const Path& path)
    {
        CPL_LOG_SS(Info, "Search images in '" << path.string() << "':");

        fs::file_status status = fs::status(path);
        if (!fs::exists(status))
        {
            CPL_LOG_SS(Error, "Path '" << path << "' is not exist!");
            return false;
        }
        if (!fs::is_directory(status))
        {
            CPL_LOG_SS(Error, "Path '" << path << "' is not directory!");
            return false;
        }

        size_t _begin = _imageInfos.size();
        if (_options.subDirectories)
        {
            for (fs::recursive_directory_iterator it(path); it != fs::recursive_directory_iterator(); ++it)
            {
                if (it->is_regular_file() && IsWanted(it->path()))
                    Push(*it);
            }
        }
        else
        {
            for (fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
            {
                if (it->is_regular_file() && IsWanted(it->path()))
                    Push(*it);
            }
        }
        Annotate(true);

        return true;
    }

    bool ImageFinder::IsWanted(const Path & path)
    {
        String ext = Cpl::ToLowerCase(path.extension().string());
        for (size_t i = 0; i < _options.imageExtensions.size(); ++i)
            if (ext == _options.imageExtensions[i])
                return true;
        return false;
    }

    void ImageFinder::Push(const DirEntry& entry)
    {
        ImageInfo * info = new ImageInfo();
        info->path = entry.path().string();
        info->size = entry.file_size();
        info->time = entry.last_write_time();
        String ext = Cpl::ToLowerCase(entry.path().extension().string());
        if (ext == ".png")
            info->format = SimdImageFilePng;
        else if (ext == ".jpg")
            info->format = SimdImageFileJpeg;
        else
            info->format = SimdImageFileUndefined;
        _imageInfos.push_back(info);
        Annotate(false);
    }

    void ImageFinder::Annotate(bool last)
    {
        if (last || _imageInfos.size() >= _previous + 1000)
        {
            _previous = _imageInfos.size();
            std::cout << _previous - _begin << "\r" << std::flush;
        }
        if(last)
            std::cout << std::endl << std::flush;
    }
}

