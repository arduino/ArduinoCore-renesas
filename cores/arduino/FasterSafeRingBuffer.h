/*
  Copyright (c) 2020 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifdef __cplusplus

#ifndef _FASTER_SAFE_RING_BUFFER_
#define _FASTER_SAFE_RING_BUFFER_
#include "sync.h"


// Note: Below is a modified version of the RingBuffer.h, that removes the
// count member variable, such that the producer and consumer don't need to
// modify the same members.  Allows one or both of them to run without needing
// to disable interrupts for reading and writing members.

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
namespace arduino {

#define SERIAL_BUFFER_SIZE 64

template <int N>
class SaferRingBufferN
{
  public:
    uint8_t _aucBuffer[N] ;
    volatile int _iHead ;
    volatile int _iTail ;

  public:
    SaferRingBufferN( void ) ;
    void store_char( uint8_t c ) ;
    void clear();
    int read_char();
    int available();
    int availableForStore();
    int peek();
    bool isFull();

  private:
    int nextIndex(int index);
    inline bool isEmpty() const { return (_iHead == _iTail); }
};

typedef SaferRingBufferN<SERIAL_BUFFER_SIZE> SaferRingBuffer;


template <int N>
SaferRingBufferN<N>::SaferRingBufferN( void )
{
    memset( _aucBuffer, 0, N ) ;
    clear();
}

template <int N>
void SaferRingBufferN<N>::store_char( uint8_t c )
{
  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  int newhead = nextIndex(_iHead);
  if (newhead != _iTail)
  {
    _aucBuffer[_iHead] = c ;
    _iHead = newhead;
  }
}

template <int N>
void SaferRingBufferN<N>::clear()
{
  _iHead = 0;
  _iTail = 0;
}

template <int N>
int SaferRingBufferN<N>::read_char()
{
  if (_iHead == _iTail)
    return -1;

  uint8_t value = _aucBuffer[_iTail];
  _iTail = nextIndex(_iTail);

  return value;
}

template <int N>
int SaferRingBufferN<N>::available()
{
  // grab state of head and tail, to keep result consistent 
  int head = _iHead;
  int tail = _iTail;

  if (head >= tail) 
    return head - tail;
  return N + head - tail;
}

template <int N>
int SaferRingBufferN<N>::availableForStore()
{
  // grab state of head and tail, to keep result consistent 
  int head = _iHead;
  int tail = _iTail;
  if (head >= tail) return N - 1 - head + tail;
  return tail - head - 1;
}

template <int N>
int SaferRingBufferN<N>::peek()
{
  if (isEmpty())
    return -1;

  return _aucBuffer[_iTail];
}

template <int N>
int SaferRingBufferN<N>::nextIndex(int index)
{
  return (uint32_t)(index + 1) % N;
}

template <int N>
bool SaferRingBufferN<N>::isFull()
{
  int newhead = nextIndex(_iHead);
  return  (newhead == _iTail);
}

///////////////////////////////////

// Protect writes for potential multiple writers 
template <int N>
class FasterSafeWriteRingBufferN : public SaferRingBufferN<N>
{
  public:
    void store_char( uint8_t c ) ;
};

typedef FasterSafeWriteRingBufferN<SERIAL_BUFFER_SIZE> FasterSafeWriteRingBuffer;


template <int N>
void FasterSafeWriteRingBufferN<N>::store_char(uint8_t c) {
  synchronized {
    SaferRingBufferN<N>::store_char(c);
  }
}

template <int N>
class FasterSafeReadRingBufferN : public SaferRingBufferN<N>
{
  public:
    int read_char();
};

typedef FasterSafeReadRingBufferN<SERIAL_BUFFER_SIZE> FasterSafeReadRingBuffer;

template <int N>
int FasterSafeReadRingBufferN<N>::read_char() {
  synchronized {
    return SaferRingBufferN<N>::read_char();
  }

  // We should never reached this line because the synchronized {} block gets
  // executed at least once. However the compiler gets confused and prints a
  // warning about control reaching the end of a non-void function. This
  // silences that warning.
  return -1;
}


}

#endif /* _SAFE_RING_BUFFER_ */
#endif /* __cplusplus */
