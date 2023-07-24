#CMake - Cross Platform Makefile Generator
#Copyright 2000-2011 Kitware, Inc., Insight Software Consortium
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions
#are met:
#
#* Redistributions of source code must retain the above copyright
#  notice, this list of conditions and the following disclaimer.
#
#* Redistributions in binary form must reproduce the above copyright
#  notice, this list of conditions and the following disclaimer in the
#  documentation and/or other materials provided with the distribution.
#
#* Neither the names of Kitware, Inc., the Insight Software Consortium,
#  nor the names of their contributors may be used to endorse or promote
#  products derived from this software without specific prior written
#  permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# This module defines
#  CURL_FOUND, if false, do not try to link to CURL
#  CURL_LIBRARIES
#  CURL_INCLUDE_DIRS, where to find CURL.h
#  CURL_VERSION_STRING, the version of CURL found (since CMake 2.8.8)

IF(CURL_ROOT)
	FIND_PATH(CURL_INCLUDE_DIR
		NAMES curl/curl.h
		HINTS
			ENV CURL_DIR
		PATHS
		${CURL_ROOT}
		~/Library/Frameworks
		/Library/Frameworks
		/sw
		/opt/local
		/opt/csw
		/opt
		PATH_SUFFIXES include/curl include src
	)
	
	FIND_LIBRARY(CURL_LIBRARY
		NAMES libcurl
		HINTS
			ENV CURL_DIR
		PATHS
		${CURL_ROOT}
		~/Library/Frameworks
		/Library/Frameworks
		/sw
		/opt/local
		/opt/csw
		/opt
		PATH_SUFFIXES lib bin
	)
ELSE(CURL_ROOT)
	FIND_PATH(CURL_INCLUDE_DIR
		NAMES curl/curl.h
		HINTS
			ENV CURL_DIR
		PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/sw
		/opt/local
		/opt/csw
		/opt
		PATH_SUFFIXES include/curl include
	)
	
	FIND_LIBRARY(CURL_LIBRARY
		NAMES curl curllib libcurl_imp curllib_static libcurl
		HINTS
			ENV CURL_DIR
		PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/sw
		/opt/local
		/opt/csw
		/opt
		PATH_SUFFIXES lib bin
	)
ENDIF(CURL_ROOT)

IF(CURL_LIBRARY)
	SET( CURL_LIBRARIES "${CURL_LIBRARY}" CACHE STRING "CURL Libraries")
ENDIF()

IF(CURL_INCLUDE_DIR AND EXISTS "${CURL_INCLUDE_DIR}/curl/curl.h")
	FILE(STRINGS "${CURL_INCLUDE_DIR}/curl/curl.h" CURL_version_str REGEX "^#define[ \t]+CURL_RELEASE[ \t]+\"CURL .+\"")

	STRING(REGEX REPLACE "^#define[ \t]+CURL_RELEASE[ \t]+\"CURL ([^\"]+)\".*" "\\1" CURL_VERSION_STRING "${CURL_version_str}")
	UNSET(CURL_version_str)
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CURL
                                  REQUIRED_VARS CURL_LIBRARY CURL_INCLUDE_DIR
                                  VERSION_VAR CURL_VERSION_STRING)

MARK_AS_ADVANCED(CURL_INCLUDE_DIR CURL_LIBRARY)

set(CURL_LIBRARIES ${CURL_LIBRARY} )
set(CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIR} )


