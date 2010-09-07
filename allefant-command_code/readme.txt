  ___                                          _    ___          _      
 / __|___  _ __ ___  _ __ ___   __ _ _ __   __| |  / __|___   __| | ___ 
| |  / _ \| '_ ` _ \| '_ ` _ \ / _` | '_ \ / _` | | |  / _ \ / _` |/ _ \
| |_| (_) | | | | | | | | | | | (_| | | | | (_| | | |_| (_) | (_| |  __/
 \___\___/|_| |_| |_|_| |_| |_|\__,_|_| |_|\__,_|  \___\___/ \__,_|\___|
                                                                          
___________________________
TINS 2010 entry by Allefant

______________________
The rules of TINS 2010

1. The game's main character must be a god.
2. Graphics must look hand-drawn.
3. Sound must include voice samples.
4. Must display parts of the source code.
5. Must use polymorphism.

The first rule is obvious. In this game, you do play a god. *The* god.
The god of Allegro. Shawn Hargreaves. Although I have no idea how he
looks so I replaced him with a generic wizard.

Second and third rules are obvious as well, I tried to make everything
look hand-drawn using a special line drawing algorithm and using my
selfmade font. Sound effects are all using synthesized speech from
gespeaker.

The fourth rule is a major part of gameplay. The complete source code
used to control the mouse is displayed. It's of course up to the player
to actually write that source code first. If they don't they can't
play the game though...

The fifth rule will only get apparent in the later levels, when you
gain multiple mice. Each mouse will be a different, independent mouse,
but they all will use your same code. You better write your code
polymorphic or you will fail those levels. So basically it's up to
the player to implement the rule again, but if they fail at it they
can't win the level.

_________
The story

You play for the Wizard of Allegro. But he has lost his precious coins.
To retrieve them, he uses his great magic. Which allows him to command
his mouse Mysha (and its friends).

_____________
How to run it

I'm using Allegro 5 and Python, using the official Python wrapper
included with A5. In Linux this means, compile A5 (including the
audio, primitives, ogg and png addons) and also enable the Python
wrapper. Then place the generated allegro.py along with the game
sources and make sure all the .so files are in the linker search path.
I included an executable script "commandcode" which I use myself to
run the game.

For Windows it's basically the same. I'll also try to have a py2exe
executable ready which can just be run.

For OSX it's also the same, as long as the A5 .dylibs can be found
at runtime everything will work.

__________
Tools used

- Geany (for writing code)
- Gimp (for drawing mouse and wizard)
- Gespeaker (for recording synth voices)
- Audacity (for postprocessing sound effects)
- LMMS (for composing the music)

______
Manual

To make Mysha move, you have to write code. The code works like this.
Each command is a single letter. Some commands accept operands. An
operand is a variable or a number.

A variable is X or Y or Z. A number is an optional minus sign followed
by a sequence of digits.

To separate two numbers place whitespace between them.

The commands:

M

Makes Mysha move one step. Like this:
______      ______
|M |  |  -> |  |M |
|__|__|     |__|__|

J

Makes Mysha jump up.
______       _____
|  |  |     |M |  |
|__|__|     |__|__|
|  |  |  -> |  |  |
|__|__|     |__|__|
|M |  |     |  |  |
|__|__|     |__|__|

If there's a ledge Mysha will grab it:
______       _____
|  |  |     |  |M |
|__|__|     |__|__|
|  |L |  -> |  |L |
|__|__|     |__|__|
|M |  |     |  |  |
|__|__|     |__|__|

H

Makes Mysha hop over a gap in the floor.
_________     _________       
|  |  |  |    |  |  |  |
|__|__|__|    |__|__|__|
|M |  |  | -> |  |  |M | 
|__|__|__|    |__|__|__|   
|F |  |F |    |F |  |F |
|__|__|__|    |__|__|__|   

Also works to jump up.
_________     _________       
|  |  |  |    |  |  |M |
|__|__|__|    |__|__|__|
|M |  |F | -> |  |  |F | 
|__|__|__|    |__|__|__|   
|F |  |  |    |F |  |  |
|__|__|__|    |__|__|__|

T

Makes mysha turn around.

E

Ends the program and starts from the beginning.

G <position>

The GOTO command. Examples:

G 0 This is the same as E.
G 25 Jumps to the first line.
G 75 Jumps to the last line.
G 99 Jumps to the bottom right corner in the code window.

L <variable> <x> <y>

This stores information about a position relative to Mysha into a
variable. The position is like this when looking right and Mysha on
0 0. If Mysha is looking left the positions are flipped. I.e.
2 0 is always 2 ahead no matter which way Mysha looks.
____________  
|0-2|1-2|2-2|   
|__ |__ |__ |         
|0-1|1-1|2-1|   
|__ |__ |__ |   
|0 0|1 0|2 0| 
|__ |__ |__ |      
|0 1|1 1|2 1|   
|__ |__ |__ |      
Examples:

L X 0 1 Look what is below Mysha and store it in X.
L Y 1 1 Look what is one step ahead and store it in Y.
L Z 0 -2 Look what is two tiles above and store it in Z.

The meaning of what's stored in the variable is:
0 = nothing
1 = floor
2 = coin

Note how these are all equivalent:
"L X -1 1"
"LX -1 1"
"LX-1 1"
"LX-1+1"
But how this is *not* the same:
"LX-11"

I <variable> <value> <position>

This is like the G command, but it only changes the position if the
given variable has the given value. Examples:

"I X 1 25" If X is 1 then go to the beginning of the second line.
"L X 1 I X 1 25 H" If there's floor ahead go to line 2. Else hop.

= <variable> <value>

Sets the variable to the value. "= X 3" will set X to 3.

+ <variable> <value>
- <variable> <value>

Adds to/Subtracts from the variable.

"+ X 2" X = X + 2
"- Y 3" Y = Y - 3
