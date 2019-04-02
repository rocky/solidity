file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

file(GLOB libsolidity_sources "libsolidity/*.cpp")
file(GLOB libsolidity_headers "libsolidity/*.h")
file(GLOB liblangutil_sources "liblangutil/*.cpp")
# file(GLOB liblangutil_headers "liblangutil/*.h")

# Add below
# ${liblangutil_sources} ${liblangutil_headers}
add_executable(solptest ${sources} ${headers}
    ${contracts_sources} ${contracts_headers}
    ${libsolidity_sources} ${libsolidity_headers}
)
target_link_libraries(solptest PRIVATE solidity ${Boost_PROGRAM_OPTIONS_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

if (NOT Boost_USE_STATIC_LIBS)
    target_compile_definitions(solptest PUBLIC -DBOOST_TEST_DYN_LINK)
endif()
