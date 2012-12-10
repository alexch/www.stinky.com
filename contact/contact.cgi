#!/usr/local/bin/perl
require "cgi-lib.pl";
require "output.pl";
require "ctime.pl";

if (&ReadParse(*input)) {

    
 @fields = (
	    "Last",
	    "First",
	    "Title",
	    "Company",
	    "Work",
	    "Home",
	    "Fax",
	    "Other",
	    "Email",
	    "Address",
	    "City",
	    "State",
	    "Zip",
	    "Country",
	    "URL",
	    "Birthday",
	    "Note",
	    "Category"
	    );
 $delim = "\t";
 $print_date = 1;
 $print_ip = 1;

 &write_output("contact.txt");

 print &PrintHeader;
 print "<html><head><title>Thanks</title></head><body>\n";
 print "Thanks for the info.";
}

else {

    print &PrintHeader;

    print `cat contact.html`;

}



     



