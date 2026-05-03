local L = {}

L.lexer = "gui4cli"

L.singleLineComment = "// "

L.keywords = {
	[0] = "None",
	[1] = "None",
	[2] = "None",
	[3] = "None",
	[4] = "None",
}

L.styles = {
	["DEFAULT"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["COMMENT LINE"] = {
		id = 1,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["COMMENT"] = {
		id = 2,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["GLOBAL"] = {
		id = 5,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["EVENT"] = {
		id = 5,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["ATTRIBUTE"] = {
		id = 16,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
	},
	["CONTROL"] = {
		id = 16,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
	},
	["COMMAND"] = {
		id = 16,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
	},
	["STRING"] = {
		id = 6,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["OPERATOR"] = {
		id = 10,
		fgColor = rgb(0x000080),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
}
return L
