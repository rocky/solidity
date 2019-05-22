add_executable(isolptest
	isoltest.cpp
	IsolTestOptions.cpp
	../Options.cpp
	../Common.cpp
	../TestCase.cpp
	../libsolidityp0/util/TestFileParser.cpp
	../libsolidityp0/SyntaxTest.cpp
	../libsolidityp0/AnalysisFramework.cpp
)
target_link_libraries(isolptest PRIVATE solidityp0 ${Boost_PROGRAM_OPTIONS_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

if (NOT Boost_USE_STATIC_LIBS)
    target_compile_definitions(isolptest PUBLIC -DBOOST_TEST_DYN_LINK)
  endif()

add_executable(tokens tokens.cpp)
target_link_libraries(tokens PRIVATE langutil)
