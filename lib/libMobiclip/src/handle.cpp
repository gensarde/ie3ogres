#include "private/handle.hpp"

void *MOHandlePrivate::operator new(size_t size)
{
    return MO_Malloc(size);
}

void MOHandlePrivate::operator delete(void *ptr)
{
    MO_Free(ptr);
}

MOHandlePrivate::MOHandlePrivate()
{
    this->VideoStream = NULL;
    this->Y = NULL;
    this->UV = NULL;
    this->EnhancedY = NULL;
    this->EnhancedUV = NULL;
    this->ReadBuffer[0] = NULL;
    this->ReadBuffer[1] = NULL;
    this->LastQuantizer = NULL;
    this->AudioUnpackDataArray = NULL;
    this->KeyframeArray = NULL;
}

MOHandlePrivate::~MOHandlePrivate()
{
    this->Free();
}

u32 MOHandlePrivate::Init(MOStreamFile *stream, u32 nbBufferedFrameMax)
{
    this->VideoStream = stream;
    this->PositionBeginning = stream->Position;

    if (!this->VideoStream->Read(&this->VideoHeader, 0x30)) {
        return FALSE;
    }

    if ((this->VideoHeader.ModsString[0] != 'M') ||
        (this->VideoHeader.ModsString[1] != 'O') ||
        (this->VideoHeader.ModsString[2] != 'D') ||
        (this->VideoHeader.ModsString[3] != 'S') ||
        (this->VideoHeader.TagId[0] != 'N') ||
        ((this->VideoHeader.TagId[1] != '2') && (this->VideoHeader.TagId[1] != '3')) ||
        (this->VideoHeader.TagIdSizeDword != 10)
    ) {
        return FALSE;
    }

    if ((this->VideoHeader.TagId[0] == 'N') && (this->VideoHeader.TagId[1] == '3')) {
        char stack2[2];
        u16 stack0;

        do {
            if (!this->VideoStream->Read(&stack2, sizeof(stack2))) {
                return FALSE;
            }
            if (!this->VideoStream->Read(&stack0, sizeof(stack0))) {
                return FALSE;
            }
            u32 tmpPos = stack0 * 4;
            if (!this->VideoStream->SetPosition(tmpPos + this->VideoStream->Position)) {
                return FALSE;
            }
        } while ((stack2[0] != 'H') || (stack2[1] != 'E'));
    }

    if ((this->VideoHeader.AudioCodec != 0) &&
        (this->VideoHeader.AudioCodec != 1) &&
        (this->VideoHeader.AudioCodec != 2) &&
        (this->VideoHeader.AudioCodec != 3) &&
        (this->VideoHeader.AudioCodec != 4)
    ) {
        return FALSE;
    }

    this->VideoUnpackData = static_cast<__Vx2Unpack *>(MO_Malloc(sizeof(*this->VideoUnpackData)));
    MI_CpuClearFast(this->VideoUnpackData, sizeof(*this->VideoUnpackData));
    this->VideoUnpackFunc = MOPrivate_VideoCodeRelocate();
    this->VideoUnpackData->Table0 = MOPrivate_VideoDataRelocate();
    this->VideoUnpackData->Table1 = __Vx2Table1;
    this->VideoUnpackData->TableClip = MOPrivate_MinMaxDataRelocate() + 0x40;
    
    this->ColorConversionData.VideoWidth = this->VideoHeader.Width;
    this->ColorConversionData.VideoHeight = this->VideoHeader.Height;
    this->ColorConversionData.Vx2BlitTable = MOPrivate_BlitDataRelocate();

    this->VideoUnpackData->Width = this->VideoHeader.Width;
    this->VideoUnpackData->Height = this->VideoHeader.Height;

    if (this->VideoHeader.Width > 256) {
        return FALSE;
    }

    if (nbBufferedFrameMax < MO_NB_BUFFERED_FRAME_MIN) {
        return FALSE;
    }
    this->NbVideoBuffer = nbBufferedFrameMax;

    this->Y = static_cast<u8 **>(MO_Malloc(nbBufferedFrameMax * 4));
    if (!this->Y) return FALSE;

    this->UV = static_cast<u8 **>(MO_Malloc(this->NbVideoBuffer * 4));
    if (!this->UV) return FALSE;

    for (u32 i = 0; i < this->NbVideoBuffer; i++) {
        this->Y[i] = static_cast<u8 *>(MO_Malloc(this->VideoHeader.Height * 256));
        if (!this->Y[i]) return FALSE;
        
        this->UV[i] = static_cast<u8 *>(MO_Malloc((this->VideoHeader.Height / 2) * 256));
        if (!this->UV[i]) return FALSE;

        MI_CpuClearFast(this->Y[i], this->VideoHeader.Height * 256);
        MI_CpuFill8(this->UV[i], 0x80, (this->VideoHeader.Height / 2) * 256);
    }

    this->LastQuantizer = static_cast<u32 *>(MO_Malloc(this->NbVideoBuffer * 4));
    if (!this->LastQuantizer) return FALSE;

    this->CurrentFrameNumber = 0;
    this->VideoBufferIndex[0] = 0;

    for (int i = 1; i < MO_NB_BUFFERED_FRAME_MIN; i++) {
        this->VideoBufferIndex[i] = this->NbVideoBuffer - i;
    }

    this->VideoBufferBlitIndex = 0;
    this->CurrentDst = 0;
    this->CurrentBlit = 0;

    for (int i = 0; i < 2; i++) {
        this->ReadBuffer[i] = static_cast<u8 *>(MO_Malloc(this->VideoHeader.BiggestFrame + 0x400));
        if (!this->ReadBuffer[i]) return FALSE;
    }

    if (this->VideoHeader.AudioCodec != 0) {
        this->AudioUnpackDataArray = static_cast<__AudioUnpack *>(MO_Malloc(this->VideoHeader.NbChannel * sizeof(*this->AudioUnpackDataArray)));
        if (!this->AudioUnpackDataArray) return FALSE;
        MI_CpuClearFast(this->AudioUnpackDataArray, this->VideoHeader.NbChannel * sizeof(*this->AudioUnpackDataArray));
    }

    u32 keyfrmPos = this->VideoStream->Position;

    this->VideoStream->SetPosition(this->VideoHeader.KeyframeIndexOffset + this->PositionBeginning);
    
    this->KeyframeArray = static_cast<MOKeyframe *>(MO_Malloc(this->VideoHeader.KeyframeCount * sizeof(*this->KeyframeArray)));
    if (!this->VideoStream->Read(this->KeyframeArray, this->VideoHeader.KeyframeCount * sizeof(*this->KeyframeArray))) {
        return FALSE;
    }

    this->VideoStream->SetPosition(keyfrmPos);

    if (this->VideoHeader.AudioCodec == 1)
    {
        int i;
        u32 audioPos = this->VideoStream->Position;
        
        this->VideoStream->SetPosition(this->VideoHeader.AudioOffset + this->PositionBeginning);

        for (i = 0; i < this->VideoHeader.NbChannel; i++) {
            u32 result = this->VideoStream->Read(
                this->AudioUnpackDataArray[i].SxUnpack.Codebook,
                sizeof(this->AudioUnpackDataArray[i].SxUnpack.Codebook)
            );
            if (!result) {
                return FALSE;
            }

            this->AudioUnpackDataArray[i].SxUnpack.Increment = 2;
        }

        this->VideoStream->SetPosition(audioPos);
    }
    else if (this->VideoHeader.AudioCodec == 2)
    {
        for (int i = 0; i < this->VideoHeader.NbChannel; i++) {
            //*(u32 *)((int)&this->AudioUnpackDataArray[i].SxUnpack + 0x1cc) = 2;
            this->AudioUnpackDataArray[i].SxUnpack.unk[113] = 2;
        }
    }

    u32 buffer;
    this->VideoStream->Read(&buffer, sizeof(buffer));
    this->ReadBufferSize[0] = buffer >> 14;
    this->ReadBufferNbAudioPacket[0] = buffer & 0x3fff;
    this->ReadBufferOffset[0] = 0;
    this->VideoStream->Read(this->ReadBuffer[0], this->ReadBufferSize[0] + 4);
    this->CurrentReadBuffer = 1;

    return TRUE;
}

void MOHandlePrivate::Free(void)
{
    if (this->VideoStream) {
        this->VideoStream->Close();
        delete this->VideoStream;
    }

    MO_Free(this->VideoUnpackData);

    if (this->Y) {
        for (int i = 0; i < this->NbVideoBuffer; i++) {
            MO_Free(this->Y[i]);
        }
        MO_Free(this->Y);
    }
    
    if (this->UV) {
        for (int i = 0; i < this->NbVideoBuffer; i++) {
            MO_Free(this->UV[i]);
        }
        MO_Free(this->UV);
    }
    
    if (this->LastQuantizer) {
        MO_Free(this->LastQuantizer);
    }
    
    if (this->EnhancedY) {
        MO_Free(this->EnhancedY);
    }
    
    if (this->EnhancedUV) {
        MO_Free(this->EnhancedUV);
    }

    for (int i = 0; i < 2; i++) {
        if (this->ReadBuffer[i]) {
            MO_Free(this->ReadBuffer[i]);
        }
        this->ReadBuffer[i] = NULL;
    }

    if (this->KeyframeArray) {
        MO_Free(this->KeyframeArray);
    }
    
    if (this->AudioUnpackDataArray) {
        MO_Free(this->AudioUnpackDataArray);
    }
    
    this->VideoStream = NULL;
    this->Y = NULL;
    this->UV = NULL;
    this->EnhancedY = NULL;
    this->EnhancedUV = NULL;
    this->LastQuantizer = NULL;
    this->AudioUnpackDataArray = NULL;
    this->KeyframeArray = NULL;
}

u32 MOHandlePrivate::GetSoundFrequency(void)
{
    return this->VideoHeader.Frequency;
}

u32 MOHandlePrivate::GetNbSoundTrack(void)
{
    return this->VideoHeader.NbChannel;
}

u32 MOHandlePrivate::GetFps(void)
{
    return this->VideoHeader.Fps;
}

u32 MOHandlePrivate::GetVideoWidth(void)
{
    return this->VideoHeader.Width;
}

u32 MOHandlePrivate::GetVideoHeight(void)
{
    return this->VideoHeader.Height;
}

u32 MOHandlePrivate::GetNbFrame(void)
{
    return this->VideoHeader.FrameCount;
}

u32 MOHandlePrivate::GetNbIFrame(void)
{
    return this->VideoHeader.KeyframeCount;
}

u32 MOHandlePrivate::GetCurrentFrameNumber(void)
{
    return this->CurrentFrameNumber;
}

u32 MOHandlePrivate::GetIFrameInfo(u32 index)
{
    if (((int)index < 0) || (this->VideoHeader.KeyframeCount >= index)) {
        return 0;
    }
    return this->KeyframeArray[index].Index;
}

u32 MOHandlePrivate::GetNbSamplePerAudioPacket(void)
{
    if (this->VideoHeader.Frequency == 0) {
        return 0;
    }
    if ((this->VideoHeader).AudioOffset != 0) {
        return 0x100;
    } else {
        return 0x80;
    }
}

u32 MOHandlePrivate::NextFrame(void)
{
    if (this->CurrentFrameNumber == this->VideoHeader.FrameCount) {
        return FALSE;
    }

    this->CurrentFrameNumber++;
    int curReadBuffer = this->CurrentReadBuffer;
    this->CurrentReadBuffer = (this->CurrentReadBuffer + 1) % 2;

    this->VideoStream->WaitAsync();
    
    u32 idx = this->ReadBufferOffset[this->CurrentReadBuffer] + this->ReadBufferSize[this->CurrentReadBuffer];
    u32 buffer = *reinterpret_cast<u32 *>(&this->ReadBuffer[this->CurrentReadBuffer][idx]);
    this->ReadBufferSize[curReadBuffer] = buffer >> 14;
    this->ReadBufferNbAudioPacket[curReadBuffer] = buffer & 0x3fff;
    this->VideoUnpackData->Data = &this->ReadBuffer[this->CurrentReadBuffer][this->ReadBufferOffset[this->CurrentReadBuffer]];

    if (this->CurrentFrameNumber < this->VideoHeader.FrameCount) {
        this->ReadBufferOffset[curReadBuffer] = this->VideoStream->ReadAsync(
            this->ReadBuffer[curReadBuffer], this->ReadBufferSize[curReadBuffer] + 4
        );
    }

    for (int i = 0; i < MO_NB_BUFFERED_FRAME_MIN; i++) {
        this->VideoUnpackData->Y[i] = this->Y[this->VideoBufferIndex[i]];
        this->VideoUnpackData->UV[i] = this->UV[this->VideoBufferIndex[i]];
    }

    this->TotalAudioPacketToUnpack = this->VideoHeader.NbChannel * this->ReadBufferNbAudioPacket[this->CurrentReadBuffer];
    
    return TRUE;
}

u32 MOHandlePrivate::JumpToIframe(u32 index)
{
    if (((int)index < 0) || (index >= this->VideoHeader.KeyframeCount)) {
        return FALSE;
    }
    if (!this->VideoStream->SetPosition(this->PositionBeginning + this->KeyframeArray[index].Offset)) {
        return FALSE;
    }

    this->CurrentFrameNumber = this->KeyframeArray[index].Index;
    this->VideoStream->WaitAsync();
    this->CurrentReadBuffer = 1;

    u32 buffer;
    this->VideoStream->Read(&buffer, sizeof(buffer));
    this->ReadBufferSize[0] = buffer >> 14;
    this->ReadBufferNbAudioPacket[0] = buffer & 0x3fff;
    this->ReadBufferOffset[0] = 0;
    this->VideoStream->Read(this->ReadBuffer[0], this->ReadBufferSize[0] + 4);

    return TRUE;
}

u32 MOHandlePrivate::JumpBeginning(void)
{
    return this->JumpToIframe(0);
}

u32 MOHandlePrivate::JumpNextIframe(void)
{
    int i;
    
    for (i = 0; i < this->VideoHeader.KeyframeCount; i++) {
        if (this->CurrentFrameNumber <= this->KeyframeArray[i].Index) {
            break;
        }
    }

    if (i >= this->VideoHeader.KeyframeCount - 1) {
        return FALSE;
    }

    return this->JumpToIframe(i + 1);
}

u32 MOHandlePrivate::JumpPreviousIframe(void)
{
    int i;

    for (i = 0; i < this->VideoHeader.KeyframeCount; i++) {
        if (this->CurrentFrameNumber <= this->KeyframeArray[i].Index) {
            break;
        }
    }

    if (i >= 2) {
        i = i - 2;
    } else {
        i = 0;
    }

    while ((i > 0) && (this->KeyframeArray[i].Index + 1 == this->KeyframeArray[i + 1].Index)) {
        i--;
    }

    return this->JumpToIframe(i);
}

u32 MOHandlePrivate::FrameUnpackVideo(void)
{
    if (this->CurrentBlit > this->CurrentDst) {
        return FALSE;
    }

    u32 size;
    if (*reinterpret_cast<u16 *>(this->VideoUnpackData->Data) & 0x8000) {
        this->CurrentVideoFrameIsKeyFrame = TRUE;
        size = 4;
    } else {
        this->CurrentVideoFrameIsKeyFrame = FALSE;
        size = 0;
    }
    
    this->VideoUnpackData->Data += this->VideoUnpackFunc(this->VideoUnpackData);

    if ((this->VideoHeader.TagId[0] == 'N') && (this->VideoHeader.TagId[1] == '3')) {
        this->VideoUnpackData->Data += size;
    }

    this->LastQuantizer[this->VideoBufferIndex[0]] = this->VideoUnpackData->LastQuantizer;
    this->CurrentNbAudioPacketUnpacked = 0;
    this->CurrentChannelIndex = 0;

    for (int i = 0; i < MO_NB_BUFFERED_FRAME_MIN; i++) {
        this->VideoBufferIndex[i]++;
        
        if (this->NbVideoBuffer == this->VideoBufferIndex[i]) {
            this->VideoBufferIndex[i] = 0;
        }
    }

    this->CurrentDst++;

    return TRUE;
}

u32 MOHandlePrivate::BlitVideo(u16* outFrameBufferPtr, u32 outFrameBufferPitch, u32 qualityEnhancement)
{
    typedef struct {
        u8 *Y;
        u8 *UV;
        u8 *EnhancedY;
        u8 *EnhancedUV;
        u32 Width;
        u32 Height;
        u32 unk18;
        const u8 *unk1C;
        u32 isQualityEnhancementHigh;
    } Struct_Vx2Deblock;
    
    Struct_Vx2Deblock stack;
    
    if (this->CurrentBlit >= this->CurrentDst) {
        return FALSE;
    }

    if (qualityEnhancement != MO_QUALITY_ENHANCEMENT_NONE)
    {
        if (this->EnhancedY == NULL) {
            this->EnhancedY = static_cast<u8 *>(MO_Malloc(this->VideoHeader.Height * 256));
            if (!this->EnhancedY) return FALSE;
        }
        if (this->EnhancedUV == NULL) {
            this->EnhancedUV = static_cast<u8 *>(MO_Malloc((this->VideoHeader.Height / 2) * 256));
            if (!this->EnhancedUV) return FALSE;
        }

        stack.Y = this->Y[this->VideoBufferBlitIndex];
        stack.UV = this->UV[this->VideoBufferBlitIndex];
        stack.EnhancedY = this->EnhancedY;
        stack.EnhancedUV = this->EnhancedUV;
        stack.Width = this->VideoHeader.Width;
        stack.Height = this->VideoHeader.Height;
        stack.unk18 = this->LastQuantizer[this->VideoBufferBlitIndex];
        stack.unk1C = MOPrivate_MinMaxDataRelocate();
        if (qualityEnhancement == MO_QUALITY_ENHANCEMENT_HIGH) {
            stack.isQualityEnhancementHigh = FALSE;
        } else {
            stack.isQualityEnhancementHigh = TRUE;
        }
        
        __Vx2Deblock(&stack);

        this->ColorConversionData.Y = this->EnhancedY;
        this->ColorConversionData.UV = this->EnhancedUV;
    }
    else
    {
        this->ColorConversionData.Y = this->Y[this->VideoBufferBlitIndex];
        this->ColorConversionData.UV = this->UV[this->VideoBufferBlitIndex];
    }

    this->ColorConversionData.Dst = outFrameBufferPtr;
    this->ColorConversionData.ScreenPitch = outFrameBufferPitch * 2;
    YuvToArgb(&this->ColorConversionData);

    this->CurrentBlit++;
    this->VideoBufferBlitIndex++;
    if (this->VideoBufferBlitIndex == this->NbVideoBuffer) {
        this->VideoBufferBlitIndex = 0;
    }

    return TRUE;
}

u32 MOHandlePrivate::SkipVideo(void)
{
    if (this->CurrentBlit >= this->CurrentDst) {
        return FALSE;
    }

    this->CurrentBlit++;
    this->VideoBufferBlitIndex++;
    if (this->VideoBufferBlitIndex == this->NbVideoBuffer) {
      this->VideoBufferBlitIndex = 0;
    }
    
    return TRUE;
}

u32 MOHandlePrivate::GetFrameNbAudioPacket(void)
{
    return this->ReadBufferNbAudioPacket[this->CurrentReadBuffer];
}

u32 MOHandlePrivate::FrameUnpackAudioPacket(s16* outSoundBuffer)
{
    if (this->CurrentNbAudioPacketUnpacked == this->TotalAudioPacketToUnpack) {
        return FALSE;
    }

    u32 dataSize;
    if (this->VideoHeader.AudioCodec == 0)
    {
        return FALSE;
    }
    else if (this->VideoHeader.AudioCodec == 1)
    {
        this->AudioUnpackDataArray[this->CurrentChannelIndex].SxUnpack.Src = reinterpret_cast<u16 *>(this->VideoUnpackData->Data);
        this->AudioUnpackDataArray[this->CurrentChannelIndex].SxUnpack.Dst = outSoundBuffer;
        this->VideoUnpackData->Data += __SxCodec(&this->AudioUnpackDataArray[this->CurrentChannelIndex]);
    }
    else if (this->VideoHeader.AudioCodec == 2)
    {
        this->AudioUnpackDataArray[this->CurrentChannelIndex].SxUnpack.Src = reinterpret_cast<u16 *>(this->VideoUnpackData->Data);
        this->AudioUnpackDataArray[this->CurrentChannelIndex].SxUnpack.Dst = outSoundBuffer;
        __FaCodec(&this->AudioUnpackDataArray[this->CurrentChannelIndex]);
        dataSize = 0x28;
        this->VideoUnpackData->Data += dataSize;
    }
    else if (this->VideoHeader.AudioCodec == 3)
    {
        if (this->CurrentVideoFrameIsKeyFrame == TRUE) {
            ImaAdpcmSetContext(&this->AudioUnpackDataArray[this->CurrentChannelIndex], this->VideoUnpackData->Data);
            this->VideoUnpackData->Data += 4;
        }
        dataSize = 0x80;
        ImaAdpcmDecode(&this->AudioUnpackDataArray[this->CurrentChannelIndex], this->VideoUnpackData->Data, dataSize, outSoundBuffer);
        this->VideoUnpackData->Data += dataSize;
    }
    else
    {
        dataSize = 0x200;
        MI_CpuCopy16(this->VideoUnpackData->Data, outSoundBuffer, dataSize);
        this->VideoUnpackData->Data += dataSize;
    }

    this->CurrentChannelIndex++;
    if (this->CurrentChannelIndex == this->VideoHeader.NbChannel) {
        this->CurrentChannelIndex = 0;
        this->CurrentVideoFrameIsKeyFrame = FALSE;
    }

    this->CurrentNbAudioPacketUnpacked++;

    return TRUE;
}

u32 MOHandlePrivate::FrameSkipAudioPacket(void)
{
    static const int vbrTableSize[4] = {20, 14, 12, 10};

    if (this->CurrentNbAudioPacketUnpacked == this->TotalAudioPacketToUnpack) {
        return FALSE;
    }
    
    if (this->VideoHeader.Frequency == 0) {
        return FALSE;
    }

    if (this->VideoHeader.AudioCodec == 1)
    {
        // this is certainly wrong
        int idx = (*reinterpret_cast<u16 *>(this->VideoUnpackData->Data + 2) / 4096) & 3;
        
        this->VideoUnpackData->Data += vbrTableSize[idx];
    }
    else if (this->VideoHeader.AudioCodec == 2)
    {
        this->VideoUnpackData->Data += 0x28;
    }
    else if (this->VideoHeader.AudioCodec == 3)
    {
        if (this->CurrentVideoFrameIsKeyFrame == TRUE) {
            this->VideoUnpackData->Data += 0x84;
        } else {
            this->VideoUnpackData->Data += 0x80;
        }
    }
    else
    {
        this->VideoUnpackData->Data += 0x200;
    }

    this->CurrentChannelIndex++;
    if (this->CurrentChannelIndex == this->VideoHeader.NbChannel) {
        this->CurrentChannelIndex = 0;
        this->CurrentVideoFrameIsKeyFrame = FALSE;
    }

    this->CurrentNbAudioPacketUnpacked++;

    return TRUE;
}
