
add_subdirectory(src)

if (Wt_FOUND AND Boost_FOUND)
  add_subdirectory(web)
  add_subdirectory(wui)
endif()


