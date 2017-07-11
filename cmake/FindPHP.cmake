find_path(PHP_INCLUDE_DIR main/php.h
	/usr/include/php4
	/usr/include/php5
	/usr/local/include/php4
	/usr/local/include/php5
	/usr/include/php
	/usr/local/include/php
	/usr/local/apache/php
	"c:/Program Files/php/include"
)

find_library(PHP_LIBRARIES
	NAMES libphp5ts.lib php5 php4 libphp
	PATHS ${LIBSBML_DEPENDENCY_DIR}/lib
		/usr/lib /usr/local/lib
		/usr/lib/i386-linux-gnu
	DOC "The file name of the PHP library."
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PHP REQUIRED_VARS PHP_INCLUDE_DIR PHP_LIBRARIES)

#MESSAGE("PHP_INCLUDE_PATH = ${PHP_INCLUDE_PATH}")
#MESSAGE("PHP_LIBRARIES    = ${PHP_LIBRARIES}")
