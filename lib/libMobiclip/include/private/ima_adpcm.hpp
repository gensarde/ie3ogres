#ifndef NITRO_LIBMOBICLIP_IMA_ADPCM_H
#define NITRO_LIBMOBICLIP_IMA_ADPCM_H

#pragma once

#include <nitro.h>

#include "private/util.hpp"

void ImaAdpcmSetContext(__AudioUnpack * audioUnpack, u8 *data);
void ImaAdpcmDecode(__AudioUnpack * audioUnpack, u8 *data, u32 size, s16 *outSoundBuffer);

#endif //NITRO_LIBMOBICLIP_IMA_ADPCM_H
