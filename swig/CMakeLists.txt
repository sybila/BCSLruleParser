option(WITH_PYTHON "Build Python SWIG module" OFF)
option(WITH_PHP "Build PHP SWIG module" OFF)

if (WITH_PHP OR WITH_PYTHON)
	find_package(SWIG REQUIRED)
	include(${SWIG_USE_FILE})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})

	add_library(RuleParserInterface interface.cpp)
endif()

if (WITH_PYTHON)
	add_subdirectory(python)
endif()

if (WITH_PHP)
	add_subdirectory(php)
endif()
