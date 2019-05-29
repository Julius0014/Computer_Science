#!/usr/bin/python
import sys
import re
sumall = 0
out = 0
rP = 0 #right parenthesis Count
lP = 0 #Left  parenthesis Count
file = open(sys.argv[1], "r")
def nextp(rt,place):
    if rt[place] == "(":
        return place
    else:
        return nextp(rt,place+1)
    
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
        print(n1,n2)
        return n1*n2
    elif op == "/":
        print(n1,n2)
        return(n1/n2)
    elif op == "<":
        return(n1<n2)
    elif op == ">":
        return(n1>n2)

def prefixEval(rt,gp):
    global p,rP,lP,sumall,out
    five = " 5"
    if rt[gp] == "(":
        rP = rP + 1
        prefixEval(rt,gp+1)
    elif rt[gp] == ")":
        lP = lP + 1
        return sumall 
    else:
        rt[gp+1]= rt[gp+1].rstrip(" ");
        if rt[gp + 1] == "(":
            saveop = rt[gp]
            nextpl = nextp(rt,gp+3)
            prefixEval(rt,gp+1)
            sum1 = sumall
            prefixEval(rt,nextpl);
            sum2 = sumall
            out = pr(saveop,sum1,sum2 )
        elif rt[gp+1].isdecimal():
            num1= int(rt[gp+1])
            if rt[gp+2] == "(":
                prefixEval(rt,gp+2) 
                num2 = sumall
                sumall =  pr(rt[gp],num1,num2) 
                out = prefixEval(rt,gp+3)
            else:
                num2= int(rt[gp+2])
                sumall =  pr(rt[gp],num1,num2)
                out = prefixEval(rt,gp+3)
        else:
            num1= int(rt[gp+1])
            num2= int(rt[gp+2])
            sumall =  pr(rt[gp],num1,num2)
            out = prefixEval(rt,gp+3)
def prefixReader(Line):
    print(">"+ Line)
    #preFix = re.compile(r'(\(|[*+/-]|or|and|[0-9]\s\)|[0-9]\)|[0-9][0-9]|\))')  
    preFix = re.compile(r'\(|[*+/-]|or|and|[0-9]+\s*|\)')  
    rT = preFix.findall(Line)
    print(rT)
    global p,rP,lP,sumall
    rP= 0
    lP = 0
    num =0
    prefixEval(rT,num)

while True:
    fileLine = file.readline()
    if fileLine == "":
        break
    fileLine = fileLine.rstrip('\n')   
    prefixReader(fileLine)
    print(out)
