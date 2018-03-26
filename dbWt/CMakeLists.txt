 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             #
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Date: 18.02.15 D.Bertini
# Create a library called "libFairDbWt"

Message("-- Wt is FOUND .... compiling FairDbWt Project starting ... ")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-potentially-evaluated-expression")

set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}
  ${FAIRROOT_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${BASE_INCLUDE_DIRECTORIES}

  ${Wt_INCLUDE_DIR}
  ${Wt_INCLUDE_DIR}/Wt
  ${CMAKE_SOURCE_DIR}/dbWt
  ${CMAKE_SOURCE_DIR}/dbWt/treeview
  ${CMAKE_SOURCE_DIR}/dbWt/graphics
  ${CMAKE_SOURCE_DIR}/dbWt/auth/model
  ${CMAKE_SOURCE_DIR}/dbWt/REST
  ${CMAKE_SOURCE_DIR}/dbInterface
  ${CMAKE_SOURCE_DIR}/dbValidation
  ${CMAKE_SOURCE_DIR}/dbInput
  ${CMAKE_SOURCE_DIR}/dbIO
  ${CMAKE_SOURCE_DIR}/dbUtils
)

include_directories(${INCLUDE_DIRECTORIES})

if (Boost_FOUND)
  Message("-- FairDbWt ... copying config file ...")
  file(COPY  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/macros/dbconfig.sh DESTINATION ${CMAKE_BINARY_DIR}/bin/ )
endIf (Boost_FOUND)

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

set(NO_DICT_SRCS
auth/model/User.C
auth/model/Session.C
FairDbWtApplication.cxx
FairDbWtMainWidget.cxx
FairDbWtConnMySQLWidget.cxx
FairDbWtItemsTpl.cxx
FairDbWtParFactory.cxx
treeview/FairDbWtFileModel.cxx
treeview/FairDbWtFileEditDialog.cxx
treeview/FairDbWtTreeView.cxx
treeview/FairDbWtParTreeTable.cxx
treeview/FairDbWtParTreeTableNode.cxx
treeview/FairDbWtPaintedImage.cxx
treeview/CsvUtil.cxx
treeview/FolderView.cxx
graphics/FairDbWtTCanvas.cxx
graphics/FairDbWtTCanvasWidget.cxx
graphics/FairDbWtRootBrowser.cxx
graphics/FairDbWtRootCanvas.cxx
graphics/FairDbWtFileLoader.cxx
graphics/FairDbWtFileTreeTable.cxx
graphics/FairDbWtFileTreeTableNode.cxx
graphics/FairDbWtTFileNode.cxx
graphics/FairDbWtJsRootProxy.cxx
graphics/FairDbWtConfigViewDialog.cxx
graphics/FairDbWtCanvasWidget.cxx
graphics/FairDbWtRtopFrame.cxx

REST/FairDbWtUserSessionStore.cxx
REST/FairDbWtBaseResource.cxx
REST/FairDbWtAuthResource.cxx
REST/FairDbWtUserResource.cxx
REST/FairDbWtAdminResource.cxx
)

set(SRCS
REST/FairDbWtPasswordService.cxx
)

CHANGE_FILE_EXTENSION(*.cxx *.h INSTALL_HEADERS "${NO_DICT_SRCS}")
CHANGE_FILE_EXTENSION(*.C *.h INSTALL_HEADERS "${INSTALL_HEADERS}")

foreach ( hdr ${INSTALL_HEADERS} )
    get_filename_component( dir ${hdr} DIRECTORY )
    install( FILES ${hdr} DESTINATION include )
endforeach()

install(FILES "REST/FairDbWtGenericResource.h" DESTINATION include)
install(FILES "REST/FairDbWtGenericResource.tpl" DESTINATION include)

set(LINKDEF FairDbWtLinkDef.h)
set(LIBRARY_NAME FairDbWt)

if (Boost_FOUND)
  set(DEPENDENCIES
    RHTTP Base FairDB ParBase Tree Postscript
    boost_thread boost_system boost_serialization boost_signals
    boost_date_time  boost_regex boost_program_options boost_signals boost_random boost_system
    ${Wt_LIBRARY} ${Wt_HTTP_LIBRARY} ${Wt_LIBRARIES}
)

else (Boost_FOUND)
  # will not work anyway without boost !
  set(DEPENDENCIES Base FairDB ParBase Tree Postscript
    ${Wt_LIBRARY} ${Wt_HTTP_LIBRARY}
)
endif (Boost_FOUND)

# Add libcrypt in case of Linux OSes
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(DEPENDENCIES ${DEPENDENCIES} crypt)
    message(STATUS ">>> --FairDbWt-- Found Linux ---> ${DEPENDENCIES}")
endif()

GENERATE_LIBRARY()

if (Boost_FOUND)

  set(Exe_Names
    fairdbwt.wt
  )

  set(Exe_Source
    fairdbwt.cxx
  )

  list(LENGTH Exe_Names _length)
  math(EXPR _length ${_length}-1)

  foreach(_file RANGE 0 ${_length})
    list(GET Exe_Names ${_file} _name)
    list(GET Exe_Source ${_file} _src)
    set(EXE_NAME ${_name})
    set(SRCS ${_src})
    set(DEPENDENCIES FairDbWt)
    GENERATE_EXECUTABLE()
  endforeach(_file RANGE 0 ${_length})
endif (Boost_FOUND)

#add_subdirectory(examples)
