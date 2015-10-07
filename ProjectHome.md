The Bleu-Romtools are a set of tools that I've created over the years that have helped me create and edit ROMs for Arcade game hardware.  Some of these tools will work with other systems as well.

[Genroms](Genroms.md) will take the output from your assembler/compiler as an Intel Hex File (IHX), and pad it out into a defined ROM space.  It will also pre-load the ROM space with ROM image files, so that you can make quick, reproducible patches for ROMs

[TuracoCL](TuracoCL.md) is a portable command line reimplementation of the [TuracoClassic](TuracoClassic.md) tool.  It has no graphical interface (yet), and is meant to be run in an automated fashion from the command line or a makefile, which was not possible with older versions of this codebase.  It loads and saves PCX and ROM file formats.  Current development efforts will add an optional curses-based user interface, to allow for ROM editing from shell windows.

[TuracoClassic](TuracoClassic.md) is a DOS-based sprite image editor for arcade graphics ROMs.  It is quite old, and only works on DOS.  It is highly based on [AGE](AGE.md), but mostly re-implemented from
scratch with future expansion and flexibility in mind.  [TuracoClassic](TuracoClassic.md) requires MS-DOS libraries and has no command-line interface to control it.  Note: this does work really well under DosBox or Boxer emulation.

[AGE](AGE.md) is an even older DOS-based sprite image editor, like Turaco.  It has a primitive graphical interface, and a Pac-Man map editor.  It has its shortcomings, but works. (Untested in DosBox/Boxer.)