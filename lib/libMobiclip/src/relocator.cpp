#include "private/relocator.hpp"

u32 *MO_ITCMAdress;
u32 MO_ITCMSize;
u32 *MO_DTCMAdress;
u32 MO_DTCMSize;
u32 *MO_VideoCodeAdress;
u32 *MO_VideoDataAdress;
u8 *MO_BlitDataAdress;
u8 *MO_MinMaxDataAdress;

u32 MO_GetOptimalITCMSize(void)
{
    return MO_OPTIMAL_ITCM_SIZE;
}

u32 MO_GetOptimalDTCMSize(void)
{
    return MO_OPTIMAL_DTCM_SIZE;
}

void MO_InitITCMArea(void *itcmPtr, u32 size)
{
    MO_ITCMAdress = static_cast<u32 *>(itcmPtr);
    MO_ITCMSize = size & ~3;
    MO_VideoCodeAdress = NULL;
}

void MO_InitDTCMArea(void *dtcmPtr, u32 size)
{
    MO_DTCMAdress = static_cast<u32 *>(dtcmPtr);
    MO_DTCMSize = size & ~3;
    MO_VideoDataAdress = NULL;
    MO_BlitDataAdress = NULL;
    MO_MinMaxDataAdress = NULL;
}

u32 (*MOPrivate_VideoCodeRelocate(void))(__Vx2Unpack *)
{
    if (MO_VideoCodeAdress == NULL) {
        if (MO_ITCMSize >= MO_OPTIMAL_ITCM_SIZE) {
            MO_VideoCodeAdress = MO_ITCMAdress;
            MI_CpuCopyFast(__Vx2Codec, MO_ITCMAdress, MO_OPTIMAL_ITCM_SIZE);
            MO_ITCMAdress += 0x1967;
            MO_ITCMSize -= MO_OPTIMAL_ITCM_SIZE;
        }
        else {
            return __Vx2Codec;
        }
    }
    return reinterpret_cast<u32 (*)(__Vx2Unpack *)>(MO_VideoCodeAdress);
}

const u32 *MOPrivate_VideoDataRelocate(void)
{
    if (MO_VideoDataAdress == NULL) {
        if (MO_DTCMSize >= 0x2100) {
            MO_VideoDataAdress = MO_DTCMAdress;
            MI_CpuCopyFast(__Vx2Table0, MO_DTCMAdress, 0x2100);
            MO_DTCMAdress += 0x840;
            MO_DTCMSize -= 0x2100;
        } else {
            return __Vx2Table0;
        }
    }
    return MO_VideoDataAdress;
}

const u8 *MOPrivate_BlitDataRelocate(void)
{
    if (MO_BlitDataAdress == NULL) {
        if (MO_DTCMSize >= 0x300) {
            MO_BlitDataAdress = reinterpret_cast<u8 *>(MO_DTCMAdress);
            MI_CpuCopyFast(__Vx2BlitTable, MO_DTCMAdress, 0x300);
            MO_DTCMAdress += 0xC0;
            MO_DTCMSize -= 0x300;
        } else {
            return __Vx2BlitTable;
        }
    }
    return MO_BlitDataAdress;
}

const u8 *MOPrivate_MinMaxDataRelocate(void)
{
    if (MO_MinMaxDataAdress == NULL) {
        if (MO_DTCMSize >= 0x180) {
            MO_MinMaxDataAdress = reinterpret_cast<u8 *>(MO_DTCMAdress);
            MI_CpuCopyFast(__Vx2MinMaxTable, MO_DTCMAdress, 0x180);
            MO_DTCMAdress += 0x60;
            MO_DTCMSize -= 0x180;
        } else {
            return __Vx2MinMaxTable;
        }
    }
    return MO_MinMaxDataAdress;
}
