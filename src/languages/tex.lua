local L = {}

L.lexer = "tex"

L.singleLineComment = "% "

L.extensions = {
	"tex",
}

L.keywords = {
}

L.styles = {
	["DEFAULT"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["SPECIAL"] = {
		id = 1,
		fgColor = rgb(0xFF8000),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["GROUP"] = {
		id = 2,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["SYMBOL"] = {
		id = 3,
		fgColor = rgb(0x800000),
		bgColor = rgb(0x000000),
	},
	["COMMAND"] = {
		id = 4,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["TEXT"] = {
		id = 5,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
}
return L
