#!/bin/sh

MODULENAME=kafuka
VERSION=`grep v  ChangeLog | head -n 1 | cut -d" " -f2 | sed 's/[v:]//g'`

BUILD_DIR=$1
build_method=$2

if [ $# -lt 1 ]; then
	echo "Usage: build.sh build_dir build_method"
	echo "build_method: debug release"
	echo "For example:sh build.sh ./pack runtime release"
	exit 1
fi



if [ "$1" = "-v" ]; then
    echo $VERSION
    exit 0
fi

BUILD_TIME=`date "+%Y%m%d"`

#定义记录日志的函数
writeLog()
{
    # print time
    time=`date "+%D %T"`
    echo "[$time] $*"
}

#如果输入的目录不存在，则进行创建
if [ ! -d $BUILD_DIR ]; then
	writeLog "The build directory you input does not exits ! I will make it for you !"
	mkdir -p $BUILD_DIR
fi

#定义当前工作目录
WORK_DIR=`pwd`

MODULE_INSTALL_NAME=$MODULENAME-install
MODULE_INSTALL_DIR=$WORK_DIR/$MODULE_INSTALL_NAME

if [ ! -d $MODULE_INSTALL_DIR ]; then
   mkdir -p $MODULE_INSTALL_DIR
else
   rm -rf  $MODULE_INSTALL_DIR
   mkdir -p $MODULE_INSTALL_DIR
fi

build_osplatform_name()
{
	arch=`uname -m`
	SFS_TARGET_PLATFORM=$FUDE_OS_SPECIFICATION-$arch
}

#检查编译环境
check_build_env()
{	
	#cd $WORK_DIR
	#if [ -f "librdkafka.patch" ]; then
	#	patch --dry-run -p0 < librdkafka.patch
	#fi
		
	return 0
}

build_base()
{
	cd $WORK_DIR/librdkafka
	chmod +x conf*
	./configure --prefix=../dep
	make 
	make install
	
	cd $WORK_DIR/src
	make
	

	return 0
}

build_install()
{
	#cp -a $WORK_DIR/dep/include/librdkafka/* $BUILD_DIR/include/
	#cp -a $WORK_DIR/dep/lib/* $BUILD_DIR/lib/
	cp -a $WORK_DIR/src/libdmsg.* $BUILD_DIR/lib/
	cp -a $WORK_DIR/include/* $BUILD_DIR/include/
}

build()
{
	writeLog "$MODULENAME build start!"
	start_time=`date +%s`
	
	mkdir -p $BUILD_DIR/include $BUILD_DIR/lib
	
	check_build_env
	build_base
	build_install
		
	end_time=`date +%s`
	total=`expr $end_time - $start_time`
	writeLog "$MODULENAME build success! Total Build Time:$total seconds"
}

build

