local L = {}

L.lexer = "props"

L.singleLineComment = "; "

L.first_line = {
	"^%[.+%][\r\n]",
}

L.extensions = {
	"ini",
	"inf",
	"url",
	"wer",
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
	["SECTION"] = {
		id = 2,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
		fontStyle = 9,
	},
	["ASSIGNMENT"] = {
		id = 3,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["DEFVAL"] = {
		id = 4,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(0x000000),
	},
	["KEY"] = {
		id = 5,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
		fontStyle = 2,
	},
}
return L
