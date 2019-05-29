import re
win = {}
loses = {}
#add
#add all values to all lists
def add(table,lis):
    lis.rstrip("\n")
    table[lis]="0"
    sname = re.match(r'\w{,4}' ,lis)
    sname = sname.group()
    win[sname] = 0
    loses[sname] = 0
def printteams (table):
    print("\tTeam\tWins\tLoses")
    for name in sorted(table):
        name.rstrip("\n")
        sname = re.match(r'\w{,4}' ,name)
        if sname != None:
            sname = sname.group()
            print("\t" + sname + "\t"+ str(win[sname]) + "\t"+str(loses[sname]))
def addwin(namewin):
    win[namewin] = win[namewin] + 1
def addlose(namelose):
    loses[namelose] = loses[namelose] + 1
