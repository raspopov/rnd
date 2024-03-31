cmake_minimum_required( VERSION 3.27 )

project( rnd
	VERSION			1.0.0.0
	DESCRIPTION			"The utility for the creation and verification of files filled with a pseudo-random sequence of bytes."
	HOMEPAGE_URL		"https://github.com/raspopov/rnd"
	LANGUAGES			CXX
)
set( PROJECT_VENDOR		"CherubicSoft" )
set( PROJECT_COPYRIGHT	"Copyright (C) 2024 Nikolay Raspopov <raspopov@cherubicsoft.com>" )
set( PROJECT_DOMAIN		"cherubicsoft.com" )

set( CMAKE_CXX_STANDARD 17 )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<CONFIG>" )

if( MSVC )

	add_compile_options( /MT /utf-8 /W4 /sdl /permissive- /Gy )
	add_compile_options( $<$<NOT:$<CONFIG:Debug>>:/GL> )
	add_link_options( $<$<NOT:$<CONFIG:Debug>>:/LTCG> )
	
else()

	set( CMAKE_CXX_FLAGS "-static -Wall -Wextra -Wpedantic" )

endif()

add_executable( rnd
	rnd.cpp
)

target_include_directories( rnd PRIVATE ${CMAKE_CURRENT_BINARY_DIR} )

# Version
configure_file( ${CMAKE_SOURCE_DIR}/version.h.in version.h @ONLY )
target_sources( rnd PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/version.h )

# Windows resources
if( WIN32 )
	configure_file( ${CMAKE_SOURCE_DIR}/resources.rc.in resources.rc @ONLY )
	target_sources( rnd PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/resources.rc )
endif()

# Installer
set( CPACK_VERBATIM_VARIABLES			ON )
set( CPACK_STRIP_FILES					ON )
set( CPACK_INCLUDE_TOPLEVEL_DIRECTORY	OFF )
set( CPACK_PACKAGE_NAME					${PROJECT_NAME} )
set( CPACK_PACKAGE_VENDOR				${PROJECT_VENDOR} )
set( CPACK_RESOURCE_FILE_LICENSE		${CMAKE_SOURCE_DIR}/LICENSE )
set( CPACK_PACKAGE_DESCRIPTION			${PROJECT_DESCRIPTION} )
set( CPACK_PACKAGE_INSTALL_DIRECTORY	${CPACK_PACKAGE_NAME} )
set( CPACK_GENERATOR ZIP )
include( CPack )
install( TARGETS rnd
	DESTINATION .
)
install( FILES ${CPACK_RESOURCE_FILE_LICENSE}
	DESTINATION .
)
