class binstr:
    string = ''
    cursor = 0
    def __init__(self, string=''):
        self.string = string

    def getbyte(self):
        value = ord(self.string[self.cursor])
        self.cursor = self.cursor+1
        return value
    def getlong(self):
        s = self.string[self.cursor:]
        self.cursor = self.cursor+4
        return (ord(s[0])<<24) + (ord(s[1])<<16) + (ord(s[2])<<8) + (ord(s[3]))
    def getstr(self):
        length = ord(self.string[self.cursor])
        s = self.string[self.cursor+1 : self.cursor+1+length]
        self.cursor = self.cursor+1+length
        return s

    def putbyte(self, value):
        self.string = self.string + chr(value)
    def putlong(self, value):
        self.string = self.string + chr(value>>24) + chr((value<<8)>>24) + chr((value<<16)>>24) + chr((value<<24)>>24)
    def putstr(self, string):
        if len(string) > 256:
            raise Exception("string length more than 256")
        self.string = self.string + chr(len(string)) + string

    def len(self):
        return len(self.string)
    def rewind(self):
        self.cursor = 0
    def getCursor(self):
        return self.cursor
    def setCursor(self, cursor):
        self.cursor = cursor
    def toString(self):
        return self.string

