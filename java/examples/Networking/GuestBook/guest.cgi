#!/bin/sh
# guest.cgi

echo "received:" $QUERY_STRING >> log.txt
echo "Content-type: text/plain"
echo 
echo "Thank you for your comments!"
