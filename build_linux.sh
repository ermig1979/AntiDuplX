ECHO="0"
HT="1"
BUILD_DIR=build

if [ ${ECHO} == "1" ]; then echo "Build AntiDuplX in '${BUILD_DIR}':"; fi

if [ ! -d $BUILD_DIR ]; then mkdir $BUILD_DIR; fi

cmake ./prj/cmake -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ] ; then 	exit; fi

if [ ${HT} == "1" ]; then THREADS="$(nproc)"; else THREADS="$(grep "^core id" /proc/cpuinfo | sort -u | wc -l)"; fi

cmake --build ./build --config Release --parallel$THREADS
if [ $? -ne 0 ] ; then 	exit; fi
