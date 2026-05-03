local L = {}

L.lexer = "rust"

L.singleLineComment = "// "

L.extensions = {
	"rs",
}

L.keywords = {
	[0] = "abstract as async become box break const continue crate do dyn else enum extern false final fn for if impl in let loop macro match mod move mut override priv pub ref return self static struct super trait true try type typeof unsafe unsized use virtual where while yield",
	[1] = "bool char f32 f64 i128 i16 i32 i64 i8 isize str u128 u16 u32 u64 u8 usize",
	[2] = "Self",
	[3] = "None",
	[4] = "None",
	[5] = "None",
	[6] = "None",
}

L.styles = {
	["DEFAULT"] = {
		id = 32,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["WHITESPACE"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["BLOCK COMMENT"] = {
		id = 1,
		fgColor = rgb(0x007F00),
		bgColor = rgb(0x000000),
	},
	["LINE COMMENT"] = {
		id = 2,
		fgColor = rgb(0x007F00),
		bgColor = rgb(0x000000),
	},
	["BLOCK DOC COMMENT"] = {
		id = 3,
		fgColor = rgb(0x3F703F),
		bgColor = rgb(0x000000),
	},
	["LINE DOC COMMENT"] = {
		id = 4,
		fgColor = rgb(0x3F703F),
		bgColor = rgb(0x000000),
	},
	["NUMBER"] = {
		id = 5,
		fgColor = rgb(0x007F7F),
		bgColor = rgb(0x000000),
	},
	["KEYWORDS 1"] = {
		id = 6,
		fgColor = rgb(0x00007F),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["KEYWORDS 2"] = {
		id = 7,
		fgColor = rgb(0x00007F),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["KEYWORDS 3"] = {
		id = 8,
		fgColor = rgb(0x00007F),
		bgColor = rgb(0x000000),
	},
	["KEYWORDS 4"] = {
		id = 9,
		fgColor = rgb(0x00007F),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["KEYWORDS 5"] = {
		id = 10,
		fgColor = rgb(0x00007F),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["KEYWORDS 6"] = {
		id = 11,
		fgColor = rgb(0x00007F),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["KEYWORDS 7"] = {
		id = 12,
		fgColor = rgb(0x00007F),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["REGULAR STRING"] = {
		id = 13,
		fgColor = rgb(0x7F007F),
		bgColor = rgb(0x000000),
	},
	["RAW STRING"] = {
		id = 14,
		fgColor = rgb(0xB090B0),
		bgColor = rgb(0x000000),
	},
	["CHARACTER"] = {
		id = 15,
		fgColor = rgb(0x7F007F),
		bgColor = rgb(0x000000),
	},
	["OPERATOR"] = {
		id = 16,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["IDENTIFIER"] = {
		id = 17,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["LIFETIME"] = {
		id = 18,
		fgColor = rgb(0x007F7F),
		bgColor = rgb(0x000000),
	},
	["MACRO"] = {
		id = 19,
		fgColor = rgb(0x7F7F00),
		bgColor = rgb(0x000000),
	},
	["LEXICAL ERROR"] = {
		id = 20,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["BYTE STRING"] = {
		id = 21,
		fgColor = rgb(0x7F007F),
		bgColor = rgb(0x000000),
	},
	["RAW BYTE STRING"] = {
		id = 22,
		fgColor = rgb(0xB090B0),
		bgColor = rgb(0x000000),
	},
	["BYTE CHARACTER"] = {
		id = 23,
		fgColor = rgb(0x7F007F),
		bgColor = rgb(0x000000),
	},
}
return L
