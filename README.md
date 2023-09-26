AntiDuplX
=========
AntiDuplX - cross platform command line tool to find and remove similar pictures.

Building of AntiDuplX for Linux
===============================

On Ubuntu 22.04.2 LTS there is following way to build AntiDuplX:

	sudo apt-get update
	sudo apt-get install git cmake g++ nasm
	git clone -b master --recurse-submodules -v https://github.com/ermig1979/AntiDuplX.git AntiDuplX
	cd AntiDuplX
	mkdir build
	cmake ./prj/cmake -B ./build -DCMAKE_BUILD_TYPE=Release
	cmake --build ./build --config Release --parallel$(nproc)

There are addition build parameters:

* `ADX_INFO` - Print build information. It is switched on by default.
* `ADX_GET_VERSION` - Call scipt to get AntiDuplX version. It is switched on by default.
* `ADX_USE_SYSTEM_TURBOJPEG` - Use system libjpeg-turbo. It is switched off by default.

Building of AntiDuplX for Windows
=================================

To build the tool for Windows 32/64 you need to use Microsoft Visual Studio 2022. 
There are project files are in the directory: 

`./prj/vs2022/`

Using of AntiDuplX
==================

AntiDuplX using example:

	./AntiDuplX -id=./pict_dir_1 -id=./pict_dir_2 -ct=0.05 -of=./dupl_list.txt

Where following parameters are used:

* `-id=./pict_dir_1` or `--imageDirectories=./pict_dir_1` - a path to directory with images. You can set several directories.
* `-ct=0.05` or `--compareThreshold=0.05` - an image compare threshold. Standard deviation threshold for duplicates. By default it is equal to 0.05.
* `-of=./dupl_list.txt` or `--outFile=./dupl_list.txt` - a file name to save list with found duplicated and damaged images. A path to image with poor quality is printed at the beginning of line.

Also you can use parameters:
* `--help` or `-h` - to print this help message.
* `--version` or `-v` - to print AntiDuplX version.
* `--imageExtensions=.jpg` or `-ie=.jpg` - an image file extensions to search. You can set several extensions. By default this parameter is equal to (.jpg, .png).
* `--subDirectories=1` or `-sd=1` - to search images in sub-directories. By default this parameter is turned on.
* `--logLevel=3` or `-ll=3` - a log level. It can be: 0 - None, 1 - Error, 2 - Warning, 3 - Info, 4 - Verbose, 5 - Debug. By default this parameter is equal to 3 (Info).
* `--performanceReport=1` or `-pr=1` - a flag to print performance report. By default this parameter is turned off.
* `--compareSize=32x32` or `-cs=32x32` - an image compare size. It can be 16x16, 32x32 or 64x64. By default this parameter is equal to 32x32.
* `--threadNumber=4` or `-tn=4` - a number of work threads to load and compare images. By default this parameter is equal to -1 (use of all available threads).
* `--deleteDupls=1` or `-dd=1` - a flag to auto delete found image duplicates. By default this parameter is turned off.
* `--deleteBads=1` or `-db=1` - a flag to auto delete found bad (damaged) images. By default this parameter is turned off.
