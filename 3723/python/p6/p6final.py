#!/usr/bin/python
import sys
import re
openP = 0 #right parenthesis Count
closeP = 0 #Left  parenthesis Count
gp = 0
pos =0
sumall = 0
opr = {"*","+","/","and","or","<",">","-"}
file = open(sys.argv[1], "r")
def pr(op,n1,n2):
    if op == "+":
        return(n1+n2)
    elif op == "-":
        return(n1-n2)
    elif op == "and":
        return(n1 and n2)
    elif op == "or":
        return(n1 or n2)
    elif op == "*":
        return(n1*n2)
    elif op == "/":
        return(n1/n2)
    elif op == "<":
        return(n1<n2)
    elif op == ">":
        return(n1>n2)
def getValue(rt,place):
    global pos,sumall,openP,closeP
    rt[place] = rt[place].rstrip(" ");
    if rt[place] in opr:
        saveopr = rt[place]
        pos = pos +1
        num1 = prefixEval(rt,pos)
        num2 = prefixEval(rt,pos)
        sumall = pr(saveopr,num1,num2)
        prefixEval(rt,pos)
        if not openP == closeP:
            return sumall
        elif closeP == openP :
            return print( pr(saveopr,num1,num2))
def prefixEval(rt,gp):
    global p,openP,closeP,sumall,pos

    rt[gp] = rt[gp].rstrip(" ");
    if rt[gp] == "(":
        openP = openP + 1
        pos = pos + 1
        return getValue(rt,pos)
    if rt[gp] == ")":
        closeP = closeP + 1
        pos = pos +1
        return
    elif rt[gp].isdecimal():
        pos = pos +1
        return int(rt[gp])

def countP(str):
    count = 0
    for x in str:
        if x == ")":
            count = count + 1
        if x == "(":
            print("")



def prefixReader(Line):
    global pos
    print(">"+ Line)
    preFix = re.compile(r'\(|[<>*+/-]|or|and|[0-9]+\s*|\)')  
    rT = preFix.findall(Line)
    global p,rP,lP,sumall
    openP= 0
    closeP = 0
    #prefixEval(rT,pos)
    try:
        prefixEval(rT,pos)

        print(openP)
    except Exception as p:
        if openP == closeP:
            print(openP)
            print("Missing closing ')'")



    openP= 0
    closeP = 0
    pos =0

while True:
    fileLine = file.readline()
    if fileLine == "":
        break
    fileLine = fileLine.rstrip('\n')   
    prefixReader(fileLine)

