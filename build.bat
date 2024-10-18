@chcp 65001 > nul
windres "src/icon.rc" -o "icon.o"
gcc -fexec-charset=GBK -finput-charset=GBK src/main.c src/config.c src/command.c icon.o -o autobatch.exe
@del icon.o
@pause
autobatch