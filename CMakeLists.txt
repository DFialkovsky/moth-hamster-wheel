

#--------------------------------------------------------------------------------
# This is an example project that uses CMake as a build system to compile a Qt4.x
# based project.
# To have CMake pick a Qt installation of your choice that won't be found
# automatically, set the CMAKE_PREFIX_PATH environment variable.
# For example: "export CMAKE_PREFIX_PATH=/usr/local/trolltech/qt4.3.5"
#

PROJECT (mothHamsterWheel)
cmake_minimum_required(VERSION 2.8)
add_definitions(-g -std=c++11)

#--------------------------------------------------------------------------------
# This is the list of source files that need to be compiled
#--------------------------------------------------------------------------------
SET( mothHamsterWheel_SRCS
    main.cpp
    mainWindow.cpp
    newParser.cpp
    dataEntry.cpp
    filePrinter.cpp
)

#--------------------------------------------------------------------------------
# another list, this time it includes all header files that should be treated with moc
#--------------------------------------------------------------------------------
SET( mothHamsterWheel_MOC_HDRS
    mainWindow.hpp
)

#--------------------------------------------------------------------------------
# .ui files are not used in this project
#--------------------------------------------------------------------------------
SET( mothHamsterWheel_UIS
# empty
)

#--------------------------------------------------------------------------------
# and finally any resource files. I do not have any in this example
#--------------------------------------------------------------------------------
SET( mothHamsterWheel_RCS
#empty
)

SET (mothHamsterWheel_GEN_HDRS
    newParser.hpp
    rawEntry.hpp
    dataEntry.hpp
    filePrinter.hpp
    confReader.hpp
)

#--------------------------------------------------------------------------------
# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the
#  IF(WIN32) conditional.
IF (WIN32)
    SET (QT_USE_QTMAIN TRUE)
ENDIF (WIN32)

#--------------------------------------------------------------------------------
# this command finds Qt4 libraries and sets all required variables
# note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 REQUIRED )

#--------------------------------------------------------------------------------
# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
INCLUDE( ${QT_USE_FILE} )

#--------------------------------------------------------------------------------
# this command will generate rules that will run rcc on all files from mothHamsterWheel_RCS
# in result mothHamsterWheel_GEN_RCS variable will contain paths to files produced by rcc
QT4_ADD_RESOURCES( mothHamsterWheel_GEN_RCS ${mothHamsterWheel_RCS} )

#--------------------------------------------------------------------------------
# this will run uic on .ui files:
QT4_WRAP_UI( mothHamsterWheel_GEN_UIS ${mothHamsterWheel_UIS} )

#--------------------------------------------------------------------------------
# and finally this will run moc on the headers
QT4_WRAP_CPP( mothHamsterWheel_GEN_MOC_HDRS ${mothHamsterWheel_MOC_HDRS} )



FIND_PACKAGE(Boost COMPONENTS date_time REQUIRED)


#--------------------------------------------------------------------------------
# we need this to be able to include headers produced by uic in our code
# CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES()
#  works just like INCLUDEPATH from qmake

set (XLS_LIB_INCLUDE_DIR 	 ${CMAKE_CURRENT_LIST_DIR}/xlslib/)
if (WIN32)
    set (XLS_LIB_LIB_DIR		 ${CMAKE_CURRENT_LIST_DIR}/xlslib-lib/libxls.dll)
elseif(UNIX)
    set (XLS_LIB_LIB_DIR		 ${CMAKE_CURRENT_LIST_DIR}/xlslib-lib/libxls.so)
endif()

INCLUDE_DIRECTORIES (
    ${Boost_INCLUDE_DIR}
    ${XLS_LIB_INCLUDE_DIR}
)

#--------------------------------------------------------------------------------
# For Apple set the icns file containing icons
IF(APPLE)
  # set how it shows up in the Info.plist file
  SET(MACOSX_BUNDLE_ICON_FILE mothHamsterWheel.icns)
  # set where in the bundle to put the icns file
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/mothHamsterWheel.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  # include the icns file in the target
  SET(mothHamsterWheel_SRCS ${mothHamsterWheel_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/mothHamsterWheel.icns)
ENDIF(APPLE)

#--------------------------------------------------------------------------------
#  CMake's way of creating an executable
ADD_EXECUTABLE( mothHamsterWheel MACOSX_BUNDLE WIN32
  ${mothHamsterWheel_SRCS}
  ${mothHamsterWheel_GEN_MOC_HDRS}
  ${mothHamsterWheel_GEN_UIS}
  ${mothHamsterWheel_GEN_RCS}
  ${mothHamsterWheel_GEN_HDRS}
)


#--------------------------------------------------------------------------------
# Tell CMake which libraries we need to link our executable against.
TARGET_LINK_LIBRARIES ( mothHamsterWheel  ${QT_LIBRARIES} ${Boost_DATE_TIME_LIBRARY} ${XLS_LIB_LIB_DIR})



#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
SET(plugin_dest_dir bin)
SET(qtconf_dest_dir bin)
SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/mothHamsterWheel")
IF(APPLE)
  SET(plugin_dest_dir mothHamsterWheel.app/Contents/MacOS)
  SET(qtconf_dest_dir mothHamsterWheel.app/Contents/Resources)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/mothHamsterWheel.app")
ENDIF(APPLE)
IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/mothHamsterWheel.exe")
ENDIF(WIN32)

#--------------------------------------------------------------------------------
# Install the mothHamsterWheel application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
INSTALL(TARGETS mothHamsterWheel
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${plugin_dest_dir}/plugins COMPONENT Runtime)

#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
INSTALL(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
    " COMPONENT Runtime)


#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# directories to look for dependencies
SET(DIRS ${QT_LIBRARY_DIRS})

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.
INSTALL(CODE "
    file(GLOB_RECURSE QTPLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
    " COMPONENT Runtime)


# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well
set(CPACK_BINARY_DRAGNDROP ON)
include(CPack)
