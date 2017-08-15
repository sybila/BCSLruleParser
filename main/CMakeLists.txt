option(WITH_BIN "Build executable" ON)

if (WITH_BIN)
	add_executable(RuleParserBin main.cpp)
	target_link_libraries(RuleParserBin RuleParserLib)
	install(TARGETS RuleParserBin DESTINATION bin)
endif()
