#include <Mobiclip.h>

#include "private/handle.hpp"

#include <nitro/version_begin.h>
static char id_string[] = SDK_MIDDLEWARE_STRING("Actimagine", "Mobiclip SDK V1.2.1");
#include <nitro/version_end.h>

static u32 Init(MOHandlePrivate *handle, MOStreamFile *stream, u32 nbBufferedFrameMax);

static u32 Init(MOHandlePrivate *handle, MOStreamFile *stream, u32 nbBufferedFrameMax)
{
    SDK_USING_MIDDLEWARE(id_string);

    return handle->Init(stream, nbBufferedFrameMax);
}

MOHandle MO_OpenMovie(FSFile *file, u32 nbBufferedFrameMax)
{
    MOStreamFile *streamFile = new MOStreamFile();
    if (streamFile == NULL) {
        return NULL;
    }
    
    if (!streamFile->Open(file)) {
        delete streamFile;
        return NULL;
    }

    MOHandlePrivate *handle = new MOHandlePrivate();
    if (!Init(handle, streamFile, nbBufferedFrameMax)) {
        delete handle;
        return NULL;
    }

    return reinterpret_cast<MOHandle>(handle);
}

void MO_CloseMovie(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        delete reinterpret_cast<MOHandlePrivate *>(handle);
    }
}

u32 MO_GetAudioFrequency(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetSoundFrequency();
    }
    return 0;
}

u32 MO_GetNbAudioTrack(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetNbSoundTrack();
    }
    return 0;
}

u32 MO_GetVideoFps(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetFps();
    }
    return 0;
}

u32 MO_GetVideoWidth(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetVideoWidth();
    }
    return 0;
}

u32 MO_GetVideoHeight(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetVideoHeight();
    }
    return 0;
}

u32 MO_GetNbFrame(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetNbFrame();
    }
    return 0;
}

u32 MO_GetNbIFrame(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetNbIFrame();
    }
    return 0;
}

u32 MO_GetNbSamplePerAudioPacket(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetNbSamplePerAudioPacket();
    }
    return 0;
}

u32 MO_GetCurrentFrameNumber(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetCurrentFrameNumber();
    }
    return 0;
}

u32 MO_GetIFrameInfo(MOHandle handle, u32 index)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetIFrameInfo(index);
    }
    return 0;
}

u32 MO_ReadFrame(MOHandle handle)
{
    if (handle == MO_INVALID_HANDLE) {
        return FALSE;
    }
    return reinterpret_cast<MOHandlePrivate *>(handle)->NextFrame() == TRUE;
}

u32 MO_JumpBeginning(MOHandle handle)
{
    if (handle == MO_INVALID_HANDLE) {
        return FALSE;
    }
    return reinterpret_cast<MOHandlePrivate *>(handle)->JumpBeginning() == TRUE;
}

u32 MO_JumpNextIframe(MOHandle handle)
{
    if (handle == MO_INVALID_HANDLE) {
        return FALSE;
    }
    return reinterpret_cast<MOHandlePrivate *>(handle)->JumpNextIframe() == TRUE;
}

u32 MO_JumpPreviousIframe(MOHandle handle)
{
    if (handle == MO_INVALID_HANDLE) {
        return FALSE;
    }
    return reinterpret_cast<MOHandlePrivate *>(handle)->JumpPreviousIframe() == TRUE;
}

u32 MO_JumpToIframe(MOHandle handle, u32 index)
{
    if (handle == MO_INVALID_HANDLE) {
        return FALSE;
    }
    return reinterpret_cast<MOHandlePrivate *>(handle)->JumpToIframe(index) == TRUE;
}

u32 MO_UnpackFrameImage(MOHandle handle)
{
    if (handle == MO_INVALID_HANDLE) {
        return FALSE;
    }
    return reinterpret_cast<MOHandlePrivate *>(handle)->FrameUnpackVideo() == TRUE;
}

u32 MO_BlitFrameImage(MOHandle handle, u16* outFrameBufferPtr, u32 outFrameBufferPitch, u32 qualityEnhancement)
{
    if (handle == MO_INVALID_HANDLE) {
        return FALSE;
    }
    return reinterpret_cast<MOHandlePrivate *>(handle)->BlitVideo(outFrameBufferPtr, outFrameBufferPitch, qualityEnhancement) == TRUE;
}

u32 MO_SkipFrameImage(MOHandle handle)
{
    if (handle == MO_INVALID_HANDLE) {
        return FALSE;
    }
    return reinterpret_cast<MOHandlePrivate *>(handle)->SkipVideo() == TRUE;
}

u32 MO_GetFrameNbAudioPacket(MOHandle handle)
{
    if (handle != MO_INVALID_HANDLE) {
        return reinterpret_cast<MOHandlePrivate *>(handle)->GetFrameNbAudioPacket();
    }
    return 0;
}

u32 MO_BlitFrameSoundOnePacket(MOHandle handle, s16* outSoundBuffer)
{
    if (handle == MO_INVALID_HANDLE) {
        return FALSE;
    }
    return reinterpret_cast<MOHandlePrivate *>(handle)->FrameUnpackAudioPacket(outSoundBuffer) == TRUE;
}

u32 MO_SkipFrameSoundOnePacket(MOHandle handle)
{
    if (handle == MO_INVALID_HANDLE) {
        return FALSE;
    }
    return reinterpret_cast<MOHandlePrivate *>(handle)->FrameSkipAudioPacket() == TRUE;
}
