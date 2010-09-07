#include "stages.hpp"
#include "time.hpp"
#include "logging.hpp"
#include "tilemap.hpp"
#include "colours.hpp"
#include "levelmaps.hpp"
#include "animation.hpp"
#include "overlay.hpp"

Stage* stage;


bool MainMenuEntry();
bool MainMenuCycle();
bool MainMenuGraphics(BITMAP* buffer);

bool LevelEntry();
bool LevelCycle();
bool LevelGraphics(BITMAP* buffer);

bool SelectAIEntry();
bool SelectAICycle();
bool SelectAIGraphics(BITMAP* buffer);

bool HighScoreEntry();
bool HighScoreCycle();
bool HighScoreGraphics(BITMAP* buffer);

bool GoodBadEntry();
bool GoodBadCycle();
bool GoodBadGraphics(BITMAP* buffer);

Stage main_menu_stage(MainMenuEntry, MainMenuCycle, MainMenuGraphics);
Stage level_stage(LevelEntry, LevelCycle, LevelGraphics);
Stage select_ai_stage(SelectAIEntry, SelectAICycle, SelectAIGraphics);
Stage high_scores_stage(HighScoreEntry, HighScoreCycle, HighScoreGraphics);
Stage select_good_bad_stage(GoodBadEntry, GoodBadCycle, GoodBadGraphics);


struct MainMenuData {
    BITMAP* gui;
    int which_option;
    Time::Timer wait_between_jump;
    Time::Timer ignore_input;
}MainMenuData;

bool MainMenuEntry()
{
    erlog("Main menu stage first entry");
    MainMenuData.which_option = 0;
    MainMenuData.ignore_input.SetTicks(20);

    erlog("Main menu stage first entry complete");
    return true;
}

bool MainMenuCycle()
{
    if (MainMenuData.ignore_input.Finished()) {
        if (key[KEY_UP] && MainMenuData.which_option != 0 && MainMenuData.wait_between_jump) { --MainMenuData.which_option; MainMenuData.wait_between_jump.SetTicks(10); }
        if (key[KEY_DOWN] && MainMenuData.which_option != 3 && MainMenuData.wait_between_jump) { ++MainMenuData.which_option; MainMenuData.wait_between_jump.SetTicks(10); }

        if (key[KEY_ENTER] || key[KEY_SPACE]) {
            switch (MainMenuData.which_option) {
                case 0:
                    erlog("Going to level stage");
                    GoToStage(&select_good_bad_stage);
                    break;
                case 1:
                    erlog("Going to select ai stage");
                    GoToStage(&select_ai_stage);
                    break;
                case 2:
                    erlog("Going to high score stage");
                    GoToStage(&high_scores_stage);
                    break;
                case 3:
                    erlog("Quitting");
                    return false;
            }
        }
        if (key[KEY_ESC])
            return false;
    }
    MainMenuData.wait_between_jump.Update();
    MainMenuData.ignore_input.Update();
    return true;
}

void DrawBox(BITMAP* bmp, Vector top_left, Vector bottom_right, int colour, float wobble, float amplitude, int inner_rad, int outer_rad, float hardness)
{
    CrayonLine(bmp, Vector(top_left.X()-3-rand()%4, top_left.Y()-3+rand()%6), Vector(bottom_right.X()+3+rand()%4, top_left.Y()-3+rand()%6), colour, wobble, amplitude, inner_rad, outer_rad, hardness);
    CrayonLine(bmp, Vector(top_left.X()-3-rand()%4, bottom_right.Y()-3+rand()%6), Vector(bottom_right.X()+3+rand()%4, bottom_right.Y()-3+rand()%6), colour, wobble, amplitude, inner_rad, outer_rad, hardness);
    CrayonLine(bmp, Vector(top_left.X()-3+rand()%6, top_left.Y()-3-rand()%4), Vector(top_left.X()-3+rand()%6, bottom_right.Y()+3+rand()%4), colour, wobble, amplitude, inner_rad, outer_rad, hardness);
    CrayonLine(bmp, Vector(bottom_right.X()-3+rand()%6, top_left.Y()-3-rand()%4), Vector(bottom_right.X()-3+rand()%6, bottom_right.Y()+3+rand()%4), colour, wobble, amplitude, inner_rad, outer_rad, hardness);
}

bool MainMenuGraphics(BITMAP* buffer)
{
    if (!MainMenuData.gui) {
        MainMenuData.gui = create_bitmap(480, 480);
        if (!MainMenuData.gui) {
            erlog("Couldn't create the gui", 3);
            return false;
        }
        clear_to_color(MainMenuData.gui, Colour::MagicPink());
        DrawBox(MainMenuData.gui, Vector(90, 20), Vector(390, 420), Colour::Red(), 0.01*(float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX, 1, 2, 0.2);
        floodfill(MainMenuData.gui, 200, 200, Colour::White());
    }

    draw_sprite(buffer, MainMenuData.gui, 80, 0);

    textout_centre_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, "Play", 320, 100, MainMenuData.which_option==0?Colour::Red():Colour::Black(), -1);
    textout_centre_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, "Set Chase Strategy", 320, 160, MainMenuData.which_option==1?Colour::Red():Colour::Black(), -1);
    textout_centre_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, "View High Scores", 320, 210, MainMenuData.which_option==2?Colour::Red():Colour::Black(), -1);
    textout_centre_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, "Quit", 320, 270, MainMenuData.which_option==3?Colour::Red():Colour::Black(), -1);

    return true;
}


struct LevelData {
    Tilemap tilemap;
    Time::Timer tile_map_pauser;
    Player* player;
    int player_x, player_y;

    int prev_read_life;
    BITMAP* life_tally;
    int prev_read_ammo;
    BITMAP* ammo_tally;
    Overlay overlay;
}LevelData;

bool LevelEntry()
{
    erlog("Level first entry");

    GenerateTilemap();

    LevelData.tilemap.SetUpMap(generated_tilemap, generated_objectmap);
    LevelData.tilemap.SetAllVisibilities(0);

    LevelData.overlay.Init();

    LevelData.life_tally = create_bitmap(200, 30);
    clear_to_color(LevelData.life_tally, Colour::White());
    LevelData.prev_read_life = 10;

    LevelData.ammo_tally = create_bitmap(200, 30);
    clear_to_color(LevelData.ammo_tally, Colour::White());
    LevelData.prev_read_life = 10;

    LevelData.player = LevelData.tilemap.PlayerObject(LevelData.player_x, LevelData.player_y);

    erlog("Level first entry complete");
    return true;
}

bool LevelCycle()
{
    LevelData.tilemap.Update();

    LevelData.tilemap.PlayerObject(LevelData.player_x, LevelData.player_y);

    if (key[KEY_ESC])
        GoToStage(&main_menu_stage);
    //LevelData.tilemap.Purge();

    if (LevelData.player->HowManyLives() == 0)
        GoToStage(&high_scores_stage);

    return true;
}

bool LevelGraphics(BITMAP* buffer)
{
    extern bool play_as_good;
    //clear_to_color(buffer, Colour::GUIGold());
    int player_x, player_y;
    Player* player = LevelData.tilemap.PlayerObject(player_x, player_y);
    LevelData.tilemap.Draw(buffer);

    if (LevelData.prev_read_life != player->HowManyLives()) {
        LevelData.prev_read_life = player->HowManyLives();
        clear_to_color(LevelData.life_tally, Colour::MagicPink());
        CrayonTally(LevelData.life_tally, Vector(5, 5), LevelData.prev_read_life, Colour::Carbon(), 18);
    }

    if (LevelData.prev_read_ammo != player->HowManyAmmo()) {
        LevelData.prev_read_ammo = player->HowManyAmmo();
        clear_to_color(LevelData.ammo_tally, Colour::MagicPink());
        CrayonTally(LevelData.ammo_tally, Vector(5, 5), LevelData.prev_read_ammo, play_as_good ? Colour::Turquoise() : Colour::Purple(), 18);
    }

    LevelData.overlay.Draw(buffer);
    textprintf_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, 50, 433, Colour::Carbon(), -1, "Lives:");
    draw_sprite(buffer, LevelData.life_tally, 130, 430);
    textprintf_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, 50, 456, Colour::Carbon(), -1, "Ammo:");
    draw_sprite(buffer, LevelData.ammo_tally, 130, 450);


    textprintf_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, 400, 440, Colour::Carbon(), -1, play_as_good ? "Praise: %d" : "Fear: %d", player->Reward());

    return true;
}




struct HighScoreData {
    BITMAP* gui;
    std::vector<std::string> names;
    std::vector<int> scores;
    Time::Timer ignore_input;
    Time::Timer hang_around;
}HighScoreData;


bool HighScoreEntry()
{
    HighScoreData.ignore_input.SetTicks(20);

    // Load High Scores
    std::fstream fin("high.scores", std::fstream::in);
    if (!fin) {
        erlog("Couldn't load high scores", 2);
        GoToStage(&main_menu_stage);
        return true;
    }
    std::string name;
    int score;
    while (!fin.eof()) {
        fin >> name >> score;
        HighScoreData.names.push_back(name);
        HighScoreData.scores.push_back(score);
    }
    HighScoreData.hang_around.SetTicks(900);

    return true;
}

bool HighScoreCycle()
{
    if (HighScoreData.ignore_input.Finished()) {
        if (key[KEY_ESC]) {
            GoToStage(&main_menu_stage);
            return true;
        }
    }

    if (HighScoreData.hang_around.Finished()) {
        GoToStage(&main_menu_stage);
        return true;
    }

    HighScoreData.hang_around.Update();
    HighScoreData.ignore_input.Update();
    return true;
}

bool HighScoreGraphics(BITMAP* buffer)
{
    if (!HighScoreData.gui) {
        HighScoreData.gui = create_bitmap(480, 480);
        if (!HighScoreData.gui) {
            erlog("Couldn't create the gui", 3);
            return false;
        }
        clear_to_color(HighScoreData.gui, Colour::MagicPink());
        DrawBox(HighScoreData.gui, Vector(90, 20), Vector(390, 420), Colour::Red(), 0.01*(float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX, 1, 2, 0.2);
        floodfill(HighScoreData.gui, 200, 200, Colour::White());
    }

    draw_sprite(buffer, HighScoreData.gui, 80, 0);
    textout_centre_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, "High Scores", 320, 40, Colour::Red(), -1);

    for (int i=0; i<(int)HighScoreData.names.size() && i < 10; ++i) {
        textprintf_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, 200, 100 + i * 30, Colour::Black(), -1, "%s", HighScoreData.names[i].c_str());
        textprintf_right_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, 440, 100 + i * 30, Colour::Black(), -1, "%d", HighScoreData.scores[i]);
    }
    return true;
}


Direction(*EnemyChaseStrategy)(Vector pos, Vector player_pos, Tilemap* map) = BasicStrategy;

struct SelectAIData {
    BITMAP* gui;
    int which_selected;
    BITMAP* code1, *code2;
    Time::Timer ignore_input;
}SelectAIData;


bool SelectAIEntry()
{
    erlog("Select AI menu first cycle");
    SelectAIData.gui = NULL;
    SelectAIData.code1 = NULL;
    SelectAIData.code2 = NULL;
    SelectAIData.which_selected = EnemyChaseStrategy == BasicStrategy ? 0 : 1;
    SelectAIData.ignore_input.SetTicks(20);
    erlog("Select AI menu first cycle complete");
    return true;
}

bool SelectAICycle()
{
    if (SelectAIData.ignore_input.Finished()) {
        if (key[KEY_UP]) SelectAIData.which_selected = 0;
        if (key[KEY_DOWN]) SelectAIData.which_selected = 1;
        if (key[KEY_ENTER] || key[KEY_SPACE]) {
            EnemyChaseStrategy = SelectAIData.which_selected == 0 ? BasicStrategy : AStarStrategy;
            GoToStage(&main_menu_stage);
        }
    }

    SelectAIData.ignore_input.Update();
    return true;
}

std::string code1line[10] = {
    "Direction BasicStrategy(Vector pos, Vector player_pos, Tilemap* map)",
    "{",
    "Vector dist = player_pos - pos;",
    "if (fabs(dist.X()) >= fabs(dist.Y())) {",
    "    if (dist.XInt() == 0) return nowhere;",
    "    if (dist.X() < 0) {",
    "        if (map->GetTile(pos.XInt()-1, pos.YInt()).IsPassable() && (map->GetTile(pos.XInt()-1, pos.YInt()).IsEmpty() || pos+v_left == player_pos))",
    "            return left;",
    "    } else if (map->GetTile(pos.XInt()+1, pos.YInt()).IsPassable() && (map->GetTile(pos.XInt()+1, pos.YInt()).IsEmpty() || pos+v_right == player_pos))",
    "            return right;"
};

std::string code2line[10] = {
    "Direction AStarStrategy(Vector pos, Vector player_pos, Tilemap* map)",
    "{",
    "    bool can_get_through[10][9];",
    "    for (int i=0; i<10; ++i) {",
    "        for (int j=0; j<9; ++j) {",
    "            if (map->GetTile(i, j).IsPassable() && (map->GetTile(i, j).IsEmpty() || (i == player_pos.XInt() && j == player_pos.YInt())))",
    "                can_get_through[i][j] = true;",
    "            else",
    "                can_get_through[i][j] = false;",
    "        }"
};

bool SelectAIGraphics(BITMAP* buffer)
{
    if (!SelectAIData.gui) {
        SelectAIData.gui = create_bitmap(640, 480);
        if (!SelectAIData.gui) {
            erlog("Couldn't create the gui", 3);
            return false;
        }
        clear_to_color(SelectAIData.gui, Colour::MagicPink());
        DrawBox(SelectAIData.gui, Vector(20, 20), Vector(620, 420), Colour::Red(), 0.01*(float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX, 1, 2, 0.2);
        floodfill(SelectAIData.gui, 200, 200, Colour::White());
    }

    if (!SelectAIData.code1) {
        SelectAIData.code1 = create_bitmap(400, 140);
        if (!SelectAIData.code1) {
            erlog("Couldn't create code graphic 1");
            return false;
        }
        clear_to_color(SelectAIData.code1, Colour::MagicPink());
        for (int i=0; i<10; ++i)
            textprintf_ex(SelectAIData.code1, font, 0, i*10+2, Colour::Black(), -1, "%s", code1line[i].c_str());
        /*set_trans_blender(0, 0, 0, 10);
        drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
        for (int i=4; i<400; i+=4) {
            rectfill(SelectAIData.code1, i, 0, 400, 140, Colour::White());
        }
        drawing_mode(DRAW_MODE_SOLID, 0, 0, 0);*/
    }
    if (!SelectAIData.code2) {
        SelectAIData.code2 = create_bitmap(400, 140);
        if (!SelectAIData.code2) {
            erlog("Couldn't create code graphic 2");
            return false;
        }
        clear_to_color(SelectAIData.code2, Colour::MagicPink());
        for (int i=0; i<10; ++i)
            textprintf_ex(SelectAIData.code2, font, 0, i*10+2, Colour::Black(), -1, "%s", code2line[i].c_str());
        /*set_trans_blender(0, 0, 0, 10);
        drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
        for (int i=4; i<400; i+=4) {
            rectfill(SelectAIData.code2, i, 0, 400, 140, Colour::White());
        }
        drawing_mode(DRAW_MODE_SOLID, 0, 0, 0);*/
    }


    draw_sprite(buffer, SelectAIData.gui, 0, 0);

    textprintf_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, 50, 100, SelectAIData.which_selected==0?Colour::Red():Colour::Black(), -1, "Easy");
    textprintf_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, 50, 300, SelectAIData.which_selected==1?Colour::Red():Colour::Black(), -1, "Hard");

    draw_sprite(buffer, SelectAIData.code1, 140, 60);
    draw_sprite(buffer, SelectAIData.code2, 140, 260);

    return true;
}



struct GoodBadData {
    BITMAP* gui;
    int which_option;
    Time::Timer ignore_input;
}GoodBadData;

bool GoodBadEntry()
{
    GoodBadData.ignore_input.SetTicks(20);
    extern bool play_as_good;
    GoodBadData.which_option = play_as_good ? 0 : 1;

    return true;
}

bool GoodBadCycle()
{
    if (GoodBadData.ignore_input.Finished()) {
        if (key[KEY_UP]) GoodBadData.which_option = 0;
        if (key[KEY_DOWN]) GoodBadData.which_option = 1;
        if (key[KEY_ENTER] || key[KEY_SPACE]) {
            extern bool play_as_good;
            play_as_good = GoodBadData.which_option == 0 ? true : false;
            GoToStage(&level_stage);
            return true;
        }

        if (key[KEY_ESC])
            GoToStage(&main_menu_stage);
    }

    GoodBadData.ignore_input.Update();
    return true;
}

bool GoodBadGraphics(BITMAP* buffer)
{

    if (!GoodBadData.gui) {
        GoodBadData.gui = create_bitmap(480, 480);
        if (!GoodBadData.gui) {
            erlog("Couldn't create the gui", 3);
            return false;
        }
        clear_to_color(GoodBadData.gui, Colour::MagicPink());
        DrawBox(GoodBadData.gui, Vector(90, 20), Vector(390, 320), Colour::Red(), 0.01*(float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX, 1, 2, 0.2);
        floodfill(GoodBadData.gui, 200, 200, Colour::White());
    }

    draw_sprite(buffer, GoodBadData.gui, 80, 0);

    textout_centre_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, "Good", 320, 100, GoodBadData.which_option==0?Colour::Red():Colour::Black(), -1);
    textout_centre_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, "or", 320, 160, Colour::Black(), -1);
    textout_centre_ex(buffer, (FONT*)global_datafile[GLOBDAT_handwriting_font].dat, "Evil", 320, 210, GoodBadData.which_option==1?Colour::Red():Colour::Black(), -1);

    return true;
}
