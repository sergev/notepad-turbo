local L = {}

L.lexer = "xml"

L.first_line = {
	"^<%?xml"
}

L.extensions = {
	"xml",
	"xaml",
	"xsl",
	"xslt",
	"xsd",
	"xul",
	"kml",
	"svg",
	"mxml",
	"xsml",
	"wsdl",
	"xlf",
	"xliff",
	"xbl",
	"sxbl",
	"sitemap",
	"gml",
	"gpx",
	"plist",
	"vcproj",
	"vcxproj",
	"csproj",
	"csxproj",
	"vbproj",
	"dbproj",
}

L.properties = {
	["fold.html"] = "1",
}

L.keywords = {
}

L.styles = {
	["XMLSTART"] = {
		id = 12,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(0x000000),
	},
	["XMLEND"] = {
		id = 13,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(0x000000),
	},
	["DEFAULT"] = {
		id = 0,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["COMMENT"] = {
		id = 9,
		fgColor = rgb(0x008000),
		bgColor = rgb(0x000000),
	},
	["NUMBER"] = {
		id = 5,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(0x000000),
	},
	["DOUBLESTRING"] = {
		id = 6,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["SINGLESTRING"] = {
		id = 7,
		fgColor = rgb(0x8000FF),
		bgColor = rgb(0x000000),
		fontStyle = 1,
	},
	["TAG"] = {
		id = 1,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
	},
	["TAGEND"] = {
		id = 11,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
	},
	["TAGUNKNOWN"] = {
		id = 2,
		fgColor = rgb(0x0000FF),
		bgColor = rgb(0x000000),
	},
	["ATTRIBUTE"] = {
		id = 3,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(0x000000),
	},
	["ATTRIBUTEUNKNOWN"] = {
		id = 4,
		fgColor = rgb(0xFF0000),
		bgColor = rgb(0x000000),
	},
	["SGMLDEFAULT"] = {
		id = 21,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
	},
	["CDATA"] = {
		id = 17,
		fgColor = rgb(0xFF8000),
		bgColor = rgb(0x000000),
	},
	["ENTITY"] = {
		id = 10,
		fgColor = rgb(0xa0a0a0),
		bgColor = rgb(0x000000),
		fontStyle = 2,
	},
}
return L
