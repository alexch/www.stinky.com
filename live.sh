#!/bin/sh
#rsync -Cavz --exclude '*~' --exclude live.sh . www.stinky.com:/home/web/www.stinky.com/
#rsync -Cavz --exclude '*~' --exclude live.sh . alex@www.stinky.com:/var/webapps/stinky
date > deploy.txt
rsync -Cavz --exclude '*~' --exclude live.sh . purplet@secure00.secure-transact.net:/home/purplet/public_html/stinky

