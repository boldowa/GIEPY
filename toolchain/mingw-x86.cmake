#-----------------------------------------------------------
# mingw toolchain
#-----------------------------------------------------------

set(MINGW ON)
set(CMAKE_SYSTEM_NAME Windows)
set(TOOL_PREFIX "i686-w64-mingw32-")
if(CLANG)
        set(CMAKE_C_COMPILER "${TOOL_PREFIX}clang")
        set(CMAKE_CXX_COMPILER "${TOOL_PREFIX}clang++")
else()
        set(CMAKE_C_COMPILER "${TOOL_PREFIX}gcc")
        set(CMAKE_CXX_COMPILER "${TOOL_PREFIX}g++")
endif()
set(CMAKE_RC_COMPILER ${TOOL_PREFIX}windres)
set(CMAKE_RC_COMPILE_OBJECT 
	"<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCES> -o <OBJECT>")
#set(CMAKE_LINKER "${TOOL_PREFIX}g++")
#set(CMAKE_AR "${TOOL_PREFIX}ar")

set(CMAKE_FIND_ROOT_PATH "/usr/i686-w64-mingw32/")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

