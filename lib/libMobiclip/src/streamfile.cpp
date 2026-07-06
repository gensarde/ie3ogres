#include "private/stream.hpp"

void *MOStreamFile::operator new(size_t size)
{
    return MO_Malloc(size);
}

void MOStreamFile::operator delete(void *ptr)
{
    MO_Free(ptr);
}

BOOL MOStreamFile::Open(FSFile *file)
{
    this->MOFile = file;
    this->Size = FS_GetLength(file);
    this->Position = FS_GetPosition(this->MOFile);
    this->AsyncInProgress = FALSE;

    return TRUE;
}

u32 MOStreamFile::SetPosition(u32 position)
{
    if (this->AsyncInProgress == TRUE) {
        FS_WaitAsync(this->MOFile);
        this->AsyncInProgress = FALSE;
    }
    
    if (!FS_SeekFile(this->MOFile, position, FS_SEEK_SET)) {
        return FALSE;
    }

    this->Position = position;
    
    return TRUE;
}

u32 MOStreamFile::Read(void *buffer, u32 len)
{
    if (this->AsyncInProgress == TRUE) {
        FS_WaitAsync(this->MOFile);
        FS_SeekFile(this->MOFile, this->Position, FS_SEEK_SET);
        this->AsyncInProgress = FALSE;
    }

    if (FS_ReadFile(this->MOFile, buffer, len) == -1) {
        return FALSE;
    }

    this->Position += len;

    return TRUE;
}

u32 MOStreamFile::ReadAsync(void *buffer, u32 len)
{
    u32 readOffset = this->Position & ~0x1FF;
    u32 r4 = this->Position - readOffset;
    s32 readSize = len + r4;

    if (readSize & 0x1FF) {
        readSize = (readSize & ~0x1FF) + 0x200;
    }

    FS_SeekFile(this->MOFile, readOffset, FS_SEEK_SET);

    if (FS_ReadFileAsync(this->MOFile, buffer, readSize) == -1) {
        return 0;
    }
    
    this->Position += len;
    this->AsyncInProgress = TRUE;
    
    return r4;
}

void MOStreamFile::WaitAsync(void)
{
    if (this->AsyncInProgress == TRUE) {
        FS_WaitAsync(this->MOFile);
    }

    this->AsyncInProgress = FALSE;
}

void MOStreamFile::Close(void)
{
    this->WaitAsync();
}
