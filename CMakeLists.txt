#-----------------------------------------------------------
# CMakeList for GIEPY
#-----------------------------------------------------------

cmake_minimum_required(VERSION 3.1)

if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "No build type selected, default to Release")
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING 
                "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

set(WIN200OBJ "")
if(MSVC)
	option(UNICODE "Use UNICODE" OFF)
	option(LINK_WIN2000 "Link msvcrt_win2000.obj" OFF)
endif()
if(LINK_WIN2000)
	set(WIN2000OBJ "msvcrt_win2000.obj")
endif()

#--- link directory
link_directories(
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}/components"
        "."
        ".."
)

# library project dir
set(BOLIBDIR "${CMAKE_CURRENT_SOURCE_DIR}/../bolib")
set(JANSSONDIR "${CMAKE_CURRENT_SOURCE_DIR}/../jansson")

### add common defines
macro(add_common_define target def)
	set(${target} "${${target}} ${DEFINE_PREFIX}${def}")
endmacro(add_common_define)

### Visual Studio Solution Name
project(giepy C)

### visual studio directory config
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "giepy")

###### specify C and C++ compile option
### specify unix compile option
if(UNIX OR MINGW)
	## define prefix(gcc)
	set(DEFINE_PREFIX "-D")

	## set C common compile flags
	set(CMAKE_C_FLAGS "-fpic -ansi -Wall -Wno-format -Wconversion")
	#set(CMAKE_C_FLAGS "-fpic -Wall -Wno-format -Wconversion")

	## set C Release compile flags
        set(CMAKE_C_FLAGS_RELEASE "-O2 -ffunction-sections -fdata-sections")

	## set C Debug compile option
	set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")

	## set linker link flag
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s -Wl,--gc-sections")
	set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "-T")
        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-s -Wl,--gc-sections")
endif()

if(MSVC)
	## define prefix(gcc)
	set(DEFINE_PREFIX "/D")

	# todo : define it
endif(MSVC)

### add common define
if(UNIX)
        add_common_define(CMAKE_C_FLAGS UNIX)
endif(UNIX)
if(MINGW OR MSVC)
        add_common_define(CMAKE_C_FLAGS WIN32)
        add_common_define(CMAKE_C_FLAGS _WIN32)
        add_common_define(CMAKE_C_FLAGS Windows)
	if(UNICODE)
		add_common_define(CMAKE_C_FLAGS UNICODE)
		add_common_define(CMAKE_C_FLAGS _UNICODE)
	endif()
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        add_common_define(CMAKE_C_FLAGS Linux)
endif()

add_common_define(CMAKE_C_FLAGS_DEBUG DEBUG)
add_common_define(CMAKE_C_FLAGS_RELEASE RELEASE)
add_common_define(CMAKE_C_FLAGS_RELEASE NDEBUG)

if(MSVC)
	# todo : define it
endif(MSVC)

### build jansson
add_subdirectory(../jansson "${CMAKE_CURRENT_BINARY_DIR}/jansson")
set_target_properties(jansson
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/components"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/components"
        )
if(MINGW OR WIN32)
        set_target_properties(jansson PROPERTIES PREFIX "")
endif()
if(MINGW)
	# static link
	set_target_properties(jansson PROPERTIES COMPILE_FLAGS "-static-libgcc -Wl,-Bstatic,-lpthread")

	# cp932
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexec-charset=cp932")
endif()
if(MSVC)
	set_target_properties(jansson PROPERTIES LINK_FLAGS "/MANIFEST:NO ${WIN2000OBJ}")
endif()


### build bolib
add_subdirectory(../bolib "${CMAKE_CURRENT_BINARY_DIR}/bolib")
if(MSVC)
	set_target_properties(bolib PROPERTIES LINK_FLAGS "/MANIFEST:NO ${WIN2000OBJ}")
endif()


### build componets dll(mewthree)
file(GLOB CORE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/mewthree/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/smw/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/dll/asardll.c"
        )
set(DLL_RESOURCE "")
if(MINGW OR MSVC)
        set(DLL_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/res/mewthree.rc")
endif()
add_library(mewthree SHARED ${CORE_SOURCES} ${DLL_RESOURCE})
set_target_properties(mewthree 
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/components"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/components"
        )
if(MINGW OR WIN32)
        set_target_properties(mewthree PROPERTIES PREFIX "")
endif()
target_include_directories(mewthree PRIVATE
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_BINARY_DIR}/bolib"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/common"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/asar"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/mewthree"
        "${BOLIBDIR}/include"
        "${BOLIBDIR}/include/common"
        "${BOLIBDIR}/include/file"
        "${PROJECT_BINARY_DIR}/jansson/include"
        )
if(UNIX)
        target_link_libraries(mewthree dl asar)
endif(UNIX)
target_link_libraries(mewthree bolib jansson)
#target_link_libraries(mewthree bolib)
if(MSVC)
	set_target_properties(mewthree PROPERTIES LINK_FLAGS "/MANIFEST:NO ${WIN2000OBJ}")
endif()


### build cui app
file(GLOB CUI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cui/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/dll/*.c"
        )
set(CUI_RESOURCE "")
if(MINGW OR MSVC)
	set(CUI_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/res/giepy.rc")
endif()
add_executable(giepy ${CUI_SOURCES} ${CUI_RESOURCE})
target_include_directories(giepy PRIVATE
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_BINARY_DIR}/bolib"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/giepy"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/common"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/asar"
        "${BOLIBDIR}/include"
        "${BOLIBDIR}/include/common"
        "${BOLIBDIR}/include/file"
        )
if(UNIX)
        target_link_libraries(giepy mewthree)
endif(UNIX)
if(MSVC)
	set_target_properties(giepy PROPERTIES LINK_FLAGS "/MANIFEST:NO ${WIN2000OBJ}")
endif()
target_link_libraries(giepy bolib)


### build gui app
if(MINGW OR MSVC)
	file(GLOB GUI_SOURCES
		"${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/win32/*.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/dll/*.c"
		)
	set(GUI_RESOURCE "")
	if(MINGW OR MSVC)
		set(GUI_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/res/piee.rc")
	endif()
	add_executable(piee WIN32 ${GUI_SOURCES} ${GUI_RESOURCE})
	target_include_directories(piee PRIVATE
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_BINARY_DIR}/bolib"
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/include/dll"
		"${CMAKE_CURRENT_SOURCE_DIR}/include/giepy"
		"${CMAKE_CURRENT_SOURCE_DIR}/include/common"
		"${CMAKE_CURRENT_SOURCE_DIR}/include/asar"
		"${BOLIBDIR}/include"
		"${BOLIBDIR}/include/common"
		"${BOLIBDIR}/include/file"
		)
	target_link_libraries(piee comctl32 shell32 bolib)
	if(MINGW)
		# Windows application
		set_target_properties(piee PROPERTIES COMPILE_FLAGS "-DGUI -DWINVER=0x0500")
		set_target_properties(piee PROPERTIES LINK_FLAGS "-mwindows")
	elseif(MSVC)
		set_target_properties(piee PROPERTIES COMPILE_FLAGS "/DGUI /DWINVER=0x0500")
		set_target_properties(piee PROPERTIES LINK_FLAGS "/MANIFEST:NO ${WIN2000OBJ}")
	endif()
endif()

