# Wymagaj tej wersji - dopiero w niej jest nowe makro FindPkgConfig
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.0 FATAL_ERROR)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0048 NEW)
endif(COMMAND cmake_policy)

PROJECT (gtkforms VERSION 0.9)
INCLUDE (../tiliae/tiliae.cmake)
SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET (WITH_PARSER               ON CACHE BOOL "")
SET (WITH_CPPABI               ON CACHE BOOL "")
SET (BUILD_LIB                 ON CACHE BOOL "")
SET (BUILD_TEST                OFF CACHE BOOL "")
SET (BUILD_NAVIGATION          OFF CACHE BOOL "")
SET (BUILD_SUBMISSION          OFF CACHE BOOL "")
SET (BUILD_CALCULATOR          OFF CACHE BOOL "")
SET (BUILD_CATALOG             OFF CACHE BOOL "")
SET (BUILD_SHARED_LIBS         ON CACHE BOOL "")

SET (CMAKE_VERBOSE_MAKEFILE 0)

#TODO -Wall
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall" CACHE INTERNAL "cxx compiler flags")

SET(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -Wall -DTILIAE_DLL" CACHE INTERNAL "c release compiler flags")
SET(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -DTILIAE_DLL" CACHE INTERNAL "cxx release compiler flags")

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK "-pthread")
IF (WITH_PARSER)
ADD_DEFINITIONS (-DWITH_PARSER)
ENDIF ()
SET (CMAKE_INSTALL_PREFIX /usr)

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

IF (WITH_PARSER)
        TILIAE_PARSE ("${PROJECT_SOURCE_DIR}/../tiliae/build/parser/tiliaeparser" "gtkforms" "src/Reflection_input.cc" "src/Reflection_output_gtkforms.cc.h")
ENDIF ()
        LIST (APPEND LIB_SOURCES "src/Reflection_input.cc")
        LIST (APPEND LIB_SOURCES "src/Reflection_output_gtkforms.cc.h")

        INCLUDE_DIRECTORIES (src)
        LIST (APPEND LIB_SOURCES
                "src/Config.h"
                "src/RegexHelper.h"
                "src/GtkForms.h"
                "src/mapping/PixbufColumn.h"
                "src/mapping/TableMapping.h"
                "src/mapping/TextViewMapping.cc"
                "src/mapping/AdjustmentMapping.h"
                "src/mapping/GValueVariant.h"
                "src/mapping/IMapping.h"
                "src/mapping/PixbufMapping.h"
                "src/mapping/AssociationMap.h"
                "src/mapping/RawData.h"
                "src/mapping/Column.h"
                "src/mapping/AdjustmentMapping.cc"
                "src/mapping/GValueVariant.cc"
                "src/mapping/MappingDTO.h"
                "src/mapping/PixbufMapping.cc"
                "src/mapping/GObjectWrapperPlugin.h"
                "src/mapping/Mapping.h"
                "src/mapping/GObjectWrapperPlugin.cc"
                "src/mapping/TextViewMapping.h"
                "src/mapping/Column.cc"
                "src/mapping/TableMapping.cc"
                "src/mapping/TableFilterMapping.h"
                "src/mapping/TableFilterMapping.cc"
                "src/mapping/Mapping.cc"
                "src/mapping/PixbufColumn.cc"
                "src/App.h"
                "src/decorator/ValidationDecorator.cc"
                "src/decorator/IPageDecorator.h"
                "src/decorator/Decorator.h"
                "src/decorator/ValidationDecorator.h"
                "src/decorator/AbstractPageDecorator.h"
                "src/decorator/AbstractPageDecorator.cc"
                "src/decorator/GtkTreeViewDecorator.h"
                "src/decorator/GtkTreeViewDecorator.cc"
                "src/decorator/DimensionRestoreDecorator.h"
                "src/decorator/DimensionRestoreDecorator.cc"
                "src/decorator/CloseOnEscapeDecorator.h"
                "src/decorator/CloseOnEscapeDecorator.cc"
                "src/view/AbstractView.h"
                "src/view/View.h"
                "src/view/Slot.cc"
                "src/view/AbstractView.cc"
                "src/view/Slot.h"
                "src/view/BuilderView.h"
                "src/view/BuilderView.cc"
                "src/view/EmptyView.h"
                "src/Context.cc"
                "src/ValidationAndBindingResult.cc"
                "src/signalAdapter/SignalAdapter.h"
                "src/signalAdapter/AbstractSignalAdapter.h"
                "src/signalAdapter/RowActivatedAdapter.h"
                "src/signalAdapter/RowActivatedAdapter.cc"
                "src/signalAdapter/ISignalAdapter.h"
                "src/signalAdapter/KeyPressEventAdapter.h"
                "src/signalAdapter/KeyPressEventAdapter.cc"
                "src/signalAdapter/FileChooserAdapter.h"
                "src/signalAdapter/FileChooserAdapter.cc"
                "src/Logging.h"
                "src/controller/Controller.h"
                "src/controller/AbstractAccessor.h"
                "src/controller/AbstractAccessor.cc"
                "src/controller/HierarchicalAccessor.h"
                "src/controller/HierarchicalAccessor.cc"
                "src/controller/QuitEvent.cc"
                "src/controller/RefreshEvent.cc"
                "src/controller/QuitEvent.h"
                "src/controller/IEvent.h"
                "src/controller/DummyController.h"
                "src/controller/DefaultQuitHandler.cc"
                "src/controller/DefaultQuitHandler.h"
                "src/controller/SubmitEvent.cc"
                "src/controller/RefreshEvent.h"
                "src/controller/IHandler.h"
                "src/controller/AbstractController.h"
                "src/controller/SubmitEvent.h"
                "src/controller/AbstractController.cc"
                "src/controller/ViewsToOpen.h"
                "src/controller/ViewsToOpen.cc"
                "src/Context.h"
                "src/App.cc"
                "src/validator/IValidator.h"
                "src/validator/NotBlankValidator.cc"
                "src/validator/NotBlankValidator.h"
                "src/validator/AbstractValidator.h"
                "src/validator/Validator.h"
                "src/RegexHelper.cc"
                "src/ValidationAndBindingResult.h")

        ADD_LIBRARY (${CMAKE_PROJECT_NAME} ${LIB_SOURCES})

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

ADD_CUSTOM_TARGET("publish" DEPENDS gtkforms COMMAND rm -Rf /tmp/installdir)
ADD_CUSTOM_COMMAND(TARGET "publish" COMMAND mkdir -p /tmp/installdir)
ADD_CUSTOM_COMMAND(TARGET "publish" COMMAND make install DESTDIR=/tmp/installdir)

FIND_PROGRAM (FPM fpm)
ADD_CUSTOM_COMMAND (TARGET "publish" COMMAND ${FPM} -f -s dir -t deb -n libgtkforms -v ${PROJECT_VERSION} -C /tmp/installdir/ -p libgtkforms_VERSION_ARCH.deb -d 'libtiliae >= 0.9' -d 'libgtk-3-0 >= 3.20.9' -d 'libboost-log1.61.0 >= 1.61.0' -d 'libboost-regex1.61.0 >= 1.61.0' -d 'libboost-atomic1.61.0 >= 1.61.0' -d 'libboost-chrono1.61.0 >= 1.61.0' -d 'libboost-date-time1.61.0 >= 1.61.0' -d 'libboost-filesystem1.61.0 > 1.61.0' -d 'libboost-system1.61.0 >= 1.61.0' -d 'libboost-thread1.61.0 >= 1.61.0'  usr/lib)
ADD_CUSTOM_COMMAND (TARGET "publish" COMMAND ${FPM} -f -s dir -t deb -n libgtkforms-dev -v ${PROJECT_VERSION} -C /tmp/installdir/ -p libgtkforms-dev_VERSION_ARCH.deb -d 'libgtkforms = ${PROJECT_VERSION}' usr/include)
FIND_PROGRAM (DPKGSIG dpkg-sig)
ADD_CUSTOM_COMMAND (TARGET "publish" COMMAND ${DPKGSIG} -k 9EDE3F68 -s builder *.deb)
FIND_PROGRAM (SCP scp)
ADD_CUSTOM_COMMAND (TARGET "publish" COMMAND ${SCP} libgtkforms_${PROJECT_VERSION}_amd64.deb iwasz@iwasz.pl:/home/iwasz/domains/iwasz.pl/public_html/deb/)
ADD_CUSTOM_COMMAND (TARGET "publish" COMMAND ${SCP} libgtkforms-dev_${PROJECT_VERSION}_amd64.deb iwasz@iwasz.pl:/home/iwasz/domains/iwasz.pl/public_html/deb/)
