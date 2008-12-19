#!/bin/bash


##
# This script gathers and compiles tools for
# cross compiling and developing on the
# lpc2148 olimex board.
##


#####################################
# some utility vars

#status
SUCCESS=0
FAIL=42

SCRIPTNAME=`basename $0`
KERN_MAJOR_VERSION=`uname -r |  awk -F"." '{ print $1 }'`
KERN_MINOR_VERSION=`uname -r |  awk -F"." '{ print $2 }'`

GPG_PRELUDE="gpg -q --keyring gnu-keyring.gpg --verify"
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
board=2148
export PREFIX=$HOME/lpc-kit/LPC/$board
export BUILD=$PREFIX/build
export SRC=$PREFIX/src


#################################################
# VERSIONS
# Some newer versions are not always compatible with
# other tools or older versions...
#################################################
# http://gcc.gnu.org/
export GCC_VERSION=4.3.2
#export GCC_VERSION=4.2.3

# http://www.gnu.org/software/binutils/
export BINUTILS_VERSION=2.19
#export BINUTILS_VERSION=2.17

# http://sourceware.org/gdb/
export GDB_VERSION=6.8

# http://expat.sourceforge.net/
export EXPAT_VERSION=2.0.1

# http://sourceware.org/newlib/
export NEWLIB_VERSION=1.16.0

##### Maybe Eclipse precludes using Insight
##### 64 bit version of insight problematic!
#export INSIGHT_VERSION=6.7.1
########################################

# other vars
# FROM: http://gcc.gnu.org/mirrors.html
#GNU_KEYS="745C015A B75C61B8 902C9419 E0A38377 FF325CF3 F71EDF1C 4AE55E93"

export DIRS_TO_CLEAN="$BUILD 
                      $PREFIX/share 
                      $PREFIX/man 
                      $PREFIX/lib 
                      $PREFIX/lib* 
                      $PREFIX/info 
                      $PREFIX/bin 
                      $PREFIX/arm-elf 
                      $PREFIX/include"

########################################
######## FUNCTIONS #####################
########################################

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


#########################################
#########################################
#########################################

# start work

infomsg  "$SCRIPTNAME started on `date`"

# clean old directories, too many dependencies.
clean_directories


if [ ! -d $PREFIX ] 
then 
    mkdir -p $PREFIX
    mkdir $PREFIX/bin
fi

if [ ! -d $SRC ]
then
    mkdir $SRC 
fi


if [ ! -d $BUILD ]
then
    mkdir $BUILD
fi

#      command="gpg -q --recv-keys --keyserver pgp.mit.edu $i"
#      command="gpg --import gnu-keyring.gpg"
# Update gnu keyring
      command="pushd $HOME/.gnupg"
      excmd "$command"
      command="wget ftp://ftp.gnu.org/gnu/gnu-keyring.gpg"
      excmd "$command"
      command="popd"
      excmd "$command"


############################################################
# change to SRC directory and download needed distributions.
############################################################
cd $SRC

# the ubiquitous gnu compiler collection (gcc)
infomsg "wget gcc $GCC_VERSION - gnu compiler"
if [ ! -e gcc-$GCC_VERSION.tar.bz2 ] || [ ! -e gcc-$GCC_VERSION.tar.bz2.sig ];
then
    command="wget ftp://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.bz2"
    excmd "$command"

    command="wget ftp://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.bz2.sig"
    excmd "$command"
fi
# command="gpg -q --keyring gnu-keyring.gpg --verify gcc-$GCC_VERSION.tar.bz2.sig"
command="$GPG_PRELUDE gcc-$GCC_VERSION.tar.bz2.sig"
excmd "$command"

# binutils is  as  and  ld  and others for gnu gcc
infomsg "wget binutils $BINUTILS_VERSION - utilities for cross compiling"
if [ ! -e binutils-$BINUTILS_VERSION.tar.bz2 ] || [ ! -e binutils-$BINUTILS_VERSION.tar.bz2.sig ];
then
    command="wget ftp://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.bz2"
    excmd "$command"
    command="wget ftp://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.bz2.sig"
    excmd "$command"
fi
command="$GPG_PRELUDE binutils-$BINUTILS_VERSION.tar.bz2.sig"
excmd "$command"

# get expat XML parse libs for gdb
infomsg "wget expat version: $EXPAT_VERSION - expat XML parse libs for gdb"
if  [ ! -e expat-$EXPAT_VERSION.tar.gz ];
then
    command="wget http://downloads.sourceforge.net/expat/expat-$EXPAT_VERSION.tar.gz"
    excmd "$command"
fi

# get gdb
infomsg "wget gdb $GDB_VERSION - gnu debugger"
if  [ ! -e gdb-$GDB_VERSION.tar.bz2 ] || [ ! -e gdb-$GDB_VERSION.tar.bz2.sig  ];
then
    command="wget ftp://ftp.gnu.org/gnu/gdb/gdb-$GDB_VERSION.tar.bz2"
    excmd "$command"
    command="wget ftp://ftp.gnu.org/gnu/gdb/gdb-$GDB_VERSION.tar.bz2.sig"
    excmd "$command"
fi
command="$GPG_PRELUDE gdb-$GDB_VERSION.tar.bz2.sig"
excmd "$command"


# get newlib, an embedded c library
infomsg "wget newlib - embedded c library"
if  [ ! -e newlib-$NEWLIB_VERSION.tar.gz ];
then
    command="wget ftp://sources.redhat.com/pub/newlib/newlib-$NEWLIB_VERSION.tar.gz"
    excmd "$command"
fi

# some sort of configuration file to build the arm cross compile
infomsg "arm configuration file."
if  [ ! -e t-arm-elf ];
then
    command="wget http://www.gnuarm.com/t-arm-elf"
    excmd "$command"
fi


# get insight debugger
#if  [ ! -e insight-$INSIGHT_VERSION.tar.bz2 ]
#then
#    wget ftp://sourceware.org/pub/insight/releases/insight-$INSIGHT_VERSION.tar.bz2
#    wget ftp://sourceware.org/pub/insight/releases/md5.sum
#fi
#SUM=`cat md5.sum | sed 's/ .*//'`
#checkmd5=`md5sum insight-6.7.1.tar.bz2 | sed 's/ .*//'`
#infomsg
#if [ "$checkmd5" != "$SUM" ]
#then
#    infomsg -e $boldtext$redtext"The checksums do not match." $normaltext
#    exit 1
#fi


########################################################
# cd to build directory and unpack.
########################################################
cd $BUILD
command="tar -xzf $SRC/newlib-$NEWLIB_VERSION.tar.gz"
excmd "$command"

command="tar -xjf $SRC/binutils-$BINUTILS_VERSION.tar.bz2"
excmd "$command"

command="tar -xjf $SRC/gcc-$GCC_VERSION.tar.bz2"
excmd "$command"

command="tar -xzf $SRC/expat-$EXPAT_VERSION.tar.gz"
excmd "$command"

command="tar -xjf $SRC/gdb-$GDB_VERSION.tar.bz2"
excmd "$command"

#tar -xjf $SRC/insight-$INSIGHT_VERSION.tar.bz2
#check_exit $? "tar -xjf $SRC/insight-$INSIGHT_VERSION.tar.bz2"


mkdir $BUILD/binutils
mkdir $BUILD/newlib
mkdir $BUILD/gcc
mkdir $BUILD/gcc2
mkdir $BUILD/expat
mkdir $BUILD/gdb
#mkdir $BUILD/insight

mkdir $PREFIX/info
mkdir $PREFIX/info/dir
mkdir $PREFIX/info/dir/dir

export PATH=$PREFIX/bin:$PATH
infomsg "path is now: $PATH"

##############################################
# start configuring and compiling tools
##############################################

#### BINUTILS #################################

infomsg "binutils"
cd $BUILD/binutils

command="$BUILD/binutils-$BINUTILS_VERSION/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft  --enable-languages=\"c,c++\" "
excmd "$command"

command="$MAKE_CMD"
excmd "$command"

command="$MAKE_CMD install"
excmd "$command"

##### GCC ################################


infomsg "Build gcc"
cd $BUILD/gcc

command="$BUILD/gcc-$GCC_VERSION/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft  --enable-languages=\"c,c++\" "
excmd "$command"


command="$MAKE_CMD all-gcc"
excmd "$command"
command="$MAKE_CMD install-gcc"
excmd "$command"


###### NEWLIB ###############################

infomsg "Build newlib"
cd $BUILD/newlib

command="$BUILD/newlib-$NEWLIB_VERSION/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft"
excmd "$command"

command="$MAKE_CMD"
excmd "$command"

command="$MAKE_CMD install"
excmd "$command"

#####  GCC2 ################################

infomsg "Build gcc2"
cd $BUILD/gcc2

command="$BUILD/gcc-$GCC_VERSION/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft --enable-languages=\"c,c++\" --with-newlib --with-headers=$BUILD/newlib-$NEWLIB_VERSION/newlib/libc/include"
excmd "$command"

command="$MAKE_CMD"
excmd "$command"

command="$MAKE_CMD install"
excmd "$command"

### expat ##############################
infomsg "Build expat"
cd $BUILD/expat

command="$BUILD/expat-$EXPAT_VERSION/configure --prefix=$PREFIX"
excmd "$command"

command="$MAKE_CMD"
excmd "$command"

command="$MAKE_CMD install"
excmd "$command"

####  GDB  #################################

infomsg "Build gdb"
cd $BUILD/gdb

command="$BUILD/gdb-$GDB_VERSION/configure --target=arm-elf --prefix=$PREFIX --with-libexpat-prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft"
excmd "$command"

command="$MAKE_CMD"
excmd "$command"

command="$MAKE_CMD install"
excmd "$command"

###  INSIGHT ##################################

#infomsg "Build insight"
#cd $BUILD/insight
#rm -rf *
#command="$BUILD/insight-$INSIGHT_VERSION/configure --target=arm-elf --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft  --with-newlib --with-headers=$BUILD/newlib-$NEWLIB_VERSION/newlib/libc/include"
#excmd "$command"
#command="$MAKE_CMD"
#excmd "$command"
#command="$MAKE_CMD install"
#excmd "$command"

echo
echo
echo ############################################################
echo ############################################################
echo -e "$greentext $boldtext Done with $0 $normaltext"
echo ############################################################
echo ############################################################
echo
echo


infomsg "$SCRIPTNAME ended on `date`"

exit 0
