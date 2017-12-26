#!/usr/bin/env bash

find . -iname "*.d" -exec sh -c '../build/daedalusx3 "$1" > "$1.pretty"' -- {} \;

#for file in *.d; do ../build/daedalusx3 "$file" > "$file.pretty"; done

