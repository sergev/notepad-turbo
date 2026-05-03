local L = {}

L.name = "Text"

L.lexer = "null"

L.disableFoldMargin = true

L.extensions = {
    "",
    "txt",
}

L.styles = {
    ["DEFAULT"] = {
        id = 0,
        fgColor = rgb(0xa0a0a0),
        bgColor = rgb(0x000000),
        fontStyle = 0,
    },
}

return L
