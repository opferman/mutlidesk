#
# Toby Opferman
#   MakeFile For DTLSOCKETS.DLL
#   Copyright (c) October 2001
#

   
 
TARGET = mdesk.dll
TARGETDIR = ..\..\..\binaries
DEFFILE = mdesk.def

CPP = cl
RSC = rc.exe
F90 = df.exe
MTL = midl.exe
REBASE = rebase.exe

OBJDIR = .\obj\i386

RES_PROJ=/I "\programs\mdesk\src\inc" \
        /D "WIN32" /D "_WINDOWS" /fo$(OBJDIR)\mdesk.res

CPP_PROJ=/nologo /MD /W3 /Oxs /Zi \
	/I "\programs\mdesk\src\inc" \
        /D "WIN32" /D "_WINDOWS" \
	/Fr$(OBJDIR)\\ /Fo$(OBJDIR)\\ /Fd$(OBJDIR)\\ /c

LIB32= link.exe /DLL
LIB32_FLAGS = /nologo /out:$(TARGETDIR)\$(TARGET) /pdb:mdesk.pdb /debug /debugtype:both /DEF:$(DEFFILE) 


RESFILE = \
        $(OBJDIR)\mdesk.res

OBJS   = \
	$(OBJDIR)\mdesk.obj 

LIBS = \
        USER32.LIB

# This is a comment
$(TARGETDIR)\$(TARGET): $(OBJDIR) $(TARGETDIR) $(OBJS) $(RESFILE)
    $(LIB32) $(LIB32_FLAGS) $(LIBS) $(OBJS) $(RESFILE)
    $(REBASE) -b 0x00100000 -x $(TARGETDIR) -a $(TARGETDIR)\$(TARGET)

{.\}.c{$(OBJDIR)}.obj::
	$(CPP) $(CPP_PROJ) $<


{.\}.rc{$(OBJDIR)}.res::
	$(RSC) $(RES_PROJ) $<

$(OBJDIR):
    if not exist "$(OBJDIR)/$(NULL)" mkdir "$(OBJDIR)"

$(TARGETDIR):
    if not exist "$(TARGETDIR)/$(NULL)" mkdir "$(TARGETDIR)"


CLEAN:
	-@erase /S /Q $(OBJDIR)
