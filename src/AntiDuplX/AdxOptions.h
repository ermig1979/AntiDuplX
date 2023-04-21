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
        bool performanceReport;
        double compareThreshold;
        size_t threadNumber;
        String outFile;
        bool deleteDupls;
        bool deleteBads;

        Options(int argc, char* argv[])
            : ArgsParser(argc, argv, true)
        {
            if (HasArg("-h", "--help"))
            {
                PrintHelp();
                exit(0);
            }
            imageDirectories = GetArgs(Strings({ "-id", "--imageDirectories" }), Strings({"."}));
            imageExtensions = GetArgs(Strings({ "-ie", "--imageExtensions" }), Strings( { ".jpg", ".png"}));
            subDirectories = Cpl::ToVal<bool>(GetArg2("-sd", "--subDirectory", "1"));
            logLevel = (Cpl::Log::Level)Cpl::ToVal<Int>(GetArg2("-ll", "--logLevel", "3", false));
            performanceReport = Cpl::ToVal<bool>(GetArg2("-pr", "--performanceReport", "1"));
            compareThreshold = Cpl::ToVal<double>(GetArg2("-ct", "--compareThreshold", "0.05", false));
            threadNumber = std::min<size_t>(std::max<size_t>(Cpl::ToVal<size_t>(GetArg2("-lt", "--threadNumber", "-1", false)), 1), std::thread::hardware_concurrency());
            outFile = GetArg2("-of", "--outFile", "out.txt", false);
            deleteDupls = Cpl::ToVal<bool>(GetArg2("-dd", "--deleteDupls", "0"));
            deleteBads = Cpl::ToVal<bool>(GetArg2("-db", "--deleteBads", "0"));
        }

        ~Options()
        {
        }

        void PrintHelp()
        {
            std::cout << "AntiDuplX is a command line tool to search of simular images." << std::endl << std::endl;
            std::cout << "Using example:" << std::endl << std::endl;
            std::cout << "  ./AntiDuplX -id=./pict_dir_1 -ct=0.05 -id=./pict_dir_2 -of=./dupl_list.txt" << std::endl << std::endl;
            std::cout << "Where following parameters are used:" << std::endl << std::endl;
            std::cout << "  -id=./pict_dir_1       - a path to directory with images. " << std::endl;
            std::cout << "                           You can set several directories." << std::endl << std::endl;
            std::cout << "  -ct=0.05               - an image compare threshold." << std::endl;
            std::cout << "                           Standard deviation threshold for duplictes." << std::endl;
            std::cout << "                           By default it is equal to 0.05." << std::endl << std::endl;
            std::cout << "  -of=./dupl_list.txt or - a file name to save list with found duplicated and damaged images." << std::endl << std::endl;
        }
    };
}

