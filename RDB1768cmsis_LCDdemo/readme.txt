Code Red Technologies RDB1768 Board - LCD Demo
==============================================

[ Note that this project requires the relevant CMSIS library 
  project as well as the LCDLib project to exist in the same 
  workspace. ]

This demonstration project shows how the LCD screen on the RDB1768
can be used. This is done by making use of the associated 
RDB1768cmsis_LCDLib project.

The demonstration is broken up into four parts, with a short pause 
between each part. 

The first part prints the a variety of bitmaps to the LCD, in addition 
to several strings and individual characters.

The second part uses a number of provided primitives for drawing
various lines and shapes to the screen.

The third part displays a very simple animation, consisting of the
Code Red logo moving down, then back up the screen.

The fourth part turns the LCD screen into a "terminal window" to
which both strings and individual characters can be printed. Text
is printed in the window until it reaches the bottom, when the
text scrolls up to allow more text to be displayed.

~~~~~~~~~~~~
Note that this example is only suitable for use with Red Suite / 
LPCXpresso IDE v3.6.x (v3.8.x for Linux) or later, as it makes 
use of linker related functionality introduced in this release.

More details at:

http://support.code-red-tech.com/CodeRedWiki/EnhancedManagedLinkScripts




