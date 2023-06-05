/*
  Copyright (c) 2014 Arduino.  All right reserved.

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

#ifndef _ARDUINO_FIFO_BUFFER_DH_
#define _ARDUINO_FIFO_BUFFER_DH_

#include <stdint.h>
#include <string.h>
#include <Arduino.h>

namespace arduino {


#define FIFO_DEFAULT_SIZE 64

template <typename T, uint32_t size = FIFO_DEFAULT_SIZE>
class FifoBuffer
{
  private:
    uint32_t nextIndex(uint32_t index) {
      synchronized {
        return (uint32_t)(index + 1) % size;
      }
    }
    inline bool isEmpty() const { return (_numElems == 0); }
    T _aucBuffer[size] ;
    uint32_t _iHead ;
    uint32_t _iTail ;
    uint32_t _numElems;
  public:
    /* ---------------------------------------------------------------------- */
    FifoBuffer( void ) {
      memset( _aucBuffer, 0, size * sizeof(T) ) ;
      clear();
    }
    /* ---------------------------------------------------------------------- */
    bool store( T c ) {
      bool rv = true;
      synchronized {
        if (!isFull()) {
          _aucBuffer[_iHead] = c ;
          _iHead = nextIndex(_iHead);
          _numElems++;
        }
        else {
          rv = false;
        }
      }
      return rv;
    }
    /* ---------------------------------------------------------------------- */
    void clear() {
      _iHead = 0;
      _iTail = 0;
      _numElems = 0;
    }
    /* ---------------------------------------------------------------------- */
    T read(bool *read_ok) {
      *read_ok = true;
      if (isEmpty()) {
        *read_ok = false;
        return _aucBuffer[0];
      }
      synchronized {
        T value = _aucBuffer[_iTail];
        _iTail = nextIndex(_iTail);
        _numElems--;
      
        return value;
      }
    }
    /* ---------------------------------------------------------------------- */
    int available() {
      synchronized {
        return _numElems;
      }
    }
    /* ---------------------------------------------------------------------- */
    int freePositions() {
      synchronized {
        return (size - _numElems);
      }
    }
    /* ---------------------------------------------------------------------- */
    T peek() {
      if (isEmpty())
        return -1;

      return _aucBuffer[_iTail];
    }
    /* ---------------------------------------------------------------------- */
    bool isFull() {
      synchronized {
        return (_numElems == size);
      }
    }
    /* ---------------------------------------------------------------------- */
    uint32_t lenght()  const { return size; }

  
};


} //namespace arduino

#endif /* _ARDUINO_FIFO_BUFFER_DH_ */
#endif /* __cplusplus */