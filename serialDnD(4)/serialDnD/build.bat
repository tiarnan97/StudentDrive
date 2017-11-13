@echo off
echo Building...
PATH=%PATH%;C:\Program Files\arm_sdk\bin;C:\Program Files\arm_sdk\arm-none-eabi\bin;C:\Program Files\arm_sdk\lpc21isp_197
make clean
make
echo --------------------------------------------
echo *  Instructions
echo *  Press RESET and Mode/ISP buttons
echo *  release RESET 
echo *  then release Mode/ISP
echo --------------------------------------------

pause
echo Downloading...
lpc21isp -term main.hex com2 9600 12000000
echo Done
pause