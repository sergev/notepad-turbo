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
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["STRING"] = {
		id = 119,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["STRING SINGLE QUOTE"] = {
		id = 120,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["KEYWORD"] = {
		id = 121,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["NUMBER"] = {
		id = 122,
		fgColor = rgb(0xFF8000),
		bgColor = rgb(0x000000),
	},
	["VARIABLE"] = {
		id = 123,
		fgColor = rgb(0x800080),
		bgColor = rgb(0x000000),
	},
	["COMMENT"] = {
		id = 124,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["COMMENT LINE"] = {
		id = 125,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["OPERATOR"] = {
		id = 127,
		fgColor = rgb(0x000080),
		bgColor = rgb(0x000000),
	},
}

return L
