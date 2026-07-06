#ifndef NITRO_LIBMOBICLIP_STREAM_H
#define NITRO_LIBMOBICLIP_STREAM_H

#pragma once

#include <nitro.h>
#include <Mobiclip.h>

class MOStream
{
public:
    MOStream() {
        this->Size = 0;
        this->Position = 0;
    }
    virtual ~MOStream() { }

    virtual u32 SetPosition(u32 position);
    virtual u32 Read(void *buffer, u32 len);
    virtual u32 ReadAsync(void *buffer, u32 len);
    virtual void WaitAsync(void);
    virtual void Close(void);

    u32 Size;
    u32 Position;
};

class MOStreamFile : public MOStream
{
public:
    void *operator new(size_t size);
    void operator delete(void *ptr);

    virtual ~MOStreamFile() { }
    
    virtual u32 SetPosition(u32 position);
    virtual u32 Read(void *buffer, u32 len);
    virtual u32 ReadAsync(void *buffer, u32 len);
    virtual void WaitAsync(void);
    virtual void Close(void);
    
    BOOL Open(FSFile *file);
    
    FSFile *MOFile;
    u8 AsyncInProgress;
};

// unused
class MOStreamMemory : public MOStream
{
public:
    void *operator new(size_t size);
    void operator delete(void *ptr);

    virtual ~MOStreamMemory() { }
    virtual u32 SetPosition(u32 position);
    virtual u32 Read(void *buffer, u32 len);
    virtual u32 ReadAsync(void *buffer, u32 len);
    virtual void WaitAsync(void);
    virtual void Close(void);
        
    BOOL Open(u8 *streamData, u32 size);

    u8 *StreamData;
};

#endif //NITRO_LIBMOBICLIP_STREAM_H
