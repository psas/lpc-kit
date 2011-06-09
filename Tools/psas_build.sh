#!/bin/bash


WHOAMI=`whoami`
SUCCESS=0
FAIL=42

SCRIPTNAME=`basename $0`
KERN_MAJOR_VERSION=`uname -r |  awk -F"." '{ print $1 }'`
KERN_MINOR_VERSION=`uname -r |  awk -F"." '{ print $2 }'`

GPG_KEYRING=/tmp/gnu-keyring.gpg
GPG_PRELUDE="gpg -q --keyring $GPG_KEYRING --verify"
MAKE_CMD="make -j2"

# if this is non-zero, then we have a 64 bit kernel.
s4BIT=`uname -a | grep -c x86_64`

PWD=`pwd`

#time
DATE=`'date'`
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
export SRC=$HOME/Projects/lpc-kit/toolchain/src
export PREFIX=/opt/cross



#################################################
# VERSIONS
# Some newer versions are not always compatible with
# other tools or older versions...
#################################################

# gcc
#export GCC_VERSION=4.3.2
#export GCC_VERSION=4.2.3
export GCC_VERSION=4.2.1

# binutils
export BINUTILS_VERSION=2.19
#export BINUTILS_VERSION=2.17 # does not compile with gcc 4.3

# gdb
export GDB_VERSION=6.6

# newlib
export NEWLIB_VERSION=1.17.0
#export NEWLIB_VERSION=1.16.0
#export NEWLIB_VERSION=1.15.0  # makeinfo madness in this version

export DIRS_TO_CLEAN="$PREFIX/*\
$SRC/target\
$SRC/gcc-*\
$SRC/newlib-*\
$SRC/binutils-*\
$SRC/gdb-*"

######## FUNCTIONS #####################

######### Exit status check function 
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
# Executes command passed as string then evaluates
# exit value.
excmd () {

    infomsg "Command is: $1"
    eval $command
    check_exit $? "Command is: $1"

}

### Clean directories 
clean_directories () {

    for i in $DIRS_TO_CLEAN
    do
        if [ -e $i ]
        then
            sudo rm -rf $i
        fi
    done
}

################################################
# prints out a standard info message from script
# call with message string: infomsg "Hello"

infomsg () {

    echo "===> $SCRIPTNAME : $1"

}

#########################################
#########################################
# start work
#########################################

infomsg  "$SCRIPTNAME started on `date`"
infomsg "Does not work with gcc version 4.4.3."

# clean old directories, too many dependencies.
clean_directories

if [ ! -d $PREFIX ] 
then 
    sudo mkdir -p $PREFIX
fi

if [ ! -d $SRC ]
then
    mkdir -p $SRC 
fi

sudo chmod -R ugo+rwx $PREFIX

# Update gnu keyring
command="wget -nv -O $GPG_KEYRING ftp://ftp.gnu.org/gnu/gnu-keyring.gpg"
excmd "$command"


############################################################
# change to SRC directory and download needed distributions.
############################################################

command="mkdir -p $SRC/Dist"
excmd "$command"
command="cd $SRC/Dist"
excmd "$command"

# gcc
infomsg "wget -nv gcc $GCC_VERSION - gnu compiler"
if [ ! -e gcc-$GCC_VERSION.tar.bz2 ] || [ ! -e gcc-$GCC_VERSION.tar.bz2.sig ];
then
    command="wget -nv ftp://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.bz2"
    excmd "$command"

    command="wget -nv ftp://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.bz2.sig"
    excmd "$command"
fi
pwd
command="$GPG_PRELUDE gcc-$GCC_VERSION.tar.bz2.sig"
excmd "$command"

# binutils is:  as, ld  and others for gnu gcc
infomsg "wget -nv binutils $BINUTILS_VERSION - utilities for cross compiling"
if [ ! -e binutils-$BINUTILS_VERSION.tar.bz2 ] || [ ! -e binutils-$BINUTILS_VERSION.tar.bz2.sig ];
then
    command="wget -nv ftp://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.bz2"
    excmd "$command"
    command="wget -nv ftp://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.bz2.sig"
    excmd "$command"
fi
command="$GPG_PRELUDE binutils-$BINUTILS_VERSION.tar.bz2.sig"
excmd "$command"

# get gdb
infomsg "wget -nv gdb $GDB_VERSION - gnu debugger"
if  [ ! -e gdb-$GDB_VERSION.tar.bz2 ] || [ ! -e gdb-$GDB_VERSION.tar.bz2.sig  ];
then
    command="wget -nv ftp://ftp.gnu.org/gnu/gdb/gdb-$GDB_VERSION.tar.bz2"
    excmd "$command"
    command="wget -nv ftp://ftp.gnu.org/gnu/gdb/gdb-$GDB_VERSION.tar.bz2.sig"
    excmd "$command"
fi
command="$GPG_PRELUDE gdb-$GDB_VERSION.tar.bz2.sig"
excmd "$command"


# get newlib, an embedded c library
infomsg "wget -nv newlib - embedded c library"
if  [ ! -e newlib-$NEWLIB_VERSION.tar.gz ];
then
    command="wget -nv ftp://sources.redhat.com/pub/newlib/newlib-$NEWLIB_VERSION.tar.gz"
    excmd "$command"
fi

# some sort of configuration file to build the arm cross compile
infomsg "arm configuration file."
if  [ ! -e t-arm-elf ];
then
    command="wget -nv http://www.gnuarm.com/t-arm-elf"
    excmd "$command"
fi

########################################################
# cd to build directory and unpack.
########################################################
command="cd $SRC"
excmd "$command"

command="tar -xvjf Dist/binutils-$BINUTILS_VERSION.tar.bz2"
excmd "$command"

command="tar -xvjf Dist/gcc-$GCC_VERSION.tar.bz2"
excmd "$command"

command="tar -xvjf Dist/gdb-$GDB_VERSION.tar.bz2"
excmd "$command"

command="tar -xvzf Dist/newlib-$NEWLIB_VERSION.tar.gz"
excmd "$command"

command="cp Dist/t-arm-elf gcc-$GCC_VERSION/gcc/config/arm/t-arm-elf"
excmd "$command"

##############################################
# start configuring and compiling tools
##############################################

### build dependencies (incomplete-maybe not all) ##############################
infomsg "Resolve build dependencies. Install expat - aptitude requires sudo access"

command="sudo aptitude update"
excmd "$command"


command="sudo aptitude -y install build_essentials texinfo libexpat1 libexpat1-dev libncurses5-dev"
excmd "$command"

#### BINUTILS #################################

infomsg "Build binutils"
command="mkdir -p $SRC/target/arm-elf/binutils"
excmd "$command"

command="cd $SRC/target/arm-elf/binutils"
excmd "$command"

command="$SRC/binutils-$BINUTILS_VERSION/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft"
excmd "$command"

command="$MAKE_CMD all"
excmd "$command"

command="$sudo $MAKE_CMD install"
excmd "$command"

##### GCC ################################

export PATH=$PATH:$PREFIX/bin

infomsg "Build gcc"
command="mkdir -p $SRC/target/arm-elf/gcc"
excmd "$command"

command="cd $SRC/target/arm-elf/gcc"
excmd "$command"

command="sudo $SRC/gcc-4.2.1/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft --enable-languages=\"c,c++\" --with-newlib --with-headers=../../../newlib-$NEWLIB_VERSION/newlib/libc/include"
excmd "$command"

command="sudo $MAKE_CMD all-gcc"
excmd "$command"

command="sudo $MAKE_CMD install-gcc"
excmd "$command"

###### NEWLIB ###############################

infomsg "Build newlib"
command="mkdir -p $SRC/target/arm-elf/newlib"
excmd "$command"

command="cd $SRC/target/arm-elf/newlib"
excmd "$command"

command="$SRC/newlib-$NEWLIB_VERSION/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft"
excmd "$command"

command="$MAKE_CMD all"
excmd "$command"

# pass PATH into sudo environment so /opt/cross/bin is included
info "install newlib"
command="sudo env PATH=$PATH make install"
excmd "$command"

#####  GCC2 ################################

infomsg "Build gcc2"
command="cd $SRC/target/arm-elf/gcc"

command="$MAKE_CMD all"
excmd "$command"

info "install gcc2"
command="sudo env PATH=$PATH $MAKE_CMD install"
excmd "$command"

####  GDB  #################################

infomsg "Build gdb"
command="mkdir -p $SRC/target/arm-elf/gdb"
excmd $command

command="cd $SRC/target/arm-elf/gdb"
excmd $command

command="$SRC/gdb-$GDB_VERSION/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft"
excmd "$command"

command="$MAKE_CMD all"
excmd "$command"

command="sudo env PATH=$PATH $MAKE_CMD install"
excmd "$command"

command="cd $SRC; cd .."
excmd "$command"

command="sudo chown -R $WHOAMI ./*; chgrp -R $WHOAMI ./*"
excmd "$command"



echo
echo
echo ############################################################
echo ############################################################
echo -e "$greentext $boldtext Done with $0 $normaltext"
echo ############################################################
echo ############################################################
echo
echo

sudo chmod -R ugo-w $PREFIX
infomsg "$SCRIPTNAME ended on `date`"

echo
echo
echo

exit 0

