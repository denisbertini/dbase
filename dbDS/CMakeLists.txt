 # Date: 20.05.16 D.Bertini
# Create a library called "libFairDbDS" 
#
# All SQL drivers "includes" and "libraries" are not imeediately set 
# because of the problem i have with CMAKE and variable set to not-found
# Instead  those are added only if they have been previously set  


set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}
  ${FAIRROOT_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${BASE_INCLUDE_DIRECTORIES}

  ${CMAKE_SOURCE_DIR}/dbInterface
  ${CMAKE_SOURCE_DIR}/dbUtils
  ${CMAKE_SOURCE_DIR}/dbDS
  ${CMAKE_SOURCE_DIR}/dbDS/core
  ${CMAKE_SOURCE_DIR}/dbDS/utils
  ${CMAKE_SOURCE_DIR}/dbDS/procs
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIR}
)

include_directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

set(SRCS
  utils/FairDbArgMacFile.cxx
  utils/FairDbFloatXtrap.cxx
  utils/FairDbProc.cxx
  utils/FairDbProcArgs.cxx
  utils/FairDbProcCmd.cxx
  utils/FairDbProcProxy.cxx
  utils/FairDbProcRegistry.cxx
  utils/FairDbProcRootArgs.cxx
  core/FairDbADS.cxx
  core/FairDbFileSpotter.cxx
  core/FairDbTableSelector.cxx
  core/FairDbCmdOpts.cxx
  core/FairDbLogFile.cxx
  core/FairDbQAssessor.cxx
  core/FairDbTableSelector.cxx
)


CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
set(HEADERS ${HEADERS})
set(LINKDEF FairDbDSLinkDef.h)
set(LIBRARY_NAME FairDbDS)

if (Boost_FOUND)
  set(DEPENDENCIES
    Rint Base FairDB ParBase FairMQ ${ZMQ_LIBRARY_SHARED} boost_thread boost_system boost_serialization)
else (Boost_FOUND)
  set(DEPENDENCIES Rint Base FairDB ParBase)
endif (Boost_FOUND)

GENERATE_LIBRARY()

if (Boost_FOUND)
  set(Exe_Names
    dbadmin
    )

  set(Exe_Source
    procs/dbadmin.cxx		
    )

  list(LENGTH Exe_Names _length)
  math(EXPR _length ${_length}-1)

  foreach(_file RANGE 0 ${_length})
    list(GET Exe_Names ${_file} _name)
    list(GET Exe_Source ${_file} _src)
    set(EXE_NAME ${_name})
    set(SRCS ${_src})
    set(DEPENDENCIES Rint FairDB FairDbDS ParBase FairDbMQ ${ZMQ_LIBRARY_SHARED} boost_thread boost_system boost_serialization)
    GENERATE_EXECUTABLE()
  endforeach(_file RANGE 0 ${_length})
endif (Boost_FOUND)
