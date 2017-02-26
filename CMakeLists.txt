# Wymagaj tej wersji - dopiero w niej jest nowe makro FindPkgConfig
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.0 FATAL_ERROR)

PROJECT (gtkforms)
INCLUDE (../tiliae/tiliae.cmake)
SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall" CACHE INTERNAL "cxx compiler flags")

SET(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -Wall -DTILIAE_DLL" CACHE INTERNAL "c release compiler flags")
SET(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -DTILIAE_DLL" CACHE INTERNAL "cxx release compiler flags")

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK "-pthread")
ADD_DEFINITIONS (-DWITH_PARSER)

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

find_package (Boost REQUIRED COMPONENTS system prg_exec_monitor unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})
link_directories (${Boost_LIBRARY_DIRS})

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

INCLUDE_DIRECTORIES ("/usr/include/libcxxabi")

ENABLE_TESTING ()

## --------------------------------------------------------
## LIB
## --------------------------------------------------------

IF (BUILD_LIB)

        TILIAE_PARSE ("${PROJECT_SOURCE_DIR}/../tiliae/build/parser/tiliaeparser" "gtkforms" "src/Reflection_input.cc" "src/Reflection_output_gtkforms.cc.h")
        LIST (APPEND LIB_SOURCES "src/Reflection_input.cc")
        LIST (APPEND LIB_SOURCES "src/Reflection_output_gtkforms.cc.h")

        INCLUDE_DIRECTORIES (src)
        LIST (APPEND LIB_SOURCES "src/Config.h")
        LIST (APPEND LIB_SOURCES "src/RegexHelper.h")
        LIST (APPEND LIB_SOURCES "src/GtkForms.h")
        LIST (APPEND LIB_SOURCES "src/mapping/PixbufColumn.h")
        LIST (APPEND LIB_SOURCES "src/mapping/TableMapping.h")
        LIST (APPEND LIB_SOURCES "src/mapping/TextViewMapping.cc")
        LIST (APPEND LIB_SOURCES "src/mapping/AdjustmentMapping.h")
        LIST (APPEND LIB_SOURCES "src/mapping/GValueVariant.h")
        LIST (APPEND LIB_SOURCES "src/mapping/IMapping.h")
        LIST (APPEND LIB_SOURCES "src/mapping/PixbufMapping.h")
        LIST (APPEND LIB_SOURCES "src/mapping/AssociationMap.h")
        LIST (APPEND LIB_SOURCES "src/mapping/RawData.h")
        LIST (APPEND LIB_SOURCES "src/mapping/Column.h")
        LIST (APPEND LIB_SOURCES "src/mapping/AdjustmentMapping.cc")
        LIST (APPEND LIB_SOURCES "src/mapping/GValueVariant.cc")
        LIST (APPEND LIB_SOURCES "src/mapping/MappingDTO.h")
        LIST (APPEND LIB_SOURCES "src/mapping/PixbufMapping.cc")
        LIST (APPEND LIB_SOURCES "src/mapping/GObjectWrapperPlugin.h")
        LIST (APPEND LIB_SOURCES "src/mapping/Mapping.h")
        LIST (APPEND LIB_SOURCES "src/mapping/GObjectWrapperPlugin.cc")
        LIST (APPEND LIB_SOURCES "src/mapping/TextViewMapping.h")
        LIST (APPEND LIB_SOURCES "src/mapping/Column.cc")
        LIST (APPEND LIB_SOURCES "src/mapping/TableMapping.cc")
        LIST (APPEND LIB_SOURCES "src/mapping/TableFilterMapping.h")
        LIST (APPEND LIB_SOURCES "src/mapping/TableFilterMapping.cc")
        LIST (APPEND LIB_SOURCES "src/mapping/Mapping.cc")
        LIST (APPEND LIB_SOURCES "src/mapping/PixbufColumn.cc")
        LIST (APPEND LIB_SOURCES "src/App.h")
        LIST (APPEND LIB_SOURCES "src/decorator/ValidationDecorator.cc")
        LIST (APPEND LIB_SOURCES "src/decorator/IPageDecorator.h")
        LIST (APPEND LIB_SOURCES "src/decorator/Decorator.h")
        LIST (APPEND LIB_SOURCES "src/decorator/ValidationDecorator.h")
        LIST (APPEND LIB_SOURCES "src/decorator/AbstractPageDecorator.h")
        LIST (APPEND LIB_SOURCES "src/decorator/AbstractPageDecorator.cc")
        LIST (APPEND LIB_SOURCES "src/decorator/GtkTreeViewDecorator.h")
        LIST (APPEND LIB_SOURCES "src/decorator/GtkTreeViewDecorator.cc")
        LIST (APPEND LIB_SOURCES "src/decorator/DimensionRestoreDecorator.h")
        LIST (APPEND LIB_SOURCES "src/decorator/DimensionRestoreDecorator.cc")
        LIST (APPEND LIB_SOURCES "src/view/AbstractView.h")
        LIST (APPEND LIB_SOURCES "src/view/View.h")
        LIST (APPEND LIB_SOURCES "src/view/Slot.cc")
        LIST (APPEND LIB_SOURCES "src/view/AbstractView.cc")
        LIST (APPEND LIB_SOURCES "src/view/Slot.h")
        LIST (APPEND LIB_SOURCES "src/view/BuilderView.h")
        LIST (APPEND LIB_SOURCES "src/view/BuilderView.cc")
        LIST (APPEND LIB_SOURCES "src/Context.cc")
        LIST (APPEND LIB_SOURCES "src/ValidationAndBindingResult.cc")
        LIST (APPEND LIB_SOURCES "src/signalAdapter/SignalAdapter.h")
        LIST (APPEND LIB_SOURCES "src/signalAdapter/AbstractSignalAdapter.h")
        LIST (APPEND LIB_SOURCES "src/signalAdapter/RowActivatedAdapter.h")
        LIST (APPEND LIB_SOURCES "src/signalAdapter/RowActivatedAdapter.cc")
        LIST (APPEND LIB_SOURCES "src/signalAdapter/ISignalAdapter.h")
        LIST (APPEND LIB_SOURCES "src/signalAdapter/KeyPressEventAdapter.h")
        LIST (APPEND LIB_SOURCES "src/signalAdapter/KeyPressEventAdapter.cc")
        LIST (APPEND LIB_SOURCES "src/Logging.h")
        LIST (APPEND LIB_SOURCES "src/controller/Controller.h")
        LIST (APPEND LIB_SOURCES "src/controller/AbstractAccessor.h")
        LIST (APPEND LIB_SOURCES "src/controller/AbstractAccessor.cc")
        LIST (APPEND LIB_SOURCES "src/controller/HierarchicalAccessor.h")
        LIST (APPEND LIB_SOURCES "src/controller/HierarchicalAccessor.cc")
        LIST (APPEND LIB_SOURCES "src/controller/QuitEvent.cc")
        LIST (APPEND LIB_SOURCES "src/controller/RefreshEvent.cc")
        LIST (APPEND LIB_SOURCES "src/controller/QuitEvent.h")
        LIST (APPEND LIB_SOURCES "src/controller/IEvent.h")
        LIST (APPEND LIB_SOURCES "src/controller/DummyController.h")
        LIST (APPEND LIB_SOURCES "src/controller/DefaultQuitHandler.cc")
        LIST (APPEND LIB_SOURCES "src/controller/DefaultQuitHandler.h")
        LIST (APPEND LIB_SOURCES "src/controller/SubmitEvent.cc")
        LIST (APPEND LIB_SOURCES "src/controller/RefreshEvent.h")
        LIST (APPEND LIB_SOURCES "src/controller/IHandler.h")
        LIST (APPEND LIB_SOURCES "src/controller/AbstractController.h")
        LIST (APPEND LIB_SOURCES "src/controller/SubmitEvent.h")
        LIST (APPEND LIB_SOURCES "src/controller/AbstractController.cc")
        LIST (APPEND LIB_SOURCES "src/Context.h")
        LIST (APPEND LIB_SOURCES "src/App.cc")
        LIST (APPEND LIB_SOURCES "src/validator/IValidator.h")
        LIST (APPEND LIB_SOURCES "src/validator/NotBlankValidator.cc")
        LIST (APPEND LIB_SOURCES "src/validator/NotBlankValidator.h")
        LIST (APPEND LIB_SOURCES "src/validator/AbstractValidator.h")
        LIST (APPEND LIB_SOURCES "src/validator/Validator.h")
        LIST (APPEND LIB_SOURCES "src/RegexHelper.cc")
        LIST (APPEND LIB_SOURCES "src/ValidationAndBindingResult.h")
        ADD_LIBRARY (${CMAKE_PROJECT_NAME} SHARED ${LIB_SOURCES})

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
