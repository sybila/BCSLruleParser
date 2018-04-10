option(WITH_PYTHON "Build Python SWIG module" OFF)
option(WITH_PHP5 "Build PHP5 SWIG module" OFF)
option(WITH_PHP7 "Build PHP7 SWIG module" OFF)

if (WITH_PHP5 or WITH_PHP7 OR WITH_PYTHON)
	find_package(SWIG REQUIRED)
	include(${SWIG_USE_FILE})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})

	add_library(RuleParserInterface interface.cpp)
endif()

if (WITH_PYTHON)
	add_subdirectory(python)
endif()

if (WITH_PHP5 or WITH_PHP7)
	add_subdirectory(php)
endif()
