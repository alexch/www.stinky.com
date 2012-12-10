#!/bin/sh

echo "Content-type: text/html"
echo

refer=`echo "$HTTP_REFERER" | sed -e 's/\?.*//g'`

grep -i "$refer" refer.html > /dev/null || echo "<LI><A HREF=\"$refer\">$refer</A> (noticed `date`)" >> refer.html

cat index.html
