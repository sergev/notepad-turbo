local L = {}

L.lexer = "d"

L.singleLineComment = "// "

L.extensions = {
	"d",
}

L.keywords = {
	[0] = "abstract alias align asm assert auto body bool break byte case cast catch cdouble cent cfloat char class const continue creal dchar debug default delegate delete deprecated do double else enum export extern false final finally float for foreach foreach_reverse function goto idouble if ifloat immutable import in inout int interface invariant ireal is lazy long mixin module new nothrow null out override package pragma private protected public pure real ref return scope shared short static struct super switch synchronized template this throw true try typedef typeid typeof ubyte ucent uint ulong union unittest ushort version void volatile wchar while with",
	[1] = "None",
}

L.styles = {
	["DEFAULT"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["IDENTIFIER"] = {
		id = 14,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["INSTRUCTION WORD"] = {
		id = 6,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["KEWORD1"] = {
		id = 7,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["KEWORD2"] = {
		id = 8,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["KEWORD3"] = {
		id = 9,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["KEWORD4"] = {
		id = 20,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["KEWORD5"] = {
		id = 21,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["KEWORD6"] = {
		id = 22,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["NUMBER"] = {
		id = 5,
		fgColor = rgb(0xFF8000),
		bgColor = rgb(0x000000),
	},
	["STRING"] = {
		id = 10,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["CHARACTER"] = {
		id = 12,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["OPERATOR"] = {
		id = 13,
		fgColor = rgb(0x000080),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["COMMENT"] = {
		id = 1,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["COMMENT LINE"] = {
		id = 2,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["COMMENT DOC"] = {
		id = 3,
		fgColor = rgb(0x008080),
		bgColor = rgb(0x000000),
	},
	["COMMENT NESTED"] = {
		id = 4,
		fgColor = rgb(0x008080),
		bgColor = rgb(0x000000),
	},
	["COMMENT LINE DOC"] = {
		id = 15,
		fgColor = rgb(0x008080),
		bgColor = rgb(0x000000),
	},
	["COMMENT DOC KEYWORD"] = {
		id = 16,
		fgColor = rgb(0x008080),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["COMMENT DOC KEYWORD ERROR"] = {
		id = 17,
		fgColor = rgb(0x008080),
		bgColor = rgb(0x000000),
	},
	["STRING B"] = {
		id = 18,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["STRING R"] = {
		id = 19,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
}
return L
