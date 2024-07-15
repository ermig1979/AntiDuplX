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
#include "AntiDuplX/AdxResultHandler.h"

#include <filesystem>

namespace Adx
{
    namespace fs = std::filesystem;

    bool ResultHandler::Run()
    {
        CPL_PERF_FUNC();

        if (!SaveOutFile())
            return false;

        return true;
    }

    bool ResultHandler::SaveOutFile()
    {
        std::string delimiter = _options.delimiter;

        if (_options.outFile.empty())
            return true;
        fs::path path(_options.outFile);
        if (path.has_parent_path())
        {
            fs::path parent = path.parent_path();
            fs::file_status status = fs::status(parent);
            if (!fs::exists(status))
            {
                if (!fs::create_directories(parent))
                {
                    CPL_LOG_SS(Error, "Can't create output directory '" << parent.string() << "'!");
                    return false;
                }
            }
        }
        std::ofstream ofs(_options.outFile.c_str());
        if (!ofs.is_open())
        {
            CPL_LOG_SS(Error, "Can't create output file '" << path.string() << "'!");
            return false;
        }
        size_t bads = 0, dupls = 0;
        for (size_t i = 0; i < _imageInfos.size(); ++i)
        {
            const ImageInfo& info = *_imageInfos[i];
            if (info.error)
            {
                ofs << info.path << delimiter;
                ofs << info.width << "x" << info.height << delimiter;
                ofs << info.size / 1024 << "kb";
                ofs << std::endl;
                if (_options.deleteBads)
                {
                    if (!fs::remove(info.path))
                    {
                        CPL_LOG_SS(Warning, "Can't delete file '" << path.string() << "'!");
                    }
                }
                bads++;
            }
            if (info.duplicate)
            {
                ofs << info.path << delimiter;
                ofs << info.width << "x" << info.height << delimiter;
                ofs << info.size / 1024 << "kb" << delimiter;
                ofs << info.difference << delimiter;
                ofs << info.duplicate->path << delimiter;
                ofs << info.duplicate->width << "x" << info.duplicate->height << delimiter;
                ofs << info.duplicate->size / 1024 << "kb";
                ofs << std::endl;
                if (_options.deleteDupls)
                {
                    if (!fs::remove(info.path))
                    {
                        CPL_LOG_SS(Warning, "Can't delete file '" << path.string() << "'!");
                    }
                }
                dupls++;
            }
        }
        ofs.close();
        CPL_LOG_SS(Info, bads << " bads and " << dupls << " duplicates were found in " << _imageInfos.size() << " images.");
        return true;
    }
}
