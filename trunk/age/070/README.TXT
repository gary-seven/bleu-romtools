A.G.E. (Arcade Games Editor)
============================
Freeware by:
Ivan Mackintosh (ivan@rcp.co.uk)
Scott "Jerry" Lawrence (jerry@mail.csh.rit.edu)
Chris "Zwaxy" Moore (chris.moore@writeme.com)

AGE Homepage:
http://dales.rmplc.co.uk/ivan/age/

1.0 Table of Contents
---------------------

2.0 Introduction
3.0 System Requirements
4.0 Currently Supported Games
5.0 How to Use 
6.0 Command Line Options
7.0 Keyboard Summary
8.0 How to Contribute Updates



2.0 Introduction
----------------
AGE allows the graphics inside the ROMs for certain arcade
games to be edited allowing you to modify or add completely
new graphics to the game.  There is support for editing 
text strings, maps, and attributes of some games as well.


3.0 System Requirements 
-----------------------
486 running MSDOS/95/NT
ROMs for the game that you want to edit


4.0 Currently supported games
-----------------------------
AGE currently supports many different games and their variants.
For a full list of games supported including the subdirectory
names for the ROMs and the ROM names themselves type

age -listroms

from the command line. Note that this displays many screens of 
information typing

age -listroms | more

allows you to view a single screen of information at a time.

If you wish to just see which romsets are supported, without listing
each individual rom file required, try 

age -list

If you want to see a list of all supported romsets and the name 
of the set, try typing

age -listfull
 or 
age -listfull | more


5.0 How to Use
--------------
Edit AGE.INI and add the location of your rom images to the 
ROMPath. E.g.
ROMPath=c:\myroms;c:\mame\roms

With the above example AGE will look for roms on your hard disk
in the directories myroms followed by mame\roms

Run AGE and you will be presented by a list of supported games.
Cursor to the game using the cursor up and down, page up and down
or the first letter of the game you wish to edit. Then press space.

Note that if you have a mouse driver installed then the mouse 
movements simulate cursor keys. The left mouse button simulates the
space bar being pressed and the right mouse button simulates the 
'z' key being pressed.

Pressing Page Up and Page Down allows you to cycle through the
different character sets, sprite sets and tile sets contained 
within the ROMs.

For some ROM sets the < and > keys allow you to change the colours
to more suitable colours.

The 'g' key (Get) allows you to get a character from the character
set using the cursor keys and space to select. If the game has more
characters than will fit on the display they can be scrolled by
using the cursor keys.

To edit your character use the cursor keys to move around the 
pixels and space to add a new pixel of the foreground colour. This
is shown in the current colours window. The 'z' key places a pixel
of background colour.

To change pen colours press the 'c' key and select a new one from
the colour window using either space or 'z'.

The bitmap can be manipulated by a series of tools that can be 
accessed by pressing the 't' key. This includes sliding, rotating, 
clearing, etc.

Pressing tab takes you into scratch pad mode where you can draw 
using your edited character or any from the character set by 
pressing 'g' again to get a  new character to draw with. Pressing
tab returns you back to edit mode. Note that any edits you make
to the character currently being edited are reflected in the 
scratch window making it ideal for designing large sprites or 
tiled backgrounds.

To store the character back into the character set press 'p' and
select the position where you wish it to be stored.

To edit a pacman map, press 'm' to start up the map editor. There
is a file, "maped.txt" which explains how to use the map editor.

Before exiting the program the new character set can be written
back to the roms files by pressing ALT + 's'

ESC exits AGE.


6.0 Command Line Options
------------------------

Syntax:

AGE.EXE [short game name] [options ...]

[short game name] This option parameter allows you to specify the
game you wish to edit bypassing the game load dialog. The short
game name is the upto 8 character directory where the roms for the
game are stored.

[options ...]
-list / -listroms / -listfull / -listhtml
These options allow you to list the roms currently supported by AGE.
They are described in more detail in section 4.0

-makepatch
For copyright reasons modified ROMs should not be distributed in any
way to people who do not own the ROMs legally. To avoid this issue you
should only distribute the rom patches. The ROM patches only contain
the bytes that you have modified yourself and thus are not copyright.
To make a set of ROM patches for your modifications use a command line
similar to:
   age mspacman -makepatch
The ROM patches are then stored in a subdirectory called AGEPATCH

-patch / -patchq
If you download or receive any ROM patches then you can integrate then
into your original ROMs by first making a subdirectory under the game
directory that the patches are for and then placing the patches in that
directory.
The patches can then be integrated using a command line similar to:
   age mspacman -patch
Note: any modification you have made yourself will be lost!


7.0 Key Summary
---------------

F1          Quick Help
Cursor      Cursor movement (or mouse)
Space bar   Selects or draws a pixel in foreground colour 
            (or mouse left button)
Z           Draws a pixel in background colour
            (or mouse right button)
L           Loads font
ALT+S       Saves font
C           Selects colour
   P        Allows colour to be edited
   N        Creates a new palette based on the current one
G           Get a character from the character set
P           Put the current character back into the character set
PgUp        Next graphics bank
PgDown      Previous graphics bank
< >         Switch to alternative palettes for drivers that support this
F12         Save snapshot of the screen
ALT+Cursors Slide bitmap (background colour fills new space)
CTL+Cursors Rotate bitmap.
+ -			Zoom when editing sprites > 8x8
T           Bitmap Tools requester:
  K         Clear the character with the background color
  e         Eyedropper tool (get foreground color)
  E         Eyedropper tool (get background color)
  f         Flood fill with the current foreground color
  F         Flood fill with the current background color
  X         Flip the character horizontally
  Y         Flip the character vertically
  z         Rotate the character 90 degrees clockwise
  w         Rotate the character 45 degrees clockwise
  U         Shift the character up by one pixel
  D         Shift the character down by one pixel
  L         Shift the character left by one pixel
  R         Shift the character right by one pixel
  S         Swap 2 characters in the set
  F12       Save snapshot of the screen
ESC         Exits AGE if currently in edit mode
TAB         Toggles between edit and scratch mode
M           Start the map editor 
F5          Start the map editor 
  ESC       exit map editor
  Alt-S     Save the map data out
  Cursors   Move the cursor in the map
  U         Slide the map upwards
  D         Slide the map downwards
  S         Stretch the map to fit into a lo-res screen
  R         Reset the map position on the screen
  +         increase tile under cursor
  -         decrease tile under cursor
  P         toggle pellet under cursor
  PgUp      Next Pacman Map (for Ms. Pacman)
  PgDn      Previous Pacman Map (for Ms. Pacman)
  B         Toggle Base map tile view
  O         Toggle Overlay tiles view (pellets)
  H         Toggle Hardcoded bits view (power pellets)
  G         Select a tile from the tile palette (lower right corner)
  < >       Switch to alternative palettes for drivers that support this
  3         Switch to 320x200 resolution
  6         Switch to 640x480 resolution (for viewing entire map)
  F12       Save snapshot of the screen
F6          Start the text editor 
  ESC       exit text editor
  Alt-S     Save the text data out
  Alt-L     Reload the text data
  PgUp      Next category
  PgDn      Previous category
  < >       Switch to alternative palettes for drivers that support this
  Up Dn     Previous/Next string
  lft rgt   Move in the string
  Home      Start of string
  End       End of string
  Delete    Delete text
  Backspace Backspace over text
  Insert    Insert a "space" to where the cursor is
  F12       Save snapshot of the screen
F9          Save the current rom graphics set as "tempfile.pcx" (snarf)
F10         Load a sprite in from "tempfile.pcx" (barf)
  S         change pixel skip value (displayed in upper right)
  Arrows    move the cursor around the .pcx file
  HOME      Reset the bitmap and cursor to upper left
  Escape    quit - does not capture a part of the bitmap
  Enter     select the region surrounded by the cursor
  


8.0 Updates
-----------

Please send any modifications to functionality of this program or 
addition graphics drivers or better colour schemes for current drivers
to me so that they can be included in subsequent releases.

I can be contacted by email on ivan@rcp.co.uk

