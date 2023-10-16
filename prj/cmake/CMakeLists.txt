cmake_minimum_required(VERSION 3.7)
project(AntiDuplX)

option(ADX_INFO "Print build information" ON)
option(ADX_GET_VERSION "Get AntiDuplX version" ON)
option(ADX_USE_SYSTEM_TURBOJPEG "Use system libjpeg-turbo" OFF)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

include_directories(${ROOT_DIR}/src)

include_directories(${ROOT_DIR}/3rd/Cpl/src)

include(${ROOT_DIR}/prj/cmake/Simd.cmake)

if(ADX_USE_SYSTEM_TURBOJPEG)
	find_package(libjpeg-turbo REQUIRED)
else()
	include(${ROOT_DIR}/prj/cmake/libjpeg-turbo.cmake)
endif()

if(ADX_GET_VERSION)
	if (WIN32 AND NOT (CYGWIN OR MSYS))
		string(REPLACE "/" "\\" ROOT_DIR_WIN ${ROOT_DIR})
		execute_process(COMMAND "${ROOT_DIR}/prj/cmd/GetVersion.cmd" "${ROOT_DIR_WIN}" "${ADX_INFO}")
	else()
		execute_process(COMMAND bash "${ROOT_DIR}/prj/sh/GetVersion.sh" "${ROOT_DIR}" "${ADX_INFO}")
	endif()
else()
	add_definitions(-DADX_VERSION="unknown")
endif()

file(GLOB_RECURSE ADX_SRC ${ROOT_DIR}/src/AntiDuplX/*.cpp)
add_executable(AntiDuplX ${ADX_SRC})

if(ADX_USE_SYSTEM_TURBOJPEG)
	target_link_libraries(AntiDuplX ${SIMD_LIB} libjpeg-turbo::turbojpeg -ldl -lpthread -lstdc++)
else()
	target_link_libraries(AntiDuplX ${SIMD_LIB} ${TURBOJPEG_LIB} -ldl -lpthread -lstdc++)
	add_dependencies(AntiDuplX make_turbojpeg)
endif()

install(TARGETS AntiDuplX DESTINATION ${CMAKE_INSTALL_BINDIR})

add_custom_target("uninstall")
add_custom_command(TARGET "uninstall" POST_BUILD COMMENT "Uninstall AntiDuplX..." COMMAND cat install_manifest.txt | sudo xargs rm -v)
