#!/bin/bash


##
# This script builds the open ocd (On Chip Debugger)
# environment. 
# 1) run kftdlib.sh first
# 2) Download ocd software.
# Reference: http://bec-systems.com/web/content/view/74/9/
#
# USAGE: kocd.sh (-i | -u) [-h]
#            The arguments to use are:
#           -h: long help message
#           -i: Attempt an install of openOCD
#           -u: Attempt an uninstall of openOCD
#
# $Id$
#
#
##


#####################################
# some utility vars

#status
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
board="2148"
export PREFIX=$HOME/lpc-kit
export OCD=$PREFIX/LPC/$board/OCD
export BUILD=$OCD/build
export SRC=$OCD/src
export LIB=$OCD/lib
export PROJ_SOURCE=$PREFIX/Sources/OCD
export DIRS_TO_CLEAN=$SRC
###########################

echo SRC is $SRC
echo LIB is $LIB
echo BUILD is $BUILD



#################################################
#VERSIONS
# Some newer versions not always compatible with
# environment
#################################################
# export OOCD_VERSION=517
export OOCD_VERSION=709

###########################
# other vars


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
	command="rm -rf $i"
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
#    -u)
#      undo=1
#      shift 1
#    ;;

    -i)
      install=1
      shift 1
    ;;


   -h)
      help=1
      shift 1
    ;;


#    -g)
 #     filter=$2
 #     shift 2
 #   ;;


#    -ip)
#      address=$2
#      shift 2
#    ;;


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

# backup old openocd...
newdir=$SRC.$DAY$MONTH$YEAR.$TIME
if [ -d $SRC ]; then
 command="cp -r $SRC $newdir"
 excmd "$command"
fi

# clean old directories, too many dependencies.
    clean_directories


# make new directories
for i in $PREFIX $BUILD $SRC $LIB $PROJ_SOURCE
do
    if [ ! -d $i ]; then
	infomsg "creating $i"
	mkdir -p $i
    fi
done

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

    if [ ! -d $PROJ_SOURCE/openocd ]; then
        pushd $PROJ_SOURCE
       infomsg "No existing version: attempting checkout of new version: $OCD_VERSION"
       command="svn checkout -r $OOCD_VERSION svn://svn.berlios.de/openocd/trunk openocd"
       excmd "$command"
        popd
    fi

    # what version is in $PROJ_SOURCE right now?
    current_ocd_version=`svn info  $PROJ_SOURCE/openocd | grep Revision | awk '{print $2}'`

    infomsg "open ocd version in $PROJ_SOURCE (from svn) is: $current_ocd_version"

    if [ $current_ocd_version -ne $OOCD_VERSION ]; then
     pushd $PROJ_SOURCE
     
     # backup old openocd...
     newdir=openocd.ver-$current_ocd_version
     command="cp -r $SRC $newdir"
     excmd "$command"

     infomsg "Local version ($PROJ_SOURCE/openocd) out of date, attempting checkout of new version: $OCD_VERSION"
     command="svn checkout -r $OOCD_VERSION svn://svn.berlios.de/openocd/trunk openocd"
     excmd "$command"
     popd
    fi

    command="cp -r $PROJ_SOURCE/openocd ."
    excmd "$command"

    pushd $SRC/openocd

    command="./bootstrap"
    excmd "$command"

    command="./configure --enable-ft2232_ftd2xx --prefix=$OCD"
#    command="./configure --enable-ft2232_libftd2xx --prefix=$OCD"
    excmd "$command"

    command="make -j2"
    excmd "$command"   
    command="make install"
    excmd "$command"


fi





################# END OF openOCD ################################

##############################################




echo ############################################################
echo ############################################################
echo -e "$greentext $boldtext Done with $0 $normaltext"
echo ############################################################
echo ############################################################


infomsg "$SCRIPTNAME ended on `date`"

exit $SUCCESS
