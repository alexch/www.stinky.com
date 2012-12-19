#!/usr/bin/perl
eval '$'.$1.'$2;' while $ARGV[0] =~ /^([A-Za-z_0-9]+=)(.*)/ && shift;
			# process any FOO=bar switches

$, = ' ';		# set output field separator
$\ = "\n";		# set output record separator

while (<>) {
    ($chord,$gif) = split(' ', $_, 9999);

    print "<p>$chord<br><img SRC='http://www.alligatorboogaloo.com/uke/chords/chord_$gif.gif' WIDTH='44' HEIGHT='55' BORDER='0'></p>";
}
