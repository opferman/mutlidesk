@echo off
del *.obj
del *.res
cl /Ox /Zi /c worker.c /I"c:\program files\microsoft visual studio\vc98\include"
cl /Ox /Zi /c md2k.c /I"c:\program files\microsoft visual studio\vc98\include"
cl /Ox /Zi /c queue.c /I"c:\program files\microsoft visual studio\vc98\include"
rc /r /i"c:\program files\microsoft visual studio\vc98\include" md2k.rc 
link /SUBSYSTEM:WINDOWS /OUT:md2ka.exe queue.obj md2k.obj worker.obj md2k.res gdi32.lib kernel32.lib user32.lib shell32.lib Comctl32.lib /LIBPATH:"c:\program files\microsoft visual studio\vc98\lib" -pdb:md2ka.pdb -debugtype:both -debug
rebase -b 0x00400000 -x .\ -a md2ka.exe
@echo on
