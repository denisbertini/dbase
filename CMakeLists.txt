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


Set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/dbase/cmake/modules" ${CMAKE_MODULE_PATH})


Set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}/fairtools
  ${CMAKE_SOURCE_DIR}/base
  ${CMAKE_SOURCE_DIR}/base/steer
  ${CMAKE_SOURCE_DIR}/base/event
  ${CMAKE_SOURCE_DIR}/base/field
  ${CMAKE_SOURCE_DIR}/base/sim
  ${CMAKE_SOURCE_DIR}/parbase
  ${CMAKE_SOURCE_DIR}/dbase
  ${CMAKE_SOURCE_DIR}/dbase/dbInterface
  ${CMAKE_SOURCE_DIR}/dbase/dbValidation
  ${CMAKE_SOURCE_DIR}/dbase/dbUtils
  ${CMAKE_SOURCE_DIR}/input/db
  ${CMAKE_SOURCE_DIR}/dbase/dbInput
  ${CMAKE_SOURCE_DIR}/dbase/dbIO
  ${CMAKE_SOURCE_DIR}/parbase
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${ROOT_INCLUDE_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

# ---- Libraries
Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
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
install(FILES "dbInput/db_detector_def.h" DESTINATION include)
install(FILES "dbInput/prepare_db.sql" DESTINATION share/dbInput)


CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
set(HEADERS ${HEADERS} ${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbReader.h ${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbWriter.h
  ${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbGenericParSet.h)
set(LINKDEF FairDBLinkDef.h)
set(LIBRARY_NAME FairDB)

if(ROOT_VERSION_MAJOR VERSION_LESS 6 )
set(DEPENDENCIES Net RIO Cint Core ParBase jsoncpp)
else()
set(DEPENDENCIES Net RIO Core ParBase jsoncpp)
endif()

GENERATE_LIBRARY()

if (ZMQ_FOUND AND Boost_FOUND)
  add_subdirectory(dbMQ)
  add_subdirectory(dbDS)
endif()

find_package(Wt)

if (Wt_FOUND AND Boost_FOUND)
add_subdirectory(dbWt) 
endif()

if (Boost_FOUND)
add_subdirectory(dbExamples) 
endif()


OPTION(JSONCPP_WITH_TESTS "Compile and (for jsoncpp_check) run JsonCpp test executables" OFF)
OPTION(JSONCPP_WITH_POST_BUILD_UNITTEST "Automatically run unit-tests as a post build step" OFF)
OPTION(BUILD_SHARED_LIBS "Build jsoncpp_lib as a shared library." ON)
OPTION(BUILD_STATIC_LIBS "Build jsoncpp_lib static library." ON)
add_subdirectory(db3rdParty/jsoncpp)
