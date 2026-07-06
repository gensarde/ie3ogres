#ifndef NITRO_LIBMOBICLIP_UTIL_H
#define NITRO_LIBMOBICLIP_UTIL_H

#pragma once

#include <nitro.h>

typedef struct {
    u16 *Src;
    s16 *Dst;
    union {
        u8 Internal[2232];
        u32 unk[2232 / 4];
    };
    u8 Codebook[3124];
    u32 Increment;
} __SxUnpack;

typedef struct {
    __SxUnpack SxUnpack;
} __AudioUnpack;

typedef struct {
    u8 *Data;
    u32 Width;
    u32 Height;
    u8 *Y[6];
    u8 *UV[6];
    const u32 *Table0;
    const u32 *Table1;
    const u8 *TableClip;
    u32 YuvFormat;
    u8 IntraMode[40];
    u32 QuantZigZag8x8[64];
    u32 QuantZigZag4x4[16];
    u32 Internal[128];
    u32 LastQuantizer;
    u32 VlcAddress;
    u32 MvPred[2];
    u32 Mv[36];
} __Vx2Unpack;

typedef struct {
    u8 *Y;
    u8 *UV;
    u16 *Dst;
    int ScreenPitch;
    int VideoWidth;
    int VideoHeight;
    const u8 *Vx2BlitTable;
} YuvToArgbData;

typedef struct {
    u32 Index;
    u32 Offset;
} MOKeyframe;

typedef struct {
    char ModsString[4];
    char TagId[2];
    u16 TagIdSizeDword;
    u32 FrameCount;
    u32 Width;
    u32 Height;
    u32 Fps;
    u16 AudioCodec;
    u16 NbChannel;
    u32 Frequency;
    u32 BiggestFrame;
    u32 AudioOffset;
    u32 KeyframeIndexOffset;
    u32 KeyframeCount;
} MOHeader;

#endif //NITRO_LIBMOBICLIP_UTIL_H
