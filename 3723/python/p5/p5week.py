import re
import p5team
# all of the phrases that result in a win 
winWords = {"BEAT", "BEATS", 
    "DEFEAT", "DEFEATS", "DEFEATED",
    "SLAUGHTER", "SLAUGHTERS","SLAUGHTERED",
    "WHIP", "WHIPS", "WHIPPED",
    "TOP", "TOPS", "TOPPED",
    "UPSET", "UPSETS",
    "SHUT", "SHUTS", "SHUTS OUT"
    "EAT", "EATS",
    "DESTROY", "DESTROYS", "DESTROYED", 
    "SOAR", "SOARS", "SOAR OVER", "SOARS OVER"
}
#
# all of the phrases that result in a loss 
loseWords = { "LOSES TO", "LOST TO",
    "BEAT BY", "BEATEN BY", 
    "DEFEATED BY",
    "SLAUGHTERED BY",
    "WHIPPED BY",
    "TOPPED BY",
    "UPSET BY", 
    "EATEN BY", 
    "DESTROYED BY"
}
#lose(Word)
#Checks if the words in a inputs contain any phases of the words that result in a loss
# returns true or false
def lose(Word):
    for phrase in loseWords:
        if phrase in Word:
            return True;
#win(Word)
#Checks if the words in a inputs contain any phases of the words that result in a win
# returns true or false
def win(word):
    for phrase in winWords:
        if phrase in word:
            return True;
#echoInput(fname,sname,firstScore,secondScore, table)
#Prints the score of the games
def echoInput(fname,sname,firstScore,secondScore, table):
    if int(firstScore) < int(secondScore):
        print('\t'+sname + " "+ fname +" "+ secondScore +" "+ firstScore)
    else:
        print('\t'+fname + " "+ sname + " "+ firstScore + " "+ secondScore)

#processInput(inputs,table):
#takes the inputs then spilt the inputs to find the teams names and score  
def processInput(inputs,table):
    team1 = re.match(r'^(\w+)',inputs)
    score1 = re.search(r'(\d+)\D+(\d+)$',inputs)
    team = re.compile(r'(\w+)(,|\s?\d+)')
    team2 = team.search(inputs) 
    for name in sorted(table):
        name.rstrip("\n")
        if team1.group() in name:
            fname = re.match(r'\w{,4}' ,name)
            fname = fname.group()
        if team2.group(1) in name:
            sname = re.match(r'\w{,4}' ,name)
            sname = sname.group()
    print(">>>"+inputs.rstrip('\n'))
    if lose(inputs):
        p5team.addwin(sname)
        p5team.addlose(fname)
    elif win(inputs):
        p5team.addlose(sname)
        p5team.addwin(fname)
    elif int(score1.group(1)) < int(score1.group(2)):
        p5team.addwin(sname)
        p5team.addlose(fname)
        print("Unknown verb, by score, assume team 2 won")
    else:
        p5team.addlose(sname)
        p5team.addwin(fname)
        print("Unknown verb, by score, assume team 1 won")
    echoInput(fname,sname, score1.group(1),score1.group(2),table)

