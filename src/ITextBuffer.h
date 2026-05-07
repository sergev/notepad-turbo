#pragma once

#include <string>

struct ITextBuffer {
    virtual ~ITextBuffer() = default;
    virtual std::string flatText() const = 0;
    virtual int selectionStart() const = 0;
    virtual int selectionEnd() const = 0;
    virtual void replaceSelection(const std::string &text) = 0;
    virtual void replaceAll(const std::string &newContent) = 0;
};
