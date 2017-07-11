find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

set_property(SOURCE interface.i PROPERTY CPLUSPLUS ON)

SWIG_ADD_MODULE(RuleParserPy python interface.i)
SWIG_LINK_LIBRARIES(RuleParserPy RuleParserLib)
SWIG_LINK_LIBRARIES(RuleParserPy ${PYTHON_LIBRARIES})
