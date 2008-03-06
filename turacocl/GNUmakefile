# Makefile for Turaco CL
#  yorgle@gmail.com
#  Scott Lawrence
#
# $Id: Makefile,v 1.31 2003/05/31 03:40:20 jerry Exp $
#


NAME    := turacocl
PNAME	:= "Turaco CL"
DIRNAME := turacoCL

VERSION := 011
VDATE   := "2008.03.02"


##########################################################

# use this next line for unix-based systems, otherwise comment
# it out, and use the MSDOS line for MS-DOS

ARCH := $(shell uname -s)


ARCH ?= NetBSD-CMD

#ARCH := MSDOS

##########################################################
# dos-switching stuff...  (automagic!)

CURSES := YES

ifeq ($(ARCH),MSDOS)
TARGET  := $(NAME).exe
else
TARGET  := $(NAME)
endif

ifeq ($(ARCH),NetBSD)
DNCURSES := -DUSE_CURSES
LIBS += -lcurses
else
DNCURSES := -DUSE_CURSES -DNCURSES
LIBS += -lncurses
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
	src/errors.c \
	src/params.c \
	src/main.c

ifeq ($(CURSES),YES)
SRCS := $(SRCS) \
	src/gui/app.c \
	src/gui/bits.c \
	src/gui/filereq.c \
	src/gui/menu.c \
	src/gui/popup.c \
	src/gui/status.c \
	src/gui/tmgui.c 
endif

SAMPLES := 

OBJS := $(SRCS:%.c=%.o)

##########################################################
# stuff for various helpful targets

all:	$(TARGET)


tidy:
	@echo Removing core files
	@rm -f $(TARGET).core core
	@echo Removing generated object files
	@rm -f $(OBJS)

clean:	tidy
	@echo Removing built executable $(TARGET)
	@rm -f $(TARGET)
	@echo Removing ancillary files
	@rm -f $(TARGET) *pnm *pcx pacman.5*
	@rm -f .gdb_history
	@rm -f .DS_Store

backup: clean
	cd .. ;\
	tar -cvf turacocl-$(VERSION).tar $(DIRNAME) ;\
	gzip -f turacocl-$(VERSION).tar

##########################################################
# stuff for making binary distributions

DISTNAME := t_CL_$(VERSION)

DISTFILES := $(TARGET) README.txt LICENSE.txt inidriv.txt bugs.txt

dosbindist: $(TARGET)
	strip $(TARGET)
	cp txt/bugs.txt .
	cp txt/inidriv.txt .
	cp txt/README.txt .
	pkzip $(DISTNAME).zip $(DISTFILES)
	rm -f README.txt .
	rm -f inidriv.txt
	rm -f bugs.txt


unixdist: $(TARGET)
	strip $(TARGET)
	cp txt/bugs.txt .
	cp txt/inidriv.txt .
	cp txt/README.txt .
	tar -cvf $(DISTNAME)_$(ARCH).tar $(DISTFILES)
	gzip -f $(DISTNAME)_$(ARCH).tar
	rm -f README.txt
	rm -f inidriv.txt
	rm -f bugs.txt

srcdist: clean
	rm -rf $(DISTNAME)s
	mkdir $(DISTNAME)s
	cp -r *.txt Makefile ini sample src txt $(DISTNAME)s
	cd $(DISTNAME)s ; rm -rf sample/tapper
	cd $(DISTNAME)s ; find . -name "CVS" | xargs rm -rf 
	cd $(DISTNAME)s ; find . -name ".cvsignore" | xargs rm -rf 

dossrcdist: srcdist
	zip -r $(DISTNAME)S.zip $(DISTNAME)s
	rm -rf $(DISTNAME)s

unixsrcdist: srcdist
	tar -cvf $(DISTNAME)S.tar $(DISTNAME)s
	gzip -f $(DISTNAME)S.tar
	rm -rf $(DISTNAME)s

##########################################################
# test targets

test: all
	./turacocl -gui

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


COMMONFLAGS := -O -Wall -pedantic -g \
	-D$(ARCH) \
	$(DNCURSES) \
	-DBL_APPNAME=\"$(TARGET)\" \
	-DBL_PNAME=\"$(PNAME)\" \
	-DBL_VERSION=\"$(VERSION)\" \
	-DBL_DATE=\"$(VDATE)\"\
	\
	-Isrc \
	-Isrc/gfxlib \
	-Isrc/gfxlib/lsio \
	-Isrc/turaco \
	-Isrc/ini \
	-Isrc/game \
	-Isrc/gui

CXXFLAGS := $(COMMONFLAGS) -mt -instances=static 

CFLAGS := $(COMMONFLAGS)

# for test builds, use this one:
#LDFLAGS := -g -lefence
LDFLAGS := -g 

NLIBS := 

##########################################################
# build
.c.o:
	@echo Build $<
	@$(CC) $(CFLAGS) -o $@ -c $<

##########################################################
# link!
$(TARGET):	$(OBJS)
	@echo Link $@
	@$(CXX) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

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
