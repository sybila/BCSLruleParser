option(WITH_JS "Compile into JavaScript (requires Emscripten)" OFF)
option(WITH_WASM "Compile into WASM (requires Emscripten)" OFF)

if (WITH_JS OR WITH_WASM)
	add_executable(RuleParserJs main.cpp)
	target_link_libraries(RuleParserJs RuleParserLib)
	set_target_properties(RuleParserJs PROPERTIES LINK_FLAGS "-s EXPORTED_FUNCTIONS='[\"_RuleParserParse\", \"_RuleParserDealloc\"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\"]'")

	if (WITH_WASM)
		set_target_properties(RuleParserJs PROPERTIES LINK_FLAGS "-s WASM=1")
	endif()

	install(TARGETS RuleParserJs DESTINATION js)

	if (WITH_WASM)
		install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RuleParserJs.wasm DESTINATION js)
	else()
		install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RuleParserJs.js.mem DESTINATION js)
	endif()
endif()
