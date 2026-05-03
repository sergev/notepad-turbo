local L = {}

L.lexer = "smalltalk"

L.extensions = {
	"st",
}

L.keywords = {
	[0] = "ifTrue: ifFalse: whileTrue: whileFalse: ifNil: ifNotNil: whileTrue whileFalse repeat isNil notNil",
}

L.styles = {
	["DEFAULT"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["STRING"] = {
		id = 1,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["NUMBER"] = {
		id = 2,
		fgColor = rgb(0xFF8000),
		bgColor = rgb(0x000000),
	},
	["COMMENT"] = {
		id = 3,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["SYMBOL"] = {
		id = 4,
		fgColor = rgb(0x408080),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["BINARY"] = {
		id = 5,
		fgColor = rgb(0x000080),
		bgColor = rgb(0x000000),
	},
	["BOOL"] = {
		id = 6,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
	},
	["SELF"] = {
		id = 7,
		fgColor = rgb(0x8080FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["SUPER"] = {
		id = 8,
		fgColor = rgb(0x0080FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["NIL"] = {
		id = 9,
		fgColor = rgb(0x8080C0),
		bgColor = rgb(0x000000),
	},
	["GLOBAL"] = {
		id = 10,
		fgColor = rgb(0x800000),
		bgColor = rgb(0x000000),
	},
	["RETURN"] = {
		id = 11,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["SPECIAL"] = {
		id = 12,
		fgColor = rgb(0x808000),
		bgColor = rgb(0x000000),
	},
	["KWS END"] = {
		id = 13,
		fgColor = rgb(0x0080FF),
		bgColor = rgb(0x000000),
	},
	["ASSIGN"] = {
		id = 14,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(0x000000),
	},
	["CHARACTER"] = {
		id = 15,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["SPECIAL SELECTOR"] = {
		id = 16,
		fgColor = rgb(0xFF80C0),
		bgColor = rgb(0x000000),
	},
}
return L
