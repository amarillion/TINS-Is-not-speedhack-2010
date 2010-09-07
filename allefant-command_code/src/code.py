import base

class Code:
    def __init__(self):
        self.ip = 0
        self.code = [" "] * 100
        self.variables = {
            "a" : 0,
            "b" : 0,
            "c" : 0,
            "d" : 0,
            "e" : 0,
            "x" : 0,
            "y" : 0,
            "z" : 0
        }
    
    def next(self):
        com = self.code[self.ip]
        self.ip += 1
        if self.ip == 100: self.ip = 0
        return com
    
    def num(self):
        if self.code[self.ip] in " +": self.next()
        neg = False
        if self.code[self.ip] == "-":
            self.next()
            neg = True
        d = None
        if self.code[self.ip].isdigit():
            com = self.next()
            d = int(com)
            if self.code[self.ip].isdigit():
                com = self.next()
                d = d * 10 + int(com)
        if neg and d != None: d *= -1
        return d

    def op(self):
        v = self.num()
        if v == None:
            v = self.variables[self.var()]
        return v
    
    def com(self):
        if self.code[self.ip] == " ": self.next()
        word = ""
        if self.code[self.ip].isalpha():
            word = self.next()
        if not word:
            if self.code[self.ip] in "+-=_?":
                word = self.next()
        return word

    def var(self):
        v = self.com()
        if len(v) != 1 or v not in "xyzabcd":
            v = "e"
        return v
    
    def get(self, x, y):
        game = base.Base.game
        tx = self.mysha.x // 32
        ty = self.mysha.y // 32
        if self.mysha.left: tx -= x
        else: tx += x
        ty += y
        if tx < 0: return 0
        if tx > 19: return 0
        if ty < 0: return 0
        if ty > 9: return 0
        c = game.level[1 + ty][1 + tx]
        if c in "#-_|": return 1
        if c == "*": return 2
        return 0

    def execute(self, mysha):
        self.mysha = mysha
        word = self.com()
        if word in ["=", "set"]:
            v = self.var()
            self.variables[v] = self.op()
            return "set"
        elif word in ["a", "+", "add"]:
            v = self.var()
            self.variables[v] += self.op()
            return "plus"
        elif word in ["s", "-", "sub"]:
            v = self.var()
            self.variables[v] -= self.op()
            return "minus"
        elif word in ["m", "mov", "move"]: return "move"
        elif word in ["h", "hop"]: return "hop"
        elif word in ["j", "jmp", "jump"]: return "jump"
        elif word in ["t", "tur", "turn"]: return "turn"
        elif word in ["e", "end"]:
            self.ip = 0
            return "end"
        elif word in ["l", "look"]:
            z = self.var()
            x = self.op()
            y = self.op()
            self.variables[z] = self.get(x, y)
            return "look"
        elif word in ["?", "i", "if"]:
            x = self.op()
            y = self.op()
            ip = self.op() % 100
            if x == y: self.ip = ip
            return "if"
        elif word in ["_", "g", "go", "goto"]:
            self.ip = self.op() % 100
            return "goto"
        return ""
