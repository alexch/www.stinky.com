#!/usr/bin/perl
require "cgi-lib.pl";
require "ctime.pl";

chdir("/digiweb/usr/stinky/public_html/alison");

$topics = "topics";
$separator = "---";
#$url = &MyURL; #"http://www.cs.columbia.edu/~chaffee/alison/date.cgi";
$url = "date.cgi";

$options{"table"} = 0;	# use a table border to surround each msg
$options{"msg_hr_size"} = 6;	# how thick is the HR between messages
$options{"strong_buttons"} = 1;	# make the button text <STRONG>
$options{"view"} = 1;		# 0=list, 1=threaded

print &PrintHeader;

$is_input = &ReadParse(*input);
$topic = $input{"topic"};
if ($topic eq "") {
    $topic = "date";
}
$options{"view"} = $input{"view"} if ($input{"view"});

if ($is_input && $topic) {

	# read the topic file
	&read_topic($topic);

	$action = $input{"action"};
	$action =~ tr/A-Z/a-z/;

	# add a new message
	if ($action eq "add_reply") {
		&add_reply;
print STDERR "topic=$topic\n";
		&read_topic($topic);
		&print_topic;
	}
	elsif ($action eq "new_reply")
	{
		&new_reply;
	}
	else # topic, no action
	{	
		&print_topic;
	}

} else {

	$action = $input{"action"};
	$action =~ tr/A-Z/a-z/;

	if ($action eq "new_topic")
	{
		&new_topic;
	}
	elsif ($action eq "add_topic")
	{
		&add_topic;
		&read_topic($topic);
		&print_topic;
	}
	else
	{
		# show the index of topics
	
#		&read_topics;
#		&print_topics;
	    $topic = "date";
	    &read_topic($topic);
	    &print_topic;
	}
}

sub good { 1; }

# read the topics list
sub read_topics {
	# the topics file has a simple format:
	# file\ttitle
	if (open(FILE, "$topics"))
	{

		for (	$i = 0; $line = <FILE>; ++$i) {
			chop($line);
			($topics[$i], $titles[$i]) = ($line =~ /(\S*)\t(.*)/);

		}
		$cTopics = $i;
		close(FILE);
	}
} #read_topics

#read a single topic
sub read_topic {
	local ($topic) = @_;

	open(DATAFILE, "$topic.data") || die "Couldn't open $topic.data\n";

	# first look for intro HTML
	$intro = "";
	while (($line = <DATAFILE>) !~ /^$separator/ && $line) {
		$intro .= $line;
	}

	$i = 0;
	do {

		chop($id[$i] = <DATAFILE>);
		chop($subject[$i] = <DATAFILE>);
		chop($date[$i] = <DATAFILE>);
		chop($from[$i] = <DATAFILE>);
		chop($href[$i] = <DATAFILE>);
		chop($email[$i] = <DATAFILE>);
		chop($re[$i] = <DATAFILE>);
		chop($replies[$i] = <DATAFILE>);

		# set the reverse-index (id->index) array
		$index{$id[$i]} = $i;

		$text[$i] = undef;
		chop($line = <DATAFILE>);
		do
		{
			$text[$i] .= $line . "\n";
			chop($line = <DATAFILE>);
		}
		while(($line !~ /^$separator/) && !eof(DATAFILE));


		$i++;

	} while (!eof(DATAFILE));
	$count = $i;

	$title = $subject[0];

	close(DATAFILE);
}

sub print_topics {
    open(IN, "date.html");
   print while (<IN>);
    close(IN);

	print <<"---";
<H1>Topics</H1>
<OL>
---
	for ($i=0; $i<$cTopics; ++$i) {
		print "<LI><a href=\"$url?topic=$topics[$i]\">$titles[$i]</a></LI>\n";
	}
	print "</OL><P>";
	print "<strong>" if ($options{"strong_buttons"});
	print "[<A HREF=\"$url?action=new_topic\">New Topic</A>]";
	print "</strong>" if ($options{"strong_buttons"});
	print "<BR>";
	&print_hr;
	print "</HTML>";

} # print_topics

sub print_topic {

	print "<HTML>\n";

	if ($intro) {
	    if ($intro =~ /^file:/) {
		($file) = ($intro =~ /^file:(.*)$/);
		open(FILE, $file);
		print while (<FILE>);
		close(FILE);
	    } else {		
		print $intro . "\n";
		print "<HR>\n";
	    }
	}
	else {
		print "<title>$title</title>\n";
	}

	print STDOUT "<H1>$title</H1>\n";

	#introductory message (message #0)

	if ($text[0]) {
		&print_basic_headers(0);
		($parent_href, $parent) = ($re[0] =~ /(\S*)\s*(.*)/);
#		if ($re[0]) {
#			print "<TT>In response to:</TT> $re[0]<br>\n";
#		}
		print "<P>";
		print $text[0];
	}

	&print_message_index;

	if ($options{"view"} == 1) {
	
		# should clear this array
		for ($i=1; $i<$count; ++$i) {
			$printed[$i] = 0;
		}

		for ($i=1; $i<$count; ++$i) {
			if (!$printed[$i]) {
				&recursive_print_message($i);
			}
		}

		

	} else {
		for ($i = 1; $i < $count; ++$i) {	
			&print_message($i);
			&print_message_buttons;
	
		}
	}

	# bottom of page
	&print_hr;
	&print_header_buttons;

	print "</HTML>\n";
}

sub print_message_index {
	# index
#	&print_hr;
	if ($count > 1) {
		print "<H3>Replies:</H3>\n";
		print STDOUT "<A NAME=\"index\"></A>";
		print STDOUT "<OL>\n";
		for ($i = 1; $i < $count; ++$i) {
			print STDOUT "<LI><A HREF=\"#$id[$i]\">$subject[$i]</A>\n";
			if ($from[$i]) {
				print "<I>(by $from[$i])</I>\n";
			}
		}
		print STDOUT "</OL>\n";

		&print_header_buttons;
		print "<P>\n";

	}
	else
	{
		print "<H4>No replies.</H4>\n";
	}
}

sub print_basic_headers {
    local($i) = @_;
    if ($from[$i]) {
	print "<TT>From:</TT> </a>";
	if ($href[$i]) {
	    print "<A HREF=\"$href[$i]\">$from[$i]</A>\n"
	    } else {
		print "$from[$i]\n";
	    }
    
	if ($email[$i]) {
	    print "<I>(<a href=\"mailto:$email[$i]\">$email[$i]</a>)</I>\n";
	}
	print "<BR>\n";
    }
    if ($date[$i]) {
	print "<TT>Date:</TT> $date[$i]\n";
	print "<BR>\n";
    }
} # print_basic_headers

sub print_header_buttons {
	print "<strong>" if ($options{"strong_buttons"});
	print "[<A HREF=\"$parent_href?xxx=$$\">Back to <i>$parent</i></A>] ";
	print "[<A HREF=\"$url?topic=$topic&action=new_reply\">Reply</A> to <i>$title</i>] ";
	print "</strong>" if ($options{"strong_buttons"});
	print "\n";
}

sub recursive_print_message {
	local ($i) = @_;

	&print_message($i);
	&print_message_buttons;
	$printed[$i] = 1;

	foreach $reply ($replies[$i] =~ /\#(\w+)/g) {
		next if ($reply == "");
print stderr "Reply $reply\n";
		print "<BLOCKQUOTE>\n";
		&recursive_print_message(&lookup_id($reply));
		print "</BLOCKQUOTE>\n";
	}
}

sub print_message {
	local ($i) = @_;

	# name the anchor as the horizontal rule
	print "<A NAME=\"$id[$i]\">";
	&print_hr;
	print "</A>\n";

	if ($options{"table"}) {
		print "<TABLE BORDER><TR><TD>\n";
	}

	# header
	# subject
#	print "<TT>Subject:</TT> ";

	print "<TT>$i.</TT> ";
	print "<B>$subject[$i]</B>\n";
	print "<BR>\n";

	# from, date
	&print_basic_headers($i);

	if ($re[$i])		
	{
		print "<TT>In Reply To:</TT> <A HREF=\"$re[$i]\">$subject[&lookup_id($re[$i])]</A>\n";
		print "<BR>\n";
	}
	if ($replies[$i])
	{
		print "<TT>Replies:</TT>";
		$bat = 0;
		foreach $reply ($replies[$i] =~ /\#(\w+)\W*/g) {
#			next if ($reply == "");
			if ($bat) { print ", "; }
			if ($the_index = $index{$reply}) {
				$the_reply = "#" . $reply;
			}
			else
			{
				$the_num = "another topic";
				$the_reply = $reply;
			}
			print " <A HREF=\"$the_reply\">[$the_index]</A>";
			$bat = 1;
		}
		print "<BR>\n";
	}
		if ($options{"table"}) {
		print "</TR></TD><TR><TD>\n";
	}

	print "<BR>\n";
	print $text[$i] . "\n";
	print "<P>\n";

	if ($options{"table"}) {
		print "</TR></TD></TABLE>\n";
	}
}

sub print_message_buttons {

	print "<strong>" if ($options{"strong_buttons"});
	print "[<A HREF=\"$url?topic=$topic&action=new_reply&re=$id[$i]\">Reply</A> to this message] ";
	print "[<A HREF=\"#index\">Return to List of Replies</A>] ";
	print "<P>\n";
	print "</strong>" if ($options{"strong_buttons"});

}
 
sub choose_id {
	local($id);
	for ($id = $count+1; $index{$id}; ++$id ) {}
	return $id;
}

# appends a reply to an existing message
sub append_reply {
	local($msg, $reply) = @_;
#	warn "adding reply: $msg $reply";

	rename("$topic.data", "$topic.data.bak") &&
		open(IN, "<$topic.data.bak") || die("Couldn't open $topic.data.bak\n");
	open(OUT, ">$topic.data") || die("Couldn't open $topic.data\n");
	flock(OUT, 2);

	$count = 0;
	while ($line = <IN>) {
		chop($line);
		if ($line =~ /^$separator/) {
			$fSep = 1;
			$id = "";
		}
		elsif ($fSep) {
			$id = $line;
			$fSep = 0;
		}
		if ($id eq $msg) {
			$count++;
		}
		if ($count == 8) {	# replies is the 8th line
			if ($line ne "") {$line .= " ";}
			$line .= $reply;
			$count = 0;
		}
		print OUT $line . "\n";
	}
	flock(OUT, 8);
	close(IN);
	close(OUT);
	unlink("$topic.data.bak");		
}

# new_reply
# print the form for adding a reply
sub new_reply {

	$re = $input{"re"};

	if($re) {
		$re_id = $re;
		$re_id =~ s/^#//;
		$subj = $subject[$index{$re_id}];
		if ($subj !~ /^Re:/) {
			$subject = "Re: " . $subj;
		}
		else {
			$subject = $subj;
		}
	}

	# reply -> send a form
	print <<"---END FORM---";
<title>Reply to $title</title>
<h1>Reply to $title</h1>
<h3>Original message:</h3>
<blockquote><font size=-1>
$text[$index{$re_id}]
</font></blockquote>
<h3>Your reply:</h3>
<FORM ACTION="$url" METHOD=POST>
<INPUT TYPE=hidden NAME="topic" VALUE="$topic">
<INPUT TYPE=hidden NAME="action" VALUE="add_reply">
<INPUT TYPE=hidden NAME="re" VALUE="#$re">
Your name: <INPUT NAME="from" size=50><br>
Your home page URL: <INPUT NAME="href" size=40> (optional)<br>
Your email address: <INPUT NAME="email" size=30> (optional)<br>
Subject: <INPUT NAME="subject" VALUE="$subject" size=55><br>
Enter your reply below: (Separate paragraphs with blank lines. You can use HTML codes too!)<br>
<TEXTAREA NAME="body" ROWS=10 COLS=60></TEXTAREA> <P>
<INPUT TYPE="submit" VALUE="Submit Reply"> <P>
</FORM>
<HR> <P>
[<A HREF="$url?topic=$topic">Return</A> to <i>$title</i>] 

---END FORM---


} # new_reply

sub add_reply {
	# input
	$id = &choose_id;
	$subject = $input{"subject"};
	chop($date = &ctime(time));
	$from = $input{"from"};
	$href = $input{"href"};
	$email = $input{"email"};
	$re = $input{"re"};
	$body = $input{"body"};

	&write_reply($id, $subject, $date, $from, $href, $email, $re, $body);

} # add_reply

sub write_reply {
	local($id, $subject, $date, $from, $href, $email, $re, $body) = @_;

	$body = &parse_body($body);

	# output
	open(OUT, ">>$topic.data") || die "Couldn't open $topic.data for writing\n";
	flock(OUT, 2);

#	if ($id != 0) {
		print OUT $separator ."\n";
#	}
	print OUT $id ."\n";
	print OUT $subject ."\n";
	print OUT $date ."\n";
	print OUT $from ."\n";
	print OUT $href ."\n";
	print OUT $email ."\n";
	print OUT $re ."\n";
	print OUT "\n";		# no replies
	print OUT $body;
	print OUT "\n\n";

	flock(OUT, 8);
	close(OUT);

	if ($re) {
		if ($re =~ /^#/) {
			$re =~ s/^#//;
			&append_reply($re, "#" . $id);
		}
		else
		{
# replies to other topics not yet implemented
		}
	}

} # write_reply

# new_topic
# print the form for adding a topic
sub new_topic {
	print <<"---END";
<HTML>
<TITLE>New Topic</TITLE>
<H1>New Topic</H1>
<FORM ACTION="$url" METHOD=POST>
<input type=HIDDEN name="action" value="add_topic">
Your name: <INPUT NAME="from"><br>
Your home page URL: <INPUT NAME="href"> (optional)<br>
Your email address: <INPUT NAME="email"> (optional)<br>
Topic: <input name="subject" size=50><br>
Enter your message below: (use HTML codes to format)<br>
<TEXTAREA NAME="body" ROWS=10 COLS=60></TEXTAREA> <P>
<INPUT TYPE="submit" VALUE="Submit New Topic"> <P>
</FORM>
<HR> <P>
[<A HREF="$url">Return</A> to topic list]
</HTML>
---END

} # new_topic

sub add_topic {


	$subject = $input{"subject"};

	$topic = $subject;
	$topic = &choose_topic_name($topic);

	open(TOPICS, ">>$topics");
	print TOPICS "$topic\t$subject\n";
	close(TOPICS);

	chop($date = &ctime(time));
	$from = $input{"from"};
	$href = $input{"href"};
	$email = $input{"email"};
	$re = $url ."\tBlind Date Page";
	$body = $input{"body"};

	&write_reply(0,
		$subject,
		$date, $from, $href, $email, $re, $body);

}

sub choose_topic_name {

	local($topic) = @_;

	$name = $topic;
	$name =~ tr/A-Z/a-z/;
	$name =~ s/^(\w{1,8}).*/$1/;	# up to 8 chars

	$tag = ($name =~ /\d*$/);
	while (-e $name . ".data") {
		$tag++;
#		print STDERR $tag . "\n";
		$name =~ s/\d*$/$tag/;
	}

	return $name;
}

sub lookup_id {
	local ($id) = @_;

	return $index{$id};

}

sub print_hr {
	if ($size = $options{"msg_hr_size"}) {
		print "<HR SIZE=$size>\n";
	}
	else {
		print "<HR>\n";
	}
}

sub parse_body {
	local ($body) = @_;

	$body =~ s/\r//g;
	$body =~ s/^($separator)/ $1/g;
	$body =~ s/\n{2,}/\n<P>\n/g;

	return $body;
}

	
