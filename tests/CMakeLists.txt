#-------------------------------------------------
# GIEPY test
#-------------------------------------------------

set(CPPUTEST_LIB CppUTest)

# Compile flags
set(GIEPY_TEST_FLAGS "-Wall -std=c++11 -Wpedantic")

if(EXTENSIONS)
	list(APPEND CPPUTEST_LIB CppUTestExt)
endif(EXTENSIONS)

file(GLOB_RECURSE TESTSOURCES
	"."  "*.cpp"
)

add_executable(mewthree_test ${TESTSOURCES})
target_link_libraries(mewthree_test mewthree ${CPPUTEST_LIB})

if(NOT MSVC)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		if(GIEPY_EXECUTE_TEST)
			add_flags(GIEPY_TEST_FLAGS "-include CppUTest/MemoryLeakDetectorNewMacros.h")
			add_flags(GIEPY_TEST_FLAGS "-include CppUTest/MemoryLeakDetectorMallocMacros.h")
		endif()
	endif()
endif(NOT MSVC)
set_target_properties(mewthree_test PROPERTIES COMPILE_FLAGS ${GIEPY_TEST_FLAGS})


