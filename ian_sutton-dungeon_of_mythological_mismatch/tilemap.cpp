#include <cmath>
#include "tilemap.hpp"
#include "animation.hpp"
#include "colours.hpp"
#include "levelmaps.hpp"
#include "screen.hpp"
#include "vector.hpp"
#include "logging.hpp"

const Vector tile_dim(64, 48);

void Tile::Update(int x_tile, int y_tile, Tilemap* map)
{
    if (object != NULL)
        object->Update(x_tile, y_tile, map);
}


void Tile::Draw(BITMAP* buffer, Vector top_left)
{
    int graphic = 5;
    switch (tile_type) {
        case 0: graphic = GLOBDAT_tile_wall1; break;
        case 1: graphic = GLOBDAT_tile_wall2; break;
        case 2: graphic = GLOBDAT_tile_wall3; break; // You never know, there might be enough room for them all
        case 3: graphic = GLOBDAT_tile_path1; break;
        case 4: graphic = GLOBDAT_tile_path2; break;
        default:
        case 5: graphic = GLOBDAT_tile_path3; break;
        case 6: graphic = GLOBDAT_tile_bloodstainedpath;
    }

    if (global_datafile[graphic].dat != NULL)
        draw_sprite(buffer, (BITMAP*)global_datafile[graphic].dat, top_left.XInt(), top_left.YInt());
}

void Tile::DrawObjects(BITMAP* buffer, Vector top_left)
{
    if (object != NULL)
        object->Draw(buffer, top_left);
}

void Tile::SwapObjects(Tile& tile)
{
    //erlog("Swapping tile object");
    TileObject* temp_obj = object;
    object = tile.object;
    tile.object = temp_obj;
    //erlog("Checking object nullities");
    if (temp_obj != NULL) {
        //erlog("temp_obj exists");
        temp_obj->SetOwner(&tile);
    }
    if (object != NULL) {
        //erlog("object exists");
        //object->SetOwner(this);
    }
    //erlog("Tile object swapped");
}




Tilemap::Tilemap()
{
    tiles.resize(10);
    for (int i=0; i<(int)tiles.size(); ++i) {
        tiles[i].resize(9);
    }
    visible.resize(10);
    for (int i=0; i<(int)tiles.size(); ++i) {
        visible[i].resize(9);
        SetAllVisibilities(0);
    }
    is_map_loaded = false;
    citizen_saved = NULL;
    blood = NULL;
    blood_alpha = 0;
    need_to_draw_blood = false;
    need_fresh_blood = true;
    need_to_draw_message = false;
    message_scale = 0.0;
}

Tilemap::~Tilemap()
{
    if (grid)
        destroy_bitmap(grid);
    if (citizen_saved)
        destroy_bitmap(citizen_saved);
    if (blood) destroy_bitmap(blood);
    ClearMap();
}


void Tilemap::SetUpMap(int new_tile[], int new_objects[], Player* player)
{
    erlog("Initialising map");
    if (is_map_loaded) {
        erlog("A map is already loaded");
        //ClearMap();
    }
    int x = 0, y = 0;
    for (int i=0; new_tile[i] != -1 && new_objects[i] != -1; ++i) {
        tiles[x][y].SetTileType(new_tile[i]);
        switch (new_objects[i]) {
            case 0:
            default:
                tiles[x][y].SetObject(NULL);
                break;
            case 1: {
                if (player == NULL) {
                    erlog("Creating new player object");
                    player = new Player;
                    tiles[x][y].SetObject(player);
                }
                else
                    tiles[x][y].SetObject(player);
                player->FaceDirection(up);
                player->SetTileOffset(Vector(0, 32));
                movement_blocker.SetTicks(120);
                if (player->died == false) {
                    player->reward_entered_level_with = player->reward;
                    player->ammo_entered_level_with = player->ammo;
                }
                else
                    player->died = false;

                break;
            }
            case 2: {
                tiles[x][y].SetObject(new NextLevelTrigger);
                break;
            }
            // I've skipped out 3 in case I get around to adding a PrevLevelTrigger. Which would be handy so you could go back and get ammo et, but no time for the mo
            case 4:
                tiles[x][y].SetObject(new Enemy1);
                break;
            case 5:
                tiles[x][y].SetObject(new Enemy2);
                break;
            case 6:
                tiles[x][y].SetObject(new Enemy3);
                break;
            case 7: {
                Civilian* civ = new Civilian;
                civ->SetReward(10);
                civ->SetAnimation("civilian");
                tiles[x][y].SetObject(civ);
                break;
            }
            case 8:
                tiles[x][y].SetObject(new AmmoStash);
                break;
        }
        ++x;
        if (x == (int)tiles.size()) {
            x = 0;
            ++y;
        }
    }

    SetAllVisibilities(0);
    erlog("Map initialised");
    if (grid == NULL) {
        erlog("Creating grid");
        grid = create_bitmap(scr_w, scr_h);

        if (!grid) {
            erlog("Failed to allocate memory");
            return;
        }
        clear_to_color(grid, Colour::MagicPink());
        for (int i=1; i<10; ++i) {
            CrayonLine(grid, Vector(i*tile_dim.XInt(), 0), Vector(i*tile_dim.XInt(), scr_h), Colour::Carbon(), 0.02*((float)rand()/(float)RAND_MAX), 1+((float)rand()/(float)RAND_MAX));
        }
        for (int j=1; j<9; ++j) {
            CrayonLine(grid, Vector(0, j*tile_dim.YInt()), Vector(640, j*tile_dim.YInt()), Colour::Carbon(), 0.02*((float)rand()/(float)RAND_MAX), 1+((float)rand()/(float)RAND_MAX));
        }
    }
    if (citizen_saved == NULL) {
        erlog("Creating messages");
        citizen_saved = create_bitmap(640, 400);
        if (!citizen_saved) {
            erlog("Could not allocate citizen message memory", 3);
            return;
        }
        clear_to_color(citizen_saved, Colour::MagicPink());

        extern bool play_as_good;
        if (play_as_good) {
            textout_centre_ex(citizen_saved, (FONT*)global_datafile[GLOBDAT_large_handwriting_font].dat, "Citizen", citizen_saved->w/2, 0, Colour::GUIGold(), -1);
            textout_centre_ex(citizen_saved, (FONT*)global_datafile[GLOBDAT_large_handwriting_font].dat, "Saved", citizen_saved->w/2, 200, Colour::GUIGold(), -1);
        }
        else {
            textout_centre_ex(citizen_saved, (FONT*)global_datafile[GLOBDAT_large_handwriting_font].dat, "Citizen", citizen_saved->w/2, 0, Colour::BloodRed1(), -1);
            textout_centre_ex(citizen_saved, (FONT*)global_datafile[GLOBDAT_large_handwriting_font].dat, "Slaughtered", citizen_saved->w/2, 200, Colour::BloodRed1(), -1);
        }
    }
    if (blood == NULL)
        blood = create_bitmap(640, 480);

    is_map_loaded = true;
    erlog("Everything else initialised");
}

void Tilemap::Draw(BITMAP* buffer) // Uncomment the code in here to turn fog of war on
{
    for (int i=0; i<(int)tiles.size(); ++i) {
        for (int j=0; j<(int)tiles[i].size(); ++j) {
            //if (visible[i][j] > 0)
                tiles[i][j].Draw(buffer, Vector(tile_dim.XInt()*i, tile_dim.YInt()*j));
        }
    }
    draw_sprite(buffer, grid, 0, 0);

    for (int i=0; i<(int)tiles.size(); ++i) {
        for (int j=0; j<(int)tiles[i].size(); ++j) {
            //if (visible[i][j] == 256)
                tiles[i][j].DrawObjects(buffer, Vector(tile_dim.XInt()*i, tile_dim.YInt()*j));
            /*else {
                set_trans_blender(0, 0, 0, 256 - visible[i][j]);
                drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
                rectfill(buffer, tile_dim.XInt()*i, tile_dim.YInt()*j, tile_dim.XInt()*(i+1)-1, tile_dim.YInt()*(j+1)-1,  Colour::Black());
                drawing_mode(DRAW_MODE_SOLID, 0, 0, 0);
            }*/
        }
    }

    if (need_to_draw_message) {
        int w = citizen_saved->w * message_scale;
        int h = citizen_saved->h * message_scale;
        int x = scr_w/2 - w/2;
        int y = scr_h/2 - h/2;
        stretch_sprite(buffer, citizen_saved, x, y, w, h);
    }

    if (need_to_draw_blood) {
        set_trans_blender(0, 0, 0, blood_alpha);
        drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
        draw_trans_sprite(buffer, blood, 0, 0);
        drawing_mode(DRAW_MODE_SOLID, 0, 0, 0);
    }
    if (!need_to_draw_blood && need_fresh_blood && blood != NULL) {
        clear_to_color(blood, Colour::MagicPink());
        int how_many_splatters = 4 + rand() % 6;
        for (int i=0; i<how_many_splatters; ++i)
            draw_sprite(blood, GetBloodSplatter(), rand() % 540, rand() % 300);
        need_fresh_blood = false;
    }
}

void Tilemap::Update()
{
    for (int i=0; i<(int)tiles.size(); ++i) {
        for (int j=0; j<(int)tiles[i].size(); ++j) {
            tiles[i][j].Update(i, j, this);
        }
    }

    if (need_to_draw_blood) {
        blood_alpha -=2;
        if (blood_alpha <= 0) {
            need_to_draw_blood = false;
            need_fresh_blood = true;
        }
    }

    if (need_to_draw_message)
        message_scale += 0.04;
    if (message_scale >= 1.1)
        need_to_draw_message = false;

    movement_blocker.Update();
}


Tile& Tilemap::GetTile(int x, int y)
{
    if (x < (int)tiles.size() && y < (int)tiles[x].size())
        return tiles[x][y];
    static Tile no_tile;
    return no_tile;
}


void Tilemap::SetAllVisibilities(int visibility)
{
    for (int i=0; i <(int)visible.size(); ++i)
        for (int j=0; j<(int)visible[i].size(); ++j)
            visible[i][j] = visibility;
}

void Tilemap::SetVisibility(int i, int j, int visibility)
{
    if (i < (int)visible.size() && j < (int)visible[i].size())
        visible[i][j] = visibility;
}

Player* Tilemap::PlayerObject(int& tile_x, int& tile_y)
{
    for (int i=0; i<(int)tiles.size(); ++i) {
        for (int j=0; j<(int)tiles[i].size(); ++j) {
            if (tiles[i][j].GetObject() == NULL)
                continue;
            if (tiles[i][j].GetObject()->WhatAmI() == player1) {
                tile_x = i;
                tile_y = j;
                return (Player*)tiles[i][j].GetObject();
            }
        }
    }
    erlog("Looked for player in map, but could not find it", 1);
    return NULL;
}


ObjectType Tilemap::ObjectTypeAt(int tile_x, int tile_y)
{
    TileObject* obj = ObjectAt(tile_x, tile_y);
    return (obj == NULL) ? nothing : obj->WhatAmI();
}

TileObject* Tilemap::ObjectAt(int tile_x, int tile_y)
{
    if (tile_x < (int)tiles.size() && tile_y < (int)tiles[tile_x].size())
        return tiles[tile_x][tile_y].GetObject();
    return NULL;
}

void Tilemap::ClearTileObject(int x, int y)
{
    TileObject* obj = ObjectAt(x, y);
    if (obj->WhatAmI() != player1) {
        to_purge.push_back(obj);
        need_purge = true;
    }

}

void Tilemap::ClearMap()
{
    for (int i=0; i<(int)tiles.size(); ++i) {
        for (int j=0; j<(int)tiles[i].size(); ++j) {
            ClearTileObject(i, j);
        }
    }
}

void Tilemap::Purge()
{
    if (need_purge) {
        std::vector<TileObject*>::iterator iter = to_purge.begin();
        while (iter != to_purge.end()) {
            delete (TileObject*)(*iter);
            iter++;
        }
        to_purge.clear();
    }
}

void Tilemap::PositionPlayer(Player* player, int tile_x, int tile_y)
{
    if (player != NULL && tile_x < (int)tiles.size() && tile_y < (int)tiles[tile_x].size()) {
        int cur_x, cur_y;
        PlayerObject(cur_x, cur_y);
        tiles[cur_x][cur_y].SetObject(NULL);
        tiles[tile_x][tile_y].SetObject(player);
    }
}

void Tilemap::DoCivilianSplash()
{
    extern bool play_as_good;

    if (!play_as_good) {
        need_to_draw_blood = true;
        blood_alpha = 256;
    }
    need_to_draw_message = true;
    message_scale = 0.0;

    //BlockMovement(60);
}




Player::Player()
{
    reward_entered_level_with = 0;
    reward = 0;
    anim.SetSequence(play_as_good ? "goody_playerwalk" : "baddy_playerwalk");
    current_level = 1;
    offset_from_tile = v_home;

    lives = 8;
    died = false;
    ammo = 5;
    ammo_entered_level_with = 5;

    max_ammo_from_stash = 3;
}

void Player::Update(int x, int y, Tilemap* map)
{
    map->SetVisibility(x-2, y-1, 50);
    map->SetVisibility(x-2, y, 50);
    map->SetVisibility(x-2, y+1, 50);
    map->SetVisibility(x+2, y-1, 50);
    map->SetVisibility(x+2, y, 50);
    map->SetVisibility(x+2, y+1, 50);
    map->SetVisibility(x-1, y-2, 50);
    map->SetVisibility(x, y-2, 50);
    map->SetVisibility(x+1, y-2, 50);
    map->SetVisibility(x-1, y+2, 50);
    map->SetVisibility(x, y+2, 50);
    map->SetVisibility(x+1, y+2, 50);

    map->SetVisibility(x-1, y-1, 200);
    map->SetVisibility(x-1, y, 256);
    map->SetVisibility(x-1, y+1, 200);
    map->SetVisibility(x, y-1, 256);
    map->SetVisibility(x, y, 256);
    map->SetVisibility(x, y+1, 256);
    map->SetVisibility(x+1, y-1, 200);
    map->SetVisibility(x+1, y, 256);
    map->SetVisibility(x+1, y+1, 200);

    anim.Tick();
    time_until_next_step.Update();
    can_do_special_ability.Update();
    time_until_stop_shot.Update();

    bool move_tile = false;
    Vector which_tile_go_to;

    offset_from_tile *= 0.8;
    if (!map->IsMovementBlocked()) {
        if (key[KEY_UP]) {
            if (map->GetTile(x, y-1).IsPassable() && time_until_next_step.Finished()) {
                move_tile = true;
                which_tile_go_to = Vector(x, y-1);
                offset_from_tile = Vector(0.0, tile_dim.Y());
            }
            FaceDirection(up);
        }
        else if (key[KEY_DOWN]) {
            if (map->GetTile(x, y+1).IsPassable() && time_until_next_step.Finished()) {
                move_tile = true;
                which_tile_go_to = Vector(x, y+1);
                offset_from_tile = Vector(0.0, -tile_dim.Y());
            }
            FaceDirection(down);
        }
        else if (key[KEY_LEFT]) {
            if (map->GetTile(x-1, y).IsPassable() && time_until_next_step.Finished()) {
                move_tile = true;
                which_tile_go_to = Vector(x-1, y);
                offset_from_tile = Vector(tile_dim.X(), 0.0);
            }
            FaceDirection(left);
        }
        else if (key[KEY_RIGHT]) {
            if (map->GetTile(x+1, y).IsPassable() && time_until_next_step.Finished()) {
                move_tile = true;
                which_tile_go_to = Vector(x+1, y);
                offset_from_tile = Vector(-tile_dim.X(), 0.0);
            }
            FaceDirection(right);
        }

        if (key[KEY_SPACE] && can_do_special_ability && ammo > 0) {
            TileObject* first_object_found = NULL;
            int x_tile_stopped_at, y_tile_stopped_at;
            switch (dir) {
                case up: {
                    for (int i=y-1; i>=0; --i) {
                        x_tile_stopped_at = x;
                        y_tile_stopped_at = i;
                        first_object_found = map->ObjectAt(x, i);
                        if (first_object_found != NULL || !map->GetTile(x, i).IsPassable())
                            break;
                        if (i==0) break;
                    }
                    break;
                }
                case down: {
                    for (int i=y+1; i<9; ++i) {
                        x_tile_stopped_at = x;
                        y_tile_stopped_at = i;
                        first_object_found = map->ObjectAt(x, i);
                        if (first_object_found != NULL || !map->GetTile(x, i).IsPassable())
                            break;
                    }
                    break;
                }
                case left: {
                    for (int i=x-1; i>=0; --i) {
                        x_tile_stopped_at = i;
                        y_tile_stopped_at = y;
                        first_object_found = map->ObjectAt(i, y);
                        if (first_object_found != NULL || !map->GetTile(i, y).IsPassable())
                            break;
                        if (i==0) break;
                    }
                    break;
                }
                case right: {
                    for (int i=x+1; i<10; ++i) {
                        x_tile_stopped_at = i;
                        y_tile_stopped_at = y;
                        first_object_found = map->ObjectAt(i, y);
                        if (first_object_found != NULL || !map->GetTile(i, y).IsPassable())
                            break;
                    }
                    break;
                }
                case nowhere:
                default:
                    break;
            }
            if (first_object_found != NULL && first_object_found->WhatAmI() == enemy) {
                map->GetTile(x_tile_stopped_at, y_tile_stopped_at).SetTileType(5);
                delete (Enemy1*)first_object_found;
                map->GetTile(x_tile_stopped_at, y_tile_stopped_at).SetObject(NULL);
                map->GetTile(x_tile_stopped_at, y_tile_stopped_at).SetTileType(GLOBDAT_tile_bloodstainedpath);
            }
            time_until_next_step.SetTicks(24);
            time_until_stop_shot.SetTicks(20);
            can_do_special_ability.SetTicks(60);
            where_to_shoot = Vector(x_tile_stopped_at, y_tile_stopped_at);
            --ammo;
        }
    }

    if (move_tile) {
        // Check whats on the tile
        Tile& new_tile = map->GetTile(which_tile_go_to.XInt(), which_tile_go_to.YInt());
        TileObject* object_on_new_tile = new_tile.GetObject();
        ObjectType obj_type;
        if (object_on_new_tile == NULL)
            obj_type = nothing;
        else
            obj_type = object_on_new_tile->WhatAmI();
        switch (obj_type) {
            default:
            case nothing:
                owner->SwapObjects(map->GetTile(which_tile_go_to.XInt(), which_tile_go_to.YInt()));
                time_until_next_step.SetTicks(10);
                break;
            case civilian1:
                last_reward_recieved = ((Civilian*)(object_on_new_tile))->GetReward();
                reward += last_reward_recieved;
                new_tile.RemoveObject();
                owner->SwapObjects(new_tile);
                time_until_next_step.SetTicks(10);
                map->DoCivilianSplash();
                break;
            case enemy:
                CollidedWithEnemy(map);
                break;
            case nextlevel:
                ++current_level;
                GenerateTilemap();
                map->SetUpMap(generated_tilemap, generated_objectmap, this);
                map->SetAllVisibilities(1);
                break;
            case ammo_stash:
                ammo += 1+rand() % max_ammo_from_stash-1;
                delete (AmmoStash*)(new_tile.RemoveObject());;
                owner->SwapObjects(new_tile);
                time_until_next_step.SetTicks(10);
                break;
        }
    }
}

void Player::Draw(BITMAP* buffer, Vector pos)
{
    /*float rotation;
    switch (dir) {
        default:
        case up: rotation = 0; break;
        case down: rotation = M_PI; break;
        case left: rotation = -M_PI/2; break;
        case right: rotation = M_PI/2; break;
    }*/
    anim.Draw(buffer, pos + offset_from_tile);//, 1.0, false, rotation);

    if (!time_until_stop_shot.Finished()) {
        erlog("Drawing shot");

        extern bool play_as_good;
        int colour = play_as_good ? Colour::Turquoise() : Colour::Purple();

        switch (dir) {
            case up:
                CrayonLine(buffer, pos + Vector(50, 4), Vector((0.5+where_to_shoot.X())*tile_dim.X(), (0.5+where_to_shoot.Y())*tile_dim.Y()), colour, 0.01, 1, 2, 3);
                break;
            case down:
                CrayonLine(buffer, pos + Vector(4, 44), Vector((0.5+where_to_shoot.X())*tile_dim.X(), (0.5+where_to_shoot.Y())*tile_dim.Y()), colour, 0.01, 1, 2, 3);
                break;
            case left:
                CrayonLine(buffer, pos + Vector(4, 10), Vector((0.5+where_to_shoot.X())*tile_dim.X(), (0.5+where_to_shoot.Y())*tile_dim.Y()), colour, 0.01, 1, 2, 3);
                break;
            case right:
                CrayonLine(buffer, pos + Vector(60, 38), Vector((0.5+where_to_shoot.X())*tile_dim.X(), (0.5+where_to_shoot.Y())*tile_dim.Y()), colour, 0.01, 1, 2, 3);
                break;
            default:
                break;
        }
    }
}

void Player::CollidedWithEnemy(Tilemap* map)
{
    map->BlockMovement(60);
    --lives;
    reward = reward_entered_level_with;
    ammo = ammo_entered_level_with;
    map->SetUpMap(generated_tilemap, generated_objectmap, this);
    FaceDirection(up);
    offset_from_tile = Vector(0, 32);
    died = true;
}


Civilian::Civilian()
{
    how_much_reward = 10;
}

void Civilian::Update(int x_tile, int y_tile, Tilemap* map)
{
    anim.Tick();
}

void Civilian::Draw(BITMAP* buffer, Vector pos)
{
    float rotation;
    switch (dir) {
        default:
        case up: rotation = 0; break;
        case down: rotation = M_PI; break;
        case left: rotation = -M_PI/2; break;
        case right: rotation = M_PI/2; break;
    }
    anim.Draw(buffer, pos, 1.0, false, rotation);
}



Direction BasicStrategy(Vector pos, Vector player_pos, Tilemap* map)
{
    Vector dist = player_pos - pos;
    if (fabs(dist.X()) >= fabs(dist.Y())) {
        if (dist.XInt() == 0) return nowhere;
        if (dist.X() < 0) {
            if (map->GetTile(pos.XInt()-1, pos.YInt()).IsPassable() && (map->GetTile(pos.XInt()-1, pos.YInt()).IsEmpty() || pos+v_left == player_pos))
                return left;
        } else if (map->GetTile(pos.XInt()+1, pos.YInt()).IsPassable() && (map->GetTile(pos.XInt()+1, pos.YInt()).IsEmpty() || pos+v_right == player_pos))
                return right;
    }
    if (dist.YInt() == 0) return nowhere;
    if (dist.Y() < 0) {
        if (map->GetTile(pos.XInt(), pos.YInt()-1).IsPassable() && (map->GetTile(pos.XInt(), pos.YInt()-1).IsEmpty() || pos+v_up == player_pos))
            return up;
    } else if (map->GetTile(pos.XInt(), pos.YInt()+1).IsPassable() && (map->GetTile(pos.XInt(), pos.YInt()+1).IsEmpty() || pos+v_down == player_pos))
            return down;
    return nowhere;
}

Direction AStarStrategy(Vector pos, Vector player_pos, Tilemap* map)
{
    bool can_get_through[10][9];
    for (int i=0; i<10; ++i) {
        for (int j=0; j<9; ++j) {
            if (map->GetTile(i, j).IsPassable() && (map->GetTile(i, j).IsEmpty() || (i == player_pos.XInt() && j == player_pos.YInt())))
                can_get_through[i][j] = true;
            else
                can_get_through[i][j] = false;
        }
    }
    ////// I'll finish this later
    return left;

}


Enemy::Enemy()
{
    step_time = 30;
    SetChaseStrategy(BasicStrategy);
}




void Enemy::Update(int x_tile, int y_tile, Tilemap* map)
{
    anim.Tick();
    time_until_next_step.Update();
    offset_from_tile *= 0.9;
    int player_x, player_y;

    map->PlayerObject(player_x, player_y);

    // Check if this enemy actually cares about the player at the moment

    Vector dist((float)player_x - (float)x_tile, (float)player_y - (float)y_tile);

    if (dist.X()*dist.X() + dist.Y()*dist.Y() <= radius_of_caring*radius_of_caring) { // I'm pretty sure its quicker to multiply that run a sqare root. Doubt it makes any real difference though
        FaceDirection(Strategy(Vector(x_tile, y_tile), Vector(player_x, player_y), map));
        if (!map->IsMovementBlocked()) {
            int new_x = x_tile, new_y = y_tile;
            Vector tile_offset;
            switch (dir) {
                case up:
                    new_y = y_tile > 0 ? (int)y_tile - 1 : y_tile;
                    tile_offset = Vector(0.0, tile_dim.Y());
                    break;
                case down:
                    new_y = y_tile < 10 ? y_tile + 1 : y_tile;
                    tile_offset = Vector(0.0, -tile_dim.Y());
                    break;
                case left:
                    new_x = x_tile > 0 ? (int)x_tile - 1 : x_tile;
                    tile_offset = Vector(tile_dim.X(), 0.0);
                    break;
                case right:
                    new_x = x_tile < 10 ? x_tile + 1 : x_tile;
                    tile_offset = Vector(-tile_dim.X(), 0.0);
                    break;
                case nowhere:
                default:
                    // Stay still
                    break;
            }

            if (time_until_next_step) {
                offset_from_tile = tile_offset;
                TileObject* obj_swapping_with = map->ObjectAt(new_x, new_y);
                if (obj_swapping_with != NULL && obj_swapping_with->WhatAmI() == player1)
                    ((Player*)(map->ObjectAt(new_x, new_y)))->CollidedWithEnemy(map);
                else {
                    owner->SwapObjects(map->GetTile(new_x, new_y));
                    time_until_next_step.SetTicks(step_time);
                }
            }
        }
    }
}

void Enemy::Draw(BITMAP* buffer, Vector pos)
{

    /*float rotation;
    switch (dir) {
        default:
        case up: rotation = 0; break;
        case down: rotation = M_PI; break;
        case left: rotation = -M_PI/2; break;
        case right: rotation = M_PI/2; break;
    }*/
    anim.Draw(buffer, pos + offset_from_tile);//, 1.0, false, rotation);
}
