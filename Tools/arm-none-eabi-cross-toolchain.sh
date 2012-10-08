#!/bin/bash
#Must be run with root privileges (sudo, etc)
#todo: verbose option (and maybe super extra verbose as well)
#todo: find the latest versions automatically

#This takes a really, really long time.
#todo: make this go faster!

MYSRCPATH=$HOME/src/cross-tools
MYPREFIX=/opt/cross
GCC=gcc-4.7.2
GDB=gdb-7.5
BINUTILS=binutils-2.22
NEWLIB=newlib-1.20.0
TARGET=arm-none-eabi
CONFIG_OPTS="--target=$TARGET --prefix=$MYPREFIX \
    --enable-interwork --enable-multilib --with-float=soft --quiet"

apt-get install texinfo libexpat1 libexpat1-dev

#------Download sources------
if ! mkdir -p $MYSRCPATH/Dist ; then
    return
fi
cd $MYSRCPATH/Dist

wget -nc ftp://ftp.gnu.org/gnu/gcc/$GCC/$GCC.tar.bz2
wget -nc ftp://ftp.gnu.org/gnu/gcc/$GCC/$GCC.tar.bz2.sig
if gpg --verify $GCC.tar.bz2.sig ; then
    return
fi

wget -nc ftp://ftp.gnu.org/gnu/binutils/$BINUTILS.tar.bz2
wget -nc ftp://ftp.gnu.org/gnu/binutils/$BINUTILS.tar.bz2.sig
if gpg --verify $BINUTILS.tar.bz2.sig ; then
    return
fi

wget -nc ftp://ftp.gnu.org/gnu/gdb/$GDB.tar.bz2
wget -nc ftp://ftp.gnu.org/gnu/gdb/$GDB.tar.bz2.sig
if gpg --verify $GDB.tar.bz2.sig ; then
    return
fi

wget -nc ftp://sources.redhat.com/pub/newlib/$NEWLIB.tar.gz
wget -nc ftp://sources.redhat.com/pub/newlib/md5.sum
if cat md5.sum | grep $NEWLIB | md5sum -c ; then 
    return
fi

#------Extract sources------
extract_prog(){
    if ! test -e $1; then
        echo "-----Extracting $1----"
        tar xjf Dist/$1.tar.bz2
    else
        echo "-----Skipping $1 extracton, already exists-----"
    fi
}
cd $MYSRCPATH
extract_prog $BINUTILS
extract_prog $GCC
extract_prog $NEWLIB
extract_prog $GDB

#------Build and install the toolchain------
make_and_install(){
    if make -s all; then
        echo "-----error making $1-----"
        return
    fi 
    if make -s install; then
        echo "-----error make installing $1-----"
        return
    fi
}

config_and_build(){
    #todo: find a way to stop '*** removing */Makefile to force reconfigure
    #(that is, dont force a reconfigure if it's already configured)
    #todo: find a way to suppress 'libtool:' messages    
    echo "-----Building $1-----"
    mkdir -p $MYSRCPATH/target/$TARGET/$1
    cd $MYSRCPATH/target/$TARGET/$1
    ../../../$1/configure $2
    make_and_install $1
}

export PATH=$PATH:$MYPREFIX/bin

config_and_build $BINUTILS "$CONFIG_OPTS"
config_and_build $GCC "$CONFIG_OPTS --enable-languages=\"c,c++\" \
     --with-newlib --with-headers=../../../$NEWLIB/newlib/libc/include"
config_and_build $NEWLIB "$CONFIG_OPTS"

echo "-----Re-building $GCC-----"
cd $MYSRCPATH/target/$TARGET/gcc
make_and_install "$GCC again"

config_and_build $GDB "$CONFIG_OPTS"

echo "-----Done-----"

