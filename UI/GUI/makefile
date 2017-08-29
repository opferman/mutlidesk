#
# Toby Opferman
#   MakeFile For Multidesk 2001
#   Copyright (c) December 2001
#

   
 
TARGET = md2001.exe
TARGETDIR = ..\..\..\binaries


CPP = cl
RSC = rc.exe
F90 = df.exe
MTL = midl.exe
REBASE = rebase.exe

OBJDIR = .\obj\i386


RES_PROJ=/I "\programs\mdesk\src\inc" \
        /D "WIN32" /D "_WINDOWS" /fo$(OBJDIR)\mdui.res
  

CPP_PROJ=/nologo /MD /W3 /Oxs /Zi \
	/I "\programs\mdesk\src\inc" \
        /D "WIN32" /D "_WINDOWS" \
	/Fr$(OBJDIR)\\ /Fo$(OBJDIR)\\ /Fd$(OBJDIR)\\ /c

LIB32= link.exe
LIB32_FLAGS = /nologo /out:$(TARGETDIR)\$(TARGET) /pdb:mdui.pdb /debug /debugtype:both  


RESFILE = \
        $(OBJDIR)\mdui.res

OBJS   = \
	$(OBJDIR)\entry.obj \
        $(OBJDIR)\mdui.obj

LIBS = \
        USER32.LIB \
        advapi32.lib \
        shell32.lib \
        $(TARGETDIR)\MDESK.LIB

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
