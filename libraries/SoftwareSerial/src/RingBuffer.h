/* The MIT License (MIT)
 *
 * Copyright (c) 2022 Arduino SA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
#include <memory>
#include "Arduino.h"

template <class T> class RingBuffer {
    private:
        size_t _size;
        size_t capacity;
        volatile size_t tail;
        volatile size_t head;
        std::unique_ptr<T[]> buff;

    private:
        inline size_t next_pos(size_t x) {
            return (((x) + 1) % (capacity));
        }

    public:
        RingBuffer(size_t size=0): _size(0),
         capacity(size), tail(0), head(0), buff(nullptr) {
            if (capacity) {
                buff.reset(new T[capacity]);
            }
        }

        void reset() {
            tail = head = 0;
        }

        size_t size() {
            return _size;
        }

        size_t empty() {
            return tail == head;
        }

        bool put(T data) {
            bool ret = false;
            if (buff && (next_pos(head) != tail)) {
                _size++;
                buff[head] = data;
                head = next_pos(head);
                ret = true;
            }
            return ret;
        }

        T get(bool peek=false) {
            if (buff && (tail != head)) {
                T data = buff[tail];
                if (!peek) {
                    _size--;
                    tail = next_pos(tail);
                }
                return data;
            }
            return T();
        }
};
#endif //__RING_BUFFER_H__
