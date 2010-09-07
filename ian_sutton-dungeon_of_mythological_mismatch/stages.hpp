#ifndef STAGES_HPP
#define STAGES_HPP

#include <allegro.h>
#include "vector.hpp"

class Stage {
    public:
        Stage(bool (*FirstEntry)(), bool (*LogicCycle)(), bool (*Graphics)(BITMAP*)) { first_entry_func = FirstEntry; logic_cycle_func = LogicCycle; graphics_func = Graphics; }

        void SetFirstEntryFunction(bool (*FirstEntry)()) { first_entry_func = FirstEntry; }
        void SetLogicCycleFunction(bool (*LogicCycle)()) { logic_cycle_func = LogicCycle; }
        void SetGraphicsFunction(bool (*Graphics)(BITMAP*)) { graphics_func = Graphics; }

        bool FirstCycle() { return first_entry_func(); }
        bool Cycle() { return logic_cycle_func(); }
        bool Draw(BITMAP* buf) { return graphics_func(buf); }

    protected:
        bool (*first_entry_func)();
        bool (*logic_cycle_func)();
        bool (*graphics_func)(BITMAP*);

};

void GoToStage(Stage* which);

extern Stage* stage;

extern Stage main_menu_stage;
extern Stage level_stage;
extern Stage select_ai_stage;
extern Stage high_scores_stage;
extern Stage select_good_bad_stage;


#endif
