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

#include "Cpl/Args.h"
#include "Cpl/Log.h"

namespace Adx
{
    struct Options : public Cpl::ArgsParser
    {
        Strings imageDirectories;
        Strings imageExtensions;
        bool subDirectories;
        Cpl::Log::Level logLevel;
        String logFile;
        double compareThreshold;
        size_t loadThreads;
        size_t matchThreads;
        String duplStrategy;
        bool performanceReport;

        Options(int argc, char* argv[])
            : ArgsParser(argc, argv, true)
        {
            imageDirectories = GetArgs("-id", Strings({ "." }));
            imageExtensions = GetArgs("-ie", Strings( { ".jpg", ".png"}));
            subDirectories = Cpl::ToVal<bool>(GetArg("-sd", "1"));
            logLevel = (Cpl::Log::Level)Cpl::ToVal<Int>(GetArg2("-ll", "--logLevel", "3", false));
            logFile = GetArg2("-lf", "--logFile", "", false);
            compareThreshold = Cpl::ToVal<double>(GetArg2("-ct", "--compareThreshold", "0.05", false));
            loadThreads = std::min<size_t>(std::max<size_t>(Cpl::ToVal<size_t>(GetArg2("-lt", "--loadThreads", "1", false)), 1), std::thread::hardware_concurrency());
            matchThreads = std::min<size_t>(std::max<size_t>(Cpl::ToVal<size_t>(GetArg2("-mt", "--matchThreads", "1", false)), 1), std::thread::hardware_concurrency());
            duplStrategy = GetArg2("-ds", "--duplStrategy", "info", false, Strings({ "info", "delete" }));
            performanceReport = Cpl::ToVal<bool>(GetArg2("-pr", "--performanceReport", "1"));
        }

        ~Options()
        {
        }
    };
}

