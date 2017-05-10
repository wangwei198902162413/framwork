1.安装tbnet tbsys qconf_base 直接build.sh 
2.安装tair 需要先export TBLIB_ROOT=(安装tbsys 库的lib的绝对路径) cd tair 目录下 sh cplib.sh 及可以安装 或者 将头文件/include/tbsys /include/tbnet 目录下，lib/libtbnet.a /lib/libtbsys.a
(建议使用第二种方式)



/*直接build 既可以安装tbsys tbnet  安装到了.lib下  export TBLIB_ROOT= lib 目录 make tair


. 设置环境变量 TBLIB_ROOT


　　取得源代码后, 先指定环境变量 TBLIB_ROOT 为需要安装的目录. 这个环境变量在后续 tair 的编译安装中仍旧会被使用到.


　　比如要安装到当前用户的lib目录下, 则指定export TBLIB_ROOT="~/lib"。
export_TBLIB_ROOT="/app/hbhome/OFCS_HOME/billing/app_dupchecker/tb-common-utils/lib"

        b.设置头文件路径

       因为tbnet和tbsys在两个不同的目录，但它们的源码文件里头文件的互相引用却没有加绝对或相对路径，将两个目录的源码加入到C++环境变量中即可。


　　CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/home/tair/tair/tb-common-utils/tbsys/src:/home/tair/tair/tb-common-utils/tbnet/src

:/app/hbhome/OFCS_HOME/billing/app_dupchecker/tb-common-utils/tbsys/src:/app/hbhome/OFCS_HOME/billing/app_dupchecker/tb-common-utils/tbnet/src
　　export CPLUS_INCLUDE_PATH*/
