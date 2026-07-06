#ifndef NITRO_LIBMOBICLIP_HANDLE_H
#define NITRO_LIBMOBICLIP_HANDLE_H

#pragma once

#include <nitro.h>
#include <Mobiclip.h>

#include "private/stream.hpp"
#include "private/relocator.hpp"
#include "private/util.hpp"
#include "private/ima_adpcm.hpp"

extern const u32 __Vx2Table0[2112]; // .rodata
extern const u32 __Vx2Table1[2112]; // .rodata

class MOHandlePrivate
{
public:
    void *operator new(size_t size);
    void operator delete(void *ptr);

    MOHandlePrivate();
    ~MOHandlePrivate();
    u32 Init(MOStreamFile *stream, u32 nbBufferedFrameMax);
    void Free(void);

    u32 GetSoundFrequency(void);
    u32 GetNbSoundTrack(void);
    u32 GetFps(void);
    u32 GetVideoWidth(void);
    u32 GetVideoHeight(void);
    u32 GetNbFrame(void);
    u32 GetNbIFrame(void);
    u32 GetNbSamplePerAudioPacket(void);
    u32 GetCurrentFrameNumber(void);
    u32 GetIFrameInfo(u32 index);
    u32 NextFrame(void);
    u32 JumpBeginning(void);
    u32 JumpNextIframe(void);
    u32 JumpPreviousIframe(void);
    u32 JumpToIframe(u32 index);
    u32 FrameUnpackVideo(void);
    u32 BlitVideo(u16* outFrameBufferPtr, u32 outFrameBufferPitch, u32 qualityEnhancement);
    u32 SkipVideo(void);
    u32 GetFrameNbAudioPacket(void);
    u32 FrameUnpackAudioPacket(s16* outSoundBuffer);
    u32 FrameSkipAudioPacket(void);
    
private:
    MOStream *VideoStream;
    MOHeader VideoHeader;
    __Vx2Unpack *VideoUnpackData;
    u32 (*VideoUnpackFunc)(__Vx2Unpack *);
    YuvToArgbData ColorConversionData;
    __AudioUnpack *AudioUnpackDataArray;
    u8 **Y;
    u8 **UV;
    u32 *LastQuantizer;
    u8 *EnhancedY;
    u8 *EnhancedUV;
    u8 *ReadBuffer[2];
    u32 ReadBufferSize[2];
    u32 ReadBufferOffset[2];
    u32 ReadBufferNbAudioPacket[2];
    int CurrentReadBuffer;
    MOKeyframe *KeyframeArray;
    u32 CurrentFrameNumber;
    u32 CurrentBlit;
    u32 CurrentDst;
    u32 CurrentVideoFrameIsKeyFrame;
    u32 NbVideoBuffer;
    u32 VideoBufferIndex[6];
    u32 VideoBufferBlitIndex;
    u32 TotalAudioPacketToUnpack;
    u32 CurrentNbAudioPacketUnpacked;
    u32 CurrentChannelIndex;
    u32 PositionBeginning;
};

#ifdef __cplusplus
extern "C" {
#endif

extern void YuvToArgb(YuvToArgbData *);
extern u32 __Vx2Codec(__Vx2Unpack *); // .rodata
extern u8 __FaCodec(__AudioUnpack *); // .rodata
extern u8 __SxCodec(__AudioUnpack *); // .rodata
extern void __Vx2Deblock(void *); // .rodata

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //NITRO_LIBMOBICLIP_HANDLE_H
