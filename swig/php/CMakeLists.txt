find_package(PHP REQUIRED)
if (WIN32)
	add_definitions(-DZEND_WIN32 -DPHP_WIN32 -DZEND_WIN32_FORCE_INLINE -DZEND_DEBUG=0 -DZTS=1 -DSNPRINTF_H=1)
endif()

include_directories(${PHP_INCLUDE_DIR})
include_directories(${PHP_INCLUDE_DIR}/main)
include_directories(${PHP_INCLUDE_DIR}/Zend)
include_directories(${PHP_INCLUDE_DIR}/TSRM)

set_property(SOURCE interface.i PROPERTY CPLUSPLUS ON)

if (WITH_PHP5)
	SWIG_ADD_MODULE(RuleParserPhp php interface.i)
else()
	SWIG_ADD_MODULE(RuleParserPhp php7 interface.i)
endif()

SWIG_LINK_LIBRARIES(RuleParserPhp RuleParserInterface)
SWIG_LINK_LIBRARIES(RuleParserPhp RuleParserLib)
SWIG_LINK_LIBRARIES(RuleParserPhp ${PHP_LIBRARIES})

install(TARGETS ${SWIG_MODULE_RuleParserPhp_REAL_NAME}  DESTINATION lib/php)
install(FILES ${CMAKE_BINARY_DIR}/swig/python/RuleParserPhp.php DESTINATION lib/php)
