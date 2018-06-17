#-------------------------------------------------
# GIEPY main sources
#-------------------------------------------------

# Compile flags
if(MSVC)
	set(GIEPY_FLAGS "")
else(MSVC)
	set(GIEPY_FLAGS "-fno-strict-aliasing")
	# encode flags
	add_flags(GIEPY_FLAGS "-fexec-charset=cp932")
	# Warning flags
	add_flags(GIEPY_FLAGS "-Wall -Wno-format -Wconversion")
	# Test flags
	if(GIEPY_EXECUTE_TEST)
		add_flags(GIEPY_FLAGS "-include CppUTest/MemoryLeakDetectorNewMacros.h")
		add_flags(GIEPY_FLAGS "-include CppUTest/MemoryLeakDetectorMallocMacros.h")
		if(GIEPY_COVERAGE)
			add_flags(GIEPY_FLAGS "--coverage")
		endif(GIEPY_COVERAGE)
	endif(GIEPY_EXECUTE_TEST)
endif(MSVC)


link_directories(
        "${CMAKE_CURRENT_BINARY_DIR}/components"
)




# ---------Build Mewthree module
file(GLOB Mewthree_Sources
	"${CMAKE_CURRENT_SOURCE_DIR}/common/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/mewthree/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/smw/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/dll/asardll.c"
	)
if(MINGW OR MSVC)
	set(Mewthree_Res "${CMAKE_CURRENT_SOURCE_DIR}/res/mewthree.rc")
endif(MINGW OR MSVC)
add_library(mewthree SHARED ${Mewthree_Sources} ${Mewthree_Res})
set_target_properties(mewthree PROPERTIES COMPILE_FLAGS "${GIEPY_FLAGS}")
set_target_properties(mewthree PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/components"
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/components"
        )
if(MINGW OR WIN32)
        set_target_properties(mewthree PROPERTIES PREFIX "")
else(MINGW OR WIN32)
	target_link_libraries(mewthree dl asar)
	if(GIEPY_EXECUTE_TEST)
		target_link_libraries(mewthree CppUTest)
		if(GIEPY_COVERAGE)
			target_link_libraries(mewthree gcov)
		endif(GIEPY_COVERAGE)
	endif(GIEPY_EXECUTE_TEST)
endif(MINGW OR WIN32)
target_link_libraries(mewthree bolib2 jansson)
if(MSVC)
	set_target_properties(mewthree PROPERTIES LINK_FLAGS "/MANIFEST:NO ${WIN2000OBJ}")
endif(MSVC)




# ---------Build CUI Application

file(GLOB CUI_Sources
        "${CMAKE_CURRENT_SOURCE_DIR}/common/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/cui/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/dll/*.c"
        )
if(MINGW OR MSVC)
	set(CUI_Resource "${CMAKE_CURRENT_SOURCE_DIR}/res/giepy.rc")
endif(MINGW OR MSVC)
add_executable(giepy ${CUI_Sources} ${CUI_Resource})
set_target_properties(giepy PROPERTIES
	COMPILE_FLAGS "${GIEPY_FLAGS}"
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
	)
if(UNIX OR APPLE)
        target_link_libraries(giepy mewthree)
	if(GIEPY_COVERAGE)
		target_link_libraries(giepy gcov)
	endif(GIEPY_COVERAGE)
endif(UNIX OR APPLE)
if(MSVC)
	set_target_properties(giepy PROPERTIES LINK_FLAGS "/MANIFEST:NO ${WIN2000OBJ}")
endif(MSVC)
target_link_libraries(giepy bolib2)




# ---------Build GUI Application

if(MSVC OR MINGW)
	file(GLOB GUI_Sources
		"${CMAKE_CURRENT_SOURCE_DIR}/common/*.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/win32/*.c"
		"${CMAKE_CURRENT_SOURCE_DIR}/dll/*.c"
		)
	set(GUI_Resource "${CMAKE_CURRENT_SOURCE_DIR}/res/piee.rc")
	add_executable(piee ${GUI_Sources} ${GUI_Resource})
	set_target_properties(piee PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
	target_link_libraries(piee comctl32 shell32 bolib2)
	if(MINGW)
		# Windows application
		set_target_properties(piee PROPERTIES COMPILE_FLAGS "${GIEPY_FLAGS} -DGUI -DWINVER=0x0500")
		set_target_properties(piee PROPERTIES LINK_FLAGS "-mwindows")
	elseif(MSVC)
		set_target_properties(piee PROPERTIES COMPILE_FLAGS "${GIEPY_FLAGS} /DGUI /DWINVER=0x0500")
		set_target_properties(piee PROPERTIES LINK_FLAGS "/MANIFEST:NO ${WIN2000OBJ}")
	endif()
	if(GIEPY_COVERAGE)
		target_link_libraries(piee gcov)
	endif(GIEPY_COVERAGE)
endif(MSVC OR MINGW)

