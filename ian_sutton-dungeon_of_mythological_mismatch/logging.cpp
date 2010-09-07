#include "logging.hpp"
#include "time.hpp"
#include "colours.hpp"

Logging erlog;

Logging::Message::Message(std::string text)
{
    this->text = text;
    error_level = 0;
    timestamp = Time::Timestamp("%d/%m/%y %S:%M:%H");
}

Logging::Message::Message(std::string text, int error_level)
{
    this->text = text;
    this->error_level = error_level;
    timestamp = Time::Timestamp("%d/%m/%y %S:%M:%H");
}

void Logging::Message::Print(BITMAP* bitmap, FONT* font, int x, int y, bool show_timestamp)
{
    if (show_timestamp) {
        switch (error_level) {
            case 3: textprintf_ex(bitmap, font, x, y, Colour::Red(), -1, "%s: %s", timestamp.c_str(), text.c_str()); break;
            case 2: textprintf_ex(bitmap, font, x, y, Colour::Green(), -1, "%s: %s", timestamp.c_str(), text.c_str()); break;
            case 1: textprintf_ex(bitmap, font, x, y, Colour::Blue(), -1, "%s: %s", timestamp.c_str(), text.c_str()); break;
            case 0:
            default: textprintf_ex(bitmap, font, x, y, Colour::White(), -1, "%s: %s", timestamp.c_str(), text.c_str()); break;
        }
    }
    else {
        switch (error_level) {
            case 3: textprintf_ex(bitmap, font, x, y, Colour::Red(), -1, "%s", text.c_str()); break;
            case 2: textprintf_ex(bitmap, font, x, y, Colour::Green(), -1, "%s", text.c_str()); break;
            case 1: textprintf_ex(bitmap, font, x, y, Colour::Blue(), -1, "%s", text.c_str()); break;
            case 0:
            default: textprintf_ex(bitmap, font, x, y, Colour::Black(), -1, "%s", text.c_str()); break;
        }
    }
}

Logging::Logging()
{
    show_error_level[0] = true;
    show_error_level[1] = true;
    show_error_level[2] = true;
    show_error_level[3] = true;
    Say("Log Started");
    alpha = 30;
}

Logging::~Logging()
{
    Say("Log Ended");
    if (fout.is_open())
        fout.close();
}


bool Logging::LogToFile(std::string filename)
{
    if (!fout.is_open()) {
        fout.open(filename.c_str(), std::fstream::out | std::fstream::app);
        if (!fout.is_open())
            return false;
        std::list<Logging::Message>::iterator iter = msg.begin();
        std::list<Logging::Message>::iterator end = msg.end();
        while (iter != end) {
            switch ((*iter).ErrorLevel()) {
                default:
                case 0: fout << "[message]\t "; break;
                case 1: fout << "[warning]\t "; break;
                case 2: fout << "[error]\t\t "; break;
                case 3: fout << "[critical error] "; break;
            }
            fout << (*iter).Timestamp() << ": " << (*iter).Text() << "\n";
            fout.flush();
            iter++;
        }
        return true;
    }
    return false;
}

void Logging::StopLoggingToFile()
{
    if (fout.is_open()) {
        Say("Log file closed");
        fout.close();
    }
}

void Logging::Draw(BITMAP* bitmap, int x, int y, int how_many)
{
    if (alpha <= 0) // No point doing anything as nothings going to be drawn
        return;
    if (!buffer) {
        buffer = create_bitmap(640, 480);
        if (!buffer) {
            Say("Failed to create bitmap for screen logging", 1);
            return;
        }
    }
    clear_to_color(buffer, Colour::MagicPink());
    int how_many_drawn = 0;
    std::list<Logging::Message>::reverse_iterator iter = msg.rbegin();
    std::list<Logging::Message>::reverse_iterator end = msg.rend();
    while (iter != end && how_many_drawn < how_many) {
        if (show_error_level[(*iter).ErrorLevel()]) {
            (*iter).Print(buffer, font, x, (how_many - how_many_drawn) * 10, show_timestamps);
            ++how_many_drawn;
        }
        iter++;
    }
    if (alpha >= 256)
        draw_sprite(bitmap, buffer, x, y);
    else {
        set_trans_blender(0, 0, 0, Alpha());
        drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
        draw_trans_sprite(bitmap, buffer, x, y);
        drawing_mode(DRAW_MODE_SOLID, 0, 0, 0);
    }
}

void Logging::Say(std::string message, int error_level)
{
    msg.push_back(Message(message, error_level));
    if (fout.is_open()) {
        std::list<Logging::Message>::reverse_iterator iter = msg.rbegin();
        switch (error_level) {
            default:
            case 0: fout << "[message]\t "; break;
            case 1: fout << "[warning]\t "; break;
            case 2: fout << "[error]\t\t "; break;
            case 3: fout << "[critical error] "; break;
        }
        fout << (*iter).Timestamp() << ": " << (*iter).Text() << "\n";
        fout.flush();
    }
}

void Logging::SetErrorLevel(int error_level)
{
    show_error_level[0] = false;
    show_error_level[1] = false;
    show_error_level[2] = false;
    show_error_level[3] = false;

    switch (error_level) {
        case 0: show_error_level[0] = true;
        case 1: show_error_level[1] = true;
        case 2: show_error_level[2] = true;
        case 3: show_error_level[3] = true;

    }
}











