                             ~~ Elemental Sun ~~

                       Ben 'Bruce "entheh" Perry' Davis
                        http://bdavis.strangesoft.net/
                             entheh@users.sf.net

This is my entry into TINS 2010, a 72-hour weekend game programming
competition that ran from 3 to 6 September.

  http://tins.amarillion.org/2010/

                                ~~ Licence ~~

Feel free to distribute unmodified versions of the game for all to enjoy.
Make sure you include the source code.

This game is free. If you paid for it, the Sun God will strike down the
person who took your money. To expedite matters, please tell me who that was.

                              ~~ Compiling me ~~

If you are using Windows and you can't find an executable file to run, then
try downloading the version of the game for Windows from here:

  http://bdavis.strangesoft.net/elemsun-win.zip

If you wish to compile yourself (e.g. for Mac or Linux), then first you will
need to make sure you have Allegro 4.4.0.1 and DUMB v0.9.3. Please note that
Allegro 4.9 or Allegro 5 are not compatible with Elemental Sun (but you can
have one of those installed as well if you like). You don't need to download
JPGAlleg separately, since it comes with Allegro 4.4.0.1 and is compiled
automatically when you compile Allegro.

  Allegro 4.4.0.1: http://www.talula.demon.co.uk/allegro/
  DUMB v0.9.3:     http://dumb.sf.net/

Once those are set up, a command such as the following should do the trick
for MinGW and other GCC-based systems:

  compile.bat

Or equivalently:

  g++ *.cpp -o elemsun.exe -ljpgalleg -laldmb -ldumb -lalleg44

On Linux you should replace -lalleg44 with `allegro-config --libs` (including
the grave accents), and you don't need the .exe extension.

Sorry, there's no makefile or other fancy build system. I used Eclipse for
development, and it just took care of everything for me.

                               ~~ Playing me ~~

Simply run the game and it should all make sense!

                             ~~ The TINS rules ~~

When TINS starts, some rules are published for everyone's games to adhere to.
My interpretation of the rules is described below.

Genre rule #90:
Deities & Demigods: the player character is a god, half-god, 
angel or other divine being, doing the kinds of things gods do.

  You are a Sun God, doing the kinds of things I can only imagine Sun Gods
  might maybe do.

Artistical rule #132:
Hand-drawn: feature hand-drawn or hand-painted graphics.
You may use actual scanned drawings, or just simulate them.

  I drew the background, elements, puffs of smoke, font, and mouse pointer on
  premium inkjet paper using a pencil and a set of 40 "COLOUR PLAY" crayons.
  
  I scanned them using a Canon CanoScan N670U. Windows 7 didn't like it, so I
  connected it to my Ubuntu Linux computer instead and it Just Worked.
  
  I'm not sure why I even bothered with this third-party Microsoft Windows
  rubbish!

Artistical rule #105:
Use samples of human(-like) voice.

  It's subtle, but it's there. Listen closely. There is a use of voice, above
  and beyond the three sound effects where I used my mouth but not my voice.
  Can you spot it?
  
  My interpretation of this rule is rather cheeky, but I think the game would
  have been spoilt if I had done it any other way!

Technical rule #69:
Polymorphism: create two pieces of code that implement the same interface,
and let the user select one.

  I created many. Choosing between them constitutes the core game mechanic.
  You drag functions on to elements, and they all get used when you activate
  the Sun.
  
  Just to make waves, I made all the functions static. It's C++'s fault for
  overloading the keyword several times.

Technical rule #34:
View Source: the game must display a piece
of its own source code somewhere.

  Well, what better code to display than the contents of the functions you
  place next to the elements?
  
  I wonder if everyone else will have done the same thing I did and rolled
  the two technical rules together. The idea practically bit me as soon as I
  read the rules!

                            ~~ Closing message ~~

My favourite things about TINS:

- The sudden competition-wide burst of creation!
- The excuse to be horribly irresponsible with one's circadian rhythms.
- The adrenaline!
- The friendships between the participants!

Thank you to Amarillion for running another awesome competition. I will now
have to sleep for a week (see the in-game 'About' screen), but once that's
out of the way, I shall enjoy playing the other entries!
