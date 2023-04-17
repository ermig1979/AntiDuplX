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
        for (size_t d = 0; d < _options.imageDirectories.size(); ++d)
        {
            if (!Find(_options.imageDirectories[d]))
                return false;
        }
        return true;
    }

    CPL_INLINE bool IsWanted(const fs::path & path, const Strings & extensions)
    {
        String ext = Cpl::ToLowerCase(path.extension().string());
        for (size_t i = 0; i < extensions.size(); ++i)
            if (ext == extensions[i])
                return true;
        return false;
    }

    bool ImageFinder::Find(const String& path)
    {
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

        if (_options.subDirectories)
        {
            for (fs::recursive_directory_iterator it(path); it != fs::recursive_directory_iterator(); ++it)
            {
                if (it->is_regular_file() && IsWanted(it->path(), _options.imageExtensions))
                {
                    ImageInfo imageInfo;
                    imageInfo.path = it->path().string();
                    imageInfo.size = it->file_size();
                    _imageInfos.push_back(imageInfo);
                }
            }
        }
        else
        {
            for (fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
            {
                if (it->is_regular_file() && IsWanted(it->path(), _options.imageExtensions))
                {
                    ImageInfo imageInfo;
                    imageInfo.path = it->path().string();
                    imageInfo.size = it->file_size();
                    _imageInfos.push_back(imageInfo);
                }
            }
        }

        return true;
    }
}

