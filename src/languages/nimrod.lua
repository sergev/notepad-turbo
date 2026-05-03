local L = {}

L.lexer = "nimrod"

L.singleLineComment = "! "

L.extensions = {
	"nim",
}

L.keywords = {
	[0] = "addr and as asm block break case cast const continue converter discard div elif else end enum except exception finally for from generic if implies import in include is isnot iterator lambda macro method mod nil not notin object of or out proc ptr raise ref return shl shr template try tuple type var when where while with without xor yield",
}

L.styles = {
	["WHITE SPACE"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["COMMENT"] = {
		id = 1,
		fgColor = rgb(0x007F00),
		bgColor = rgb(0x000000),
	},
	["NUMBER"] = {
		id = 2,
		fgColor = rgb(0x007F7F),
		bgColor = rgb(0x000000),
	},
	["STRING"] = {
		id = 3,
		fgColor = rgb(0x7F007F),
		bgColor = rgb(0x000000),
	},
	["SINGLE QUOTED STRING"] = {
		id = 4,
		fgColor = rgb(0x7F007F),
		bgColor = rgb(0x000000),
	},
	["KEYWORD"] = {
		id = 5,
		fgColor = rgb(0x00007F),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["TRIPLE QUOTES"] = {
		id = 6,
		fgColor = rgb(0x7F0000),
		bgColor = rgb(0x000000),
	},
	["TRIPLE DOUBLE QUOTES"] = {
		id = 7,
		fgColor = rgb(0x7F0000),
		bgColor = rgb(0x000000),
	},
	["CLASS NAME DEFINITION"] = {
		id = 8,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["FUNCTION OR METHOD NAME DEFINITION"] = {
		id = 9,
		fgColor = rgb(0x007F7F),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["OPERATORS"] = {
		id = 10,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["IDENTIFIERS"] = {
		id = 11,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["COMMENT-BLOCKS"] = {
		id = 12,
		fgColor = rgb(0x7F7F7F),
		bgColor = rgb(0x000000),
	},
	["END OF LINE WHERE STRING IS NOT CLOSED"] = {
		id = 13,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["HIGHLIGHTED IDENTIFIERS"] = {
		id = 14,
		fgColor = rgb(0x407090),
		bgColor = rgb(0x000000),
	},
	["DECORATORS"] = {
		id = 15,
		fgColor = rgb(0x805000),
		bgColor = rgb(0x000000),
	},
}
return L
