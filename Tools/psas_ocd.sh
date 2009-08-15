#!/bin/bash

##
# This script builds the open ocd (On Chip Debugger)
# environment. 
# 1) run psas_ftdilib.sh first
# 2) Download ocd software.
# Reference: http://bec-systems.com/web/content/view/74/9/
#
# USAGE: psas_ocd.sh (-i ) [-h]
#            The arguments to use are:
#           -h: long help message
#           -i: Attempt an install of openOCD
#
##

#####################################
# some utility vars

#status
WHOAMI=`whoami`
SUCCESS=0
FAIL=42
SCRIPTNAME=`basename $0`
KERN_MAJOR_VERSION=`uname -r |  awk -F"." '{ print $1 }'`
KERN_MINOR_VERSION=`uname -r |  awk -F"." '{ print $2 }'`

#time
PWD=`pwd`
DATE=`date`
DAY=`'date' | awk '{print $3}'`
MONTH=`'date' | awk '{print $2}'`
YEAR=`'date' | awk '{print $6}'`

TIME=`'date' | awk '{print $4}' | sed 's/:/-/g'`
#colors
redtext="\033[31m"
greentext="\033[32m"
boldtext="\033[1m"
normaltext="\033[0m"


############################
#INSTALL DIRECTORIES
export SRC=$HOME/lpc-kit/toolchain/ocd-src
export PREFIX=/opt/psas/ocd
export DIRS_TO_CLEAN=$PREFIX
###########################

echo SRC is $SRC
echo PREFIX is $PREFIX

#################################################
#VERSIONS
# Some newer versions not always compatible with
# environment
#################################################
# export OOCD_VERSION=517
# export OOCD_VERSION=709
export OOCD_VERSION=1257


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

### Clean directories ##
clean_directories () {
    for i in $DIRS_TO_CLEAN
    do
        if [ -e $i ]
        then
            command="sudo rm -rf $i"
            excmd "$command"
        fi
    done
}

################################################
# Prints out help message
infohelp () {
    echo
    echo "USAGE: $SCRIPTNAME -i [-h]
    The arguments to use are:
    -h: long help message
    -i: Attempt an install of openOCD"

    echo
}
################################################
################################################
################################################

# start work

infomsg "$SCRIPTNAME started on `date`"

# look at cmd line

install=0
#undo=0
help=0

while [ $# -gt 0 ]
do
    case $1
        in

        -i)
        install=1
        shift 1
        ;;


        -h)
        help=1
        shift 1
        ;;


        *)
        infohelp
        shift 1
        ;;
    esac
done

if [ "$help" -eq 1 ]; then
    infohelp

    echo "    
    install openocd - Additional Info 
    http://bec-systems.com/web/content/view/74/9/,
    http://www.openhardware.net/Embedded_ARM/OpenOCD_JTAG/
    1. svn checkout svn://svn.berlios.de/openocd/trunk openocd
    2. cd openocd
    3. ./bootstrap
    4. ./configure --enable-ft2232_ftd2xx
    5. make
    6. make install
    "

    exit $FAIL
elif [ "$install" -eq 1 ]; then
    infomsg "Starting."
else
    infohelp
    exit $FAIL
fi


######## STARTING SCRIPT #########################


###########################
# lpc environment

infomsg "listing of ocd source directory"
if [ -d $SRC ]; then
    command="ls $SRC"
    excmd "$command"
fi

# backup old openocd...
newdir=$SRC.$DAY$MONTH$YEAR.$TIME
if [ -d $SRC ]; then
    command="cp -r $SRC $newdir"
    excmd "$command"
fi

# clean old directories, too many dependencies.
clean_directories

# make new directories
for i in $PREFIX $SRC
do
    if [ ! -d $i ]; then
        infomsg "creating $i"
        command="sudo mkdir -p $i"
        excmd "$command"
    fi
    command="sudo chmod -R ugo+rwx $i"
    excmd "$command"
done

command="sudo chown -R $WHOAMI $SRC"
command="sudo chgrp -R $WHOAMI $SRC"
excmd "$command"

##########################
### retrieve the software

if [ "$install" -eq 1 ]; then

    pushd $SRC

    # vars needed only for 64 bit OS?
    #  => The 32-bit environment sets int, long and pointer to 32 bits and generates code that runs on any i386 system.

    #  => The 64-bit environment sets int to 32 bits and long and pointer to 64 bits and generates code for AMD’s x86-64 architecture.

    # You can pass -m64 or -m32 as follows
    # For 32 bit version:
    # $ gcc -m32 -o output32 hello.c
    # For 64 bit version :
    # $ gcc -m64 -o output64 hello.c

    CFLAGS+=-m32
    export CFLAGS
    LDFLAGS+=-L/usr/local/lib
    export LDFLAGS

    # get openocd software
    # decide whether local copy of software is up to date.
    infomsg "Request to install openocd v. $OOCD_VERSION"

    if [ ! -d $SRC/openocd ]; then
        pushd $SRC
        infomsg "No existing version: attempting checkout of new version: $OCD_VERSION"
        command="svn checkout -r $OOCD_VERSION svn://svn.berlios.de/openocd/trunk openocd"
        excmd "$command"
        popd
    fi

    # what version is in $PROJ_SOURCE right now?
    current_ocd_version=`svn info  $SRC/openocd | grep Revision | awk '{print $2}'`

    infomsg "open ocd version in $SRC (from svn) is: $current_ocd_version"

    if [ $current_ocd_version -ne $OOCD_VERSION ]; then
        pushd $SRC

        # backup old openocd...
        newdir=openocd.ver-$current_ocd_version
        command="cp -r $SRC $newdir"
        excmd "$command"

        infomsg "Local version ($SRC/openocd) out of date, attempting checkout of new version: $OCD_VERSION"
        command="svn checkout -r $OOCD_VERSION svn://svn.berlios.de/openocd/trunk openocd"
        excmd "$command"
        popd
    fi

    pushd $SRC/openocd

    command="./bootstrap"
    excmd "$command"

    command="./configure --enable-ft2232_ftd2xx --prefix=$PREFIX"
    #    command="./configure --enable-ft2232_libftd2xx --prefix=$PREFIX"
    excmd "$command"

    command="make -j2"
    excmd "$command"   
    command="make install"
    excmd "$command"
fi

# remove write permissions from /opt/psas
for i in "/opt/psas" $PREFIX $SRC
do
    if [ -d $i ]; then
        infomsg "changing permissions on $i"
        command="sudo chmod -R ugo-w $i"
        excmd "$command"
    fi
done

################# END OF openOCD ################################

echo ############################################################
echo ############################################################
echo -e "$greentext $boldtext Done with $0 $normaltext"
echo ############################################################
echo ############################################################

infomsg "$SCRIPTNAME ended on `date`"

echo
echo

exit $SUCCESS
