local L = {}

L.lexer = "registry"

L.singleLineComment = "; "

L.extensions = {
	"reg",
}

L.keywords = {
}

L.styles = {
	["DEFAULT STYLE"] = {
		id = 32,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["DEFAULT"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["COMMENT"] = {
		id = 1,
		fgColor = rgb(0x007F00),
		bgColor = rgb(0x000000),
	},
	["VALUE NAME"] = {
		id = 2,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
	},
	["STRING"] = {
		id = 3,
		fgColor = rgb(0x7F007F),
		bgColor = rgb(0x000000),
	},
	["HEX DIGIT"] = {
		id = 4,
		fgColor = rgb(0x7F0B0C),
		bgColor = rgb(0x000000),
	},
	["VALUE TYPE"] = {
		id = 5,
		fgColor = rgb(0x00007F),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["ADDED KEY"] = {
		id = 6,
		fgColor = rgb(0x530155),
		bgColor = rgb(0x000000),
	},
	["REMOVED KEY"] = {
		id = 7,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(0x000000),
	},
	["ESCAPED CHARACTERS IN STRINGS"] = {
		id = 8,
		fgColor = rgb(0x7D8187),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["GUID IN KEY PATH"] = {
		id = 9,
		fgColor = rgb(0x7B5F15),
		bgColor = rgb(0x000000),
	},
	["GUID IN STRING"] = {
		id = 10,
		fgColor = rgb(0x7B5F15),
		bgColor = rgb(0x000000),
	},
	["PARAMETER"] = {
		id = 11,
		fgColor = rgb(0x0B6561),
		bgColor = rgb(0x000000),
	},
	["OPERATORS"] = {
		id = 12,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
}
return L
