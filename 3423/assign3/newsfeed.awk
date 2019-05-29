BEGIN {
    FS = " ";
    Saccepted=0;
    Naccepted=0;
    Qaccepted=0;
    Sreject=0;
    Nreject=0;
    Qreject=0;
    Scan=0;
    Ncan=0;
    Qcan=0;
    Stime= "";
}
{ 
    if(NR == 1) Stime = $1" "$2" "$3
        if ($5 == "swrinde"){
            if( $4 == "+" || $4 == "c" || $4 == "j" ) Saccepted++;
            else{
                Sreject++;
            }
            if($4 == "c")Scan++
        }
    if ($5 == "news.cais.net"){
        if( $4 == "+" || $4 == "c" || $4 == "j" ) Naccepted++;
        else{
            Nreject++;
        }
        if($4 == "c")Ncan++
    }
    if ($5 == "?"){
        if( $4 == "+" || $4 == "c" || $4 == "j" ) Qaccepted++;
        else{
            Qreject++;
        }
        if($4 == "c")Qcan++
    }
}
END{
        print "\t\t","accepted\t","rejected\t","canceled"
        print "swrinde:\t",Saccepted"\t\t",Sreject"\t\t",Scan
        print "news.cais.net:\t",Naccepted"\t\t",Nreject"\t\t",Ncan
        print "?:\t\t",Qaccepted"\t\t",Qreject"\t\t",Qcan
        print "Start Time = "Stime "\tEnd Time =",$1,$2,$3
}
