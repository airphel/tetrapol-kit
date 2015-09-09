#pragma once

#include <tetrapol/frame.h>
#include <stdbool.h>
#include <stdint.h>

typedef union {
    struct {
        unsigned int y : 1;
        unsigned int x : 1;
    };
    unsigned int xy : 2;
} asb_t;

// now only data frame, in future might comprise different types of frame
typedef struct {
    frame_type_t fr_type;
    int nerrs;      ///< nonzero value indicate uncorrected errors in block
    // 74 bytes is required for data frame, but 2 extra bits are reqired
    // because they are used by decoding algorithm
    // 126 bits for voice frame
    // TODO: 152 bits for high rate data frames? (or use BCH and reduce it to 96)
    // TODO: 152 bits for RACH frames?
    // TODO: 152 bits for training frame?
    // TODO: 152 bits for SCH/TI frame?
    uint8_t data[126];
    union {
        uint8_t err[74];
        uint8_t _tmpe[76];  // extra space, data frame have 2 stuffing bits
    };
} data_block_t;

bool data_block_check_crc(data_block_t *data_blk);

/**
  Decode frame, decodes only firts part of frame, common to data and voice
  frames.

  @param data_blk pointer to struct data_block_t for storing result.
  @param data Should contains frame data (withought synchronization block).
  @param fr_type Expected frame type.
  */
void data_block_decode_frame1(data_block_t *data_blk, const uint8_t *data,
        frame_type_t fr_type);

/**
  Decode remaining part of frame.

  @param data_blk Block initialized by data_block_decode_frame1.
  @param data Input, frame data.
  */
void data_block_decode_frame2(data_block_t *data_blk, const uint8_t *data);

asb_t data_block_get_asb(data_block_t *data_blk);

