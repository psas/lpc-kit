#!/bin/bash

# Script to install lpc21isp program.

#####################################
# some utility vars

#status
SUCCESS=0
FAIL=42

SCRIPTNAME=`basename $0`
KERN_MAJOR_VERSION=`uname -r |  awk -F"." '{ print $1 }'`
KERN_MINOR_VERSION=`uname -r |  awk -F"." '{ print $2 }'`

# if this is non-zero, then we have a 64 bit kernel.
s4BIT=`uname -a | grep -c x86_64`

PWD=`pwd`

#time
DATE=`date`
DAY=`'date' | awk '{print $3}'`
MONTH=`'date' | awk '{print $2}'`
YEAR=`'date' | awk '{print $6}'`

#colors
redtext="\033[31m"
greentext="\033[32m"
boldtext="\033[1m"
normaltext="\033[0m"


############################
#INSTALL DIRECTORIES
board="2148"
export PREFIX=$HOME/PSAS/LPCDEV
export ISP=$PREFIX/LPC/$board/ISP
export TOOLS=$PREFIX/Tools
export SOURCES=$PREFIX/Sources
###########################

###########################
# other vars
export DIRS_TO_CLEAN=$ISP 


########################################
######## FUNCTIONS #####################
########################################

######### Exit status check function 
## Call with two args, the $? and string (a comment)
check_exit() {

    if [ $1 -ne 0 ] 
    then
	echo ##############################################################
	echo -e "$redtext $0 :\tNon-zero return. $2 $normaltext"
	echo ##############################################################
	exit 1
    fi
    return 0
}


### Clean directories ##

clean_directories () {

    for i in $DIRS_TO_CLEAN
    do
	if [ -e $i ]
	then
	    command="rm -rf $i"
            excmd "$command"
	fi
    done
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

################################################
# Prints out help message
infohelp () {
    echo
    echo "USAGE: $SCRIPTNAME (-i | -u) [-h]
            The arguments to use are:
           -h: long help message
           -i: Attempt an install of openOCD
           -u: Attempt an uninstall of openOCD"
    echo
}
################################################
################################################

# start work

infomsg "$SCRIPTNAME started on `date`"

# clean old directories, too many dependencies.
clean_directories

# make new directories
for i in $ISP
do
    if [ ! -d $i ]; then
	infomsg "creating $i"
	mkdir -p $i
    fi
done

cd $ISP

# go find at:
# http://tech.groups.yahoo.com/group/lpc21isp/files/Beta%20versions/
#command="cp $SOURCES/lpc21isp_155.zip ."
command="cp $SOURCES/lpc21isp_163.zip ."
excmd "$command"

#command="unzip ./lpc21isp_155.zip"
command="unzip ./lpc21isp_163.zip"
excmd "$command"

# command="make -f Makefile.gnu"
command="make"
excmd "$command"

infomsg "$SCRIPTNAME ended on `date`"


exit $SUCCESS

