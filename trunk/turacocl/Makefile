# Makefile for Turaco CL
#  yorgle@gmail.com
#  Scott Lawrence
#
# $Id: Makefile,v 1.31 2003/05/31 03:40:20 jerry Exp $
#


NAME    := turacocl
DIRNAME := turacoCL

VERSION := 0.8
VERSTAR := 08
VDATE   := "2003.05.30"


##########################################################

# use this next line for unix-based systems, otherwise comment
# it out, and use the MSDOS line for MS-DOS

ARCH := $(shell uname -s)

#ARCH := MSDOS

##########################################################
# dos-switching stuff...  (automagic!)

ifeq ($(ARCH),MSDOS)
TARGET  := $(NAME).exe
else
TARGET  := $(NAME)
endif

##########################################################
SRCS := \
	src/gfxlib/color.c \
	src/gfxlib/io_util.c \
	src/gfxlib/image.c \
	src/gfxlib/font.c \
	src/gfxlib/endian.c \
	src/gfxlib/ls.c \
	\
	src/gfxlib/lsio/ppm.c \
	src/gfxlib/lsio/pcx.c \
	\
	src/ini/jstr.c \
	src/ini/ini.c \
	\
	src/game/driver.c \
	src/game/checker.c \
	src/game/romio.c \
	src/game/coding.c \
	src/game/turaco.c \
	src/game/tilemap.c \
	\
	src/params.c \
	src/main.c

SAMPLES := 

OBJS := $(SRCS:%.c=%.o)

##########################################################
# stuff for various helpful targets

all:	$(TARGET)


tidy:
	rm -f core
	rm -f $(OBJS)

clean:	tidy
	rm -f $(TARGET) *pnm *pcx pacman.5*
	rm -f .gdb_history
	rm -f .DS_Store

backup: clean
	cd .. ;\
	tar -cvf turacocl-$(VERSION).tar $(DIRNAME) ;\
	gzip -f turacocl-$(VERSION).tar

##########################################################
# stuff for making binary distributions

UNIXDISTNAME := t_CL_$(VERSION)
DOSDISTNAME  := t_CL_$(VERSTAR)

DISTFILES := $(TARGET) README.txt LICENSE.txt inidriv.txt bugs.txt

dosbindist: $(TARGET)
	strip $(TARGET)
	cp txt/bugs.txt .
	cp txt/inidriv.txt .
	cp txt/README.txt .
	pkzip $(DOSDISTNAME).zip $(DISTFILES)
	rm -f README.txt .
	rm -f inidriv.txt
	rm -f bugs.txt


unixdist: $(TARGET)
	strip $(TARGET)
	cp txt/bugs.txt .
	cp txt/inidriv.txt .
	cp txt/README.txt .
	tar -cvf $(UNIXDISTNAME)_$(ARCH).tar $(DISTFILES)
	gzip -f $(UNIXDISTNAME)_$(ARCH).tar
	rm -f README.txt
	rm -f inidriv.txt
	rm -f bugs.txt

srcdist: clean
	rm -rf $(DOSDISTNAME)s
	mkdir $(DOSDISTNAME)s
	cp -r *.txt Makefile ini sample src txt $(DOSDISTNAME)s
	cd $(DOSDISTNAME)s ; rm -rf sample/tapper
	cd $(DOSDISTNAME)s ; find . -name "CVS" | xargs rm -rf 
	cd $(DOSDISTNAME)s ; find . -name ".cvsignore" | xargs rm -rf 

dossrcdist: srcdist
	zip -r $(DOSDISTNAME)S.zip $(DOSDISTNAME)s
	rm -rf $(DOSDISTNAME)s

unixsrcdist: srcdist
	tar -cvf $(DOSDISTNAME)S.tar $(DOSDISTNAME)s
	gzip -f $(DOSDISTNAME)S.tar
	rm -rf $(DOSDISTNAME)s

##########################################################
# test targets

testa: all
	./turacocl -ini txt/inidriv.txt -rom sample -inf ROM -tms pacman

testb: all
	./turacocl -ini txt/inidriv.txt -inf IMG -tms pacman

testaa: all
	./turacocl -ini txt/inidriv.txt -rom sample -inf IMG

testbb: all
	./turacocl -ini txt/inidriv.txt -rom sample -inf ROM

testc: all
	./turacocl -ini sample/tapper/Tapper.ini -inf ROM

testd: all
	./turacocl -ini sample/tapper/Tapper.ini -inf IMG

testee: all
	./turacocl -ini txt/inidriv.txt -rom sample -inf ROM
	./turacocl -ini txt/inidriv.txt -rod . -inf IMG
	./turacocl -ini txt/inidriv.txt -rom . -inf ROM -dbf aaaa.pcx

testinstall: test
	cp pacman.5e /Applications/jerry/Games/MacPacMAME\ 0.58ƒ/ROMS/mspacmab/5e


##########################################################
# build targets (important stuff)

CC  := gcc
CXX := gcc


COMMONFLAGS := -O -Wall -g \
	-D$(ARCH) \
	-DJ_APPNAME=\"$(TARGET)\" \
	-DJ_VERSION=\"$(VERSION)\" \
	-DJ_DATE=\"$(VDATE)\"\
	\
	-Isrc \
	-Isrc/gfxlib \
	-Isrc/gfxlib/lsio \
	-Isrc/turaco \
	-Isrc/ini \
	-Isrc/game

CXXFLAGS := $(COMMONFLAGS) -mt -instances=static 

CFLAGS := $(COMMONFLAGS)

# for test builds, use this one:
#LDFLAGS := -g -lefence
LDFLAGS := -g 

NLIBS := 

##########################################################
# link!

$(TARGET):	$(OBJS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

##########################################################

.PHONY:	install
.PHONY:	all
.PHONY:	docs
.PHONY:	tidy
.PHONY:	clean
.PHONY:	clobber
.PHONY:	dosbindist
.PHONY:	unixbindist
.PHONY:	srcdist
.PHONY: dossrcdist
.PHONY: unixsrcdist

##########################################################

main.o:	

##########################################################
# $Id: Makefile,v 1.31 2003/05/31 03:40:20 jerry Exp $
