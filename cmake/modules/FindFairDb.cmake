 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             #
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Find FairDb installation
# Check the environment variable "FAIRDBPATH"

if(FairDb_DIR)
  set(FAIRDBPATH ${FairDb_DIR})
else()
  if(NOT DEFINED ENV{FAIRDBPATH})
    set(user_message "You did not define the environment variable FAIRDBPATH which is needed to find FairDb.\
         Please set this variable and execute cmake again." )
    if(FairDb_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR ${user_message})
    else(FairDb_FIND_REQUIRED)
      MESSAGE(WARNING ${user_message})
      return()
    endif(FairDb_FIND_REQUIRED)
  endif(NOT DEFINED ENV{FAIRDBPATH})

  set(FAIRDBPATH $ENV{FAIRDBPATH})
endif()

MESSAGE(STATUS "Setting FairDb environment…")

FIND_PATH(FAIRDB_INCLUDE_DIR NAMES FairDb.h  PATHS
  ${FAIRDBPATH}/include
  NO_DEFAULT_PATH
)

FIND_PATH(FAIRDB_LIBRARY_DIR NAMES libFairDB.so libFairDB.dylib PATHS
   ${FAIRDBPATH}/lib
  NO_DEFAULT_PATH
)

if(FAIRDB_INCLUDE_DIR AND FAIRDB_LIBRARY_DIR)
   set(FAIRDB_FOUND TRUE)
   MESSAGE(STATUS "FairDb ... - found ${FAIRDBPATH}")
   MESSAGE(STATUS "FairDb Library directory  :     ${FAIRDB_LIBRARY_DIR}")
   MESSAGE(STATUS "FairDb Include path…      :     ${FAIRDB_INCLUDE_DIR}")

else(FAIRDB_INCLUDE_DIR AND FAIRDB_LIBRARY_DIR)
   set(FAIRDB_FOUND FALSE)
   MESSAGE(FATAL_ERROR "FairDb installation not found")
endif (FAIRDB_INCLUDE_DIR AND FAIRDB_LIBRARY_DIR)

