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

#include "Cpl/String.h"
#include "Cpl/Args.h"
#include "Cpl/Log.h"
#include "Cpl/Performance.h"
#include "Cpl/File.h"

#include "Simd/SimdView.hpp"
#include "Simd/SimdImageMatcher.hpp"

#include <filesystem>

#ifdef LoadImage
#undef LoadImage
#endif

namespace Adx
{
    typedef std::filesystem::file_time_type FileTime;
    typedef std::filesystem::path Path;
    typedef std::filesystem::directory_entry DirEntry;

    typedef Cpl::Int Int;
    typedef Cpl::String String;
    typedef Cpl::Strings Strings;

    typedef Simd::View<Simd::Allocator> View;
    typedef std::vector<uint8_t> Buffer;

    inline void Sleep(unsigned int miliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
    }

    inline size_t DivHi(size_t value, size_t divider)
    {
        return (value + divider - 1) / divider;
    }

    inline size_t ValidThreadNumber(size_t threads, size_t size)
    {
        threads = std::min(threads, size);
        size_t block = DivHi(size, threads);
        return std::min(threads, DivHi(size, block));
    }
}

