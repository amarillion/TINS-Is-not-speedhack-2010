MEXICA - TINS 2010 ENTRY
========================

Introduction

Quetzalraulcoatl (in Nahuatl "feathered monkey serpent") and his often evil brother 
Tezcatlifolepoca (in Nahuatl "smoking cat mirror") were two gods in the Mexica empire, 
later also gods of the Aztecs. They represent the dual nature of the universe. 
Quetzalraulcoatl symbolizes creation and Tezcatlifilepoca destruction. 

In Tenochtitlan, capricious god Tezcatlifolepoca wants the Fifth sun to die. 
It is the duty of god Quetzalraulcoatl to save the day by picking up the human sacrifices 
thrown off by Tezcatlifolepoca, take the heart out and placing it in the sacrifices pot. 
However, Quetzalraulcoatl also has to eat enough to accomplish his task. 
Like all ancient Mexicas and Aztecs, god Quetzalraulcoatl loves tacos, 
chocolate, corn and chili. But he has to be careful of eating too much chili!!!. 

----------------------------------

How to play: 

Pick the human sacrifices and go over the sacrifice pot (dog-like stony pot). 
Eat chocolate, corn and tacos for extra health and avoid chilies (all of them!, they are all spicy). 
Ignore god Tezcatlifolepoca, he will get bored soon and will 
stop throwing dead people and food to the air.

The only keys you need are left, right and up.

After 6 sacrifices, you'll advanced to the next level with more spicy chilies :)
----------------------------------

How to compile:

----------------------------------

You need allegro 4.4 or higher, including logg add-on. 
You also need the alfont add-on.

Compile:
		$ make

Run
		$./tins10
	
You can use the following start-up options:
	
	-windowed: run the game in a window
	-showfps: show a FPS counter in the corner of the screen
	-fullscreen: run the game fullscreen

Implementation details & code re-use
----------------------------------

Some code was re-used from our Speedhack '09 entry 
"Fashionista, match or die",
in particular main.cpp, Engine.cpp, Resources.cpp, Menu.cpp,
Anim.cpp. Settings.cpp and Button.cpp contain re-used code.

All sound, graphics and the font were created originally during 
the 72h of this competition

Special requirements
----------------------------------

This game was made for the TINS 2010 competition.
For this competition our entry had to comply with 
these five "special" rules:

Genre requirement: Deities & Demigods

Fole and Raul act this time as Mexica 
gods Quetzalraulcoatl and Tezcatlifolepoca. 

Technical requirement 1: Polymorphism

The interface class IReplay is implemented in two different ways: 
ReplayRecorder and ReplayPlayback. See replay.cpp for details.

ReplayRecorder records all button presses during the game, whereas
ReplayPlayback simulates them when viewing a replay

Technical requirement 2: View Source

In the menu there is a view source option. 
Use up/down or pgup/pgdn to scroll, and left/right to flip through files.

Artistic requirement 1: Human(-like) voice samples

All sound effects were recorded by both of us, and included
in the game after passed through a few audacity filter effects.

Artistic requirement 2: Hand drawings

All drawings you see in the game are hand drawn and scanned by Max.
The font is hand-drawn, scanned and converted to ttf using the 
fontforge utility.

Copyright
----------------------------------

Copyright (C) 2010 Max & Amarillion

Code by Amarillion
Gfx & font by Max
Sfx by Amarillion & Max

Like Allegro itself, this game is gift-ware. See LICENSE.txt for details.
