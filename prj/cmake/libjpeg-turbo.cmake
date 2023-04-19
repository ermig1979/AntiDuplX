cmake_minimum_required(VERSION 3.7)

set(TURBOJPEG_DIR ${ROOT_DIR}/3rd/libjpeg-turbo) 
set(TURBOJPEG_BIN ${CMAKE_BINARY_DIR}/3rd/libjpeg-turbo) 
set(TURBOJPEG_LIB ${TURBOJPEG_BIN}/libturbojpeg.a)

include_directories(${TURBOJPEG_DIR})
 
set(TURBOJPEG_BUILD_OPTIONS
	-DCMAKE_BUILD_TYPE="Release"
	-DREQUIRE_SIMD=ON)

file(MAKE_DIRECTORY ${TURBOJPEG_BIN})	
add_custom_command(
	OUTPUT ${TURBOJPEG_LIB}
	COMMAND cmake ${TURBOJPEG_DIR} ${TURBOJPEG_BUILD_OPTIONS} && make -j$(nproc)
	POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${TURBOJPEG_LIB} ${CMAKE_BINARY_DIR} 
	WORKING_DIRECTORY ${TURBOJPEG_BIN})
	
add_custom_target(make_turbojpeg DEPENDS ${TURBOJPEG_LIB})

