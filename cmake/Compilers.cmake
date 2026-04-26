include_guard()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

else() # GCC / Clang

	#	if (CMAKE_CXX_COMPILER_VERSION_INTERNAL VERSION_LESS 13)
	#		message(FATAL_ERROR "GCC 13 or newer is required for full C++23 support. Current version: ${CMAKE_CXX_COMPILER_VERSION_INTERNAL}")
	#	endif()

	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		add_compile_options(
			-g3						 	# Max debug info.
			-fno-omit-frame-pointer  	# Keep frame pointers (better stack traces)
			-fno-optimize-sibling-calls # Disable tail call optimization
			-fno-inline                 # Disable function inlining
			-O0 					 	# no optimization.
		)

#		add_compile_options(
#			-Wall                    	# Enable most warnings
#			-Wextra                  	# Enable extra warnings
#			-Wpedantic               	# Enable pedantic warnings
#
#			-Wshadow                 	# Warn about variable shadowing
#			-Wcast-align             	# Warn about pointer casts with alignment issues
#			-Wunused                 	# Warn about unused variables
#			-Wold-style-cast         	# Warn about C-style casts
#			-Woverloaded-virtual     	# Warn about overloaded virtual functions
#
#			-Wconversion             	# Warn about type conversions
#			-Wsign-conversion        	# Warn about sign conversions
#			-Wdouble-promotion       	# Warn about float to double promotion
#			-Wformat=2               	# Warn about format string issues
#
#			#		-Werror                  	# Treat warnings as errors
#			#		$<$<CONFIG:Debug>:-Werror>  # Only treat warnings as errors in debug
#			-Weffc++                 	# Warn about violations of Effective C++ guidelines
#		)

	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		add_compile_options(
			-O2 # yes optimization.
		)
	endif()
endif()