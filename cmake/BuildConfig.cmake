########################################################################
# This cmake file contains a bunch of variables for managing the build #
########################################################################

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(ROOT_BUILD_DIR ${CMAKE_SOURCE_DIR})
set(BUILD_FOLDER_NAME "__BUILD__")

########################################################################################################################

# Makes installation more universal and straightforward.
set(
	CMAKE_INSTALL_PREFIX
	${CMAKE_BINARY_DIR}
	CACHE INTERNAL ""
)

########################################################################################################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_BUILD_DIR}/${BUILD_FOLDER_NAME}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_BUILD_DIR}/${BUILD_FOLDER_NAME}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Temp)

########################################################################################################################

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(GODOTCPP_DEBUG_CRT ON)
	add_compile_definitions(BUILD_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(BUILD_RELEASE)
endif()