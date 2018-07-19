SFML_PATH = ../../lib/SFML
SP = .\Source
HP = .\Headers

INCLUDE_PATH = /I$(SFML_PATH)/include /IHeaders

LIB_PATH = /LIBPATH:$(SFML_PATH)/lib

WINDOW_APP = /subsystem:windows /entry:mainCRTStartup

LIBS = user32.lib Advapi32.lib \
winmm.lib \
sfml-system-s.lib \
ws2_32.lib \
sfml-network-s.lib \
openal32.lib flac.lib vorbisenc.lib vorbisfile.lib vorbis.lib ogg.lib \
sfml-audio-s.lib \
opengl32.lib gdi32.lib \
sfml-window-s.lib \
freetype.lib jpeg.lib \
sfml-graphics-s.lib

INCLUDES = $(SP)\input.cpp $(SP)\logic.cpp $(SP)\render.cpp $(SP)\Player.cpp $(SP)\NormalE.cpp $(SP)\ShooterE.cpp $(SP)\SplitterE.cpp $(SP)\SplitE.cpp $(SP)\MapDemo.cpp $(SP)\Shot.cpp

.\bin\ColourKnight.exe: makefile \
$(SP)\input.cpp \
$(HP)\logic.hpp $(SP)\logic.cpp \
$(HP)\render.hpp $(SP)\render.cpp \
$(HP)\Player.hpp $(SP)\Player.cpp \
$(HP)\NormalE.hpp $(SP)\NormalE.cpp \
$(HP)\ShooterE.hpp $(SP)\ShooterE.cpp \
$(HP)\SplitterE.hpp $(SP)\SplitterE.cpp \
$(HP)\SplitE.hpp $(SP)\SplitE.cpp \
$(HP)\MapDemo.hpp $(SP)\MapDemo.cpp \
$(HP)\Shot.hpp $(SP)\Shot.cpp \
$(HP)\SharedSpace.hxx
	cl /EHsc $(INCLUDE_PATH) /Fo.\obj\ /DSFML_STATIC $(INCLUDES) /link $(LIB_PATH) $(LIBS) /out:bin\ColourKnight.exe
