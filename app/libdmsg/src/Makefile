CXX := g++
DEBUG_FLAG := -g
CXXFLAGS := $(DEBUG_FLAG) -Wall -Wextra -fPIC #-std=gnu++0x
ARFLAGS := rs

INCLUDES := -I ../include -I ../dep/include/librdkafka
LIB_DIR := -L /root/dev/lib
LIB_RDKAFKA := ../dep/lib/librdkafka.a
LIBS := -pthread -lrt -lz $(LIB_RDKAFKA)
SOURCE_FILES := $(wildcard *.cpp)
OBJECT_FILES := $(SOURCE_FILES:.cpp=.o)
LIBRARIES := libdmsg.a libdmsg.so
STATIC_LIBRARY := libdmsg.a
DYNAMIC_LIBRARY := libdmsg.so
ARTMPDIR := libtmp

all : $(LIBRARIES)

$(LIB_RDKAFKA):
	[ -d ../dep ] || mkdir ../dep
	cd ../librdkafka; ./configure --prefix=../dep
	make -C ../librdkafka
	make -C ../librdkafka install

$(LIBRARIES) : $(LIB_RDKAFKA) $(OBJECT_FILES)
	-[ -d $(ARTMPDIR) ] && rm -rf $(ARTMPDIR)
	mkdir $(ARTMPDIR)
	cd $(ARTMPDIR); $(AR) x ../$(LIB_RDKAFKA)
	$(AR) $(ARFLAGS) $(STATIC_LIBRARY) $(OBJECT_FILES) $(ARTMPDIR)/*
	$(CXX) -shared -o $(DYNAMIC_LIBRARY) $(OBJECT_FILES) $(ARTMPDIR)/*

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf $(OBJECT_FILES) $(LIBRARIES) $(ARTMPDIR)

