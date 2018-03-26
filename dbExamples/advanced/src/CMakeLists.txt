# Create a library called "Advanced"

set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}
  ${FAIRROOT_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${BASE_INCLUDE_DIRECTORIES}

  ${CMAKE_SOURCE_DIR}/dbExamples/advanced/src
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${Boost_LIBRARY_DIRS}
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

link_directories( ${LINK_DIRECTORIES})

set(SRCS
FairDbExPar1.cxx
FairDbExPar2.cxx
FairDbExPar3.cxx
FairDbExContFact.cxx
FairDbTests.cxx
)

set(HEADERS
FairDbExPar1.h
FairDbExPar2.h
FairDbExPar3.h
FairDbExContFact.h
FairDbTests.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbReader.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbWriter.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbGenericParSet.h
)


set(LINKDEF FairDbExLinkDef.h)
set(LIBRARY_NAME DBAdvanced)
set(DEPENDENCIES
      Base ParBase FairDB boost_system boost_filesystem boost_regex
)

GENERATE_LIBRARY()
