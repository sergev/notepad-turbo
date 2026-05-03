local L = {}

L.lexer = "makefile"

L.singleLineComment = "# "

L.extensions = {
	"mak",
	"mk",
}

L.keywords = {
}

L.styles = {
	["DEFAULT"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["COMMENT"] = {
		id = 1,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["PREPROCESSOR"] = {
		id = 2,
		fgColor = rgb(0x000080),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["IDENTIFIER"] = {
		id = 3,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
	},
	["OPERATOR"] = {
		id = 4,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["TARGET"] = {
		id = 5,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(0x000000),
	},
	["IDEOL"] = {
		id = 9,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 2,
	},
}
return L
