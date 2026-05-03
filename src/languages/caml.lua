local L = {}

L.lexer = "caml"

L.extensions = {
	"ml",
	"mli",
	"sml",
	"thy",
}

L.keywords = {
	[0] = "and as assert asr begin class constraint do done downto else end exception external false for fun function functor if in include inherit initializer land lazy let lor lsl lsr lxor match method mod module mutable new object of open or private rec sig struct then to true try type val virtual when while with",
	[1] = "option Some None ignore ref lnot succ pred",
	[2] = "array bool char float int list string unit",
}

L.styles = {
	["DEFAULT"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["IDENTIFIER"] = {
		id = 1,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["TAGNAME"] = {
		id = 2,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["INSTRUCTION WORD"] = {
		id = 3,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["BUILIN FUNC & TYPE"] = {
		id = 4,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
	},
	["TYPE"] = {
		id = 5,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
	},
	["LINENUM"] = {
		id = 6,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["OPERATOR"] = {
		id = 7,
		fgColor = rgb(0x000080),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["NUMBER"] = {
		id = 8,
		fgColor = rgb(0xFF8000),
		bgColor = rgb(0x000000),
	},
	["CHARACTER"] = {
		id = 9,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["STRING"] = {
		id = 11,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["COMMENT"] = {
		id = 12,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["COMMENT LINE"] = {
		id = 13,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["COMMENT DOC"] = {
		id = 14,
		fgColor = rgb(0x008080),
		bgColor = rgb(0x000000),
	},
	["COMMENT LINE DOC"] = {
		id = 15,
		fgColor = rgb(0x008080),
		bgColor = rgb(0x000000),
	},
}
return L
