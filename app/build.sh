#!/bin/bash
Folders="tbsys tbnet qconf_base"

TBLIB_ROOT=./lib
if [ -z $TBLIB_ROOT ]; then
    echo "TBLIB_ROOT has not set"
    exit;
fi

REAL_FILE=`readlink -f $0`
cd `dirname $REAL_FILE`
BASE_HOME="`pwd`"


for element in $Folders
do
cd $BASE_HOME/$element
if [ -z "$1" -o "$1" = 'make' ]; then  
	sh autogen.sh
	CXXFLAGS='-O3 -Wall -D_NO_EXCEPTION' ./configure --prefix=$BASE_HOME/lib/$element
	make -j 8
fi
if [ -z "$1" -o "$1" = 'make' -o "$1" = 'install' ]; then 
	make install
fi

if [ "$1" = 'clean' ]; then
	cd $element
	echo  $element
	make clean distclean
	sh autogen.sh clean
fi
done
echo "have installed in $TBLIB_ROOT"  
#copy the tbsys and tbnet to /lib as make tair
#TBLIB_ROOT export ./lib 安装目录  $TBLIB_ROOT/include/tbnet $TBLIB_ROOT/include/tbsys $TBLIB_ROOT/lib





