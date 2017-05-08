CXX := g++
DEBUG_FLAG := -g
CXXFLAGS := $(DEBUG_FLAG) -Wall -Wextra -DUNIT_TEST=1 -DGTEST_HAS_PTHREAD=1 -std=gnu++0x

GTEST_DIR = ./gtest
CPPFLAGS += -I$(GTEST_DIR)/include -I .
TESTS = unit_test
GTEST_HEADERS = ./gtest/include/*.h \
                ./gtest/include/internal/*.h
INCLUDES := -I ../include -I ../dep/include/librdkafka -I . -I ./gtest
LIB_DIR := -L /root/dev/lib
LIBS := -pthread ../dep/lib/librdkafka.a -lrt -lz
TESTED_DIR := ../src
SOURCE_FILES := $(wildcard *.cpp)
SOURCE_FILES += $(TESTED_DIR)/Config.cpp $(TESTED_DIR)/KafkaHandle.cpp $(TESTED_DIR)/Producer.cpp $(TESTED_DIR)/Consumer.cpp $(TESTED_DIR)/DmsgWorker.cpp
OBJECT_FILES := $(SOURCE_FILES:.cpp=.o)

all : $(TESTS)

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

unit_test: $(OBJECT_FILES) gtest_main.a
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(LIB_DIR) $^ $(LIBS) -o $@

clean:
	rm -rf $(OBJECT_FILES) $(TESTS) gtest.a gtest_main.a

