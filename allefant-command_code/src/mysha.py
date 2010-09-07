from __future__ import division
from base import Base
from allegro import *
from code import Code

wall = "#-_|"

class Mysha:
    def __init__(self, x, y, left, kind):
        global game
        game = Base.game

        self.x = x
        self.y = y
        self.jx = 0
        self.jy = 0
        self.frame = 0
        self.left = left
        self.jump = False
        self.ground = True
        self.dead = False
        self.rest = 0
        self.fy = 0
        self.coins = 0
        self.code = None
        self.kind = kind
        
        self.executing = 0
        self.command = None
        self.command_x = 0
    
    def move(self):
        self.frame += 1
        if self.frame & 3 == 0:
            dx = -4 if self.left else +4
            if self.check(-20 if self.left else 20, 0) not in wall:
                self.x += dx
    
    def turn(self):
        self.left = not self.left
        tx = self.x // 32
        dx = self.x - tx * 32
        if self.check(-dx - 1, 0) in wall:
            self.x = tx * 32 + 20
        elif self.check(32 - dx, 0) == wall:
            self.x = tx * 32 + 12
    
    def input(self, k):
        if self.jy == 0: self.jx = 0
        if k.right:
            if not self.rest:
                if self.left: self.turn()
                else: self.move()
            if self.jy == 0: self.jx = 3
        if k.left:
            if not self.rest:
                if self.left: self.move()
                else: self.turn()
            if self.jy == 0: self.jx = -3
        if k.up and not self.jump:
            if not self.rest and self.check(0, 1) in wall:
                if self.jx:
                    self.jy = 8
                else:
                    self.jy = 12
    
    def tick(self):
        if self.rest:
            self.rest -= 1

        if self.jy > 0:
            if self.check(-32 if self.left else 32, 0) not in wall:
                self.x += self.jx
            self.y -= self.jy
            self.jy -= 1
            self.jump = True
            self.ground = False

        if self.jy == 0:
            gravity = 1 + self.fy
            for i in range(gravity):
                ty = self.y // 32
                if self.y - ty * 32 != 31:
                    self.y += 1
                    if self.fy < 8:
                        self.fy += 1
                else:
                    if self.check(0, 1) not in wall:
                        if self.ground:
                            tx = self.x // 32
                            if self.left:
                                self.x -= 8
                            else:
                                self.x += 8
                            self.ground = False
                            self.y += 1
                        else:

                            grab = False
                            if self.left:
                                if self.check(-32, 8) in wall and\
                                    self.check(-32, 0) not in wall:
                                    grab = True
                            else:
                                if self.check(32, 8) in wall and\
                                    self.check(32, 0) not in wall:
                                    grab = True
                            if grab:
                                self.y = ty * 32 + 31
                                tx = self.x // 32
                                if self.left:
                                    self.x = tx * 32 - 1
                                else:
                                    self.x = tx * 32 + 32
                            else:
                                self.y += 1
                            
                    else:
                        if self.fy > 0:
                            ty = self.y // 32
                            if self.y - ty * 32 < 16:
                                self.y = ty * 32 - 1
                            self.fy = 0
                        self.ground = True
                        if self.jump:
                            self.jump = False
                            self.rest = 15
           
        c = self.check(0, 0)
        if c == "X":
            self.dead = True
            al_play_sample(game.ohnosound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0)
        
        elif c == "*":
            self.coins += 1
            self.put(0, 0, " ")
            al_play_sample(game.yaysound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0)
        
        elif c == "W":
            game.coins_collected += self.coins
            if self.coins > 0:
                al_play_sample(game.welldone, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0)
            self.coins = 0

    def check(self, x, y):
        tx = (self.x + x) // 32
        ty = (self.y + y) // 32
        return game.level[1 + ty][1 + tx]
    
    def put(self, x, y, c):
        tx = (self.x + x) // 32
        ty = (self.y + y) // 32
        row = game.level[1 + ty]
        game.level[1 + ty] = row[:1 + tx] + c + row[1 + tx + 1:]
    
    def follow_commands(self, keys):
        
        if not self.code:
            self.code = Code()
            self.code.code = game.code.code[:]
        
        if (game.ticks - self.executing > 2 and not self.command) or\
            game.ticks - self.executing > 120:
            self.command = self.code.execute(self)

            if self.command in game.sounds:
                al_play_sample(game.sounds[self.command], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0)
            
            self.executing = game.ticks
            if self.command == "move":
                self.command_x = self.x
            elif self.command == "hop":
                self.command_x = self.x
            elif self.command == "jump":
                keys.up = 1
            elif self.command == "turn":
                self.left = not self.left

        if self.command == "move":
            if self.command_x // 32 != self.x // 32:
                self.command = ""
            if self.left:
                keys.left = 1
            else:
                keys.right = 1
        elif self.command == "hop":
            tx = self.x // 32
            if self.left: keys.left = 1
            else: keys.right = 1
            if not self.left and self.x - tx * 32 >= 28:
                keys.up = 1
                self.command = "wait"
            if self.left and self.x - tx * 32 <= 3:
                keys.up = 1
                self.command = "wait"
        elif self.command == "wait":
            tx = self.x // 32
            if self.ground and not self.rest:
                if not self.left and self.command_x // 32 + 2 == self.x // 32:
                    self.command = "center"
                if self.left and self.command_x // 32 - 2 == self.x // 32:
                    self.command = "center"
        elif self.command == "jump":
            if game.ticks > self.executing and self.ground and not self.rest:
                self.command = ""
        elif self.command == "turn":
            if game.ticks - self.executing > 30:
                self.command = ""
        elif self.command == "center":
            if self.left: keys.left = 1
            else: keys.right = 1
            tx = self.x // 32
            if self.x - tx * 32 >= 12 and self.x - tx * 32 <= 20:
                self.command = ""
        else:
            self.command = ""
