#!/usr/bin/env bash
##
## filename : prime.sh
## desc	    : mencari bilangan prima dari input yang diberikan.
## author   : noone <noone.nu1@gmail.com> <chars19.blogspot.com>
## licensed : (proudly) GPL
## info     : adoption from battlife from bt3
## 

usage="Usage: `basename $0` [NUMBER]\n\tNumber above 2"
prime=1
argc=$#

#hanya jika jumlah argumen = 1
if test $argc -eq 1; then     
    #filter hanya untuk bilangan doank, retval=integer or ""
    argv1=$(echo "$1" | awk '/^[0-9]*$/')
    if test ! -z $argv1; then           #jika input tidak "", berarti integer
	if test $argv1 -ge 2; then      #jika input besar dari 
            #jika input > 9, limit=9 atau limit=input-1
	    test $argv1 -gt 9  && lim=9 || lim=$[$argv1-1]
            #-->
            #input dibagi dengan 2 hingga input-1
            #jika hasil bagi input dgn x sama dgn 0
            #berarti bukan bilangan prima dan langsung keluar
	    for ((x=2;x<$lim;x++));do
		if test $[$argv1%$x] -eq 0 ;then prime=0 ; break;fi
	    done
            #-->
	    if  test $prime -ne 0; then
            echo "$argv1 == prime" ; exit 0;
        else
	        #echo "$argv1 != prime"
            exit 1
	    fi
	fi
    fi
fi
echo -e "$usage" ;exit 1
