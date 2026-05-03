add_subdirectory(lua)
add_subdirectory(lexilla)
add_subdirectory(uchardet)

# Lexilla needs Scintilla's ILexer.h and Sci_Position.h headers (header-only, no lib)
target_include_directories(lexilla PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/scintilla/include
)
