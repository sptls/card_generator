#!/bin/bash

LIBS=`ldd CardGen.exe | grep -o -E '\/mingw64\/bin\/[a-zA-z0-9\.+-]*'`

cp $LIBS build/bin/
cp src/template.bmp build/bin
cp src/gui.glade build/bin