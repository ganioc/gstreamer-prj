// h264_sei_ntp.h
#ifndef H264_SEI_NTP
#define H264_SEI_NTP

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// #define H264_SEI_UUID_NTP_TIMESTAMP {0x60, 0x2b, 0x0d, 0xb6, 0x2d, 0x3d,
// 0x44, 0xb5, 0xab, 0x9e, 0xec, 0x8a, 0xd7, 0x1f, 0x3f, 0x8e} test
// emulation_prevention_three_byte
#define H264_SEI_UUID_NTP_TIMESTAMP                                            \
  {                                                                            \
    0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x9e, 0xec, 0x8a,    \
        0xd7, 0x1f, 0x3f, 0x8e                                                 \
  }
#define START_CODE_PREFIX_BYTES 4
#define START_CODE_PREFIX                                                      \
  { 0x00, 0x00, 0x00, 0x01 }

uint64_t now_ms();

bool h264_sei_ntp_new(uint8_t **h264_sei, size_t *length);

bool h264_sei_ntp_parse(uint8_t *h264_sei, size_t length, int64_t *delay);

#endif
