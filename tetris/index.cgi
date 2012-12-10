#!/bin/sh

refer=`echo "$HTTP_REFERER" | sed -e 's/\?.*//g'`

#echo $refer | grep "www.stinky.com" || grep -i "$refer" refer.html > /dev/null || echo "<LI><A HREF=\"$refer\">$refer</A> (noticed `date`)" >> refer.html

echo "Location: http://www.gamelan.com/pages/Gamelan.arts.game.arcade.tetris.html"
echo ""

#./tetris



