local L = {}

L.lexer = "matlab"

L.singleLineComment = "% "

L.extensions = {
	"m",
}

L.keywords = {
	[0] = "break case catch classdef continue else elseif end for function global if otherwise parfor persistent return switch try while",
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
	["COMMAND"] = {
		id = 2,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
	},
	["NUMBER"] = {
		id = 3,
		fgColor = rgb(0xFF8000),
		bgColor = rgb(0x000000),
	},
	["INSTRUCTION WORD"] = {
		id = 4,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["STRING"] = {
		id = 5,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["OPERATOR"] = {
		id = 6,
		fgColor = rgb(0x000080),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["IDENTIFIER"] = {
		id = 7,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["DOUBLE QUOTE STRING"] = {
		id = 8,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
}
return L
