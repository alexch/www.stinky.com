#!/usr/bin/perl

print "Content-type: text/html\n\n";

print "<title>Tetris Statistics</title>\n";
print "<h2>Tetris Statistics</h2>\n";

print "Here are the statistics based on the number of new games ";
print "per day. Note the immediate surge in interest, followed by ";
print "an almost compulsive, steady perseveration. This can be seen ";
print "as emblematic of the Web itself.";
print "<p><hr>\n<pre>\n";

open(LOG, "tetris.log");

while ($line = <LOG>) {
    $line =~ /\w+\W+(\w+)\W+(\w+)\W+(\w+).*/;
    $month = $2;
    $day = $3;
    $date = $2 . " " . $3;
    $data{$date}++;
    $total++;
    $dow{$1}++;
}

@keys = sort(sort_date keys(%data));
&hist ( *keys, *data);		# * means pass by reference

print "\nTotal:\t", $total, "\n";

print "\n</pre><hr>\n";

print "<h3>Breakdown by Day of The Week (not normalized)</h3>\n";
print "<pre>\n";

@days = ("Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat");
&hist( *days, *dow);

print "</pre><hr>\n";

sub sort_date {			# 
    %months = ("jan", 1, "feb", 2, "mar", 3, "apr", 4, "may", 5, "jun", 6,
	       "jul", 7, "aug", 8, "sep", 9, "oct", 10, "nov", 11, "dec", 12);

    ($mon1 = (split(' ', $a))[0]) =~ tr/A-Z/a-z/;
    $day1 = (split(' ', $a))[1];
    ($mon2 = (split(' ', $b))[0]) =~ tr/A-Z/a-z/;
    $day2 = (split(' ', $b))[1];

    ($months{$mon1} cmp $months{$mon2}) || ($day1 <=> $day2);

}

# given an associative array and the order of keys,
# prints a histogram
sub hist {
    local (*order, *array) = @_;
    # count dots
    while (($key, $val) = each %array) {
	if ($val > $high) {
	    $high = $val;
	}
    }
    $mod = $high/50;		# 50 dots for the maximum value
    foreach $key (@order) {
	$val = $array{$key};
	print $key, "\t", $val, "\t";
	
	for ($dots = $val; $dots > 0; $dots = $dots - $mod) {
	    print "*";
	}
	print "\n";
    }
}




