FIND_PATH(LIBBOOST_THREAD_INCLUDE_DIR boost/thread/thread.hpp)

FIND_LIBRARY(LIBBOOST_THREAD_LIBRARIES NAMES boost_thread)

IF (LIBBOOST_THREAD_LIBRARIES AND LIBBOOST_THREAD_INCLUDE_DIR)
  SET(LIBBOOST_THREAD_FOUND "YES")
ELSE (LIBBOOST_THREAD_LIBRARIES AND LIBBOOST_THREAD_INCLUDE_DIR)
  SET(LIBBOOST_THREAD_FOUND "NO")
ENDIF (LIBBOOST_THREAD_LIBRARIES AND LIBBOOST_THREAD_INCLUDE_DIR)
 
IF (LIBBOOST_THREAD_FOUND)
  IF (NOT LIBBOOST_THREAD_FIND_QUIETLY)
    MESSAGE(STATUS "Found LIBBOOST_THREAD: ${LIBBOOST_THREAD_LIBRARIES}")
  ENDIF (NOT LIBBOOST_THREAD_FIND_QUIETLY)
  ELSE (LIBBOOST_THREAD_FOUND)
  IF (LIBBOOST_THREAD_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find LIBBOOST_THREAD library")
  ENDIF (LIBBOOST_THREAD_FIND_REQUIRED)
ENDIF (LIBBOOST_THREAD_FOUND)
