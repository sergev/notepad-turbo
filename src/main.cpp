/*
 * This file is part of Notepad Turbo.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Turbo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "NNApplication.h"

int main(int argc, char **argv)
{
    NNApplication app(argc, argv);
    app.run();
    app.shutDown();
    return 0;
}
