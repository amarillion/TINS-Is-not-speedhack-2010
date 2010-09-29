#!/usr/bin/env python
from __future__ import division
import sys, os, glob
from math import *
from random import *
from ctypes import *

from allegro import *

from base import Base
from game import Game
import mysha, render, glyphs

def update():
    game = Base.game
    
    game.ticks += 1
    
    if game.state != "game": return

    if game.coins and game.coins_collected == game.coins:
        game.done = True
    if game.done:
        return
   
    game.tick()
   
    lost = False
    for i, mysha in enumerate(game.myshas):
        
        if mysha.dead:
            mysha.y += 8
            if mysha.kind == "S": lost = True
            continue

    if lost: return

    for mysha in game.myshas:
        if mysha.dead: continue
        if not game.coding:
            class Keys: pass
            k = Keys()
            k.left = False
            k.right = False
            k.up = False
            mysha.follow_commands(k)
            mysha.input(k)

        mysha.tick()

def main():
    w, h = 640, 480
    done = False
    need_redraw = True

    al_install_system(ALLEGRO_VERSION_INT, None)
    al_init_image_addon()
    al_init_font_addon()
    al_install_audio()
    al_init_acodec_addon()
    
    al_reserve_samples(8)
    
    game = Base.game = Game()
    game.ticks = 0
    game.levelnum = 0
    Base.level_select = 1
    game.state = "menu"
    game.menu = 0

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST);
    game.display = al_create_display(w, h)
    al_set_window_title(game.display, "Command Code")
    
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR)

    game.black = al_map_rgb_f(0, 0, 0)
    
    game.transform = byref(ALLEGRO_TRANSFORM())

    al_install_keyboard()
    al_install_mouse()
    
    welcome = al_load_sample("data/tins2010.ogg")
    al_play_sample(welcome, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0)

    game.art = al_load_bitmap("data/art.png", 0, 0)
    game.shapes = al_load_bitmap("data/shapes.png", 0, 0)
    
    game.sounds = {}
    for com in ["hop", "jump", "minus", "plus", "turn",
        "move", "set"]:
        game.sounds[com] = al_load_sample("data/" + com + ".ogg")
    
    game.ohnosound = al_load_sample("data/ohno.ogg")
    game.yaysound = al_load_sample("data/yay.ogg")
    game.welldone = al_load_sample("data/welldone.ogg")
    
    game.menusounds = []
    game.menusounds.append(al_load_sample("data/new.ogg"))
    game.menusounds.append(al_load_sample("data/continue.ogg"))
    game.menusounds.append(al_load_sample("data/reference.ogg"))
    game.menusounds.append(al_load_sample("data/select.ogg"))

    game.music = al_load_sample("data/piano.ogg")
    al_play_sample(game.music, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0)

    tw = 512
    th = 8
    game.texture = al_create_bitmap(tw, th)
    al_set_target_bitmap(game.texture)
    for y in range(th):
        for x in range(tw):
            dx = x - 3.5 if x < 5 else x - tw + 1 + 3.5 if x > tw - 5 else 0
            dy = y - 3.5 + 0.5 * sin(x * 2 * pi / 50)
            c = max(0, 1.0 - sqrt(dx * dx + dy * dy) / (3.5 + x / tw / 5))
            c = 1 - sin(pi / 2 * (1 - c))
            
            c -= random() * 0.4
            if c < 0: c = 0
            al_put_pixel(x, y, al_map_rgba_f(1, 1, 1, c))
    al_set_target_backbuffer(game.display)
    
    game.command_cache = None

    timer = al_create_timer(1 / game.FPS)

    queue = al_create_event_queue()
    al_register_event_source(queue, al_get_keyboard_event_source())
    al_register_event_source(queue, al_get_mouse_event_source())
    al_register_event_source(queue, al_get_timer_event_source(timer))
    al_register_event_source(queue, al_get_display_event_source(game.display))

    class Keys: pass
    k = Keys()
    k.left = False
    k.right = False
    k.up = False

    al_start_timer(timer)

    while not done:
        event = ALLEGRO_EVENT()

        if need_redraw and al_is_event_queue_empty(queue):
            al_clear_to_color(al_map_rgb_f(1, 1, 1))
            render.redraw()
            al_flip_display()
            need_redraw = False

        al_wait_for_event(queue, byref(event))

        prevmenu = game.menu

        if event.type in [ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_EVENT_KEY_REPEAT]:
            if game.state == "game":
                x, y = game.cursor

            if event.keyboard.keycode == ALLEGRO_KEY_RIGHT:
                k.right = True
                x += 1
            elif event.keyboard.keycode in [ALLEGRO_KEY_LEFT, ALLEGRO_KEY_BACKSPACE]:
                k.left = True
                x -= 1
            elif event.keyboard.keycode == ALLEGRO_KEY_UP:
                if game.state == "menu":
                    game.menu -= 1
                    if game.menu < 0: game.menu = 0
                k.up = True
                y -= 1
            elif event.keyboard.keycode == ALLEGRO_KEY_DOWN:
                if game.state == "menu":
                    game.menu += 1
                    if game.menu > 3: game.menu = 3
                y += 1
            
            if game.state == "game" and game.coding:
                c = event.keyboard.unichar
                if c > 0 and c < 127:

                    c = chr(c).lower()
                    if c in glyphs.letters:
                        game.code.code[x + y * 25] = c
                        x += 1

                if x == -1:
                    y -= 1
                    x = 24
                if x == 25:
                    y += 1
                    x = 0
                if y == -1: y = 3
                if y == 4: y = 0
                game.cursor = x, y

        elif event.type == ALLEGRO_EVENT_KEY_UP:
            if event.keyboard.keycode == ALLEGRO_KEY_ESCAPE:
                if game.state == "menu":
                    done = True
                else:
                    game.state = "menu"

            if event.keyboard.keycode == ALLEGRO_KEY_RIGHT:
                k.right = False
            elif event.keyboard.keycode == ALLEGRO_KEY_LEFT:
                k.left = False
            elif event.keyboard.keycode == ALLEGRO_KEY_UP:
                k.up = False
            
            elif event.keyboard.keycode == ALLEGRO_KEY_ENTER:
                if game.state == "menu":
                    if game.menu == 0:
                        game.start(Base.level_select)
                        game.state = "game"
                    elif game.menu == 1:
                        if game.levelnum != 0:
                            game.state = "game"
                    elif game.menu == 2:
                        game.state = "manual"
                        game.manual_page = 0
                    elif game.menu == 3:
                        Base.level_select += 1
                        if Base.level_select == 15:
                            Base.level_select = 1
                elif game.state == "manual":
                    game.manual_page += 1
                    if game.manual_page == 2:
                        game.state = "menu"
                elif game.state == "game":
                    if game.coding:
                        game.coding = False
                    else:
                        if game.done:
                            game.advance()
                            game.state = "game"
                        else:
                            backup = game.code.code
                            game.start(game.levelnum)
                            game.state = "game"
                            game.code.code = backup
                            continue

        elif event.type == ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = True

        elif event.type == ALLEGRO_EVENT_TIMER:
            update()
            need_redraw = True

        elif event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            game.mouse_down = True
            game.last_x = event.mouse.x
            game.last_y = event.mouse.y

        elif event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            game.mouse_down = False
            
            if game.state == "menu": game.state = "game"

        elif event.type == ALLEGRO_EVENT_MOUSE_AXES:
            pass
        
        if game.menu != prevmenu:
            al_play_sample(game.menusounds[game.menu], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0)

    al_uninstall_system()

al_main(main)
