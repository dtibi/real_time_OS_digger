tcc -I.. -c -DTURBOC clkint.C
tlib ..\XINU.LIB -+ clkint.obj
tcc -I.. -emain.exe main.c ..\XINU.LIB