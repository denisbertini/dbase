# Create a library called "CbmStsDbQa" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.


Set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}
  ${FAIRROOT_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${BASE_INCLUDE_DIRECTORIES}

  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/src
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

Link_Directories(${LINK_DIRECTORIES})

set(SRCS
CbmStsDbQaContFact.cxx
CbmStsDbQaIvPar.cxx
CbmStsDbQaIvNewPar.cxx
CbmStsDbQaChannelMapPar.cxx
CbmStsDbQaGeometryPar.cxx
CbmStsDbQaOwnershipPar.cxx
CbmStsDbQaResultsPar.cxx
CbmStsDbQaSensorPar.cxx
CbmStsDbQaPar.cxx
CbmStsDbQaSensorNewPar.cxx
CbmStsDbQaNewPar.cxx
CbmStsDbSNoisyChPar.cxx
CbmStsDbSInfoPar.cxx
CbmStsDbQa.cxx
CbmStsDbQaSUID.cxx
CbmStsDbQaSensor.cxx
CbmStsDbQaIv.cxx
)

set(HEADERS
CbmStsDbQaContFact.h
CbmStsDbQaIvPar.h
CbmStsDbQaIvNewPar.h
CbmStsDbQaChannelMapPar.h
CbmStsDbQaGeometryPar.h
CbmStsDbQaOwnershipPar.h
CbmStsDbQaResultsPar.h
CbmStsDbQaSensorPar.h
CbmStsDbQaPar.h
CbmStsDbQaSensorNewPar.h
CbmStsDbQaNewPar.h
CbmStsDbSNoisyChPar.h
CbmStsDbSInfoPar.h
CbmStsDbQa.h
CbmStsDbQaSUID.h
CbmStsDbQaSensor.h
CbmStsDbQaIv.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbReader.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbWriter.h
)


set(LINKDEF CbmStsDbQaLinkDef.h)
Set(LIBRARY_NAME CbmStsDbQa)
Set(DEPENDENCIES Base ParBase FairDB boost_system boost_filesystem boost_regex)

GENERATE_LIBRARY()
