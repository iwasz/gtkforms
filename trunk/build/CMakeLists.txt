# Wymagaj tej wersji - dopiero w niej jest nowe makro FindPkgConfig
CMAKE_MINIMUM_REQUIRED (VERSION 2.6.0 FATAL_ERROR)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET(BUILD_LIB               1)
SET(BUILD_TEST              0)
SET(BUILD_CALCULATOR        0)
SET(BUILD_CATALOG           1)

SET (CMAKE_VERBOSE_MAKEFILE 0) 
ADD_DEFINITIONS (-g -fvisibility=hidden -DTILIAE_DLL -Wall)
ADD_DEFINITIONS (-DTILIAE_DEBUG)
ADD_DEFINITIONS (-O0)

INSTALL (
    DIRECTORY "${PROJECT_SOURCE_DIR}/../src/"
    DESTINATION "include/tiliae" 
    FILES_MATCHING PATTERN "*.h"
    PATTERN "*.svn*" EXCLUDE
    PATTERN "*test*" EXCLUDE)

INSTALL (
    DIRECTORY "${PROJECT_SOURCE_DIR}/pc/"
    DESTINATION "/usr/lib/pkgconfig"
    FILES_MATCHING PATTERN "*.pc"
    PATTERN "*.svn*" EXCLUDE)

SET(Boost_ADDITIONAL_VERSIONS "1.41" "1.41.0")
find_package( Boost 1.41.0 )
include_directories(${Boost_INCLUDE_DIRS})

include (FindPkgConfig)
pkg_check_modules (XML "gtk+-2.0")
INCLUDE_DIRECTORIES (${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

pkg_check_modules (TILIAE "tiliae")
INCLUDE_DIRECTORIES (${TILIAE_INCLUDE_DIRS})
message (${TILIAE_INCLUDE_DIRS})
link_directories(${TILIAE_LIBRARY_DIRS})

pkg_check_modules (XML "libxml-2.0")
INCLUDE_DIRECTORIES (${XML_INCLUDE_DIRS})
link_directories(${XML_LIBRARY_DIRS})

pkg_check_modules (SPARSE_HASH REQUIRED "libsparsehash")
INCLUDE_DIRECTORIES (${SPARSE_HASH_INCLUDE_DIRS})

ENABLE_TESTING ()

## --------------------------------------------------------
## LIB
## --------------------------------------------------------

IF (BUILD_LIB)
    INCLUDE_DIRECTORIES (../src)      
    
    AUX_SOURCE_DIRECTORY (../src LIB_SOURCES)
    AUX_SOURCE_DIRECTORY (../src/view LIB_SOURCES)
    AUX_SOURCE_DIRECTORY (../src/application LIB_SOURCES)
    AUX_SOURCE_DIRECTORY (../src/application/init LIB_SOURCES)
    AUX_SOURCE_DIRECTORY (../src/tools LIB_SOURCES)
    AUX_SOURCE_DIRECTORY (../src/mapping LIB_SOURCES)
    add_library (gtkforms SHARED ${LIB_SOURCES})
ENDIF()

## --------------------------------------------------------
## TEST
## --------------------------------------------------------

IF(BUILD_TEST)
    AUX_SOURCE_DIRECTORY (../src/test TEST_SOURCES)
         
    add_executable (test_forms ${TEST_SOURCES})
    TARGET_LINK_LIBRARIES (test_forms ${GTK_LIBRARIES})
    TARGET_LINK_LIBRARIES (test_forms ${XML_LIBRARIES})
    TARGET_LINK_LIBRARIES (test_forms ${TILIAE_LIBRARIES})
    TARGET_LINK_LIBRARIES (test_forms gtkforms)
    TARGET_LINK_LIBRARIES (test_forms boost_unit_test_framework-gcc44)
ENDIF()

## --------------------------------------------------------
## DEMOS
## --------------------------------------------------------

IF(BUILD_CALCULATOR)
    AUX_SOURCE_DIRECTORY (../demo/calculator DEMO_SOURCES)    
    add_executable (calculator ${DEMO_SOURCES})
    TARGET_LINK_LIBRARIES (calculator ${GTK_LIBRARIES})
    TARGET_LINK_LIBRARIES (calculator ${XML_LIBRARIES})
    TARGET_LINK_LIBRARIES (calculator ${TILIAE_LIBRARIES})
    TARGET_LINK_LIBRARIES (calculator gtkforms)
ENDIF()

IF(BUILD_CATALOG)
    SET(DEMO_SOURCES)
    AUX_SOURCE_DIRECTORY (../demo/catalog DEMO_SOURCES)    
    AUX_SOURCE_DIRECTORY (../demo/catalog/model DEMO_SOURCES)    
    AUX_SOURCE_DIRECTORY (../demo/catalog/controller DEMO_SOURCES)    
    AUX_SOURCE_DIRECTORY (../demo/catalog/dao DEMO_SOURCES)    
    add_executable (catalog ${DEMO_SOURCES})
    TARGET_LINK_LIBRARIES (catalog ${GTK_LIBRARIES})
    TARGET_LINK_LIBRARIES (catalog ${XML_LIBRARIES})
    TARGET_LINK_LIBRARIES (catalog ${TILIAE_LIBRARIES})
    TARGET_LINK_LIBRARIES (catalog gtkforms)
ENDIF()
