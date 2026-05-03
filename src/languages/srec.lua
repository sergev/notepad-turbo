local L = {}

L.lexer = "srec"

L.singleLineComment = "# "

L.extensions = {
	"mot",
	"srec",
}

L.keywords = {
}

L.styles = {
	["DEFAULT"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["RECSTART"] = {
		id = 1,
		fgColor = rgb(0x7F0000),
		bgColor = rgb(0x000000),
	},
	["RECTYPE"] = {
		id = 2,
		fgColor = rgb(0x7F0000),
		bgColor = rgb(0x000000),
	},
	["RECTYPE_UNKNOWN"] = {
		id = 3,
		fgColor = rgb(0x7F0000),
		bgColor = rgb(0x000000),
		fontStyle = 2,
	},
	["BYTECOUNT"] = {
		id = 4,
		fgColor = rgb(0x7F7F00),
		bgColor = rgb(0x000000),
	},
	["BYTECOUNT_WRONG"] = {
		id = 5,
		fgColor = rgb(0xFFFF00),
		bgColor = rgb(0x000000),
	},
	["NOADDRESS"] = {
		id = 6,
		fgColor = rgb(0x7F00FF),
		bgColor = rgb(0x000000),
	},
	["DATAADDRESS"] = {
		id = 7,
		fgColor = rgb(0x007FFF),
		bgColor = rgb(0x000000),
	},
	["RECCOUNT"] = {
		id = 8,
		fgColor = rgb(0x7F00FF),
		bgColor = rgb(0x000000),
	},
	["STARTADDRESS"] = {
		id = 9,
		fgColor = rgb(0x007FFF),
		bgColor = rgb(0x000000),
	},
	["ADDRESSFIELD_UNKNOWN"] = {
		id = 10,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 2,
	},
	["DATA_ODD"] = {
		id = 12,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["DATA_EVEN"] = {
		id = 13,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["DATA_UNKNOWN"] = {
		id = 14,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 2,
	},
	["DATA_EMPTY"] = {
		id = 15,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["CHECKSUM"] = {
		id = 16,
		fgColor = rgb(0x00BF00),
		bgColor = rgb(0x000000),
	},
	["CHECKSUM_WRONG"] = {
		id = 17,
		fgColor = rgb(0xFFFF00),
		bgColor = rgb(0x000000),
	},
	["GARBAGE"] = {
		id = 18,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 2,
	},
}
return L
