AUTOMAKE_OPTIONS=foreign
#include ./../Make.defines
bin_PROGRAMS=infocheck qconf_msg_send qconf_recv_send  qconf_shm_get qconf_shm_set shm_read\
shm_write

comm_CPPFLAGS ?= -DDEF_LINUX -DSYSTEM_V -I$(TBLIB_ROOT)/qconf_base/include 
comm_LDFLAGS ?= -L$(TBLIB_ROOT)/qconf_base/lib
comm_LDADD ?= -lqonf

#infocheck_CXXFLAGS = 
infocheck_CPPFLAGS = $(comm_CPPFLAGS) -g -O0 -pg
infocheck_LDADD= $(comm_LDADD)
infocheck_LDFLAGS = $(comm_LDFLAGS)
infocheck_SOURCES = qconf_log_test.cpp 


qconf_msg_send_CPPFLAGS = $(comm_CPPFLAGS) -g -O0 -pg
qconf_msg_send_LDADD= $(comm_LDADD)
qconf_msg_send_LDFLAGS = $(comm_LDFLAGS)
qconf_msg_send_SOURCES = qonf_msg_send_test.cpp

qconf_recv_send_CPPFLAGS = $(comm_CPPFLAGS) -g -O0 -pg
qconf_recv_send_LDADD= $(comm_LDADD)
qconf_recv_send_LDFLAGS = $(comm_LDFLAGS)
qconf_recv_send_SOURCES = qonf_msg_recv_test.cpp

qconf_shm_get_CPPFLAGS = $(comm_CPPFLAGS) -g -O0 -pg
qconf_shm_get_LDADD= $(comm_LDADD)
qconf_shm_get_LDFLAGS = $(comm_LDFLAGS)
qconf_shm_get_SOURCES = qconf_shm_get_test.cpp

qconf_shm_set_CPPFLAGS = $(comm_CPPFLAGS) -g -O0 -pg
qconf_shm_set_LDADD= $(comm_LDADD)
qconf_shm_set_LDFLAGS = $(comm_LDFLAGS)
qconf_shm_set_SOURCES = qconf_shm_set_test.cpp


shm_read_CPPFLAGS = $(comm_CPPFLAGS) -g -O0 -pg
shm_read_LDADD= $(comm_LDADD)
shm_read_LDFLAGS = $(comm_LDFLAGS)
shm_read_SOURCES = shmread.c

shm_write_CPPFLAGS = $(comm_CPPFLAGS) -g -O0 -pg
shm_write_LDADD= $(comm_LDADD)
shm_write_LDFLAGS = $(comm_LDFLAGS)
shm_write_SOURCES = shmwrite.c