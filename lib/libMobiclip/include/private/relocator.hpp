#ifndef NITRO_LIBMOBICLIP_RELOCATOR_H
#define NITRO_LIBMOBICLIP_RELOCATOR_H

#pragma once

#include <nitro.h>
#include <Mobiclip.h>

#include "private/handle.hpp"
#include "private/util.hpp"

#define MO_OPTIMAL_ITCM_SIZE 0x659C
#define MO_OPTIMAL_DTCM_SIZE 0x2580

extern const u8 __Vx2MinMaxTable[384];
extern const u8 __Vx2BlitTable[778];

u32 (*MOPrivate_VideoCodeRelocate(void))(__Vx2Unpack *);
const u32 *MOPrivate_VideoDataRelocate(void);
const u8 *MOPrivate_BlitDataRelocate(void);
const u8 *MOPrivate_MinMaxDataRelocate(void);

#endif //NITRO_LIBMOBICLIP_RELOCATOR_H
