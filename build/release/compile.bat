@echo OFF
cls

del *.obj *.ilk *.pdb *.exe

REM Compile Source code

cl /nologo /O2 /Zi /GR- /EHs /MDd /W3 -I ../../include -I ../../deps/SDL2-2.0.10/include -I ../../deps/SDL2_ttf-2.0.15/include -I . -I ../../deps/Lusvarghi_lib/Include -I ../../deps/sqlite3 /c ../../src/*.cc

cl /nologo /O2 /Zi /GR- /EHs /MDd /W3 -I ../../include -I ../../deps/SDL2-2.0.10/include -I ../../deps/SDL2_ttf-2.0.15/include -I . -I ../../deps/Lusvarghi_lib/Include -I ../../deps/sqlite3 /c ../../deps/sqlite3/sqlite3.c

REM Link all the code

cl /nologo /O2 /Zi /GR- /EHs /MDd /W3 /Fe:game.exe *.obj ../../deps/SDL2-2.0.10/lib/x86/SDL2test.lib ../../deps/SDL2-2.0.10/lib/x86/SDL2main.lib ../../deps/SDL2-2.0.10/lib/x86/SDL2.lib ../../deps/SDL2-2.0.10/lib/x86/SDL2_image.lib ../../deps/SDL2_ttf-2.0.15/lib/x86/SDL2_ttf.lib ../../deps/Lusvarghi_lib/bin/lusvarghi_math_lib_d.lib opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib

move game.exe ../../bin

::Visual Studio 2017 Developer Command Prompt v15.9.21