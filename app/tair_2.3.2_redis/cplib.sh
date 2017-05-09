REAL_FILE=`readlink -f $0`
cd `dirname $REAL_FILE`
cd ..
BASE_HOME="`pwd`"
cd $BASE_HOME/lib 
mkdir include lib 
cd include 
mkdir tbnet tbsys
cd $BASE_HOME/lib/tbsys/lib 

cp * ../../lib
cd $BASE_HOME/lib/tbnet/lib
cp * ../../lib

cd $BASE_HOME/tbsys/src
cp -f *.h $BASE_HOME/lib/include/tbsys
cd $BASE_HOME/tbnet/src
cp -f *.h $BASE_HOME/lib/include/tbnet
