 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             #
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Date: 3.02.17 D.Bertini
# Create a library called "libCbmStsQaWtUi"

Message("-- Wt is FOUND .... compiling  CbmStsQaWtUi Project starting ... ")

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
  ${CMAKE_SOURCE_DIR}/dbWt/auth
  ${CMAKE_SOURCE_DIR}/dbWt/auth/model
  ${CMAKE_SOURCE_DIR}/dbInterface
  ${CMAKE_SOURCE_DIR}/dbValidation
  ${CMAKE_SOURCE_DIR}/dbInput
  ${CMAKE_SOURCE_DIR}/dbIO
  ${CMAKE_SOURCE_DIR}/dbUtils
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/src
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/wui/src
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/wui/src/main
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/wui/src/models
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/wui/src/utils
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/wui/src/widgets
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/wui/src/widgets/charts
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/wui/src/widgets/dialogs
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/wui/src/widgets/layouts
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/wui/src/widgets/views
  ${CMAKE_SOURCE_DIR}/dbExamples/cbm_sts/wui/src/db
 )

include_directories(${INCLUDE_DIRECTORIES})


set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

set(SRCS
  main/CbmStsWtQaWApplication.cxx
  widgets/layouts/CbmStsWtQaMainWidget.cxx
  widgets/layouts/CbmStsWtQaTreeTable.cxx
  widgets/layouts/CbmStsWtQaTreeTableNode.cxx
  widgets/views/CbmStsWtQaBaseView.cxx
  widgets/views/CbmStsWtQaBrowser.cxx
  widgets/views/CbmStsWtQaFolderView.cxx
  widgets/views/CbmStsWtQaFolderTreeView.cxx
  widgets/views/CbmStsWtQaPresenterView.cxx
  widgets/views/CbmStsWtQaVendorBatchView.cxx
  widgets/views/CbmStsWtQaViewsUtils.cxx
  widgets/views/CbmStsWtQaIvChartView.cxx
  widgets/views/CbmStsWtQaIvChartConfig.cxx
  widgets/views/CbmStsWtQaIvPanelList.cxx
  models/CbmStsWtQaSensorDataModel.cxx
  db/CbmStsWtQaDataObjects.cxx
  utils/CbmStsWtQaLogger.cxx
  utils/CbmStsWtQaUtils.cxx
)

set(LIBRARY_NAME CbmStsQaWtUi)

if (Boost_FOUND)
  set(DEPENDENCIES
    RHTTP
    Base FairDB ParBase
    boost_thread boost_system boost_serialization boost_signals boost_locale
    boost_date_time  boost_regex boost_program_options boost_signals boost_random boost_system
    ${Wt_LIBRARY} ${Wt_HTTP_LIBRARY} ${Wt_LIBRARIES}
)
else (Boost_FOUND)
  # will not work anyway without boost !
  set(DEPENDENCIES Base FairDB ParBase
    ${Wt_LIBRARY} ${Wt_HTTP_LIBRARY}
)
endif (Boost_FOUND)

GENERATE_LIBRARY()

if (Boost_FOUND)

  set(Exe_Names
    stsqawt.wt
  )

  set(Exe_Source
      main/stsqawt.cxx
  )

  list(LENGTH Exe_Names _length)
  math(EXPR _length ${_length}-1)

  foreach(_file RANGE 0 ${_length})
    list(GET Exe_Names ${_file} _name)
    list(GET Exe_Source ${_file} _src)
    set(EXE_NAME ${_name})
    set(SRCS ${_src})
    set(DEPENDENCIES CbmStsQaWtUi CbmStsDbQa FairDbWt)
    GENERATE_EXECUTABLE()
  endforeach(_file RANGE 0 ${_length})
endif (Boost_FOUND)
