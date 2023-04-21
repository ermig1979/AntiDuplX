# AntiDuplX
AntiDuplX - cross platform command line tool to find and remove similar pictures.

Building of AntiDuplX for Linux
==============================

On Ubuntu-2022.04.2 TLS there is following way to build AntiDuplX:

	sudo apt-get update
	sudo apt-get install git cmake g++ nasm
	git clone -b master --recurse-submodules -v https://github.com/ermig1979/AntiDuplX.git AntiDuplX
	cd AntiDuplX
	mkdir build
	cmake ./prj/cmake -B ./build -DCMAKE_BUILD_TYPE=Release
	cmake --build ./build --config Release --parallel$(nproc)

