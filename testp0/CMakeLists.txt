file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

file(GLOB libsolidityp0_sources "libsolidityp0/*.cpp")
file(GLOB libdevcore_sources "libdevcore/*.cpp")
file(GLOB libdevcore_headers "libdevcore/*.h")
file(GLOB liblangutil_sources "liblangutil/*.cpp")
file(GLOB liblangutil_headers "liblangutil/*.h")

file(GLOB libsolidity_sources "libsolidityp0/*.cpp")
file(GLOB libsolidityp0_headers "libsolidityp0/*.h")
file(GLOB libsolidity_util_sources "libsolidityp0/util/*.cpp")
file(GLOB libsolidity_util_headers "libsolidityp0/util/*.h")

add_executable(solptest ${sources} ${headers}
    ${contracts_sources} ${contracts_headers}
    ${libdevcore_sources} ${libdevcore_headers}
    ${liblangutil_sources} ${liblangutil_headers}
    ${libsolidityp0_sources} ${libsolidityp0_headers}
    ${libsolidity_util_sources} ${libsolidity_util_headers}
)
target_link_libraries(solptest PRIVATE solidityp0 devcore ${Boost_PROGRAM_OPTIONS_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

if (NOT Boost_USE_STATIC_LIBS)
    target_compile_definitions(solptest PUBLIC -DBOOST_TEST_DYN_LINK)
endif()

add_subdirectory(tools)
