/*
 * (C) 2007-2010 Taobao Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Version: $Id$
 *
 * Authors:
 *   duolong <duolong@taobao.com>
 *
 */

#ifndef TBSYS_LOG_H
#define TBSYS_LOG_H

#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <deque>
#include <string>
#include <pthread.h>
#include <sys/time.h>

#define TBSYS_LOG_LEVEL_ERROR 0
#define TBSYS_LOG_LEVEL_USER_ERROR  1
#define TBSYS_LOG_LEVEL_WARN  2
#define TBSYS_LOG_LEVEL_INFO  3
#define TBSYS_LOG_LEVEL_TRACE 4
#define TBSYS_LOG_LEVEL_DEBUG 5
#define TBSYS_LOG_LEVEL(level) TBSYS_LOG_LEVEL_##level, __FILE__, __LINE__, __FUNCTION__, pthread_self()
#define TBSYS_LOG_NUM_LEVEL(level) level, __FILE__, __LINE__, __FUNCTION__, pthread_self()
#define TBSYS_LOGGER tbsys::CLogger::getLogger()
#define TBSYS_PRINT(level, ...) TBSYS_LOGGER.logMessage(TBSYS_LOG_LEVEL(level), __VA_ARGS__)
#define TBSYS_LOG_BASE(level, ...) (TBSYS_LOG_LEVEL_##level>TBSYS_LOGGER._level) ? (void)0 : TBSYS_PRINT(level, __VA_ARGS__)
#define TBSYS_LOG(level, _fmt_, args...) ((TBSYS_LOG_LEVEL_##level>TBSYS_LOGGER._level) ? (void)0 : TBSYS_LOG_BASE(level,_fmt_, ##args))
#define TBSYS_LOG_US(level, _fmt_, args...) \
  ((TBSYS_LOG_LEVEL_##level>TBSYS_LOGGER._level) ? (void)0 : TBSYS_LOG_BASE(level, "[%ld][%ld][%ld] " _fmt_, \
                                                            pthread_self(), tbsys::CLogger::get_cur_tv().tv_sec, \
                                                            tbsys::CLogger::get_cur_tv().tv_usec, ##args))

namespace tbsys {
using std::deque;
using std::string;

/**
* @brief Â¼Ã²ÂµÂ¥ÂµÃ„ÃˆÃ•Ã–Â¾ÃÂµÃÂ³
*/
class           CLogger {
public:

  static const mode_t LOG_FILE_MODE = 0644;
    CLogger();
    ~CLogger();
    /**
     * @brief
     *
     * @param filename
     * @param fmt
     */
    void rotateLog(const char *filename, const char *fmt = NULL);
    /**
     * @brief Â½Â«ÃˆÃ•Ã–Â¾Ã„ÃšÃˆÃÃÂ´ÃˆÃ«ÃŽÃ„Â¼Ã¾
     *
     * @param level ÃˆÃ•Ã–Â¾ÂµÃ„Â¼Â¶Â±Ã°
     * @param file  ÃˆÃ•Ã–Â¾Ã„ÃšÃˆÃÃ‹Ã¹Ã”ÃšÂµÃ„ÃŽÃ„Â¼Ã¾
     * @param line  ÃˆÃ•Ã–Â¾Ã„ÃšÃˆÃÃ‹Ã¹Ã”ÃšÂµÃ„ÃŽÃ„Â¼Ã¾ÂµÃ„ÃÃÂºÃ…
     * @param function ÃÂ´ÃˆÃ«ÃˆÃ•Ã–Â¾Ã„ÃšÃˆÃÂµÃ„ÂºÂ¯ÃŠÃ½ÃƒÃ»Â³Ã†
     * @param fmt
     * @param ...
     */
    void logMessage(int level, const char *file, int line, const char *function, pthread_t tid, const char *fmt, ...) __attribute__ ((format (printf, 7, 8)));
    /**
     * @brief set log putout level
     *
     * @param level DEBUG|WARN|INFO|TRACE|ERROR
     *
     * @param wf_level set the level putout to wf log file
     */
    void setLogLevel(const char *level, const char *wf_level = NULL);
    /**
     * @brief set log file name
     *
     * @param filename log file name
     *
     * @param flag whether to redirect stdout to log file, if false, redirect it
     *
     * @param open_wf whether to open wf log file, default close
     */
    void setFileName(const char *filename, bool flag = false, bool open_wf = false);
    /**
     * @brief Â¼Ã¬Â²Ã¢ÃŽÃ„Â¼Ã¾ÃŠÃ‡Â·Ã±Ã’Ã‘Â¾Â­Â´Ã²Â¿Âª,Â±ÃªÃ—Â¼ÃŠÃ¤Â³Ã¶,Â´Ã­ÃŽÃ³ÃŠÃ¤Â³Ã¶Ã–Ã˜Â¶Â¨ÃÃ²
     */
    void checkFile();
    void setCheck(int v) {_check = v;}
    /**
     * @brief Ã‰Ã¨Ã–ÃƒÃˆÃ•Ã–Â¾ÃŽÃ„Â¼Ã¾ÃŽÃ„Â¼Ã¾ÂµÃ„Â´Ã³ÃÂ¡,Â´Ã¯ÂµÂ½maxFileSizeÂ¾ÃÃÃ‚Â´Ã²Â¿ÂªÃ’Â»Â¸Ã¶ÃŽÃ„Â¼Ã¾
     * ÃˆÃ§Â¹Ã»Â²Â»Ã‰Ã¨Ã–ÃƒÂ´Ã‹ÃÃ®Â£Â¬ÃˆÃ•Ã–Â¾ÃÂµÃÂ³Â»Ã¡ÂºÃ¶Ã‚Ã”ÃˆÃ•Ã–Â¾Â¹Ã¶Â¶Â¯
     *
     * @param maxFileSize ÃˆÃ•Ã–Â¾ÃŽÃ„Â¼Ã¾ÂµÃ„Â´Ã³ÃÂ¡
     */
    void setMaxFileSize( int64_t maxFileSize=0x40000000);
    /**
     * @brief Â±Â£ÃÃ´Ã—Ã®Â½Ã¼maxFileIndexÂ¸Ã¶ÃˆÃ•Ã–Â¾ÃŽÃ„Â¼Ã¾Â£Â¬Â³Â¬Â³Ã¶maxFileIndexÂ¸Ã¶ÃˆÃ•Ã–Â¾ÃŽÃ„Â¼Ã¾
     * Â»Ã¡Â°Â´ÃŠÂ±Â¼Ã¤ÃÃˆÂºÃ³Ã‰Â¾Â³Ã½,ÂµÂ«Â½Ã¸Â³ÃŒÃ–Ã˜Ã†Ã´ÂºÃ³ÃˆÃ•Ã–Â¾ÃÂµÃÂ³Â»Ã¡Â°Â´ÃŠÂ±Â¼Ã¤ÃÃˆÂºÃ³Ã–Ã˜ÃÃ‚ÃÂ³Â¼Ã†
     *
     * @param maxFileIndex Â±Â£ÃÃ´ÃŽÃ„Â¼Ã¾ÂµÃ„Ã—Ã®Â´Ã³Â¸Ã¶ÃŠÃ½
     */
    void setMaxFileIndex( int maxFileIndex= 0x0F);

    static inline struct timeval get_cur_tv()
    {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      return tv;
    };

    static CLogger& getLogger();

private:
    int _fd;
    int _wf_fd;
    char *_name;
    int _check;
    size_t _maxFileIndex;
    int64_t _maxFileSize;
    bool _flag;
    bool _wf_flag;

public:
    int _level;
    int _wf_level;

private:
    std::deque<std::string> _fileList;
    std::deque<std::string> _wf_file_list;
    static const char *const _errstr[];
    pthread_mutex_t _fileSizeMutex;
    pthread_mutex_t _fileIndexMutex;
};

}
#endif
