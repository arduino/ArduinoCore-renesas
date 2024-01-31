/*
  SoftwareATSE.h
  Copyright (c) 2023 Arduino SA.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SOFTWARE_ATSE_H_
#define _SOFTWARE_ATSE_H_

#include <Arduino.h>


class SoftwareATSEClass
{
public:
  SoftwareATSEClass();
  virtual ~SoftwareATSEClass();

  int begin(const char * name = "se", bool readOnly = false, const char* partition_label = NULL);
  void end();

  String serialNumber();

  long random(long min, long max);
  long random(long max);
  int random(byte data[], size_t length);

  int generatePrivateKey(int slot, byte publicKey[]);
  int generatePublicKey(int slot, byte publicKey[]);

  int ecdsaVerify(const byte message[], const byte signature[], const byte pubkey[]);
  int ecSign(int slot, const byte message[], byte signature[]);

  int SHA256(const uint8_t *buffer, size_t size, uint8_t *digest);

  int readSlot(int slot, byte data[], int length);
  int writeSlot(int slot, const byte data[], int length);

  inline int locked() { return 1; }
  inline int lock() { return 1; }

  inline int writeConfiguration(const byte config[] = nullptr) { (void)config; return 1; }


private:

};

extern SoftwareATSEClass SATSE;

#endif
