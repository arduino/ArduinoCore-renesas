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

  void debug(Stream  &u, uint8_t level = 0);

  String serialNumber();

  long random(long min, long max);
  long random(long max);
  int random(byte data[], size_t length);

  /** generatePrivateKey
   *
   * Create a new ECCurve_NIST_P256 keypair and stores it in the WiFi chip NVS.
   * Public key X Y values will be available inside publicKey buffer that should
   * be at least 64 bytes long.
   *
   * | Public key X Y values (64 bytes) |
   *
   * @param[in] slot objectID where to store the private key
   * @param[out] publicKey Buffer containing the public key X Y values
   *
   * @return 0 on Failure 1 on Success
   */
  int generatePrivateKey(int slot, byte publicKey[]);

  /** generatePublicKey
   *
   * Reads ECCurve_NIST_P256 public key from KeyID. Public key X Y values will be available
   * inside publicKey buffer that should be at least 64 bytes long.
   *
   * | Public key X Y values (64 bytes) |
   *
   * @param[in] slot objectID where is stored the keypair
   * @param[out] pubkey Buffer containing the public key X Y values
   *
   * @return 0 on Failure 1 on Success
   */
  int generatePublicKey(int slot, byte publicKey[]);

  /** ecdsaVerify
   *
   * Verify ECDSA signature using public key.
   *
   *                               Input SHA256
   *                                             ? Match ?
   * Signature -> public Key -> Original SHA256
   *
   * @param[in] message Input SHA256 used to compute the signature 32 bytes
   * @param[in] sig Input buffer containint the signature R S values 64bytes
   * @param[in] pubkey Public key X Y values 64bytes
   *
   * @return 0 on Failure (Not match) 1 on Success (Match)
   */
  int ecdsaVerify(const byte message[], const byte signature[], const byte pubkey[]);

  /** ecSign
   *
   * Computes ECDSA signature using key stored in KeyID SE050 object.
   * Output signature buffer is filled with the signature R S values
   * and should be at least 64 bytes long:
   *
   * | R values 32 bytes | S values 32 bytes |
   *
   * SHA256 -> private Key -> Signature
   *
   * @param[in] slot object ID containing the key
   * @param[in] message Input SHA256 used to compute the signature 32 bytes
   * @param[out] signature Output buffer containint the signature 64 bytes
   *
   * @return 0 on Failure 1 on Success
   */
  int ecSign(int slot, const byte message[], byte signature[]);

  /** SHA256
   *
   * One-shot SHA256
   *
   * @param[in] buffer Input data buffer
   * @param[in] size Input data length
   * @param[out] digest Output buffer should be at least 32 bytes long
   *
   * @return 0 on Failure 1 on Success
   */
  int SHA256(const uint8_t *buffer, size_t size, uint8_t *digest);

  /** readSlot
   *
   * Reads binary data from Software AT Secure Element object.
   *
   * @param[in] slot object ID containing data
   * @param[out] data Output data buffer
   * @param[in] length Number of bytes to read
   *
   * @return 0 on Failure 1 on Success
   */
  int readSlot(int slot, byte data[], int length);

  /** writeSlot
   *
   * Writes binary data into Software AT Secure Element object.
   *
   * @param[in] slot object ID
   * @param[in] data Input data buffer
   * @param[in] length Number of bytes to write
   *
   * @return 0 on Failure 1 on Success
   */
  int writeSlot(int slot, const byte data[], int length);

  inline int locked() { return 1; }
  inline int lock() { return 1; }

  inline int writeConfiguration(const byte config[] = nullptr) { (void)config; return 1; }


private:

};

extern SoftwareATSEClass SATSE;

#endif
