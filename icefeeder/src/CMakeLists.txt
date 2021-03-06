set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(Taglib REQUIRED)
set(CMAKE_REQUIRED_INCLUDES "${TAGLIB_INCLUDES}")
set(CMAKE_REQUIRED_LIBRARIES "${TAGLIB_LIBRARIES}")

find_package(LIBSHOUT REQUIRED)
set(CMAKE_REQUIRED_INCLUDES "${LIBSHOUT_INCLUDES}")
set(CMAKE_REQUIRED_LIBRARIES "${LIBSHOUT_LIBRARIES}")

find_package(LIBBOOST_THREAD REQUIRED)
set(CMAKE_REQUIRED_INCLUDES "${LIBBOOST_THREAD_INCLUDES}")
set(CMAKE_REQUIRED_LIBRARIES "${LIBBOOST_THREAD_LIBRARIES}")

find_package(LIBBOOST_BIND REQUIRED)
set(CMAKE_REQUIRED_INCLUDES "${LIBBOOST_THREAD_INCLUDES}")

find_package(LIBBOOST_PTR_VECTOR REQUIRED)
set(CMAKE_REQUIRED_INCLUDES "${LIBBOOST_PTR_VECTOR_INCLUDES}")

find_package(LIBBOOST_DATE_TIME REQUIRED)
set(CMAKE_REQUIRED_INCLUDES "${LIBBOOST_DATE_TIME_INCLUDES}")
set(CMAKE_REQUIRED_LIBRARIES "${LIBBOOST_DATE_TIME_LIBRARIES}")

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${TAGLIB_INCLUDES}
  ${LIBSHOUT_INCLUDES}
  ${LIBBOOST_THREAD_INCLUDES}
  ${LIBBOOST_PTR_VECTOR_INCLUDES}
  ${LIBBOOST_DATE_TIME_INCLUDES}
  )

LINK_LIBRARIES(
  ${TAGLIB_LIBRARIES}
  ${LIBSHOUT_LIBRARIES}
  ${LIBBOOST_THREAD_LIBRARIES}
  ${LIBBOOST_DATE_TIME_LIBRARIES}
  )

set(icefeeder_SRCS
  configfile.cpp
  icefeeder.cpp
  log.cpp
  playlist.cpp
  stream.cpp
  streamconfig.cpp
  )

add_executable(icefeeder ${icefeeder_SRCS})
