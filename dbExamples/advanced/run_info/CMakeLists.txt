# Create a library called "DbRunInfo"

set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}
  ${FAIRROOT_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${BASE_INCLUDE_DIRECTORIES}

  ${CMAKE_SOURCE_DIR}/include/
  ${CMAKE_SOURCE_DIR}/dbExamples/advanced/run_info
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

link_directories( ${LINK_DIRECTORIES})

set(SRCS
FairDbRunInfoContFact.cxx
FairDbRunInfo.cxx
)

set(HEADERS
FairDbRunInfoContFact.h
FairDbRunInfo.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbReader.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbWriter.h
${CMAKE_SOURCE_DIR}/dbInterface/FairDbGenericParSet.h
)


set(LINKDEF FairDbRunInfoLinkDef.h)
set(LIBRARY_NAME DbRunInfo)
set(DEPENDENCIES
      Base ParBase FairDB boost_system boost_filesystem boost_regex
)

GENERATE_LIBRARY()
