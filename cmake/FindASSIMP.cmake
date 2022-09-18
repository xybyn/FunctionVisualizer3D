# - Try to find Assimp
# Once done, this will define
#
# ASSIMP_FOUND - system has Assimp
# ASSIMP_INCLUDE_DIR - the Assimp include directories
# ASSIMP_LIBRARIES - link these to use Assimp
FIND_PATH( assimp_INCLUDE_DIR assimp/mesh.h
	/usr/include
	/usr/local/include
	/opt/local/include
	"${CMAKE_SOURCE_DIR}/include"
)
FIND_LIBRARY( assimp_LIBRARIES assimp
	/usr/lib64
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	"${CMAKE_SOURCE_DIR}/lib"
)
IF(assimp_INCLUDE_DIR AND assimp_LIBRARIES)
	SET( ASSIMP_FOUND TRUE )
	SET( assimp_LIBRARIES ${assimp_LIBRARIES} )
ENDIF(assimp_INCLUDE_DIR AND assimp_LIBRARIES)
IF(ASSIMP_FOUND)
	IF(NOT ASSIMP_FIND_QUIETLY)
	MESSAGE(STATUS "Found ASSIMP: ${assimp_LIBRARIES}")
	ENDIF(NOT ASSIMP_FIND_QUIETLY)
ELSE(ASSIMP_FOUND)
	IF(ASSIMP_FIND_REQUIRED)
	MESSAGE(FATAL_ERROR "Could not find libASSIMP")
	ENDIF(ASSIMP_FIND_REQUIRED)
ENDIF(ASSIMP_FOUND)