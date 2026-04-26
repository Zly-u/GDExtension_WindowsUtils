cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)

########################################################################################################################

include(cmake/GlobalDefines.cmake)
include(cmake/BuildConfig.cmake)

########################################################################################################################

project(WindowUtilsPlugin LANGUAGES CXX)

set(ENTRY_POINT "plugin_init")
set(GODOT_MIN_REQUIREMENT 4.5)
set(GDP_TEMPLATE_FILE "plugin_template.gdextension.in")

########################################################################################################################

# Source files of the Plugin.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c**)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h**)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

########################################################################################################################

include(FetchContent)
FetchContent_Declare(
    godot-cpp
    GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
    GIT_TAG 4.5
)
FetchContent_MakeAvailable(godot-cpp)

get_target_property(GODOTCPP_SUFFIX godot::cpp GODOTCPP_SUFFIX)
get_target_property(GODOTCPP_PLATFORM godot::cpp GODOTCPP_PLATFORM)

########################################################################################################################

# Delete old build files, just in case if project name has changed.
file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/demo/bin/)

configure_file(${GDP_TEMPLATE_FILE} ${PROJECT_NAME}.gdextension)
set(
    ADDITIONAL_SRCS
    ${PROJECT_NAME}.gdextension
)

########################################################################################################################

# Include the source files for CLion to be able to see available sources.
set(
    PRIVATE_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

foreach(dir IN LISTS PRIVATE_INCLUDE_DIRS)
    target_include_directories(${PROJECT_NAME} PRIVATE ${dir})
endforeach()

########################################################################################################################

# Setup appropriate naming for the built plugin files.
if(NOT DEFINED BITS)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(BITS 64)
    else()
        set(BITS 32)
    endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
	PREFIX ""
    OUTPUT_NAME ${PROJECT_NAME}${GODOTCPP_SUFFIX}
)

########################################################################################################################

# Link the godot CPP bindings
target_link_libraries(${PROJECT_NAME} PUBLIC godot::cpp)

########################################################################################################################

message("DOC_XML ${DOC_XML}")
if(DOC_XML)
	if(GODOTCPP_TARGET MATCHES "editor|template_debug")
		target_doc_sources(${PROJECT_NAME} ${DOC_XML})
	endif()
endif()

# Copy the built stuff over to the Godot project
install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_SOURCE_DIR}/demo/bin
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${ADDITIONAL_SRCS}
    DESTINATION ${CMAKE_SOURCE_DIR}/demo/bin
)
#install(
#	FILES ${CMAKE_SOURCE_DIR}/RunEditor.bat
#)







