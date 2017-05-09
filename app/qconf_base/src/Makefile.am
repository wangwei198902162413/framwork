AUTOMAKE_OPTIONS=foreign
#include ./../Make.defines
lib_LTLIBRARIES=libqonf.la
#libframework_la_CXXFLAGS = -g -O2  -pg
ACLOCAL_AMFLAGS=-I m4
comm_CPPFLAGS ?= -DDEF_LINUX -DSYSTEM_V -DHAVE_ABM -DABM_FIELD_EXTEND  
libqonf_la_CPPFLAGS = $(comm_CPPFLAGS)
libqonf_la_SOURCES =  qconf_format.cc qconf_log.cc qconf_msg.cc qconf_shm.cc md5.c qhash.c \
qhasharr.cpp


bin_PROGRAMS=qconf_shm_set qconf_shm_get

comm_CPPFLAGS ?= -DDEF_LINUX -DSYSTEM_V 
comm_LDFLAGS ?= 
comm_LDADD ?= 


qconf_shm_set_CPPFLAGS = $(comm_CPPFLAGS) -g -O0 -pg
qconf_shm_set_LDADD= $(comm_LDADD)
qconf_shm_set_LDFLAGS = $(comm_LDFLAGS)
qconf_shm_set_SOURCES = qconf_format.cc qconf_log.cc qconf_msg.cc qconf_shm.cc md5.c qhash.c \
qhasharr.cpp qconf_shm_set_test.cpp

qconf_shm_get_CPPFLAGS = $(comm_CPPFLAGS) -g -O0 -pg
qconf_shm_get_LDADD= $(comm_LDADD)
qconf_shm_get_LDFLAGS = $(comm_LDFLAGS)
qconf_shm_get_SOURCES = qconf_format.cc qconf_log.cc qconf_msg.cc qconf_shm.cc md5.c qhash.c \
qhasharr.cpp qconf_shm_get_test.cpp