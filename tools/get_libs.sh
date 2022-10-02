#!/bin/bash

LIBS=`ldd CardGen | grep -o -E '/usr/lib[64]*/[a-zA-z.0-9+-]*'`

cp $LIBS CardGen.AppDir/usr/lib/