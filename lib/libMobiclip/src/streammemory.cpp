#include "private/stream.hpp"

void *MOStreamMemory::operator new(size_t size)
{
    return MO_Malloc(size);
}

void MOStreamMemory::operator delete(void *ptr)
{
    MO_Free(ptr);
}

BOOL MOStreamMemory::Open(u8 *streamData, u32 size)
{
    this->Size = size;
    this->StreamData = streamData;

    return TRUE;
}

u32 MOStreamMemory::SetPosition(u32 position)
{
    if (position > this->Size) {
        return FALSE;
    }
    this->Position = position;
    
    return TRUE;
}

u32 MOStreamMemory::Read(void *buffer, u32 len)
{
    u32 pos = this->Position;
    u32 size = this->Size;

    if (size < pos + len) {
        len = size - pos;
    }

    MI_CpuCopyFast(this->StreamData + pos, buffer, len);
    this->Position += len;

    return TRUE;
}

u32 MOStreamMemory::ReadAsync(void *buffer, u32 len)
{
    return this->Read(buffer, len);
}

void MOStreamMemory::WaitAsync(void) { }

void MOStreamMemory::Close(void)
{
    this->Size = 0;
    this->StreamData = NULL;
    this->Position = 0;
}
