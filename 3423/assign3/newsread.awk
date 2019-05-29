BEGIN{
    FS =" ";
    lonestar["Articles"] =0
        lonestar["Groups"]=0
        lonestar["utsa.cs.2413"]=0
        lonestar["utsa.cs.2413.d"]=0
        lonestar["UT"]=0;
    runner["Articles"] =0
        runner["Groups"]=0
        runner["utsa.cs.2413"]=0
        runner["utsa.cs.2413.d"]=0
        runner["UT"]=0;
    ringer["Articles"] =0
        ringer["Groups"]=0
        ringer["utsa.cs.2413"]=0
        ringer["utsa.cs.2413.d"]=0
        ringer["UT"]=0
        rings["Articles"] =0
        rings["Groups"]=0
        rings["utsa.cs.2413"]=0
        rings["utsa.cs.2413.d"]=0
        rings["UT"]=0
        start ="" 
}
{
    if(NR == 1) start = $1" "$2" "$3
        if($6 == "lonestar.jpl.utsa.edu"){
            if ($7=="exit"){
                lonestar["Articles"]+= $9;
                lonestar["Groups"]+=$11;
            }
            if($7 == "times" ){
                lonestar["UT"]+=$13;
            }
            if ($7=="group"){
                if($8 == "utsa.cs.2413"){
                    lonestar["utsa.cs.2413"]+=$9;
                }
                if($8 == "utsa.cs.2413.d"){
                    lonestar["utsa.cs.2413.d"]+=$9;
                }
            }
        }
    if($6 == "runner.jpl.utsa.edu"){
        if ($7=="exit"){
            runner["Articles"]+= $9;
            runner["Groups"]+=$11;
        }
        if($7 == "times" ){
            runner["UT"]+=$13;
        }
        if ($7=="group"){
            if($8 == "utsa.cs.2413"){
                runner["utsa.cs.2413"]+=$9;
            }
            if($8 == "utsa.cs.2413.d"){
                runner["utsa.cs.2413.d"]+=$9;
            }
        }
    }
    if($6 == "ringer.cs.utsa.edu"){
        if ($7=="exit"){
            ringer["Articles"]+= $9;
            ringer["Groups"]+=$11;
        }
        if($7 == "times" ){
            ringer["UT"]+=$13;
        }
        if ($7=="group"){
            if($8 == "utsa.cs.2413"){
                ringer["utsa.cs.2413"]+=$9;
            }
            if($8 == "utsa.cs.2413.d"){
                ringer["utsa.cs.2413.d"]+=$9;
            }
        }
    }
    where =  match( $6,/ring[0-9]+/)
        if (where == 1) {
            if ($7=="exit"){
                rings["Articles"]+= $9;
                rings["Groups"]+=$11;
            }
            if($7 == "times" ){
                rings["UT"]+=$13;
            }
            if ($7=="group"){
                if($8 == "utsa.cs.2413"){
                    rings["utsa.cs.2413"]+=$9;
                }
                if($8 == "utsa.cs.2413.d"){
                    rings["utsa.cs.2413.d"]+=$9;
                }
            }
        }
}
END{
    print "\t\tlonestar\trunner\t\tringer\t\trings"
        print "Articles:\t" lonestar["Articles"] "\t\t" runner["Articles"] "\t\t" ringer["Articles"] "\t\t" rings["Articles"] 
        print "Groups:\t\t"   lonestar["Groups"]  "\t\t"runner["Groups"]  "\t\t"ringer["Groups"]  "\t\t"rings["Groups"] 
        print "Cs2413:\t\t"     lonestar["utsa.cs.2413"]  "\t\t"runner["utsa.cs.2413"]  "\t\t"ringer["utsa.cs.2413"]  "\t\t"rings["utsa.cs.2413"] 
        print "Cs2413.d:\t"   lonestar["utsa.cs.2413.d"]  "\t\t"runner["utsa.cs.2413.d"]  "\t\t"ringer["utsa.cs.2413.d"]  "\t\t"rings["utsa.cs.2413.d"] 
        print "User Time:\t"  lonestar["UT"]  "\t\t"runner["UT"]  "\t\t"ringer["UT"]  "\t\t"rings["UT"]
        print "Start Time = "start"\t" "End Time = "$1,$2,$3
}
