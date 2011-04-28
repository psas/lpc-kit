#!/bin/bash

# This script calls scripts to set up an environment
# for working with LPC2148 on the olimex board.

############################
#INSTALL DIRECTORIES
PREFIX=$HOME/Projects/lpc-kit/Tools

#####################################
# some utility vars

SUCCESS=0
FAIL=42
SCRIPTNAME=`basename $0`

#time
PWD=`pwd`
DAY=`'date' | awk '{print $3}'`
MONTH=`'date' | awk '{print $2}'`
YEAR=`'date' | awk '{print $6}'`

#colors
redtext="\033[31m"
greentext="\033[32m"
boldtext="\033[1m"
normaltext="\033[0m"

############################

########################################
######## FUNCTIONS #####################
########################################

## Exit status check function 
## Call with two args, the $? and string (a comment)
check_exit() {

if [ $1 -ne 0 ] 
then
    echo ############################################################
    echo -e "$redtext $boldtext Non-zero return from $2 $normaltext"
    echo ############################################################
    exit 1
fi
return 0
}

################################################
# prints out a standard info message from script
# call with message string: infomsg "Hello"
infomsg () {

    echo "===> $SCRIPTNAME : $1"

} 

################################################
# Executes command passed as string then evaluates
# exit value.
excmd () {

    infomsg "Command is: $1"
    eval $command
    check_exit $? "Command is: $1"

}

########################################


#####
# Step 1
# call psas_build.sh and log output.
infomsg "Building gcc cross-compilers"
command="$PREFIX/psas_build.sh >& psas_build.log"
excmd "$command"

##
# step 2 obsolete - no more ftdixxx

#####
# Step 3
# install openocd debian package
infomsg "Installing open-on-chip-debugger package (openocd)"
command="sudo aptitude -y install openocd expect"
excmd "$command"

#####
#####
#  Alternate Step 3 - build from sources
# call psas_ocd.sh
#infomsg "Building open on chip debugger (openocd)"
#command="$PREFIX/psas_ocd.sh -i >& psas_ocd.log"
#excmd "$command"
#####
#####

#####
# Step 4
# call psas_eclipse.sh
#infomsg "Installing Eclipse"
#command="$PREFIX/psas_eclipse.sh >& psas_eclipse.log"
#excmd "$command"


echo "=============="
echo "=============="
echo "Done with $0"
echo "=============="
echo "=============="

