add_executable(lexilla_unit_tests EXCLUDE_FROM_ALL
    unitTest.cxx
    testCharacterSet.cxx
    testInList.cxx
    testLexerSimple.cxx
    testOptionSet.cxx
    testPropSetSimple.cxx
    testSparseState.cxx
    testWordList.cxx
)

target_include_directories(lexilla_unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lexilla_unit_tests PRIVATE lexilla)
# Catch v2 triggers deprecation warnings under C++20 with -Wall
target_compile_options(lexilla_unit_tests PRIVATE -Wno-error)

add_test(NAME lexilla_unit COMMAND lexilla_unit_tests)
