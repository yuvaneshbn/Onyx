# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\onyx-voip_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\onyx-voip_autogen.dir\\ParseCache.txt"
  "onyx-voip_autogen"
  )
endif()
