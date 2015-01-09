@echo off
del *.png
del *.vtf
mogrify -background "rgba(0,0,0,0)" -flatten -format png *.xcf
