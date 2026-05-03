local L = {}

L.lexer = "latex"

L.singleLineComment = "% "

L.extensions = {
	"tex",
	"sty",
}

L.keywords = {
}

L.styles = {
	["WHITE SPACE"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["COMMAND"] = {
		id = 1,
		fgColor = rgb(0xAA0000),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["TAG OPENING"] = {
		id = 2,
		fgColor = rgb(0x880088),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["MATH INLINE"] = {
		id = 3,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
	},
	["COMMENT"] = {
		id = 4,
		fgColor = rgb(0x00AA00),
		bgColor = rgb(0x000000),
	},
	["TAG CLOSING"] = {
		id = 5,
		fgColor = rgb(0x880088),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["MATH BLOCK"] = {
		id = 6,
		fgColor = rgb(0x0000AA),
		bgColor = rgb(0x000000),
	},
	["COMMENT BLOCK"] = {
		id = 7,
		fgColor = rgb(0x00AA00),
		bgColor = rgb(0x000000),
	},
	["VERBATIM SEGMENT"] = {
		id = 8,
		fgColor = rgb(0x666666),
		bgColor = rgb(0x000000),
	},
	["SHORT COMMAND"] = {
		id = 9,
		fgColor = rgb(0xAA6600),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["SPECIAL CHAR"] = {
		id = 10,
		fgColor = rgb(0xAAAA00),
		bgColor = rgb(0x000000),
	},
	["COMMAND OPTIONAL ARGUMENT"] = {
		id = 11,
		fgColor = rgb(0xFF9900),
		bgColor = rgb(0x000000),
	},
	["SYNTAX ERROR"] = {
		id = 12,
		fgColor = rgb(0xffffff),
		bgColor = rgb(0x000000),
	},
}
return L
