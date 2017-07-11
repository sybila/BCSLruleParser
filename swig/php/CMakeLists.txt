find_package(PHP REQUIRED)
if (WIN32)
	add_definitions(-DZEND_WIN32 -DPHP_WIN32 -DZEND_WIN32_FORCE_INLINE -DZEND_DEBUG=0 -DZTS=1 -DSNPRINTF_H=1)
endif()

include_directories(${PHP_INCLUDE_DIR})
include_directories(${PHP_INCLUDE_DIR}/main)
include_directories(${PHP_INCLUDE_DIR}/Zend)
include_directories(${PHP_INCLUDE_DIR}/TSRM)

set_property(SOURCE interface.i PROPERTY CPLUSPLUS ON)

SWIG_ADD_MODULE(RuleParserPhp php interface.i)
SWIG_LINK_LIBRARIES(RuleParserPhp RuleParserLib)
SWIG_LINK_LIBRARIES(RuleParserPhp ${PHP_LIBRARIES})
