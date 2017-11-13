@echo off
echo Building...
PATH=%PATH%;..\..\bin;..\..\arm-none-eabi\bin;..\..\lpc21isp_197
make
echo --------------------------------------------
echo *  Instructions
echo *  Press RESET and Mode/ISP buttons
echo *  release RESET 
echo *  then release Mode/ISP
echo --------------------------------------------

pause
echo Downloading...
lpc21isp main.hex com2 9600 12000000
echo Done