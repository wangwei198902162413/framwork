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

#ifndef __TBSYS_BYTEBUFFER_H__
#define __TBSYS_BYTEBUFFER_H__

#include "tbsys.h"

namespace tbutil { 
    /**
     * ByteBuffer 类似于Java里面的同名类,是一个二进制流的封装
     *
     * ByteBuffer必须预先分配好固定大小的一块缓冲,当put或get的位置超过
     * 初始设定的大小，会抛出 ByteBuffer::out_of_range 异常
     * ByteBuffer 负责管理缓冲的内存,但是如果你使用wrap方法,则内存由你
     * 自己管理,所以使用wrap的时候一定知道你自己必须为自己的内存负责.
     *
     * ByteBuffer 是流的形式,支持 << 和 >>操作符
     * put和get方法是流操作符的实现,分别是放入和取出一个特定类型的元素
     * 这两个方法都会导致内部的position指针移动
     *
     * peek 方法同get一样取出当前位置的元素,但是不移动position指针
     *
     */
    class ByteBuffer
    {
        public:
            class out_of_range : public std::exception
            {
                public:
                    out_of_range(uint32_t p, uint32_t l, uint32_t s);
                    virtual ~out_of_range() throw() {}

                    virtual const char* what() const throw() ;
                private:
                    static const uint32_t MAX_ERROR_MSG_LEN = 256;
                    char errmsg_[MAX_ERROR_MSG_LEN];
            };
        public:
            explicit ByteBuffer() ;
            explicit ByteBuffer(uint32_t size);
            explicit ByteBuffer(const ByteBuffer& rhs);
            ByteBuffer(const char* data, uint32_t offset, uint32_t size);
            ByteBuffer & assign(const char* data, uint32_t offset, uint32_t size);
            ByteBuffer & wrap(char* data, uint32_t offset, uint32_t size);
            ByteBuffer & operator=(const ByteBuffer& rhs);
            virtual ~ByteBuffer();

        public:
            // generic put & get
            template <typename T> ByteBuffer & put (const T & e) throw (out_of_range);
            template <typename T> ByteBuffer & get (T & e) throw (out_of_range);
            template <typename T> ByteBuffer & put (const std::vector<T> & v) throw (out_of_range) ;
            template <typename T> ByteBuffer & get (std::vector<T> & v) throw (out_of_range);
            template <typename T> ByteBuffer & operator<<(const T &e) throw (out_of_range) { return put(e); }
            template <typename T> ByteBuffer & operator>>(T &e) throw (out_of_range) { return get(e); }

            template <typename T> const ByteBuffer & peek (T & e) const throw (out_of_range);
            template <typename T> T get () throw (out_of_range);

            // specialize put & get with std::string
            ByteBuffer & put(const std::string & e) throw (out_of_range) { return putString(e);  }
            ByteBuffer & get(std::string & e) throw (out_of_range) { return getString(e); }


            ByteBuffer & putString(const std::string & v) throw (out_of_range);
            ByteBuffer & getString(std::string & v) throw (out_of_range);

            // get data_ directly..
            /**
             * [put 放入二进制数据]
             * @param  src          [src数据]
             * @param  offset       [src 要拷贝的 初始字节]
             * @param  size         [要拷贝数据的大小]
             * @return              [返回包含数据的buffer]
             */
            virtual ByteBuffer & put(const char* src, uint32_t offset, uint32_t size)  throw (out_of_range);
            /**
             * [get description]
             * @param  dst          [要放入数据的指针]
             * @param  offset       [要放入数据的开始地址 = dst + offset ]
             * @param  size         [要拷贝的数据的字节数]
             * @return              [ByteBuffer]
             */
            virtual ByteBuffer & get(char* dst, uint32_t offset, uint32_t size) throw (out_of_range);
            // relative get method, from index of data_
            /**
             * [get description]
             * @param  index        [从index处开始拷贝 而不是默认的position_处]
             * @param  dst          [要放入数据的指针]
             * @param  offset       [要放入数据的开始地址 = dst + offset ]
             * @param  size         [要拷贝的数据的字节数]
             * @return              [ByteBuffer]
             */
            ByteBuffer & get(int index, char* dst, uint32_t offset, uint32_t size) throw (out_of_range);
            // fetch data_ directly, use them very carefully 
            ByteBuffer & getRef(int index, const char* &dst, uint32_t size) throw (out_of_range);
            const ByteBuffer & rawData(int index, const char* &dst, uint32_t size) const throw (out_of_range) ;

            template <typename T> ByteBuffer & getRef(int index, T* &dst) throw (out_of_range);
            template <typename T> ByteBuffer & getRef(int index, const T* &dst) throw (out_of_range);

        public:
            /*清空所占据的内存*/
            void      reset();
            /*清空所占据的内存，重新分配size字节数据*/
            void      reset(uint32_t size);

            uint32_t  position(uint32_t p) throw (ByteBuffer::out_of_range);
            /*position_ 表示已经使用了的数据在数组中所占的位置*/
            uint32_t  position() const { return position_; }
            /*取分配的数据的字节大小*/
            uint32_t  size() const { return size_; }
            /*余下的还未使用的数据大小*/
            int32_t   remaining() const { return size_ - position_; }

        protected:
            //malloc 申请size 内存
            char* allocate(uint32_t size) const;
            //free 占用的malloc 内存
            void  free();
            /**
             * [copy 将data的数据的offset位保存入ByteBuffer]
             * @param  data   [要保存的数据]
             * @param  offset [要保存的数据的偏移及从data取多少位数据]
             * @param  length [要保存的数据的大小]
             * @return        [返回保存了data的ByteBuffer]
             */
            ByteBuffer & copy(const char* data, uint32_t offset, uint32_t length);

        protected:
            /*数据存储地址*/
            char* data_;
            /*数据的存储的大小*/
            uint32_t size_;
            /*position_ 表示已经使用了的数据在数组中所占的位置*/
            uint32_t position_;
            bool  own_;
    };

    template <typename T>
        ByteBuffer & ByteBuffer::put(const T & e) 
        throw (ByteBuffer::out_of_range)
        {
            if (position_ + sizeof(T) > size_) 
                throw out_of_range(position_, sizeof(T), size_);
            memcpy(data_ + position_, &e, sizeof(T));
            position_ += sizeof(T);
            return *this;
        }

    template <typename T>
        ByteBuffer & ByteBuffer::get(T & e) 
        throw (ByteBuffer::out_of_range)
        {
            if (position_ + sizeof(T) > size_) 
                throw out_of_range(position_, sizeof(T), size_);
            peek(e);
            position_ += sizeof(T);
            return *this;
        }

    template <typename T>
        T ByteBuffer::get() 
        throw (ByteBuffer::out_of_range)
        {
            T e;
            get(e);
            return e;
        }


    template <typename T>
        const ByteBuffer & ByteBuffer::peek(T & e) const
        throw (ByteBuffer::out_of_range)
        {
            if (position_ + sizeof(T) > size_) 
                throw out_of_range(position_, sizeof(T), size_);
            memcpy(&e, data_ + position_, sizeof(T));
            return *this;
        }

    template <typename T>
        ByteBuffer & ByteBuffer::put(const std::vector<T> & v) 
        throw (ByteBuffer::out_of_range)
        {
            put(v.size());
            for (uint32_t i = 0; i < v.size(); ++i) 
            {
                put(v[i]);
            }
            return *this;
        }

    template <typename T>
        ByteBuffer & ByteBuffer::get(std::vector<T> & v) 
        throw (ByteBuffer::out_of_range)
        {
            uint32_t size;
            get(size);
            T e;
            for (uint32_t i = 0; i < size; ++i) 
            {
                get(e);
                v.push_back(e);
            }
            return *this;
        }

    template <typename T> 
        ByteBuffer & ByteBuffer::getRef(int index, T* &dst) 
        throw (out_of_range)
        {
            const char* ref = 0;
            getRef(index, ref, sizeof(T));
            dst = (T*) ref;
            return *this;
        }

    template <typename T> 
        ByteBuffer & ByteBuffer::getRef(int index, const T* &dst) 
        throw (out_of_range)
        {
            const char* ref = 0;
            getRef(index, ref, sizeof(T));
            dst = (const T*) ref;
            return *this;
        }

} // end namespace tfs

#endif //__BYTEBUFFER_H__













