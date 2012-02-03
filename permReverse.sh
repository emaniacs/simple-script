#!/usr/bin/env bash
##
## filename : permReverse.sh
## desc	    : konversi permission karakter, dari string ke oktal dan sebaliknya.
## author   : noone <noone.nu1@gmail.com> <chars19.blogspot.com>
## licensed : (proudly) GPL
## info     : 
## 

karakter="xwr"
nilKarakter () {
    case $1 in
	0) nk="---" ;;
	1) nk="--x" ;;
	2) nk="-w-" ;;
	3) nk="-wx" ;;
	4) nk="r--" ;;
	5) nk="r-x" ;;
	6) nk="rw-" ;;
	7) nk="rwx" ;;
    esac
}
show_oct () {
    for ((x=0;x<8;x++));do
	nilKarakter $x 
	karX=$nk
	for((y=0;y<8;y++));do
	    nilKarakter $y
	    karY=$nk
	    for((z=0;z<8;z++));do
		nilKarakter $z
		karZ=$nk
		angka="$x$y$z"
		karakter="$karX$karY$karZ"
		if test "$1" == "all";then
		    echo "$angka = $karakter"
		elif test "$1" == "$karakter"; then
		    echo "$karakter -> $angka"
		    exit 1
		elif test "$1" == "$angka"; then
		    echo "$angka -> $karakter"
		    exit 1
		fi
	    done
	done
    done
}
show_help () {
    echo "\
+-----------------------------------------------------------------------+
| Reverse permission number,                                            |
| from character to octal or octal to character                         |
| Using  : ./permReverse <octal/character permission / OPTIONS>         |
| Option : ./permReverse -list to show all.                             |
|          ./permReverse <anything> to show this page.                  |
|          'octal_permission'                                           |
|             -> must 3 numbers and that is octal.                      |
|          'character_permission'                                       |
|             -> just accept character 'rwx' or '-' in 9 characters.    |
|                                                                       |
+---------------------------------------------------------------> nu1 <-+"
    exit 1
}
if test $# -eq 1;then
    num=`echo $1 | awk '/^[0-7]*$/'`
    chr=`echo $1 | awk '/^[r,w,x,-]*$/'`
    if [ ${#num} -eq 3 ] ; then
		show_oct "$num"
#		echo "num"
    elif [ ${#chr} -eq 9 ] ;then
		show_oct "$chr"
#		echo "chr"
    else
	case $1 in
	    "-l" | "-li" | "-lis" | "-list" )
	    show_oct "all" ;;
*) show_help ;;
	esac
    fi
else
    show_help
fi


