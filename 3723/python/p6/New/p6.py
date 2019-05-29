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
# Pr - process and return the value base on the operations
#   INPUTS
#       op = the operator to use
#       n1 and n2 = numbers
#
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
#getValue - returns the values found by the number and operator
#  inputs
#    rt  - is the array of tokens 
#    place is the current place to look at 
#
#
def getValue(rt,place):
    global pos,sumall,openP,closeP
    rt[place] = rt[place].rstrip(" ");
    if rt[place] in opr:
        saveopr = rt[place]
        pos = pos +1
        num1 = prefixEval(rt,pos)
        num2 = prefixEval(rt,pos)
        if num1 is None:
            raise TypeError("Incorrect number of operands - must be 2 for " + saveopr)
        elif num2 is None:
            raise TypeError("Incorrect number of operands - must be 2 for " + saveopr)
        sumall = pr(saveopr,num1,num2)
        num = prefixEval(rt,pos)
        if not num == None and len(rt) == pos + 1 : 
            raise TypeError("Incorrect number of operands - must be 2 for " + saveopr)
        if len(rt) == pos:
            if openP > closeP:
                raise ValueError("Missing closing ')'")     
        if not openP == closeP:
            return sumall
        elif closeP == openP:
            return print( pr(saveopr,num1,num2))
    else:
            raise ValueError("Unknown function in prefix expr: " + rt[place])
#   
#
#
#
#
#

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
    elif isinstance(rt[gp],str):
        raise ValueError("Expected int found: " + rt[gp])
    else:
        raise ValueError("Missing opening '('")

def prefixReader(Line):
    global pos
    print(">"+ Line)
    preFix = re.compile(r'\(|[a-zA-Z]+|[<>*+/-]|or|and|[0-9]+\s*|\)')  
    #preFix = re.compile(r'\(|[<>*+/-]|or|and|[0-9]+\s*|\)')  
    rT = preFix.findall(Line)
    
    global p,rP,lP,sumall
    openP= 0
    closeP = 0
    pos=0
    try:
        prefixEval(rT,pos)
    except Exception as p:
        print(p)
    openP= 0
    closeP = 0
    pos =0

while True:
    fileLine = file.readline()
    if fileLine == "":
        break
    fileLine = fileLine.rstrip('\n')   
    prefixReader(fileLine)

