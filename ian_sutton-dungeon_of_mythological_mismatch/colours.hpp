#ifndef COLOURS_HPP
#define COLOURS_HPP

class Colour {
    public:

        static void Init();

        static int Black() { return black; }
        static int White() { return white; }
        static int Red() { return red; }
        static int Green() { return green; }
        static int Blue() { return blue; }
        static int Yellow() { return yellow; }
        static int Purple() { return purple; }
        static int Turquoise() { return turquoise; }
        static int BloodRed1() { return blood_red1; }
        static int BloodRed2() { return blood_red2; }
        static int BloodRed3() { return blood_red3; }
        static int Carbon() { return carbon; }

        static int GUIGold() { return gui_gold; }

        static int MagicPink() { return magic_pink; }

    protected:
        static int black;
        static int white;
        static int red;
        static int green;
        static int blue;
        static int yellow;
        static int purple;
        static int turquoise;
        static int blood_red1;
        static int blood_red2;
        static int blood_red3;
        static int carbon;
        static int gui_gold;
        static int magic_pink;

};

#endif
