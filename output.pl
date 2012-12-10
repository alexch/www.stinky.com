# output.pl
# library of routines to output fields to a file
# usage:
# require "cgi-lib.pl";
# require "output.pl";

# Version History
#  5 Jul 95     Fixed @fields bug
#               Changed $delimeter to $delim (to fix spelling)
# write_output
# usage: write_output("output.txt")
# uses the following global variables 
#(which you must set to intelligible values before calling certain subroutines)
# %input        CGI input as per cgi-lib.pl
# @fields       list of variable names to print as first fields
# $delim        field delimiter

# variables that must be set up: $input{}, $delim, @fields

sub write_output {
        local($output) = @_;

        open(OUT, ">>$output");

        if ($print_date) {
                $t = &ctime(time); chop($t);
                &fieldout($t);
        }

        if ($print_ip) {
                $ip = $ENV{"REMOTE_HOST"};
                &fieldout($ip);
        }

        foreach $field (@fields) {
                &fieldout($input{$field});
        }

        for ($i=1; $i<=$questions; ++$i) {
                $delim_save = $delim;
                if ($i==$questions) { $delim = ""; }
                eval("&fieldout(\$input{\"Q$i\"});");
                $delimeter = $delim;
        }
        print OUT "\n";

}

sub fieldout {
        local($field) = @_;
        $field =~ s/\n/ /g;
        print OUT $field, "$delim";
}

# print_file
# usage: &print_file("file.txt");

sub print_file {
        local($file) = @_;
        open ( PRINT, $file ) || die "Can't open file $file\n";
        print while (<PRINT>) ;
}

# print_sub 
# print with substitutions 
# usage: &print_sub("foo.phtml") 
# where foo.phtml is an html file with
# perl-style variables that get substituted

sub print_sub {
        local($file) = @_;

        warn "printing url";
        print $url;
        warn "evaling print url";
        eval("print \$url");

        warn "Opening $file\n";
        open(PRINTSUBINFILE, "$file");
        print "Opened $file as " . PRINTSUBINFILE . "\n";
        local($all) = "";
        while(<PRINTSUBINFILE>) {
            chop;
            s/"/\\"/g;
            print "[[$_";
            $eval = 'print "'. $_ . '\n"';
            warn $eval;
            eval($eval);;
        }
        close(PRINTSUBINFILE);
}

1;
