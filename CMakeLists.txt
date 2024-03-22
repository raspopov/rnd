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

	set( CMAKE_CXX_FLAGS "-static -Wall -Wextra -Wpedantic -flto -ffat-lto-objects" )
	set( CMAKE_AR "gcc-ar" )

endif()

add_executable( rnd
	rnd.cpp
)
