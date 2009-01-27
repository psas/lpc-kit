#!/bin/bash

##
# This script downloads and builds the eclipse environment
# for LPC2148 development
##


#####################################
# some utility vars

#status
SUCCESS=0
FAIL=42
SCRIPTNAME=`basename $0`

KERN_MAJOR_VERSION=`uname -r |  awk -F"." '{ print $1 }'`
KERN_MINOR_VERSION=`uname -r |  awk -F"." '{ print $2 }'`
# if this is non-zero, then we have a 64 bit kernel.
S4BIT=`uname -a | grep -c x86_64`


#time
PWD=`pwd`
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
export PREFIX=$HOME/lpc-kit
export IDE=$PREFIX/IDE/$board/
export BUILD=$PREFIX/build
export SRC=$PREFIX/src
export LIB=$PREFIX/lib
export TOOLS=$PREFIX/Tools
export SOURCES=$PREFIX/Sources
###########################


###############################################
#VERSIONS
#################################################
# see http://www.eclipse.org/downloads
#"Ganymede Classic"
export ECLIPSE_VERSION=3.4.1
export ECLIPSE_64="eclipse-SDK-$ECLIPSE_VERSION-linux-gtk-x86_64.tar.gz"
export ECLIPSE_32="eclipse-SDK-$ECLIPSE_VERSION-linux-gtk.tar.gz"

###########################
# other vars
export DIRS_TO_CLEAN=$IDE 

########################################
######## FUNCTIONS #####################
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
########################################


### Clean directories ##

clean_directories () {

    for i in $DIRS_TO_CLEAN
    do
	if [ -e $i ]
	then
	    rm -rf $i
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
################################################

# start work

infomsg "$SCRIPTNAME started on `date`"

# clean old directories, too many dependencies.
clean_directories

# make new directories
for i in $IDE $SOURCES
do
    if [ ! -d $i ]; then
	infomsg "creating $i"
	mkdir -p $i
    fi
done

cd $IDE


if [ "$S4BIT" -eq 1 ]; 
then
    pushd $SOURCES
    infomsg "64 BIT LINUX VERSION OF ECLIPSE"

    if [ ! -e $SOURCES/$ECLIPSE_64 ];
    then
	command="wget -nv http://mirrors.cat.pdx.edu/eclipse/eclipse/downloads/drops/R-3.4.1-200809111700/$ECLIPSE_64"
	excmd "$command"
    fi
    if [ ! -e $SOURCES/$ECLIPSE_64.md5 ]; 
    then
	command="wget -nv http://mirrors.cat.pdx.edu/eclipse/eclipse/downloads/drops/R-3.4.1-200809111700/checksum/$ECLIPSE_64.md5"
	excmd "$command"
    fi

    infomsg "Checking md5 sum"
    msum=`md5sum -c $SOURCES/$ECLIPSE_64.md5 | grep -c OK`
    if [ "$msum" -eq 0 ]; then
	infomsg "MD5 sum failed on  $SOURCES/$ECLIPSE_64"
	exit $FAIL
    fi
    popd
  
    command="tar xzf $SOURCES/$ECLIPSE_64"
    excmd "$command"

else
     pushd $SOURCES
    infomsg "32 BIT LINUX VERSION OF ECLIPSE"

    if [ ! -e $SOURCES/$ECLIPSE_32 ];
    then 
	command="wget -nv http://mirrors.cat.pdx.edu/eclipse/eclipse/downloads/drops/R-3.4.1-200809111700/$ECLIPSE_32"
	excmd "$command"
    fi
    if [ ! -e $SOURCES/$ECLIPSE_32.md5 ]; 
    then
	command="wget -nv http://mirrors.cat.pdx.edu/eclipse/eclipse/downloads/drops/R-3.4.1-200809111700/checksum/$ECLIPSE_32.md5"
	excmd "$command"
    fi

    infomsg "Checking md5 sum"
    msum=`md5sum -c $SOURCES/$ECLIPSE_32.md5 | grep -c OK`
    if [ "$msum" -eq 0 ]; then
	infomsg "MD5 sum failed on  $SOURCES/$ECLIPSE_32"
	exit $FAIL
    fi
    popd
  
    command="tar xzf $SOURCES/$ECLIPSE_32"
    excmd "$command"


fi

infomsg "C/C++ and Zylin embedded tools packages must be added from within eclipse."

echo ############################################################
echo ############################################################
echo -e "$greentext $boldtext Done with $0 $normaltext"
echo ############################################################
echo ############################################################

infomsg "$SCRIPTNAME ended on `date`"
exit $SUCCESS


