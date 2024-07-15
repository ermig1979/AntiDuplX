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
#include "AntiDuplX/AdxImageInfo.h"

#if !defined(ADX_VERSION)
#include "AntiDuplX/AdxVersion.h"
#endif

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
        Matcher::HashType compareSize;
        size_t threadNumber;
        String outFile;
        String delimiter;
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
            if (HasArg("-v", "--version"))
            {
                PrintVersion();
                exit(0);
            }
            imageDirectories = GetArgs(Strings({ "-id", "--imageDirectories" }), Strings({"."}));
            imageExtensions = GetArgs(Strings({ "-ie", "--imageExtensions" }), Strings( { ".jpg", ".png", ".jpeg"}));
            subDirectories = Cpl::ToVal<bool>(GetArg2("-sd", "--subDirectory", "1"));
            logLevel = (Cpl::Log::Level)Cpl::ToVal<Int>(GetArg2("-ll", "--logLevel", "3", false));
            performanceReport = Cpl::ToVal<bool>(GetArg2("-pr", "--performanceReport", "0"));
            compareThreshold = Cpl::ToVal<double>(GetArg2("-ct", "--compareThreshold", "0.05", false));
            compareSize = ToHashType(GetArg2("-cs", "--compareSize", "32x32", false));
            threadNumber = std::min<size_t>(std::max<size_t>(Cpl::ToVal<size_t>(GetArg2("-tn", "--threadNumber", "-1", false)), 1), std::thread::hardware_concurrency());
            outFile = GetArg2("-of", "--outFile", "out.txt", false);
            delimiter = GetArg2("-dl", "--delimiter", "\t");
            deleteDupls = Cpl::ToVal<bool>(GetArg2("-dd", "--deleteDupls", "0"));
            deleteBads = Cpl::ToVal<bool>(GetArg2("-db", "--deleteBads", "0"));
        }

        ~Options()
        {
        }


    private:
        void PrintHelp()
        {
            std::cout << "AntiDuplX is a command line tool to search of simular images." << std::endl << std::endl;
            std::cout << "Using example:" << std::endl << std::endl;
            std::cout << "  ./AntiDuplX -id=./pict_dir_1 -id=./pict_dir_2 -ct=0.05 -of=./dupl_list.txt" << std::endl << std::endl;
            std::cout << "Where following parameters are used:" << std::endl << std::endl;
            std::cout << "  -id=./pict_dir_1 or --imageDirectories=./pict_dir_1 - a path to directory with images. " << std::endl;
            std::cout << "                                                       You can set several directories." << std::endl << std::endl;
            std::cout << "  -ct=0.05 or --compareThreshold=0.05                - an image compare threshold." << std::endl;
            std::cout << "                                                       Standard deviation threshold for duplicates." << std::endl;
            std::cout << "                                                       By default it is equal to 0.05." << std::endl << std::endl;
            std::cout << "  -of=./dupl_list.txt or --outFile=./dupl_list.txt   - a file name to save list with found duplicated and damaged images." << std::endl;
            std::cout << "                                                       A path to image with poor quality is printed at the beginning of line." << std::endl << std::endl;
            std::cout << "  -dl='\t' or --delimiter='\t'                       - Create separators for fields when outputting files." << std::endl;
            std::cout << "                                                       By default,'\t'is used as a delimiter." << std::endl << std::endl;
            std::cout << "Also you can use parameters: " << std::endl << std::endl;
            std::cout << "  --help or -h                       - to print this help message." << std::endl << std::endl;
            std::cout << "  --version or -v                    - to print AntiDuplX version." << std::endl << std::endl;
            std::cout << "  --imageExtensions=.jpg or -ie=.jpg - an image file extensions to search." << std::endl;
            std::cout << "                                       You can set several extensions." << std::endl;
            std::cout << "                                       By default this parameter is equal to (.jpg, .png)." << std::endl << std::endl;
            std::cout << "  --subDirectories=1 or -sd=1        - to search images in sub-directories." << std::endl;
            std::cout << "                                       By default this parameter is turned on." << std::endl << std::endl;
            std::cout << "  --logLevel=3 or -ll=3              - a log level. It can be: 0 - None, 1 - Error, 2 - Warning, 3 - Info, 4 - Verbose, 5 - Debug." << std::endl;
            std::cout << "                                       By default this parameter is equal to 3 (Info)." << std::endl << std::endl;
            std::cout << "  --performanceReport=1 or -pr=1     - a flag to print performance report." << std::endl;
            std::cout << "                                       By default this parameter is turned off." << std::endl << std::endl;
            std::cout << "  --compareSize=32x32 or -cs=32x32   - an image compare size. It can be 16x16, 32x32 or 64x64." << std::endl;
            std::cout << "                                       By default this parameter is equal to 32x32." << std::endl << std::endl;
            std::cout << "  --threadNumber=4 or -tn=4          - a number of work threads to load and compare images." << std::endl;
            std::cout << "                                       By default this parameter is equal to -1 (use of all available threads)." << std::endl << std::endl;
            std::cout << "  --deleteDupls=1 or -dd=1           - a flag to auto delete found image duplicates." << std::endl;
            std::cout << "                                       By default this parameter is turned off." << std::endl << std::endl;
            std::cout << "  --deleteBads=1 or -db=1            - a flag to auto delete found bad (damaged) images." << std::endl;
            std::cout << "                                       By default this parameter is turned off." << std::endl << std::endl;
        } 

        void PrintVersion()
        {
            std::cout << "AntiDuplX-" << ADX_VERSION << std::endl;
        }

        Matcher::HashType ToHashType(const String& str)
        {
            if (str == "16x16")
                return Matcher::Hash16x16;
            else if (str == "32x32")
                return Matcher::Hash32x32;
            else if (str == "64x64")
                return Matcher::Hash64x64;
            else
            {
                CPL_LOG_SS(Error, "Invalid parameter --compareSize=" << str << "! . It can be 16x16, 32x32 or 64x64.");
                exit(1);
                return Matcher::Hash16x16;
            }
        }
    };
}

