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
export SRC=$HOME/Projects/lpc-kit/toolchain/
export PREFIX=/opt/ocd
export DIRS_TO_CLEAN="$PREFIX $SRC/openocd"
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
        echo "Cleaning $i"
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


# clean old directories, too many dependencies.
infomsg "Cleaning directories"
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
    pushd $SRC
    command="git clone git://openocd.git.sourceforge.net/gitroot/openocd/openocd"
    excmd "$command"
    popd

    infomsg "moving to $SRC/openocd"
    pushd $SRC/openocd

    # install basic packages
    command="sudo aptitude -y install libtool automake libftdi-dev libftdi1"
    excmd "$command"

    command="./bootstrap"
    excmd "$command"

    echo `pwd`
    command="./configure --enable-maintainer-mode --enable-ft2232_libftdi --prefix=$PREFIX"
    infomsg "Run : $command"
    echo `pwd`
#    excmd "$command"

    command="make -j2"
    infomsg "Run: $command"
#    excmd "$command"   
    command="make install"
    infomsg "Run: $command"
#    excmd "$command"
fi

# remove write permissions from /opt/
#for i in "/opt"
#do
#    if [ -d $i ]; then
#        infomsg "changing permissions on $i"
#        command="sudo chmod -R ugo-w $i"
#        excmd "$command"
#    fi
#done

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
