#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>
#include <allegro.h>
#include <string>
#include "vector.hpp"
#include "animation.hpp"
#include "time.hpp"

extern bool play_as_good;

extern const int tile_width;
extern const int tile_height;

enum ObjectType { nothing, player1, enemy, civilian1, nextlevel, prevlevel, ammo_stash };
enum Direction { left, right, up, down, nowhere };

class Tilemap;
class Tilemap;
class Tile;

class TileObject {
    public:
        TileObject() { owner = NULL, dir = up; }
        virtual void Update(int x_tile, int y_tile, Tilemap* map) = 0;
        virtual void Draw(BITMAP*, Vector pos) = 0;

        void SetOwner(Tile* owner) { this->owner = owner; }
        Tile* GetOwner() { return owner; }

        virtual ObjectType WhatAmI() = 0;

        void FaceDirection(Direction dir) { this->dir = dir; }

        void SetAnimation(std::string anim_name) { anim.SetSequence(anim_name); };

    protected:
        Animation anim;

        Tile* owner;
        Direction dir;
};

class Player : public TileObject {
    public:
        Player();
        void Update(int x_tile, int y_tile, Tilemap* map);
        void Draw(BITMAP*, Vector pos);

        ObjectType WhatAmI() { return player1; }

        bool DidLandOnCivilian() { bool did_i = landed_on_civilian; landed_on_civilian = false; return did_i; }
        int Reward() { return reward; }
        int LastReward() { return last_reward_recieved; }

        int HowManyLives() { return lives; }
        void LoseLife() { --lives; }
        void GiveLives(int how_many) { lives += how_many; }

        int HowManyAmmo() { return ammo; }
        void LoseAmmo() { --ammo; }
        void GiveAmmo(int how_many) { ammo += how_many; }

        void CollidedWithEnemy(Tilemap* map);

        void SetTileOffset(Vector offset) { offset_from_tile = offset; }

    protected:
        Time::Timer time_until_next_step;
        Time::Timer can_do_special_ability;
        Time::Timer time_until_stop_shot;

        Vector where_to_shoot;

        bool landed_on_civilian;
        int last_reward_recieved;
        int reward;
        int reward_entered_level_with;
        int ammo_entered_level_with;
        int current_level;
        Vector offset_from_tile;

        friend class Tilemap;

        int lives;
        bool died;
        int ammo;
        int max_ammo_from_stash;
};

Direction BasicStrategy(Vector pos, Vector player_pos, Tilemap* map);
Direction AStarStrategy(Vector pos, Vector player_pos, Tilemap* map);

class Enemy : public TileObject {
    public:
        Enemy();
        void Update(int x_tile, int y_tile, Tilemap* map);
        void Draw(BITMAP*, Vector pos);

        ObjectType WhatAmI() { return enemy; }

        void SetChaseStrategy(Direction(*Strategy)(Vector pos, Vector player_pos, Tilemap* map)) { this->Strategy = Strategy; }


    protected:
        Time::Timer time_until_next_step;
        Vector offset_from_tile;
        int step_time;
        int radius_of_caring; // Won't bother going after you if you are further than this number of direct steps away
        Direction (*Strategy)(Vector pos, Vector player_pos, Tilemap* map);
};

class Enemy1 : public Enemy {
    public:
        Enemy1() { radius_of_caring = 3; step_time = 20; SetAnimation(play_as_good ? "baddy_enemy1" : "goody_enemy1"); }
};

class Enemy2 : public Enemy {
    public:
        Enemy2() { radius_of_caring = 4; step_time = 18; SetAnimation(play_as_good ? "baddy_enemy2" : "goody_enemy2"); }
};

class Enemy3 : public Enemy {
    public:
        Enemy3() { radius_of_caring = 5; step_time = 15; SetAnimation(play_as_good ? "baddy_enemy3" : "goody_enemy3"); }
};

class Civilian : public TileObject {
    public:
        Civilian();

        void Update(int x_tile, int y_tile, Tilemap* map);
        void Draw(BITMAP*, Vector pos);

        void SetReward(int reward) { how_much_reward = reward; }
        int GetReward() { return how_much_reward; }

        ObjectType WhatAmI() { return civilian1; }

    protected:
        int how_much_reward;
};

class NextLevelTrigger : public TileObject {
    public:
        NextLevelTrigger() {}

        void Update(int x_tile, int y_tile, Tilemap* map) {}
        void Draw(BITMAP* bmp, Vector pos) { draw_sprite(bmp, (BITMAP*)(global_datafile[GLOBDAT_next_level_object].dat), pos.XInt(), pos.YInt());}

        ObjectType WhatAmI() { return nextlevel; }
};

class AmmoStash : public TileObject {
    public:
        AmmoStash() {}

        void Update(int x_tile, int y_tile, Tilemap* map) {}
        void Draw(BITMAP* bmp, Vector pos) { draw_sprite(bmp, (BITMAP*)(global_datafile[GLOBDAT_ammo_stash].dat), pos.XInt(), pos.YInt()); }

        ObjectType WhatAmI() { return ammo_stash; }
};




class Tile {
    public:
        Tile() { object = NULL; tile_type = 1; }
        void SetTileType(int tile_type) { this->tile_type = tile_type; };

        TileObject* GetObject() { return object; }
        TileObject* RemoveObject() { TileObject* temp_obj = object; object = NULL; return temp_obj; }
        void SetObject(TileObject* obj) { object = obj; if (obj != NULL) obj->SetOwner(this); }

        void SwapObjects(Tile& tile);

        void Draw(BITMAP* buffer, Vector top_left);
        void DrawObjects(BITMAP* buffer, Vector top_left);
        void Update(int x_tile, int y_tile, Tilemap* map);

        bool IsEmpty() { return object == NULL ? true : false; }
        bool IsPassable() { if (tile_type >= 3) return true; return false; }

    protected:
        TileObject* object;

        int tile_type;
};



class Tilemap {
    public:
        Tilemap();
        ~Tilemap();

        void SetUpMap(int tile[], int objects[], Player* player = NULL); // tile and objects MUST end in -1

        void Draw(BITMAP*);
        void Update();

        friend class TileObject;

        Tile& GetTile(int x, int y);

        void SetAllVisibilities(int visibility);
        void SetVisibility(int i, int j, int visibility);

        Player* PlayerObject(int& tile_x, int& tile_y);

        void PositionPlayer(Player*, int tile_x, int tile_y);

        ObjectType ObjectTypeAt(int tile_x, int tile_y);
        TileObject* ObjectAt(int tile_x, int tile_y);

        void ClearTileObject(int x, int y);
        void ClearMap();

        void BlockMovement(int how_long_in_ticks) { movement_blocker.SetTicks(how_long_in_ticks); }
        void UnblockMovement() { movement_blocker.SetTicks(0); }

        bool IsMovementBlocked() { return !movement_blocker; }

        void DoCivilianSplash();

        void Purge();

    protected:
        std::vector<std::vector<Tile> > tiles;
        std::vector<std::vector<int> > visible; // 0 - blacked out, 1 - tile visible, object hidden, 2 - all visible

        bool is_map_loaded;

        Time::Timer movement_blocker;

        BITMAP* grid;
        BITMAP* citizen_saved;

        BITMAP* blood;
        bool need_to_draw_blood, need_fresh_blood;
        int blood_alpha;
        bool need_to_draw_message;
        float message_scale;

        bool need_purge;
        std::vector<TileObject*> to_purge;
};




#endif
