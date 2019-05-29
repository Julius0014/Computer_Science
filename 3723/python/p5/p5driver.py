#P5Driver
#   - read from Standard in and process the inputs
import sys
import p5team
import p5week

file = open(sys.argv[1], "r")
teams = {}
inputs = file.readline()
if inputs == "TEAMS\n":  
   inputs = file.readline()
   while inputs != "ENDTEAMS\n":
    p5team.add(teams,inputs)
    inputs = file.readline();
print("INITIAL")
p5team.printteams(teams)
inputs = file.readline();
week = 1
while inputs != "END":
    if "WEEK" in inputs:
        print(">>>"+inputs.rstrip('\n'))
        inputs = file.readline()
    while inputs != "ENDWEEK\n":
        p5week.processInput(inputs,teams)
        inputs = file.readline()
    print(">>>"+inputs.rstrip('\n'))
    print("WEEK "+str(week))
    p5team.printteams(teams)
    week = week + 1
    inputs = file.readline()
