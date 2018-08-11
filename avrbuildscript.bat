@echo off
C:\progra~2\Arduino\hardware\tools\avr\bin\avr-gcc -Wall -Os -DF_CPU=8000000 -mmcu=attiny85 -o %~n1.elf %1
del /F /Q %~n1.hex
C:\progra~2\Arduino\hardware\tools\avr\bin\avr-objcopy -j .text -j .data -O ihex %~n1.elf %~n1.hex