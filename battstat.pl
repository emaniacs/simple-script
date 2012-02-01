#!/usr/bin/perl
##
## filename : battstat.pl
## desc	    : view battery status 
## author   : noone noone[dot]nu1[at]gmail[dot]com
## licensed : (proudly) GPL
## info     : adoption from battlife from bt3
## 

# find battery
if(-e "/proc/acpi/battery/BAT0"){
    $BAT="/proc/acpi/battery/BAT0";}
elsif( -e "/proc/acpi/battery/BAT1"){
    $BAT="/proc/acpi/battery/BAT1";}
else{
    printf("Battery not found\n");
    exit 0;
}

# get remaining of battery
$rem=`cat $BAT/state|egrep 'remaining'`;
$rem=$& if($rem=~m/(\d+)/);

# get charging info
$info=`cat $BAT/state|egrep 'charging'`;

# get battery status charging
$ful=`cat $BAT/info|egrep 'full'`;
$ful=$& if ($ful=~m/(\d+)/);

# get battery stat, from $info
($del, $stat) =split(":", $info);
$stat =~ s/[ \n]//g;

if( $ful == 0){
    print "Where is battery?\n";
    exit -2;
}

printf("Remaining %2.f %%, %s.\n", ($rem/$ful)*100, $stat);

exit 0;
