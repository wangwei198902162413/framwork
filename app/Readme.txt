1.��װtbnet tbsys qconf_base ֱ��build.sh 
2.��װtair ��Ҫ��export TBLIB_ROOT=(��װtbsys ���lib�ľ���·��) cd tair Ŀ¼�� sh cplib.sh �����԰�װ ���� ��ͷ�ļ�/include/tbsys /include/tbnet Ŀ¼�£�lib/libtbnet.a /lib/libtbsys.a
(����ʹ�õڶ��ַ�ʽ)



/*ֱ��build �ȿ��԰�װtbsys tbnet  ��װ����.lib��  export TBLIB_ROOT= lib Ŀ¼ make tair


. ���û������� TBLIB_ROOT


����ȡ��Դ�����, ��ָ���������� TBLIB_ROOT Ϊ��Ҫ��װ��Ŀ¼. ������������ں��� tair �ı��밲װ���Ծɻᱻʹ�õ�.


��������Ҫ��װ����ǰ�û���libĿ¼��, ��ָ��export TBLIB_ROOT="~/lib"��
export_TBLIB_ROOT="/app/hbhome/OFCS_HOME/billing/app_dupchecker/tb-common-utils/lib"

        b.����ͷ�ļ�·��

       ��Ϊtbnet��tbsys��������ͬ��Ŀ¼�������ǵ�Դ���ļ���ͷ�ļ��Ļ�������ȴû�мӾ��Ի����·����������Ŀ¼��Դ����뵽C++���������м��ɡ�


����CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/home/tair/tair/tb-common-utils/tbsys/src:/home/tair/tair/tb-common-utils/tbnet/src

:/app/hbhome/OFCS_HOME/billing/app_dupchecker/tb-common-utils/tbsys/src:/app/hbhome/OFCS_HOME/billing/app_dupchecker/tb-common-utils/tbnet/src
����export CPLUS_INCLUDE_PATH*/
