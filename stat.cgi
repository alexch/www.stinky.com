#!/usr/bin/perl

$log = "/web/sites/stinky/access";

$wwwstat = "/web/sites/stinky/wwwstat";

$|=1;
print "Content-type: text/html\n\n";

system("$wwwstat $log");
