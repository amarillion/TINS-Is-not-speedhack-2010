from __future__ import division
from allegro import *
from math import *
from base import Base
import glyphs

manual1 = """
m   move one tile...
e   end program.....
t   turn around.....
h   hop over tile...
j   jump up.........
....................
....................
= x y   x = y.......
g x   goto x........
....................


     next page
"""

manual2 = """
i x y z   if x is y.
    then goto z.....
+ x y   x = x + y...
- x y   x = x - y...
l z x y   look at xy
    and store result
    in z   xy is
    relative
    empty 0
    floor 1
    coin  2

   back to menu
"""

def line(w, c):
    if w > 6:
        al_draw_tinted_bitmap_region(game.texture, c, 0, 0, 3, 8, 0, -4, 0)
        al_draw_tinted_bitmap_region(game.texture, c, 3, 0, w - 6, 8, 3, -4, 0)
        al_draw_tinted_bitmap_region(game.texture, c, 512 - 3, 0, 3, 8, 3 + w - 6, -4, 0)

def reset():
    al_identity_transform(game.transform)
    al_use_transform(game.transform)

def move(x, y):
    al_translate_transform(game.transform, x, y)
    al_use_transform(game.transform)

def rotate(a):
    al_rotate_transform(game.transform, a)
    al_use_transform(game.transform)

global_glyphs = {}
def text(s, x, y, c, size, timepos, speed):
    if size not in global_glyphs:
        global_glyphs[size] = glyphs.get_glyphs(size)
    g = global_glyphs[size]

    s = s.lower()
    pos = 0
    t = 0
    for a in s:
        if a not in g: continue
        npos = pos
        for l in g[a].lines:
            reset()
            rotate(l.angle)
            xs, ys = l.start
            xe, ye = l.end
            move(x + xs + pos, y + ys)
            
            if timepos > t + speed:
                p = 1
            else:
                p = (timepos - t) / speed
                if p < 0: return False, 0, pos
            line(l.length * p, c)
            npos = max(npos, pos + xs, pos + xe)
            t += speed
        pos = npos + 5
    return True, t, pos

def cache_text(c, size):
    class Cache: pass
    cache = Cache()
    cache.glyphs = {}
    cache.size = {}
    rows = 2
    gs = size * 6
    w = gs * len(glyphs.letters) // rows
    h = gs * rows
    cache.bitmap = al_create_bitmap(w, h)
    al_set_target_bitmap(cache.bitmap)
    x = 0
    y = 0
    for s in glyphs.letters:
        xoff = x + size
        yoff = y + size
        done, time, width = text(s, xoff, yoff, c, size, 1000, 1)
        cache.size[s] = width
        sub = al_create_sub_bitmap(cache.bitmap, x, y, gs, gs)
        cache.glyphs[s] = sub
        x += gs
        if x >= w:
            x = 0
            y += gs
    al_set_target_backbuffer(game.display)
    return cache

def redraw_game():
    
    if game.level_new:
        game.level_new = False
        game.level_start_time = game.ticks
        
    t = game.ticks - game.level_start_time
   
    color = al_map_rgba_f(0, 0, 0, 0.5)
    s = game.FPS // 10
    for i, l in enumerate(game.lines):
        if i > (t // s): break
        elif i == t // s:
            p = min(1, (t % s) / (s * 0.5))
        else:
            p = 1
        reset()
        xs, ys = l.start

        rotate(l.angle)
        move(xs, ys)
        line(p * l.length, color)

    b = game.art
    y = -32
    for row in game.level:
        x = -32
        for c in row:
            if c == "*":
                reset()
                move(0 - 32, 0 - 32)
                rotate(game.ticks * 2 * pi / 60)
                move(x + 16, y + 16 - 5)
                color = al_map_rgba_f(0.6, 0.5, 0, 0.75)
                al_draw_tinted_bitmap_region(b, color,
                    0, 64, 64, 64, 0, 0, 0)
            elif c == "W":
                reset()
                if game.done: color = al_map_rgba_f(0.5, 0.4, 0, 1)
                else: color = al_map_rgba_f(0.3, 0, 0.2, 0.5)
                al_draw_tinted_bitmap_region(b, color,
                    64, 64, 96, 160, x - 48 + 16, y - 156 + 32, 0)
            x += 32
        y += 32
    
    for m in game.myshas:
        for i in range(2):
            if i == 0:
                b = game.shapes
                color = al_map_rgba_f(1, 1, 1, 1)
            else:
                b = game.art
                color = al_map_rgba_f(0, 0, 0, 0.75)
            reset()
            move(-32, -60)
            f = 4 if m.jump else (m.frame // 4) % 4
            al_draw_tinted_bitmap_region(b, color,
                f * 64, 0, 64, 64,
                m.x, m.y,
                ALLEGRO_FLIP_HORIZONTAL if m.left else 0)
    
    c = al_map_rgba_f(0, 0.5, 0, 0.5)
    reset()
    rotate(pi / 2)
    move(8, 360 + 8)
    line(120 - 16, c)
    move(640 - 16, 0)
    line(120 - 16, c)
    
    reset()
    move(8, 360 + 8)
    line(512, c)
    move(512, 0)
    line(640 - 16 - 512, c)
    
    reset()
    rotate(pi)
    move(640 - 8, 480 - 8)
    line(512, c)
    move(-512, 0)
    line(640 - 16 - 512, c)
    
    reset()

    if not game.command_cache:
        game.command_cache = cache_text(al_map_rgba_f(1, 1, 1, 1), 4)
    
    if not game.coding and game.myshas[0].code:
        color = al_map_rgba_f(1, 0, 0, 0.75)
        v = game.myshas[0].code.variables
        info = "x=%3d y=%3d z=%3d" % (v["x"], v["y"], v["z"])
        x = 20
        y = 360 - 20
        for c in info:
            x += 24
            g = game.command_cache.glyphs[c]
            al_draw_tinted_bitmap(g, color, x, y, 0)
    
    c = al_map_rgba_f(0, 0, 0.5, 0.75)
    cx, cy = game.cursor
    for j in range(4):
        for i in range(25):
            x = 20 + i * 24
            y = 360 + 15 + j * 25
            
            g = game.command_cache.glyphs[game.code.code[i + 25 * j]]
            al_draw_tinted_bitmap(g, c, x, y, 0)
            
            if game.coding and i == cx and j == cy and game.ticks % 60 < 50:
                g = game.command_cache.glyphs["_"]
                al_draw_tinted_bitmap(g, al_map_rgba_f(0.5, 0.5, 0, 1), x, y, 0)
            if not game.coding:
                for mysha in game.myshas:
                    if mysha.code and i + j * 25 == mysha.code.ip:
                        g = game.command_cache.glyphs[">"]
                        al_draw_tinted_bitmap(g, al_map_rgba_f(1, 0, 0, 1), x, y, 0)
    
    c = al_map_rgba_f(0, 0.5, 0, 1)
    x = 0
    y = 0
    for letter in game.title:
        g = game.command_cache.glyphs[letter]
        al_draw_tinted_bitmap(g, c, x, y, 0)
        x += game.command_cache.size[letter]

def redraw_menu():
    t = game.ticks

    c1 = al_map_rgba_f(0, 0, 0, 0.5)
    c2 = al_map_rgba_f(0.5, 0, 0, 0.5)
    
    color = c2 if game.menu == 0 else c1
    if Base.level_select == 1:
        menutext = "New"
    else:
        menutext = "New (%d)" % Base.level_select

    done, l, s = text(menutext, 210, 250, color, 5, t, 4)

    color = c2 if game.menu == 1 else c1
    done, l, s = text("Continue", 210, 300, color, 5, t, 4)
    
    color = c2 if game.menu == 2 else c1
    done, l, s = text("Reference manual", 210, 350, color, 5, t, 4)

    color = c2 if game.menu == 3 else c1
    done, l, s = text("Select Level", 210, 400, color, 5, t, 4)

    color = al_map_rgba_f(1, 0.3, 0.1, 1)
    done, l, s = text("Command Code", 50, 10, color, 16, t, 4)
    if not done: return
    t -= l
    
    color = al_map_rgba_f(0.6, 0.5, 0, 1)
    done, l, s = text("TINS 2010 entry by Allefant", 30, 100, color, 7, t, 4)
    if not done: return
    t -= l

def redraw_manual():
    if game.manual_page == 0: manual = manual1
    if game.manual_page == 1: manual = manual2
    
    lines = manual.strip().splitlines()
    y = 5
    for l in lines:
        x = 5
        for i, c in enumerate(l):
            if i == 0: color = al_map_rgba_f(1, 0, 0, 1)
            else: color = al_map_rgba_f(0, 0, 0, 0.5)
            text(c, x, y, color, 5, 1000, 1)
            x += 30
        y += 30

def redraw():
    global game
    game = Base.game
    
    if game.state == "game": redraw_game()
    elif game.state == "menu": redraw_menu()
    elif game.state == "manual": redraw_manual()
