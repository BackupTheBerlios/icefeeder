FIND_PATH(LIBBOOST_BIND_INCLUDE_DIR boost/bind.hpp)

IF (LIBBOOST_BIND_INCLUDE_DIR)
  SET(LIBBOOST_BIND_FOUND "YES")
ELSE (LIBBOOST_BIND_INCLUDE_DIR)
  SET(LIBBOOST_BIND_FOUND "NO")
ENDIF (LIBBOOST_BIND_INCLUDE_DIR)
 
IF (LIBBOOST_BIND_FOUND)
  IF (NOT LIBBOOST_BIND_FIND_QUIETLY)
    MESSAGE(STATUS "Found LIBBOOST_BIND includes: ${LIBBOOST_BIND_INCLUDE_DIR}")
  ENDIF (NOT LIBBOOST_BIND_FIND_QUIETLY)
  ELSE (LIBBOOST_BIND_FOUND)
  IF (LIBBOOST_BIND_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find LIBBOOST_BIND includes")
  ENDIF (LIBBOOST_BIND_FIND_REQUIRED)
ENDIF (LIBBOOST_BIND_FOUND)