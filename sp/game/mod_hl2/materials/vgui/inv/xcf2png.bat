@echo off
del *.png
del *.vtf
mogrify -flatten -format png *.xcf
