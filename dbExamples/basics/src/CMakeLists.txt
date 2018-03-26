 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "FairDbBasics" 

Set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}
  ${FAIRROOT_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${BASE_INCLUDE_DIRECTORIES}

  ${CMAKE_SOURCE_DIR}/dbExamples/basics/src
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
)

Link_Directories(${LINK_DIRECTORIES})

set(SRCS
FairDbTutPar.cxx
FairDbTutParBin.cxx
FairDbTutContFact.cxx
FairDbTutAccessRtdbTask.cxx
)

set(HEADERS
FairDbTutPar.h
FairDbTutParBin.h
FairDbTutContFact.h
FairDbTutAccessRtdbTask.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbReader.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbWriter.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbGenericParSet.h
)



set(LINKDEF FairDbTutLinkDef.h)
Set(LIBRARY_NAME FairDbBasics)
Set(DEPENDENCIES Base ParBase FairDB)

GENERATE_LIBRARY()



