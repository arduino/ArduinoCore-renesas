/* 
  lzss.cpp
  Original code from LZSS encoder-decoder (Haruhiko Okumura; public domain)

  https://oku.edu.mie-u.ac.jp/~okumura/compression/lzss.c
*/

/**************************************************************************************
   INCLUDE
 **************************************************************************************/

#include "lzss.h"

/**************************************************************************************
   DEFINE
 **************************************************************************************/

#define EI 11             /* typically 10..13 */
#define EJ  4             /* typically 4..5 */
#define P   1             /* If match length <= P then output one character */
#define N (1 << EI)       /* buffer size */
#define F ((1 << EJ) + 1) /* lookahead buffer size */

#define LZSS_EOF       (-1)

#define FPUTC_BUF_SIZE (64)
#define FGETC_BUF_SIZE (64)

/**************************************************************************************
   GLOBAL VARIABLES
 **************************************************************************************/

static uint32_t LZSS_FILE_SIZE = 0;
static FILE * update_file = 0;
static FILE * target_file = 0;

int bit_buffer = 0, bit_mask = 128;
unsigned char buffer[N * 2];

static char write_buf[FPUTC_BUF_SIZE];
static size_t write_buf_num_bytes = 0;
static size_t bytes_written_fputc = 0;
static SFUWatchdogResetFuncPointer wdog_feed_func = 0;

/**************************************************************************************
   PUBLIC FUNCTIONS
 **************************************************************************************/

void lzss_init(FILE * update_file_ptr, FILE * target_file_ptr, uint32_t const lzss_file_size, SFUWatchdogResetFuncPointer wdog_feed_func_ptr)
{
  update_file = update_file_ptr;
  target_file = target_file_ptr;
  LZSS_FILE_SIZE = lzss_file_size;
  wdog_feed_func = wdog_feed_func_ptr;
}

void lzss_flush()
{
  bytes_written_fputc += write_buf_num_bytes;

  if (wdog_feed_func)
    wdog_feed_func();

  fwrite(write_buf, 1, write_buf_num_bytes, target_file);

  write_buf_num_bytes = 0;
}

/**************************************************************************************
   PRIVATE FUNCTIONS
 **************************************************************************************/

void lzss_fputc(int const c)
{
  /* Buffer the decompressed data into a buffer so
   * we can perform block writes and don't need to
   * write every byte singly on the flash (which 
   * wouldn't be possible anyway).
   */
  write_buf[write_buf_num_bytes] = static_cast<char>(c);
  write_buf_num_bytes++;

  /* The write buffer is full of decompressed
   * data, write it to the flash now.
   */
  if (write_buf_num_bytes == FPUTC_BUF_SIZE)
    lzss_flush();
}

int lzss_fgetc()
{
  static uint8_t read_buf[FGETC_BUF_SIZE];
  static size_t read_buf_pos = FGETC_BUF_SIZE;
  static size_t bytes_read_fgetc = 0;
  static size_t bytes_read_from_modem = 0;

  /* lzss_file_size is set within SSUBoot:main 
   * and contains the size of the LZSS file. Once
   * all those bytes have been read its time to return
   * LZSS_EOF in order to signal that the end of
   * the file has been reached.
   */
  if (bytes_read_fgetc == LZSS_FILE_SIZE)
    return LZSS_EOF;

  /* If there is no data left to be read from the read buffer
   * than read a new block and store it into the read buffer.
   */
  if (read_buf_pos == FGETC_BUF_SIZE)
  {
    /* Read the next block from the flash memory. */
    bytes_read_from_modem += fread(read_buf, 1, FGETC_BUF_SIZE, update_file);
    /* Reset the read buffer position. */
    read_buf_pos = 0;
  }

  uint8_t const c = read_buf[read_buf_pos];
  read_buf_pos++;
  bytes_read_fgetc++;

  return c;
}

/**************************************************************************************
   LZSS FUNCTIONS
 **************************************************************************************/

void putbit1(void)
{
  bit_buffer |= bit_mask;
  if ((bit_mask >>= 1) == 0) {
    lzss_fputc(bit_buffer);
    bit_buffer = 0;  bit_mask = 128;
  }
}

void putbit0(void)
{
  if ((bit_mask >>= 1) == 0) {
    lzss_fputc(bit_buffer);
    bit_buffer = 0;  bit_mask = 128;
  }
}

void output1(int c)
{
  int mask;

  putbit1();
  mask = 256;
  while (mask >>= 1) {
    if (c & mask) putbit1();
    else putbit0();
  }
}

void output2(int x, int y)
{
  int mask;

  putbit0();
  mask = N;
  while (mask >>= 1) {
    if (x & mask) putbit1();
    else putbit0();
  }
  mask = (1 << EJ);
  while (mask >>= 1) {
    if (y & mask) putbit1();
    else putbit0();
  }
}

int getbit(int n) /* get n bits */
{
  int i, x;
  static int buf, mask = 0;

  x = 0;
  for (i = 0; i < n; i++) {
    if (mask == 0) {
      if ((buf = lzss_fgetc()) == LZSS_EOF) return LZSS_EOF;
      mask = 128;
    }
    x <<= 1;
    if (buf & mask) x++;
    mask >>= 1;
  }
  return x;
}

void lzss_decode(void)
{
  int i, j, k, r, c;

  for (i = 0; i < N - F; i++) buffer[i] = ' ';
  r = N - F;
  while ((c = getbit(1)) != LZSS_EOF) {
    if (c) {
      if ((c = getbit(8)) == LZSS_EOF) break;
      lzss_fputc(c);
      buffer[r++] = c;  r &= (N - 1);
    } else {
      if ((i = getbit(EI)) == LZSS_EOF) break;
      if ((j = getbit(EJ)) == LZSS_EOF) break;
      for (k = 0; k <= j + 1; k++) {
        c = buffer[(i + k) & (N - 1)];
        lzss_fputc(c);
        buffer[r++] = c;  r &= (N - 1);
      }
    }
  }
}
