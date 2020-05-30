@echo off
echo "cl -AM -Zi -c -Fc -Gs -G2"
echo "link /ST:8192 /CO <objfile>,,,graphics.lib+gamelib.lib,,"
