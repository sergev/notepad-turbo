local L = {}

L.lexer = "abaqus"

L.singleLineComment = ""

L.extensions = {
	"inp",
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
	["NUMBER"] = {
		id = 3,
		fgColor = rgb(0xFF8000),
		bgColor = rgb(0x000000),
	},
	["STRING"] = {
		id = 4,
		fgColor = rgb(0xFF8000),
		bgColor = rgb(0x000000),
	},
	["OPERATOR"] = {
		id = 5,
		fgColor = rgb(0x000080),
		bgColor = rgb(0x000000),
	},
	["PROCESSORS"] = {
		id = 7,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
	},
	["STARCOMMAND"] = {
		id = 10,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
	},
	["ARGUMENT"] = {
		id = 11,
		fgColor = rgb(0x8800FF),
		bgColor = rgb(0x000000),
	},
}
return L
