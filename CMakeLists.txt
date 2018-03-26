# CMake project for library compilation against FAIR ROOT
# Intended to be used for out of source compilation
# Based on CbmROOT

# Check if cmake has the required version
CMAKE_MINIMUM_REQUIRED(VERSION 3.3.2 FATAL_ERROR)

# Set name of our project to "FAIRDB". Has to be done
# after check of cmake version
project(FAIRDB)

# Check if we are on an UNIX system. If not stop with an error
# message
IF(NOT UNIX)
  MESSAGE(FATAL_ERROR "You're not on an UNIX system. The project was up to now only tested on UNIX systems, so we break here. If you want to go on please edit the CMakeLists.txt in the source directory.")
ENDIF(NOT UNIX)


#Check if necessary environment variables are set
#If not stop execution
IF(NOT DEFINED ENV{SIMPATH})
  MESSAGE(FATAL_ERROR "You did not define the environment variable SIMPATH which is nedded to find the external packages. Please set this variable and execute cmake again.")
ENDIF(NOT DEFINED ENV{SIMPATH})
SET(SIMPATH $ENV{SIMPATH})

IF(NOT DEFINED ENV{FAIRROOTPATH})
   MESSAGE(FATAL_ERROR "You did not define the environment variable FAIRROOTPATH which is needed to find FairRoot. Please set this variable and execute cmake again.")
ENDIF(NOT DEFINED ENV{FAIRROOTPATH})
SET(FAIRROOTPATH $ENV{FAIRROOTPATH})


# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/
# is checked
set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules_old" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules"  ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})


# Check if the compiler support specific C++11 features
# Up to now this is only a check since the code does not use
# any of the features of the new standard

IF(FAIRROOTPATH)
  Set(CheckSrcDir "${FAIRROOTPATH}/share/fairbase/cmake/checks")
ELSE(FAIRROOTPATH)
  Set(CheckSrcDir "${CMAKE_SOURCE_DIR}/cmake/checks")
ENDIF(FAIRROOTPATH)

if(FAIRROOTPATH)
  find_package(FairRoot)

  IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX ${FAIRROOTPATH} CACHE PATH "Install path prefix" FORCE)
  ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

  SET(MY_ROOT_INCLUDE_PATH ${FAIRROOT_INCLUDE_DIR})
  If(CMAKE_INSTALL_PREFIX)
     # Message("***** HERE *****")
     SET(MY_ROOT_INCLUDE_PATH ${MY_ROOT_INCLUDE_PATH} "${CMAKE_INSTALL_PREFIX}/include")
  EndIf(CMAKE_INSTALL_PREFIX)
endif(FAIRROOTPATH)


# Load some basic macros which are needed later on
include(FairMacros)
include(WriteConfigFile)
include(CheckCompiler)
Include(ExternalProject)
include(CheckCXX11Features)


# Check if the used compiler support C++11. If not stop with an error message
# Check also if FairSoft and FairRoot have been compiled with C++11 support
If(NOT _HAS_CXX11_FLAG)
  Message(FATAL_ERROR "The used C++ compiler (${CMAKE_CXX_COMPILER}) does not support C++11. CbmRoot can only be compiled with compilers supporting C++11. Please install such an compiler.")
EndIf()


#Check the compiler and set the compile and link flags
Check_Compiler()


Set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
Set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
Set(INCLUDE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/include")

Option(USE_PATH_INFO "Information from PATH and LD_LIBRARY_PATH are used." OFF)
If(USE_PATH_INFO)
  Set(PATH $ENV{PATH})
  If (APPLE)
    Set(LD_LIBRARY_PATH $ENV{DYLD_LIBRARY_PATH})
  Else (APPLE)
    Set(LD_LIBRARY_PATH $ENV{LD_LIBRARY_PATH})
  EndIf (APPLE)
Else(USE_PATH_INFO)
  STRING(REGEX MATCHALL "[^:]+" PATH $ENV{PATH})
EndIf(USE_PATH_INFO)

# Check if the user wants to build the project in the source
# directory
CHECK_OUT_OF_SOURCE_BUILD()

# Check if the external packages are installed into a separate install
# directory
CHECK_EXTERNAL_PACKAGE_INSTALL_DIR()


find_package(ROOT 5.32.00 REQUIRED)


# Setup Boost
Set(Boost_NO_SYSTEM_PATHS TRUE)
Set(Boost_NO_BOOST_CMAKE TRUE)

If(${ROOT_LIBRARY_DIR} MATCHES /lib/root)
  set(BOOST_ROOT ${SIMPATH})
Else(${ROOT_LIBRARY_DIR} MATCHES /lib/root)
  set(BOOST_ROOT ${SIMPATH}/basics/boost)
EndIf(${ROOT_LIBRARY_DIR} MATCHES /lib/root)
Message("-- Looking for Boost ...")
# If an older version of boost is found both of the variables below are
# cached and in a second cmake run, a good boost version is found even
# if the version is to old.
# To overcome this problem both variables are cleared before checking
# for boost.
Unset(Boost_INCLUDE_DIR CACHE)
Unset(Boost_LIBRARY_DIRS CACHE)
find_package(Boost 1.48 COMPONENTS program_options thread system timer serialization regex)
If (Boost_FOUND)
  Set(Boost_Avail 1)
Else (Boost_FOUND)
  Set(Boost_Avail 0)
EndIf (Boost_FOUND)


find_package(ZeroMQ 4.1.3)
find_package(MYSQL)
find_package(PGSQL)
find_package(SQLITE)
find_package(Wt)

# set a variable which should be used in all CMakeLists.txt
# Defines all basic include directories from fairbase
SetBasicVariables()

Set(BASE_INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${SYSTEM_INCLUDE_DIRECTORIES}
)

# Set the library version in the main CMakeLists.txt
SET(FAIRDB_MAJOR_VERSION 1)
SET(FAIRDB_MINOR_VERSION 0)
SET(FAIRDB_PATCH_VERSION 0)
SET(FAIRDB_VERSION "${FAIRDB_MAJOR_VERSION}.${FAIRDB_MINOR_VERSION}.${FAIRDB_PATCH_VERSION}")
If(NOT ROOT_FOUND_VERSION OR ROOT_FOUND_VERSION LESS 59999)
  SET(FAIRROOT_LIBRARY_PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES}
      VERSION "${FAIRDB_VERSION}"
      SOVERSION "${FAIRDB_MAJOR_VERSION}"
      SUFFIX ".so"
  )
Else()
  SET(FAIRROOT_LIBRARY_PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES}
      VERSION "${FAIRDB_VERSION}"
      SOVERSION "${FAIRDB_MAJOR_VERSION}"
  )
EndIf()

SET(FAIRDBLIBDIR ${CMAKE_BINARY_DIR}/lib)
SET(LD_LIBRARY_PATH  ${FAIRDBLIBDIR} ${Boost_LIBRARY_DIRS} ${LD_LIBRARY_PATH})

If(CMAKE_SYSTEM_NAME MATCHES Darwin)
  Include(CheckIncludeFile)
  Check_Include_File("_types/_uint8_t.h" _UINT8_T_EXIST)
EndIf()




 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             #
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "libFairDB" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(BASE_INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/dbInterface
  ${CMAKE_SOURCE_DIR}/dbValidation
  ${CMAKE_SOURCE_DIR}/dbUtils
  ${CMAKE_SOURCE_DIR}/dbInput
  ${CMAKE_SOURCE_DIR}/dbIO
  ${CMAKE_SOURCE_DIR}/db3rdParty/

  ${FAIRROOT_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${BASE_INCLUDE_DIRECTORIES}
)

Set(INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${ROOT_INCLUDE_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

# ---- Libraries
Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
)

Link_Directories(${LINK_DIRECTORIES})


# ----- FairDb Code
set(SRCS
dbValidation/Experiment.cxx
dbValidation/FairDbDetector.cxx
dbValidation/DataType.cxx
dbValidation/ValCondition.cxx
dbValidation/ValInterval.cxx
dbValidation/ValTimeStamp.cxx
dbInterface/FairDb.cxx
dbInterface/FairDbConnection.cxx
dbInterface/FairDbBufferFile.cxx
dbInterface/FairDbCache.cxx
dbInterface/FairDbConfig.cxx
dbInterface/FairDbConfigNotice.cxx
dbInterface/FairDbConfigurable.cxx
dbInterface/FairDbConfigData.cxx
dbInterface/FairDbFieldType.cxx
dbInterface/FairDbTableMetaData.cxx
dbInterface/FairDbConnectionPool.cxx
dbInterface/FairDbOutTableBuffer.cxx
dbInterface/FairDbProxy.cxx
dbInterface/FairDbResult.cxx
dbInterface/FairDbRollbackTimes.cxx
dbInterface/FairDbTableBuffer.cxx
dbInterface/FairDbStatement.cxx
dbInterface/FairDbString.cxx
dbInterface/FairDbTableInterface.cxx
dbInterface/FairDbTableInterfaceStore.cxx
dbInterface/FairDbObjTableMap.cxx
dbInterface/FairDbStopWatch.cxx
dbInterface/FairDbStopWatchManager.cxx
dbInterface/FairDbValRecord.cxx
dbInterface/FairDbValRecordFactory.cxx
dbInterface/FairRegistry.cxx
dbInterface/FairRegistryElement.cxx
dbInterface/FairRegistryElementGeneric.cxx
dbInterface/FairUtilStream.cxx
dbInterface/FairDbLogEntry.cxx
dbInterface/FairDbConfigBuffer.cxx
dbInterface/FairDbConnectionManager.cxx
dbInterface/FairDbExtSqlContent.cxx
dbInterface/FairDbSqlValidityData.cxx
dbInterface/FairDbException.cxx
dbInterface/FairDbExceptionLog.cxx
dbInterface/FairDbUtils.cxx
dbInterface/FairDbValRecordMap.cxx
dbInterface/FairDbDataTypeUnion.cxx
dbInterface/FairDbStreamer.cxx
dbInterface/FairDbParSet.cxx
dbInterface/FairDbParFactory.cxx
dbInterface/FairDbUtilType.cxx
dbInterface/FairDbUser.cxx
dbUtils/FairDbLog.cxx
dbUtils/FairDbLogFormat.cxx
dbUtils/FairDbLogOSStream.cxx
dbUtils/FairDbLogService.cxx
dbUtils/FairDbLogStat.cxx
dbUtils/FairDbLogStream.cxx
dbUtils/FairDbLogUtil.cxx
### FairDb IO
dbIO/FairParTSQLIo.cxx
dbIO/FairDetParTSQLIo.cxx
dbIO/FairGenericParTSQLIo.cxx
)

install(FILES "dbUtils/FairDbLogBFormat.h" DESTINATION include)
install(FILES "dbInterface/FairDbReader.tpl" DESTINATION include)
install(FILES "dbInterface/FairDbWriter.tpl" DESTINATION include)
install(FILES "dbInterface/FairDbGenericParSet.tpl" DESTINATION include)
install(FILES "dbInterface/FairDbRelationalParSet.tpl" DESTINATION include)
install(FILES "dbInput/db_detector_def.h" DESTINATION include)
install(FILES "dbInput/prepare_db.sql" DESTINATION share/dbInput)

### 3rdParty
install(DIRECTORY "db3rdParty/jsoncons" DESTINATION include)
install(DIRECTORY "db3rdParty/jsoncons_ext" DESTINATION include)

### cmake modules
install(FILES "cmake/modules/FindWt.cmake" DESTINATION share/fairbase/cmake/modules)
install(FILES "cmake/modules/FindFairDb.cmake" DESTINATION share/fairbase/cmake/modules)

CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
set(HEADERS ${HEADERS} ${CMAKE_SOURCE_DIR}/dbInterface/FairDbReader.h ${CMAKE_SOURCE_DIR}/dbInterface/FairDbWriter.h
  ${CMAKE_SOURCE_DIR}/dbInterface/FairDbGenericParSet.h ${CMAKE_SOURCE_DIR}/dbInterface/FairDbRelationalParSet.h)

set(LINKDEF FairDBLinkDef.h)
set(LIBRARY_NAME FairDB)

if(ROOT_VERSION_MAJOR VERSION_LESS 6 )
set(DEPENDENCIES Net RIO Cint Core ParBase)
else()
set(DEPENDENCIES Net RIO Core ParBase)
endif()

GENERATE_LIBRARY()

if (ZMQ_FOUND AND Boost_FOUND)
  add_subdirectory(dbMQ)
  add_subdirectory(dbDS)
endif()

if (Wt_FOUND AND Boost_FOUND)
add_subdirectory(dbWt)
endif()

if (Boost_FOUND)
add_subdirectory(dbExamples)
endif()

add_subdirectory(dbUtils/Qr)
