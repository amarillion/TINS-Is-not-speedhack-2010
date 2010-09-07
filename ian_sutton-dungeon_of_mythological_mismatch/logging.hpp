#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <list>
#include <string>
#include <fstream>

#include <allegro.h>

std::string Timestamp(std::string format);

class Logging {
    public:
        Logging();
        ~Logging();

        bool LogToFile(std::string filename);
        void StopLoggingToFile();
        void Draw(BITMAP* bitmap, int x, int y, int how_many);

        void Say(std::string message, int error_level = 0); // 0 = informational message, 3 = critical error
        void operator()(std::string message, int error_level = 0) { Say(message, error_level); }

        void SetErrorLevel(int error_level);
        void ToggleErrorLevelDisplay(int error_level) { show_error_level[error_level] = !show_error_level[error_level]; }

        void ToggleTimestamps() { show_timestamps = !show_timestamps; }

        void SetAlpha(int alpha) { this->alpha = alpha; if (this->alpha > 256) this->alpha = 256; if (this->alpha < 0) this->alpha = 0; }
        int Alpha() { return alpha; }

    protected:

        class Message {
            public:
                Message(std::string text);
                Message(std::string text, int error_level);

                void Print(BITMAP* bitmap, FONT* font, int x, int y, bool show_timestamp = false);
                std::string Text() { return text; }
                std::string Timestamp() { return timestamp; }
                int ErrorLevel() { return error_level; }

            protected:
                std::string timestamp;
                std::string text;
                int error_level;
        };

        std::list<Logging::Message> msg;

        std::fstream fout;

        bool show_error_level[4];
        bool show_timestamps;

        int alpha;

        BITMAP* buffer;
};

extern Logging erlog;

#endif
