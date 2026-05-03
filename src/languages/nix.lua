local L = {}

L.lexer = "nix"

L.singleLineComment = "# "

L.extensions = {
    "nix"
}

L.keywords = {
    [0] = "assert else if in inherit let or rec then with",
    [1] = "false null true",
    [2] = "abort add addDrvOutputDependencies all any attrNames attrValues baseNameOf bitAnd bitOr bitXor break builtins catAttrs ceil compareVersions concatLists concatMap concatStringsSep convertHash currentSystem currentTime deepSeq derivation dirOf div elem elemAt fetchClosure fetchGit fetchTarball fetchTree fetchurl filter filterSource findFile flakeRefToString floor foldl' fromJSON fromTOML functionArgs genList genericClosure getAttr getContext getEnv getFlake groupBy hasAttr hasContext hashFile hashString head import intersectAttrs isAttrs isBool isFloat isFunction isInt isList isNull isPath isString langVersion length lessThan listToAttrs map mapAttrs match mul nixPath nixVersion outputOf parseDrvName parseFlakeRef partition path pathExists placeholder readDir readFile readFileType removeAttrs replaceStrings seq sort split splitVersion storeDir storePath stringLength sub substring tail throw toFile toJSON toPath toString toXML trace traceVerbose tryEval typeOf unsafeDiscardOutputDependency unsafeDiscardStringContext warn zipAttrsWith",
    [3] = "runCommand",
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
    ["COMMENT BLOCK"] = {
        id = 2,
        fgColor = rgb(0x008080),
        bgColor = rgb(0x000000),
    },
    ["STRING"] = {
        id = 3,
        fgColor = rgb(0xa0a0a0),
        bgColor = rgb(0x000000),
    },
    ["STRING MULTILINE"] = {
        id = 4,
        fgColor = rgb(0xa0a0a0),
        bgColor = rgb(0x000000),
    },
    ["ESCAPECHAR"] = {
        id = 5,
        fgColor = rgb(0xa0a0a0),
        bgColor = rgb(0x000000),
        fontStyle = 1,
    },
    ["IDENTIFIER"] = {
        id = 6,
        fgColor = rgb(0xa0a0a0),
        bgColor = rgb(0x000000),
    },
    ["OPERATOR"] = {
        id = 7,
        fgColor = rgb(0x000080),
        bgColor = rgb(0x000000),
        fontStyle = 1,
    },
    ["OPERATOR_STRING"] = {
        id = 8,
        fgColor = rgb(0xa0a0a0),
        bgColor = rgb(0x000000),
        fontStyle = 1,
    },
    ["NUMBER"] = {
        id = 9,
        fgColor = rgb(0xFF8000),
        bgColor = rgb(0x000000),
    },
    ["KEY"] = {
        id = 10,
        fgColor = rgb(0x8000FF),
        bgColor = rgb(0x000000),
    },
    ["PATH"] = {
        id = 11,
        fgColor = rgb(0xa0a0a0),
        bgColor = rgb(0x000000),
    },
    ["KEYWORD1"] = {
        id = 12,
        fgColor = rgb(0x0000FF),
        bgColor = rgb(0x000000),
        fontStyle = 1,
    },
    ["KEYWORD2"] = {
        id = 13,
        fgColor = rgb(0x0000FF),
        bgColor = rgb(0x000000),
        fontStyle = 1,
    },
    ["KEYWORD3"] = {
        id = 14,
        fgColor = rgb(0x0000FF),
        bgColor = rgb(0x000000),
        fontStyle = 1,
    },
    ["KEYWORD4"] = {
        id = 15,
        fgColor = rgb(0x880088),
        bgColor = rgb(0x000000),
        fontStyle = 1,
    }
}

return L
