#
# Download GoogleTest
#
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_definitions(-DTEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DBUILD_DIR="${CMAKE_BINARY_DIR}")

set(TEST_SOURCES
    smoke_test.cpp
    util.cpp
    ini_settings_test.cpp
    sorter_test.cpp
    session_manager_test.cpp
    recent_files_test.cpp
    macro_test.cpp
    macro_recorder_test.cpp
    finder_test.cpp
)

add_executable(unit_tests EXCLUDE_FROM_ALL ${TEST_SOURCES})
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(unit_tests PRIVATE gtest_main nt_core)
gtest_discover_tests(unit_tests EXTRA_ARGS --gtest_repeat=1 PROPERTIES TIMEOUT 10)

set(UI_TEST_SOURCES
    util.cpp
    file_encoding_test.cpp
    nn_editor_test.cpp
    nn_window_test.cpp
    nn_document_test.cpp
    nn_idocument_test.cpp
)

add_executable(ui_tests EXCLUDE_FROM_ALL ${UI_TEST_SOURCES})
target_include_directories(ui_tests PRIVATE ${CMAKE_SOURCE_DIR}/test)
target_link_libraries(ui_tests PRIVATE gtest_main nt_ui)
gtest_discover_tests(ui_tests EXTRA_ARGS --gtest_repeat=1 PROPERTIES TIMEOUT 30)
