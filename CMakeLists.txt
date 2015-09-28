# Wymagaj tej wersji - dopiero w niej jest nowe makro FindPkgConfig
CMAKE_MINIMUM_REQUIRED (VERSION 2.6.0 FATAL_ERROR)

project (gtkforms)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET (BUILD_LIB                 ON CACHE BOOL "")
SET (BUILD_TEST                OFF CACHE BOOL "")
SET (BUILD_NAVIGATION          OFF CACHE BOOL "")
SET (BUILD_SUBMISSION          OFF CACHE BOOL "")
SET (BUILD_CALCULATOR          OFF CACHE BOOL "")
SET (BUILD_CATALOG             OFF CACHE BOOL "")

SET (CMAKE_VERBOSE_MAKEFILE 0)

#TODO -Wall
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" CACHE INTERNAL "cxx compiler flags")

SET(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -Wall -fvisibility=hidden -DTILIAE_DLL" CACHE INTERNAL "c release compiler flags")
SET(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -std=c++11 -Wall -fvisibility=hidden -DTILIAE_DLL" CACHE INTERNAL "cxx release compiler flags")

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK "-pthread")
ADD_DEFINITIONS (-DREFLECTION_ENABLED)

INSTALL (
    DIRECTORY "${PROJECT_SOURCE_DIR}/src/"
    DESTINATION "include/${CMAKE_PROJECT_NAME}"
    FILES_MATCHING PATTERN "*.h"
    PATTERN "*.svn*" EXCLUDE
    PATTERN "*test*" EXCLUDE)

INSTALL (
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/pc/"
    DESTINATION "lib/pkgconfig"
    FILES_MATCHING PATTERN "*.pc"
    PATTERN "*.svn*" EXCLUDE)

SET (Boost_ADDITIONAL_VERSIONS "1.41" "1.41.0")
find_package (Boost 1.41.0)
include_directories (${Boost_INCLUDE_DIRS})

include (FindPkgConfig)
pkg_check_modules (GTK REQUIRED "gtk+-3.0")
INCLUDE_DIRECTORIES (${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

pkg_check_modules (TILIAE "tiliae")

if (NOT TILIAE_FOUND)
        MESSAGE (STATUS "Tiliae not installed, using local copy.")
        SET (TILIAE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../tiliae/src/")
        SET (TILIAE_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/../tiliae/build/")
        MESSAGE (STATUS ${TILIAE_INCLUDE_DIRS})
        MESSAGE (STATUS ${TILIAE_LIBRARY_DIRS})
endif ()

INCLUDE_DIRECTORIES (${TILIAE_INCLUDE_DIRS})
link_directories(${TILIAE_LIBRARY_DIRS})

ENABLE_TESTING ()

## --------------------------------------------------------
## LIB
## --------------------------------------------------------

IF (BUILD_LIB)

        # +-------------------------------+
        # | Generate reflection database. |
        # +-------------------------------+
        FILE (MAKE_DIRECTORY reflectionDatabase)
        FIND_PROGRAM (REFLECTION_PARSER NAME "tiliaeparser" HINTS "${PROJECT_SOURCE_DIR}/../tiliae/build/parser")

        IF (NOT REFLECTION_PARSER)
                MESSAGE(FATAL_ERROR "reflectionParser not found -- ${REFLECTION_PARSER}")
        ENDIF()

        MESSAGE (STATUS "Found REFLECTION_PARSER as ${REFLECTION_PARSER}")

        ADD_CUSTOM_COMMAND (OUTPUT reflectionDatabase/ReflectionDatabase.cc
                COMMAND ${REFLECTION_PARSER} -p compile_commands.json ${CMAKE_SOURCE_DIR}/src/Reflection.cc -o reflectionDatabase/ReflectionDatabase.cc
                DEPENDS src/Reflection.cc)
#                WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

        LIST (APPEND LIB_SOURCES reflectionDatabase/ReflectionDatabase.cc)

        SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)

        INCLUDE_DIRECTORIES (src)
        AUX_SOURCE_DIRECTORY (src LIB_SOURCES)
        AUX_SOURCE_DIRECTORY (src/controller LIB_SOURCES)
        AUX_SOURCE_DIRECTORY (src/view LIB_SOURCES)
        AUX_SOURCE_DIRECTORY (src/mapping LIB_SOURCES)
        AUX_SOURCE_DIRECTORY (src/signalAdapter LIB_SOURCES)
        AUX_SOURCE_DIRECTORY (src/validator LIB_SOURCES)
        AUX_SOURCE_DIRECTORY (src/decorator LIB_SOURCES)
        add_library (${CMAKE_PROJECT_NAME} SHARED ${LIB_SOURCES})

        SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)

        INSTALL (
        TARGETS ${CMAKE_PROJECT_NAME}
        DESTINATION lib
        PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE)
ENDIF()

## --------------------------------------------------------
## TEST
## --------------------------------------------------------

IF(BUILD_TEST)
    AUX_SOURCE_DIRECTORY (src/test TEST_SOURCES)

    add_executable (test_forms ${TEST_SOURCES})
    TARGET_LINK_LIBRARIES (test_forms ${GTK_LIBRARIES})
    TARGET_LINK_LIBRARIES (test_forms ${TILIAE_LIBRARIES})
    TARGET_LINK_LIBRARIES (test_forms ${CMAKE_PROJECT_NAME})
    TARGET_LINK_LIBRARIES (test_forms boost_unit_test_framework-gcc44)
ENDIF()

## --------------------------------------------------------
## DEMOS
## --------------------------------------------------------

IF(BUILD_NAVIGATION)
    ADD_DEFINITIONS (-DREFLECTION_ENABLED)

    AUX_SOURCE_DIRECTORY (demo/navigation NAVIGATION_SOURCES)
    add_executable (navigation ${NAVIGATION_SOURCES})
    TARGET_LINK_LIBRARIES (navigation ${GTK_LIBRARIES})
    TARGET_LINK_LIBRARIES (navigation ${TILIAE_LIBRARIES})
    TARGET_LINK_LIBRARIES (navigation ${CMAKE_PROJECT_NAME})
    TARGET_LINK_LIBRARIES (navigation boost_log)
    TARGET_LINK_LIBRARIES (navigation boost_log_setup)
    TARGET_LINK_LIBRARIES (navigation boost_regex)
ENDIF()

IF(BUILD_SUBMISSION)
    ADD_DEFINITIONS (-DREFLECTION_ENABLED)

    AUX_SOURCE_DIRECTORY (demo/submission SUBMISSION_SOURCES)
    add_executable (submission ${SUBMISSION_SOURCES})
    TARGET_LINK_LIBRARIES (submission ${GTK_LIBRARIES})
    TARGET_LINK_LIBRARIES (submission ${TILIAE_LIBRARIES})
    TARGET_LINK_LIBRARIES (submission ${CMAKE_PROJECT_NAME})
    TARGET_LINK_LIBRARIES (submission boost_log)
    TARGET_LINK_LIBRARIES (submission boost_log_setup)
    TARGET_LINK_LIBRARIES (submission boost_regex)
    TARGET_LINK_LIBRARIES (submission pthread)
ENDIF()

IF(BUILD_CALCULATOR)
    AUX_SOURCE_DIRECTORY (demo/calculator DEMO_SOURCES)
    add_executable (calculator ${DEMO_SOURCES})
    TARGET_LINK_LIBRARIES (calculator ${GTK_LIBRARIES})
    TARGET_LINK_LIBRARIES (calculator ${XML_LIBRARIES})
    TARGET_LINK_LIBRARIES (calculator ${TILIAE_LIBRARIES})
    TARGET_LINK_LIBRARIES (calculator ${CMAKE_PROJECT_NAME})
ENDIF()

IF(BUILD_CATALOG)
    SET(DEMO_SOURCES)
    AUX_SOURCE_DIRECTORY (demo/catalog DEMO_SOURCES)
    AUX_SOURCE_DIRECTORY (demo/catalog/model DEMO_SOURCES)
    AUX_SOURCE_DIRECTORY (demo/catalog/controller DEMO_SOURCES)
    AUX_SOURCE_DIRECTORY (demo/catalog/dao DEMO_SOURCES)
    add_executable (catalog ${DEMO_SOURCES})
    TARGET_LINK_LIBRARIES (catalog ${GTK_LIBRARIES})
    TARGET_LINK_LIBRARIES (catalog ${XML_LIBRARIES})
    TARGET_LINK_LIBRARIES (catalog ${TILIAE_LIBRARIES})
    TARGET_LINK_LIBRARIES (catalog ${CMAKE_PROJECT_NAME})
ENDIF()

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/pc/${CMAKE_PROJECT_NAME}.pc.in ${CMAKE_CURRENT_BINARY_DIR}/pc/${CMAKE_PROJECT_NAME}.pc @ONLY)
