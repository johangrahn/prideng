#!/bin/bash

# Author: Johan Grahn <johan@cgeek.net>
# Date: 2011-08-20

# Default parameters 
NUM_REPLICAS=2
START_PORT=32000

# Tell bash to quit if any commands fails
set -e

# Iterate trough each replica and create a config file for it
for (( rep = 0; rep < $NUM_REPLICAS; rep++ ))
do
	CONFIG_FILE="rep-$rep.conf"
	
	# Clear the config file 
	echo -n "" > $CONFIG_FILE
	
	# Create a rep command for each replica in the config file
	for (( rep2 = 0; rep2 < $NUM_REPLICAS; rep2++ ))
	do
		HOST=127.0.0.1
		
		# Check so that we don't add ourself 
		if [ "$rep" != "$rep2" ]
		then
			echo "rep $rep2 $HOST $[$START_PORT + $rep2] " >> $CONFIG_FILE
		fi
	done
	
done
