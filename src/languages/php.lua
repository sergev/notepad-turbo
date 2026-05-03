local L = {}

-- Lexilla registers PHP embedded lexer as "phpscript" (see LexHTML.cxx / lmPHPSCRIPT).
L.lexer = "phpscript"

L.extensions = {
	"php",
	"php3",
	"php4",
	"php5",
	"phtml",
}

L.styles = {
	["DEFAULT"] = {
		id = 118,
		fgColor = rgb(0x000000),
		bgColor = rgb(0xFFFFFF),
	},
	["STRING"] = {
		id = 119,
		fgColor = rgb(0x808080),
		bgColor = rgb(0xFFFFFF),
	},
	["STRING SINGLE QUOTE"] = {
		id = 120,
		fgColor = rgb(0x808080),
		bgColor = rgb(0xFFFFFF),
	},
	["KEYWORD"] = {
		id = 121,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0xFFFFFF),
		fontStyle = 1,
	},
	["NUMBER"] = {
		id = 122,
		fgColor = rgb(0xFF8000),
		bgColor = rgb(0xFFFFFF),
	},
	["VARIABLE"] = {
		id = 123,
		fgColor = rgb(0x800080),
		bgColor = rgb(0xFFFFFF),
	},
	["COMMENT"] = {
		id = 124,
		fgColor = rgb(0x008000),
		bgColor = rgb(0xFFFFFF),
	},
	["COMMENT LINE"] = {
		id = 125,
		fgColor = rgb(0x008000),
		bgColor = rgb(0xFFFFFF),
	},
	["OPERATOR"] = {
		id = 127,
		fgColor = rgb(0x000080),
		bgColor = rgb(0xFFFFFF),
	},
}

return L
