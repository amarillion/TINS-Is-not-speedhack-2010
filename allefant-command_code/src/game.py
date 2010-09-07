from __future__ import division
from random import *
from math import *
from mysha import Mysha

import code

game = None

class Game:
    
    FPS = 60
    
    level0 = """
0 test
....................
....................
....................
....................
....................
....................
....................
....................
....................
.##################.
"""

    level1 = """
1 hint= write mtmtmme then hit return
....................
....................
....................
....................
....................
....................
....................
....................
..S.....*..W........
.############.......
"""

    level2 = """
2 fetch and return
....................
..##########.######.
..#.#####.##.#..###.
.....###........###.
.....###........###.
.....###........###.
.....###........###.
.....#.#........###.
.S.W......*.....###.
.##########.....###.
"""

    level3 = """
3 hop mysha hop
....................
....................
....................
....................
....................
....................
..#...#...#.........
....................
..S.*...*........W..
.##########.#######.
"""

    level4 = """
4 jump mysha jump
....................
....................
.............*......
....................
.................W..
.............######.
..*.......#########.
.......############.
..S.############....
.############.......
"""
    
    level5 = """
5 there and back again
....................
....................
....................
.........####.......
.........####.......
....................
.*...#..#..#...#....
..............###...
.S.W..........###.*.
.#########.########.
"""

    level6 = """
6 i am up there
....................
................*...
........*.*.*...#.#.
..............#.....
...W..............#.
.######.#.#.#.......
....*.*.*.........#.
.............. .#...
.S.......... .#.....
.##.#.#.#.###.#.....
"""

    level7 = """
7 forgot a coin up there
....................
.*..................
................#.#.
.##############.....
..................#.
....................
..................#.
.............. .#...
.S.W........ .#.....
.####.#.#.###.#.....
"""

    level8 = """
8 don't be afraid mysha
....................
....................
....................
....................
....................
....................
..*.................
....................
..S..............W..
.###...---......###.
"""

    level9 = """
9 uh oh
....................
....................
....................
.................*..
....................
.SW.................
.###...---......###.
.###...---......###.
.###...---......###.
.###...---......###.
"""

    level10 = """
10 mysha get up here at once
....................
....................
................*.*.
....................
..W.................
.###......___...###.
....................
.............|||....
..S.................
.###...---......###.
"""

    level11 = """
11 hello i'm nisha
....................
....................
....................
....................
.........W..........
....*...###...*.....
....###.....###.....
........###.........
..S.............T...
.########.########..
"""

    level12 = """
12 could've done it on my own
....................
................*...
......T.............
.....###.#####.###..
....................
....................
....................
................*...
..S.W...............
.#######.#.#.#.###..
"""

    level13 = """
13 get down when done
....................
.....*.*........*...
..T.................
.###.#.#.#####.###..
....................
....................
....................
................*...
..S.W...............
.#######.#.#.#.###..
"""

    level14 = """
14 Will the real Mysha please stand up
....................
.*...*...*...*...*..
.U...U...S...T...U..
.#...#...#...#...#..
...#...#...#...#....
.....#...#...#......
.......#...#........
....................
.........W..........
........###.........
"""

    level15 = """
Congratulations! You win!
....................
....................
....................
....................
....................
....................
....................
....................
.......S.W..........
......#######.......
"""

    def read_level(self, level):
        level2 = [" " * 22]
        l = level.strip().splitlines()
        title = l[0].lower()
        for row in l[1:]:
            row2 = "X" + row.strip() + "X"
            level2 += [row2]
        level2 += ["X" * 22]
        return level2, title

    def get_lines(self, level):
        class State: pass
        class Line: pass
        class Block: pass
        state = State()
        
        def get(x, y):
            return level[y][x]
        
        def check(a, b, t):
            a = a == t
            b = b == t
            c = a != b
            if c and not state.on:
                state.on = True
                state.start = x - 1, y - 1
            elif state.on and not c:
                state.on = False
                line = Line()
                line.kind = t
                line.start = state.start
                line.end = x - 1, y - 1
                self.lines.append(line)
        
        self.lines = []
        for t in "#-|_":
            # horizontal lines
            for y in range(1, 1 + 11):
                state.on = False
                for x in range(1, 1 + 21):
                    a = get(x, y)
                    b = get(x, y - 1)
                    check(a, b, t)
                   
            # vertical lines
            for x in range(1, 1 + 21):
                state.on = False
                for y in range(1, 1 + 11):
                    a = get(x, y)
                    b = get(x - 1, y)
                    check(a, b, t)
        
        self.blocks = {}
        for line in self.lines:
            if line.kind in "|-_":
                if line.kind not in self.blocks:
                    block = Block()
                    block.lines = []
                    block.kind = line.kind
                    block.x = 20
                    block.y = 10
                    block.w = 0
                    block.h = 0
                    block.left = randint(0, 1)
                    block.pos = 0
                    self.blocks[line.kind] = block
                block = self.blocks[line.kind]
                block.lines.append(line)
        
        for block in self.blocks.values():
            for ly in range(1, 1 + 10):
                for lx in range(1, 1 + 20):
                    c = get(lx, ly)
                    if c == block.kind:
                        y = ly - 1
                        x = lx - 1
                        if x < block.x: block.x = x
                        if y < block.y: block.y = y
                        if x > block.w: block.w = x
                        if y > block.h: block.h = y
            block.w -= block.x
            block.w += 1
            block.h -= block.y
            block.h += 1

        # sort lines and prepare them for rendering
        for line in self.lines:
            if random() < 0.5:
                temp = line.start
                line.start = line.end
                line.end = temp
            
            sx, sy = line.start
            ex, ey = line.end

            sx *= 32
            sy *= 32
            ex *= 32
            ey *= 32
            
            def r(): return 7 * (random() - 0.5)
            sx += r()
            sy += r()
            ex += r()
            ey += r()
            
            dx = ex - sx
            dy = ey - sy
            d = sqrt(dx * dx + dy * dy)
            line.length = d + 10

            line.angle = atan2(dy, dx)
           
            sx -= dx * 5 / d
            sy -= dy * 5 / d
            line.start = sx, sy
        
        self.lines.sort(cmp = lambda l1, l2: cmp(l2.length, l1.length))
        
        self.myshas = []
        self.coins = 0
        for y in range(1, 1 + 10):
            for x in range(1, 1 + 20):
                c = get(x, y)
                if c in "STU":
                    self.myshas.append(Mysha((x - 1) * 32 + 16,
                        (y - 1) * 32 + 31, c == "T", c))
                if c == "*":
                    self.coins += 1
    
    def start(self, levelnum):
        self.levelnum = levelnum
        self.level, self.title = self.read_level(
            getattr(self, "level" + str(levelnum)))
        self.get_lines(self.level)
        self.state = "menu"
        self.level_new = True
        self.code = code.Code()
        self.cursor = 0, 0
        self.coding = True
        self.menu = 1
        self.done = False
        self.coins_collected = 0
        
    def advance(self):
        level = self.levelnum + 1
        self.start(level)

    def setblock(self, x, y, w, h, c):
        for i in range(h):
            row = self.level[1 + y + i]
            self.level[1 + y + i] = row[:1 + x] + c * w + row[1 + x + w:]

    def tick(self):
        for block in self.blocks.values():
            dx = -0.5 if block.left else 0.5
            ptx = block.x + int((block.pos + 16) // 32)
            ty = block.y
            block.pos += dx
            tx = block.x + int((block.pos + 16) // 32)
            if tx != ptx:
                collide = False
                for j in range(block.h):
                    for i in range(block.w):
                        if self.level[1 + ty + j][1 + tx + i] in "#X":
                            collide = True
                if collide:
                    block.pos -= dx
                    block.left = not block.left
                    continue
                self.setblock(ptx, ty, block.w, block.h, " ")
                self.setblock(tx, ty, block.w, block.h, block.kind)
            for line in block.lines:
                x, y = line.start
                x += dx
                line.start = x, y
