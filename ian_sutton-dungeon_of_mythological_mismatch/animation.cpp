#include <fstream>
#include "animation.hpp"
#include "logging.hpp"
#include "colours.hpp"
#include <cmath>
#include <vector>

DATAFILE* global_datafile;

float crayon_line_wobble = 0;
float crayon_line_wobble_amplitude = 3;
int crayon_line_inner_radius = 1;
int crayon_line_outer_radius = 1;
int crayon_fade_colour = 100;
int points_down_line = 0;
Vector crayon_line_normal;

int prev_crayon_x = 0;
int prev_crayon_y = 0;

void CrayonBackPoint(BITMAP* bmp, int x, int y, int colour)
{
    circlefill(bmp, x, y, crayon_line_outer_radius, crayon_fade_colour);
    ++points_down_line;
}

void CrayonFrontPoint(BITMAP* bmp, int x, int y, int colour)
{
    circlefill(bmp, x, y, crayon_line_inner_radius, colour);
    ++ points_down_line;
}

void CrayonBackLine(BITMAP *bmp, int x, int y, int colour)
{
    Vector point(x, y);
    point += crayon_line_normal * (sin(points_down_line * crayon_line_wobble) * crayon_line_wobble_amplitude);

    do_line(bmp, prev_crayon_x, prev_crayon_y, point.XInt(), point.YInt(), crayon_fade_colour, CrayonBackPoint);
    prev_crayon_x = point.XInt();
    prev_crayon_y = point.YInt();
}

void CrayonFrontLine(BITMAP *bmp, int x, int y, int colour)
{
    Vector point(x, y);
    point += crayon_line_normal * (sin(points_down_line * crayon_line_wobble) * crayon_line_wobble_amplitude);

    do_line(bmp, prev_crayon_x, prev_crayon_y, point.XInt(), point.YInt(), colour, CrayonFrontPoint);
    prev_crayon_x = point.XInt();
    prev_crayon_y = point.YInt();
}

void CrayonLine(BITMAP* bmp, Vector start, Vector end, int colour, float wobble, float amplitude, int inner_radius, int outer_radius, float hardness)
{
    Vector line_tangent = end - start;
    if (rand()%2 == 0) // If the normal is a randomised then it should result in the lines starting off on different direction wobbles
        crayon_line_normal = Vector(-line_tangent.Y(), line_tangent.X());
    else
        crayon_line_normal = Vector(line_tangent.Y(), -line_tangent.X());

    crayon_line_normal *= (1/crayon_line_normal.Mag());
    prev_crayon_x = start.XInt();
    prev_crayon_y = start.YInt();
    crayon_line_wobble = wobble;
    crayon_line_wobble_amplitude = amplitude;
    crayon_line_inner_radius = inner_radius;
    crayon_line_outer_radius = outer_radius;

    int fg_r = getr(colour), fg_g = getg(colour), fg_b = getb(colour);
    int bg_r = (256 - fg_r)*hardness + fg_r, bg_g = (256 - fg_g)*hardness + fg_g, bg_b = (256 - fg_b)*hardness + fg_b;

    crayon_fade_colour = makecol(bg_r, bg_g, bg_b);

    points_down_line = 0;
    do_line(bmp, start.XInt(), start.YInt(), end.XInt(), end.YInt(), colour, CrayonBackLine);
    points_down_line = 0;
    prev_crayon_x = start.XInt();
    prev_crayon_y = start.YInt();
    do_line(bmp, start.XInt(), start.YInt(), end.XInt(), end.YInt(), colour, CrayonFrontLine);
}

void VerticalTallyLine(BITMAP* bmp, Vector top_left_of_line, int colour, int height)
{
    int new_top_x = top_left_of_line.XInt() + rand() % 5;
    int new_top_y = top_left_of_line.YInt() + rand() % 2;
    int new_bottom_x = top_left_of_line.XInt() + rand() % 2;
    int new_bottom_y = top_left_of_line.YInt() + height - rand() % 2;
    CrayonLine(bmp, Vector(new_top_x, new_top_y), Vector(new_bottom_x, new_bottom_y), colour, 0.02, 1, 0, 1);
}

void StrikeThroughTallyLine(BITMAP* bmp, Vector top_left_of_tally, int colour, int height)
{
    int start_x = top_left_of_tally.XInt() - rand() % 3 - 2;
    int end_x = top_left_of_tally.XInt() + 40 + rand() % 2;
    int start_y = top_left_of_tally.YInt() + height - rand() % 2;
    int end_y = top_left_of_tally.YInt() + rand() % 2;
    CrayonLine(bmp, Vector(start_x, start_y), Vector(end_x, end_y), colour, 0.02, 1, 0, 1);
}

void CrayonTally(BITMAP* bmp, Vector top_left, int number, int colour, int height)
{
    int how_many_whole = number/5;
    int how_many_not_whole = number % 5;

    for (int i=0; i<how_many_whole; ++i) {
        for (int j=0; j<4; ++j) {
            VerticalTallyLine(bmp, Vector(top_left.XInt()+i*50+j*10, top_left.YInt()), colour, height);
        }
        StrikeThroughTallyLine(bmp, Vector(top_left.X()+50*i, top_left.Y()), colour, height);
    }

    for (int i=0; i<how_many_not_whole; ++i) {
        VerticalTallyLine(bmp, Vector(top_left.XInt()+how_many_whole*50+i*10, top_left.YInt()), colour, height);
    }
}

bool is_circle_all_colour = true;
int other_colour = 0;
void CheckCircleForAllColour(BITMAP* bmp, int x, int y, int colour)
{
    int this_colour = getpixel(bmp, x, y);
    if (this_colour != colour || this_colour != other_colour)
        is_circle_all_colour = false;
}


std::vector<BITMAP*> splatters;

BITMAP* GetBloodSplatter()
{
    return splatters[rand() % splatters.size()];
}

void AddBloodSplatter(int how_many)
{
    for (int i=0; i<how_many; ++i) {
        BITMAP* bmp = create_bitmap(150, 150);
        clear_to_color(bmp, Colour::MagicPink());
        BloodSplatter(bmp, Vector(75,75));
        splatters.push_back(bmp);
    }
}

void BloodSplatter(BITMAP* bmp, Vector pos, int colour1, int colour2, int colour3)
{
    int init_max_radius = 40;
    int max_radius = init_max_radius;
    int do_how_many_max = 1;
    int do_how_many_min = 1;
    int do_how_many = 1;
    circlefill(bmp, pos.XInt(), pos.YInt(), rand() % max_radius, colour1);
    int radius;
    int x = -1, y = -1;
    while (max_radius > 0) {
        if (do_how_many_min == do_how_many_max)
            do_how_many = do_how_many_min;
        else
            do_how_many = do_how_many_min + (rand() % (do_how_many_max - do_how_many_min));
        for (int i = 0; i < do_how_many; ++i) {
            while (getpixel(bmp, x, y) != colour1) {
                x = pos.XInt() + (rand() % (4*init_max_radius)) - 2 * init_max_radius;
                y = pos.XInt() + (rand() % (4*init_max_radius)) - 2 * init_max_radius;
            }
            radius = rand() % max_radius;
            circlefill(bmp, x, y, radius, colour1);
        }
        max_radius *= 0.8;
        do_how_many_max *= 3;
        do_how_many_min *= 2.3;
        x = -1;
        y = -1;
    }

    max_radius = init_max_radius * 0.7;
    do_how_many_max = 1;
    do_how_many_min = 1;
    while (max_radius > 0) {
        if (do_how_many_min == do_how_many_max)
            do_how_many = do_how_many_min;
        else
            do_how_many = do_how_many_min + (rand() % (do_how_many_max - do_how_many_min));
        for (int i = 0; i < do_how_many; ++i) {
            while (true) {
                x = pos.XInt() + (rand() % (4*init_max_radius)) - 2 * init_max_radius;
                y = pos.XInt() + (rand() % (4*init_max_radius)) - 2 * init_max_radius;
                int pixel_colour = getpixel(bmp, x, y);
                if (pixel_colour == colour1 || pixel_colour == colour2) {
                    radius = rand() % max_radius;
                    is_circle_all_colour = true;
                    other_colour = colour1;
                    //do_circle(bmp, x, y, radius, colour2, CheckCircleForAllColour);
                    if (is_circle_all_colour) {
                        circlefill(bmp, x, y, radius, colour2);
                        break;
                    }
                }
            }
        }
        max_radius /= 1.6;
        do_how_many_max *= 2;
        do_how_many_min *= 1.5;
        x = -1;
        y = -1;
    }

    max_radius = init_max_radius * 0.4;
    do_how_many_max = 1;
    do_how_many_min = 1;
    while (max_radius > 0) {
        if (do_how_many_min == do_how_many_max)
            do_how_many = do_how_many_min;
        else
            do_how_many = do_how_many_min + (rand() % (do_how_many_max - do_how_many_min));
        for (int i = 0; i < do_how_many; ++i) {
            while (true) {
                x = pos.XInt() + (rand() % (4*init_max_radius)) - 2 * init_max_radius;
                y = pos.XInt() + (rand() % (4*init_max_radius)) - 2 * init_max_radius;
                int pixel_colour = getpixel(bmp, x, y);
                if (pixel_colour == colour1 || pixel_colour == colour2) {
                    radius = rand() % max_radius;
                    is_circle_all_colour = true;
                    other_colour = colour1;
                    //do_circle(bmp, x, y, radius, colour2, CheckCircleForAllColour);
                    if (is_circle_all_colour) {
                        circlefill(bmp, x, y, radius, colour3);
                        break;
                    }
                }
            }
        }
        max_radius /= 1.8;
        do_how_many_max *= 2;
        do_how_many_min *= 1.5;
        x = -1;
        y = -1;
    }

}


bool LoadGlobalDatafile()
{
    erlog("Loading global datafile");
    global_datafile = load_datafile("glob.dat");
    if (!global_datafile) {
        erlog("Failed to load \"glob.dat\"", 3);
        return false;
    }
    erlog("Loaded");
    return true;
}

void ReleaseGlobalDatafile()
{
    erlog("Clearing global datafile memory");
    if (!global_datafile) {
        erlog("Datafile not loaded anyway", 1);
        return;
    }
    unload_datafile(global_datafile);
    erlog("Memory cleared");
}


int AnimFrame::how_many = 0;
int AnimSequence::how_many = 0;
int Animation::how_many = 0;
std::map<std::string, AnimSequence> Animation::sequences;
bool Animation::sequences_loaded = false;


AnimFrame::AnimFrame() :
    graphic_id(-1),
    ticks(-1)
{
    ++how_many;
}

AnimFrame::AnimFrame(int graphic_id, int ticks)
{
    this->graphic_id = graphic_id;
    this->ticks = ticks;
    ++how_many;
}

AnimFrame::~AnimFrame()
{
    --how_many;
}

void AnimFrame::Draw(BITMAP* bmp, Vector pos, float scale, bool vertical_flip, float rotation, int alpha)
{
    BITMAP* graphic = (BITMAP*)global_datafile[graphic_id].dat;
    if (graphic == NULL) {
        erlog("Graphic ID out of bounds in frame draw", 3);
        return;
    }
    if (bmp == NULL) {
        erlog("Buffer out of bounds in frame draw", 3);
        return;
    }
    if (scale == 0.0 && vertical_flip == false && rotation == 0.0 && (alpha >= 256 || alpha < 0)) {
        draw_sprite(bmp, graphic, pos.XInt(), pos.YInt());
        return;
    }
    if (alpha >= 256 || alpha < 0) {
        if (vertical_flip == false) {
            rotate_scaled_sprite(bmp, graphic, pos.XInt(), pos.YInt(), fixmul(ftofix(rotation), radtofix_r), ftofix(scale));
            return;
        }
        rotate_scaled_sprite_v_flip(bmp, graphic, pos.XInt(), pos.YInt(), fixmul(ftofix(rotation), radtofix_r), ftofix(scale));
        return;
    }


    float fdiagonal = sqrt(static_cast<float>(graphic->w*graphic->w + graphic->h*graphic->h));
    int diagonal = static_cast<int>(fdiagonal + 1);
    BITMAP* temp_bmp = create_bitmap(diagonal, diagonal);

    if (vertical_flip == false) {
        rotate_scaled_sprite(temp_bmp, graphic, 0, 0, fixmul(ftofix(rotation), radtofix_r), ftofix(scale));
    }
    else {
        rotate_scaled_sprite_v_flip(temp_bmp, graphic, 0, 0, fixmul(ftofix(rotation), radtofix_r), ftofix(scale));
    }

    set_trans_blender(0, 0, 0, alpha);
    drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
    draw_trans_sprite(temp_bmp, bmp, pos.XInt(), pos.YInt());
    drawing_mode(DRAW_MODE_SOLID, 0, 0, 0);
}



Animation::Animation()
{
    seq = NULL;
    running = false;
    ticks_per_game_tick = 1.0;
    frame_ticks_remaining = 0;
    looping = true;
    how_many_loops = 0;
}

Animation::Animation(AnimSequence* seq)
{
    this->seq = seq;
    iter = seq->Start();
    end = seq->End();
    ticks_per_game_tick = 1.0;
    frame_ticks_remaining = static_cast<float>((*iter).Ticks());
    looping = true;
    how_many_loops = 0;
}

Animation::Animation(std::string seq_name)
{
    looping = true;
    SetSequence(seq_name);
}

void Animation::SetSequence(AnimSequence* seq)
{
    this->seq = seq;
    iter = seq->Start();
    end = seq->End();
    frame_ticks_remaining = static_cast<float>((*iter).Ticks());
    how_many_loops = 0;
}

void Animation::SetSequence(std::string seq_name)
{
    erlog("Setting animation sequence");
    if (!sequences_loaded) {
        LoadSequences();
    }
    if (sequences_loaded) {
        std::map<std::string, AnimSequence>::iterator map_iter = sequences.find(seq_name);
        if (map_iter == sequences.end()) {
            erlog("Requested animation doesn't exist");
        }
        else {
            use_glob_seq = true;
            glob_seq_name = seq_name;
            seq = NULL;
            iter = sequences[seq_name].Start();
            end = sequences[seq_name].End();
            ticks_per_game_tick = 1.0;
            frame_ticks_remaining = static_cast<float>((*iter).Ticks());
            running = true;
            erlog("Animation sequence set");
        }
    }
    else {
        erlog("Can't load animation without sequence list");
        seq = NULL;
        running = false;
        frame_ticks_remaining = 0;
    }
    how_many_loops = 0;
}

void Animation::Tick()
{
    if (running && (use_glob_seq || seq != NULL)) {
        float ticks_left_to_be_taken_off = ticks_per_game_tick;
        while (ticks_left_to_be_taken_off > 0.0) {
            if (ticks_left_to_be_taken_off < frame_ticks_remaining) {
                frame_ticks_remaining -= ticks_left_to_be_taken_off;
                return;
            }
            ticks_left_to_be_taken_off -= frame_ticks_remaining;
            iter++;
            if (iter == end) {
                ++how_many_loops;
                if (looping)
                    iter = use_glob_seq ? sequences[glob_seq_name].Start() : seq->Start();
                else {
                    erlog("Animation reached end. None looping.");
                    return;
                }
            }
            frame_ticks_remaining = static_cast<float>((*iter).Ticks());
        }
    }
}

void Animation::Draw(BITMAP* bmp, Vector pos, float scale, bool vertical_flip, float rotation, int alpha)
{
    if (use_glob_seq || seq != NULL)
        (*iter).Draw(bmp, pos, scale, vertical_flip, rotation, alpha);
}

bool Animation::AtEnd()
{
    return iter == end;
}

bool Animation::ReachedEndAtLeastOnce()
{
    return how_many_loops >= 1;
}

void Animation::Pause()
{
    running = false;
}

void Animation::UnPause()
{
    running = true;
}

void Animation::TogglePause()
{
    running = !running;
}

void Animation::Restart()
{
    if (use_glob_seq || seq != NULL) {
        iter = seq->Start();
        frame_ticks_remaining = static_cast<float>((*iter).Ticks());
    }
}


bool Animation::LoadSequences()
{
    if (sequences_loaded) {
        erlog("Attempted to load global sequence data. Already loaded", 1);
        return true;
    }
    erlog("Attempting to load global sequence data");


    // Load the file
    std::fstream fin("sequences.dat", std::fstream::in);
    if (!fin.is_open()) {
        erlog("Failed to load file \"sequences.dat\"", 3);
        return false;
    }

    // Parse the file. Format is "how_many_sequences sequence_name how_many_frames frame1_graphic_id frame1_ticks frame2_graphic_id frame2_ticks..."
    int how_many_sequences;
    std::string seq_name;
    int how_many_frames;
    AnimFrame cur_frame;
    AnimSequence cur_seq;
    int graphic_id, ticks;

    fin >> how_many_sequences;

    for (int i=0; i<how_many_sequences; ++i) {
        cur_seq.Clear();
        fin >> seq_name;
        if (fin.eof()) {
            erlog("Unexpected end of file: Expected sequence name", 3);
            sequences.clear();
            fin.close();
            return false;
        }
        fin >> how_many_frames;
        if (fin.eof()) {
            erlog("Unexpected end of file: Expected number of frames in sequence", 3);
            sequences.clear();
            fin.close();
            return false;
        }
        for (int i=0; i<how_many_frames; ++i) {
            fin >> graphic_id;
            if (fin.eof()) {
                erlog("Unexpected end of file: Expected graphic id", 3);
                sequences.clear();
                fin.close();
                return false;
            }
            fin >> ticks;
            if (fin.eof()) {
                erlog("Unexpected end of file: Expected number of ticks", 3);
                sequences.clear();
                fin.close();
                return false;
            }
            cur_frame.Set(graphic_id, ticks);
            cur_seq.AppendFrame(cur_frame);
        }
        sequences.insert(std::pair<std::string, AnimSequence>(seq_name, cur_seq));
        erlog(seq_name);
    }
    sequences_loaded = true;
    fin.close();
    erlog("Loaded. Hopefully.");
    return true;
}





