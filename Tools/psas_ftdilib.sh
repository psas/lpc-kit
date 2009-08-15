#!/bin/bash

##
# This script builds the ftdi lib
# library environment. 
# 1) Download and unpack ftd library for linux systems.
# 1a) Instructions for user to install ftd libs by hand
#
# Reference: http://bec-systems.com/web/content/view/74/9/
#
# USAGE: psas_ftdilib.sh (-i | -u) [-h]
#            The arguments to use are:
#           -h: long help message
#           -i: Attempt an install of ftdi libs
#           -u: Attempt an uninstall of ftdi libs
#
##


# status
SUCCESS=0
FAIL=42
SCRIPTNAME=`basename $0`
KERN_MAJOR_VERSION=`uname -r |  awk -F"." '{ print $1 }'`
KERN_MINOR_VERSION=`uname -r |  awk -F"." '{ print $2 }'`

# time
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
export SRC=$HOME/toolchain/ftdilib
export PREFIX=/opt/psas/ftdilib
export LIB=$PREFIX/lib
export DIRS_TO_CLEAN="$PREFIX $SRC $LIB"

###########################

#################################################
#VERSIONS
# Some newer versions not always compatible with
# environment
#################################################
#export FTDI_LINUX_VERSION=0.4.16
export FTDI_LINUX_VERSION=0.4.13

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
# start work
################################################

infomsg "$SCRIPTNAME started on `date`"

# look at cmd line

install=0
uninstall=0
help=0

while [ $# -gt 0 ]
do
    case $1
        in
        -u)
        uninstall=1
        shift 1
        ;;

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
    # install the ftdi library (where?)
    echo "Install ftd library by hand:"
    echo "
    ##################################################
    # Reference: http://www.openhardware.net/Embedded_ARM/OpenOCD_JTAG/
    # copy the dynamic lib with cp libftd2xx.so.0.4.13 /usr/local/lib/
    # create symlink of ln -s /usr/local/lib/libftd2xx.so.0.4.13 /usr/local/lib/libftd2xx.so
    # create symlink of ln -s /usr/local/lib/libftd2xx.so.0.4.13 /usr/lib/libftd2xx.so
    # copy ftd2xx.h and WinTypes.h into /usr/include
    # edit /etc/ld.so.conf and added the line /usr/local/lib
    # run ldconfig from a console to rebuild the runtime loader database of available libraries
    # edit /etc/fstab to add a line of none /proc/bus/usb usbfs defaults,devmode=0666 0 0
    # run mount -a from a console to mount the new USB filesystem
    ######################################################"

    echo "Alternative instructions:"
    echo " 
    ##########################################################
    # Reference: http://psas.pdx.edu/OlimexLPC2148Setup/
    # Install the FTDI library, do this as 'root'. 
    #  Note, the library provided by FTDI is libc6, i386-32bit. 
    #  I was not able to get this working on my x86-64bit linux install. 
    #  I instead installed on a 32 bit linux install.
    #
    #   1. Download this: http://www.ftdichip.com/Drivers/D2XX/Linux/libftd2xx0.4.13.tar.gz
    #   2. Decompress and untar: tar -zxf libftd2xx0.4.13.tar.gz
    #   3. Their official instructions are in the 'README.dat' file.
    #   4. Copy the header files: 'cp *.h /usr/local/include/'
    #   5. Copy the library file: 'cp libftd2xx.so.0.4.13 /usr/local/lib'
    #   6. 'cd /usr/local/lib'
    #   7. ln -s libftd2xx.so.0.4.13 libftd2xx.so
    #   8. cd /usr/lib
    #   9. ln -s /usr/local/lib/libftd2xx.so.0.4.13 libftd2xx.so
    #  10. For a 2.6 kernel: add this to /etc/fstab 'none /proc/bus/usb usbfs defaults,mode=0666 0 0'
    #  11. For a 2.4 kernel: add this to /etc/fstab 'none /proc/bus/usb usbdevfs defaults,mode=0666 0 0'
    #  12. Make sure '/usr/local/lib' is in /etc/ld.so.conf
    #  13. Run 'ldconfig'
    #  14. run 'mount -a' (it might spew out some warnings)
    ################################################################"
    exit $FAIL
elif [ "$uninstall" -eq "$install" ]; then
    infohelp
    infomsg "Both install and uninstall? Hunh? Pick one."
    exit $FAIL
else
    infomsg "Starting."
fi


######## STARTING SCRIPT #########################

# clean old directories, too many dependencies.
clean_directories

# make new directories
for i in $PREFIX $SRC $LIB
do
    if [ ! -d $i ]; then
        infomsg "creating $i"
        command="sudo mkdir -p $i"
        excmd "$command"
    fi
    infomsg "here"
    command="sudo chmod -R ugo+rwx $i"
    excmd "$command"
done

##########################
### retrieve the software

if [ "$install" -eq 1 ]; then

    command="cd $LIB"
    excmd "$command"

    # get the ftdi library
    ftdi_file="libftd2xx$FTDI_LINUX_VERSION.tar.gz"
    command="wget -nv http://www.ftdichip.com/Drivers/D2XX/Linux/$ftdi_file"

    if [ ! -e $ftdi_file ]; then
        excmd "$command"
    fi

    # unpack the library
    command="tar xzf $ftdi_file"
    excmd "$command"

fi


##################################################
# include files (header files)
if [ "$install" -eq 1 ]; then
    cd $LIB
    command="sudo cp *.h /usr/local/include/"
    excmd "$command"
elif [ "$uninstall" -eq 1 ]; then
    command="sudo rm /usr/local/include/ftd2xx.h"
    if [ -e  /usr/local/include/ftd2xx.h ]; then
        excmd "$command"
    fi
    command="sudo rm /usr/local/include/WinTypes.h"
    if [ -e  /usr/local/include/WinTypes.h ]; then
        excmd "$command"
    fi
else
    echo "Unknown territory"
    exit $FAIL
fi



#################################################
# lib files
if [ "$install" -eq 1 ]; then
    command="sudo cp libftd2xx.so.0.4.13 /usr/local/lib"
    excmd "$command"

    if [ !  -h  /usr/local/lib/libftd2xx.so ]; then
        command="sudo ln -s /usr/local/lib/libftd2xx.so.0.4.13 /usr/local/lib/libftd2xx.so"
        excmd "$command"
    fi

    if [ !  -h  /usr/lib/libftd2xx.so ]; then
        command="sudo ln -s /usr/local/lib/libftd2xx.so.0.4.13 /usr/lib/libftd2xx.so"
        excmd "$command"
    fi

elif [ "$uninstall" -eq 1 ]; then

    command="sudo rm /usr/local/lib/libftd2xx.so* "
    if [ -h  /usr/local/lib/libftd2xx.so ]; then
        excmd "$command"
    fi

    command="sudo rm /usr/lib/libftd2xx.so* "
    if [ -h  /usr/lib/libftd2xx.so ]; then
        excmd "$command"
    fi

    command="sudo rm /usr/local/lib/libftd2xx.so.0.4.13"
    if [ -e  /usr/local/lib/libftd2xx.so.0.4.13 ]; then
        excmd "$command"
    fi

else
    echo "Unknown territory"
    exit $FAIL
fi


##############################################################
# Mess with fstab for usb filesystem

if [ $KERN_MINOR_VERSION -eq 6 ]; then
    check_fstab=`grep -c usbfs /etc/fstab`
    if [ $check_fstab -eq 0 ] && [ "$install" -eq 1 ]; then
        command="sudo sh -c \"echo none /proc/bus/usb usbfs defaults,mode=0666 0 0  >> /etc/fstab\" "
        excmd "$command"
    elif    [ $check_fstab -eq 1 ] && [ "$uninstall" -eq 1 ]   ; then
#        command='sudo umount usbfs'
#        excmd "$command"
        command=" sudo sh -c \"sed -e '/^none \/proc\/bus\/usb usbfs defaults,mode=0666 0 0$/d' /etc/fstab > /tmp/fstab.$$\"; sudo mv /etc/fstab /etc/fstab.$YEAR$MONTH$DAY; sudo mv /tmp/fstab.$$ /etc/fstab"
        excmd "$command"
    else
        infomsg "Strange setup...already uninstalled?: install: $install uninstall: $uninstall check_fstab: $check_fstab"
        #	exit $FAIL	
    fi
elif [ $KERN_MINOR_VERSION -eq 4 ]; then
    check_fstab=`grep -c usbdevfs /etc/fstab`
    if [ $check_fstab -eq 0 ] && [ "$install" -eq 1 ]; then
        command="sudo sh -c \"echo none /proc/bus/usb usbdevfs defaults,mode=0666 0 0 >> /etc/fstab\" "
        excmd "$command"
    elif  [ $check_fstab -eq 1] && [ "$uninstall" -eq 1 ]; then
        command='sudo umount usbdevfs'
        excmd "$command"
        command=" sudo sh -c \"sed -e '/^none \/proc\/bus\/usb usbdevfs defaults,mode=0666 0 0$/d' /etc/fstab > /tmp/fstab.$$; sudo mv /etc/fstab /etc/fstab.$YEAR$MONTH$DAY; sudo mv /tmp/fstab.$$ /etc/fstab"
        excmd "$command"
    else
        infomsg "Strange setup...already uninstallne?: install: $install uninstall: $uninstall check_fstab: $check_fstab"
        #	exit $FAIL	
    fi
fi


check_ldso=`grep -c ^/usr/local/lib$ /etc/ld.so.conf`
if [ $check_ldso -eq 0 ] && [ "$install" -eq 1 ]; then 
    command="sudo sh -c \"echo /usr/local/lib >> /etc/ld.so.conf\" "
    excmd "$command"
elif  [ $check_ldso -eq 1 ] && [ "$uninstall" -eq 1 ]; then
    command=" sudo sh -c \"sed -e '/\/usr\/local\/lib$/d'  /etc/ld.so.conf > /tmp/ld.so.conf.$$\" ; sudo mv /etc/ld.so.conf /etc/ld.so.conf.$YEAR$MONTH$DAY; sudo mv /tmp/ld.so.conf.$$ /etc/ld.so.conf"
    excmd "$command"
else
    infomsg "Strange setup...already uninstalled?: install: $install uninstall: $uninstall check_ldso: $check_ldso"
    #  exit $FAIL	
fi

# run on install and uninstall
command="sudo ldconfig"
excmd "$command"

if [ "$install" -eq 1 ]; then
    infomsg "Mounting usb file system, may spew messages"
    command="sudo mount -a"
    infomsg "Command is: $command"
    eval $command
fi


echo ############################################################
echo ############################################################
echo -e "$greentext $boldtext Done with $0 $normaltext"
echo ############################################################
echo ############################################################


echo "$SCRIPTNAME ended on `date`"

exit 0
