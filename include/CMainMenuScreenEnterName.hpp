#include <nitro.h>
#include <string.h>

#include "allocator.hpp"
#include "archive.hpp"
#include "graphics.hpp"
#include "pac.hpp"
#include "CScreenManager.hpp"
#include "CommonScreen.hpp"
#include "CBgMenuManager.hpp"
#include "audioplayer.hpp"
#include "CFontManager.hpp"
#include "CPhonePassword.hpp"
#include "CLogicThink.hpp"
#include "CRecordManager.hpp"
#include "unk_0209A454.hpp"
#include "unk_020A0640.hpp"
#include "unk_020A9C40.hpp"

extern u16 gKeysRepeated;
extern "C" {
    int FUN_ov16_020f081c(void);
    extern void FUN_ov16_020f2184(char *text, void *dest, int width, int height, int color, int rubyColor, int charSpacing, int lineSpacing, int x, int y, int align);
    typedef struct {
        u16 *unk0;
        void *unk4;
        u32 unk8;
        u16 unkC;
        u16 unkE;
        u8 unk10;
    } SCanvas;
    extern void FUN_ov16_020f2f20(SCanvas *);
    extern void FUN_ov16_020f2f7c(SCanvas *, int);
    extern void FUN_ov16_020f2fa8(SCanvas *, int, int);
    extern void FUN_ov16_020f2fe4(SCanvas *, int);
}

typedef struct {
    int type;
    int x;
    u32 y;
    u16 chara;
} SKey;

class CMainMenuScreenEnterName : public CommonMainScreen
{
public:
    static bool FUN_ov55_02119f00(int param0);
    /* ov55 0x02119f54 */ static CFontManager::CharType getCharType(char *text);
    /* ov55 0x02119f94 */ static u16 decodeChar(char *text, CFontManager::CharType type);
    /* ov55 0x02119fc0 */ static u16 normalizeChar(u16 c, u16 *data, int size);
    static void FUN_ov55_0211a010(char *param0, char *param1, u16 *param2, int param3);
    static bool FUN_ov55_0211a0c4(char *param0, u16 *param1, int param2, char **param3, int param4);

    CMainMenuScreenEnterName(CScreenManager *manager) : manager(manager) {}
    virtual ~CMainMenuScreenEnterName() { }
    /* ov55 0x0211aa40 */ virtual void updateKeys(u16 pressed, u16 held);
    /* ov55 0x0211b618 */ virtual void updateTP(TPData *tp);
    /* ov55 0x0211cbf4 */ virtual void init(void);
    /* ov55 0x0211cd8c */ virtual void update(BOOL param1);
    /* ov55 0x0211cea4 */ virtual void updateLate(void);
    /* ov55 0x0211cf04 */ virtual void close(void);
    /* ov55 0x0211b854 */ virtual void vFUN_6C(void);
    /* ov55 0x0211b750 */ virtual void vFUN_70(void);
    /* ov55 0x0211ce90 */ virtual BOOL tryFinalize(void);
    /* ov55 0x0211d030 */ virtual void closeFiles(void);
    /* ov55 0x0211baf4 */ virtual void deallocateFile(int idx);
    /* ov55 0x0211bac4 */ virtual void deallocateFiles(void);
    /* ov55 0x0211d10c */ private: virtual void loadBG0Char(int fileIdx, u32 offset) {
        Graphics::LoadBGXCharMain(0, this->files[fileIdx].data, offset);
    }
    /* ov55 0x0211d0f0 */ private: virtual void loadBG1Char(int fileIdx, u32 offset) {
        Graphics::LoadBGXCharMain(1, this->files[fileIdx].data, offset);
    }
    /* ov55 0x0211d0d4 */ private: virtual void loadBG2Char(int fileIdx, u32 offset) {
        Graphics::LoadBGXCharMain(2, this->files[fileIdx].data, offset);
    }
    /* ov55 0x0211d0b8 */ private: virtual void loadBG3Char(int fileIdx, u32 offset) {
        Graphics::LoadBGXCharMain(3, this->files[fileIdx].data, offset);
    }
    /* ov55 0x0211bb0c */ virtual void vFUN_94(void);
    /* ov55 0x0211bfcc */ virtual void vFUN_98(void);
    /* ov55 0x0211d09c */ private: virtual void loadTempPalette(int fileIdx, int plttIdx) {
        Graphics::LoadTempPaletteFromPac(this->files[fileIdx].data, ENGINE_MAIN, plttIdx);
    }
    /* ov55 0x0211d07c */ private: virtual void setupScreen(int fileIdx, int tileIdx, int plttIdx) {
        Graphics::SetupScreen(this->files[fileIdx].data, tileIdx + 1, plttIdx);
    }
    /* ov55 0x0211a180 */ virtual void vFUN_A4(int param1);
    /* ov55 0x0211a5d8 */ virtual void setKey(int x, u32 y, SKey *key);
    /* ov55 0x0211a580 */ virtual u16 getChara(int x, u32 y);
    /* ov55 0x0211af80 */ virtual void vFUN_B0(int param1, int param2, SKey *key);
    /* ov55 0x0211a640 */ virtual void typeCharacter(void);
    /* ov55 0x0211b52c */ virtual void vFUN_B8(void);
    /* ov55 0x0211b308 */ virtual void vFUN_BC(void);
    /* ov55 0x0211a1f0 */ virtual void vFUN_C0(void);
    /* ov55 0x0211a448 */ virtual void vFUN_C4(int param1);
    /* ov55 0x0211a4f4 */ virtual void vFUN_C8(void);
    /* ov55 0x0211c05c */ virtual void vFUN_CC(int param1);
    /* ov55 0x0211b07c */ virtual void vFUN_D0(void);
    /* ov55 0x0211c178 */ virtual void vFUN_D4(void);
    /* ov55 0x0211c1bc */ virtual void vFUN_D8(void);
    /* ov55 0x0211c200 */ virtual void vFUN_DC(void);
    /* ov55 0x0211c2f0 */ virtual void vFUN_E0(void);
    /* ov55 0x0211c454 */ virtual void vFUN_E4(void);

private:
    inline u8 read_u8(u8 *src) {
        return u8(src[0]);
    }
    inline u16 read_u16(u8 *src) {
        return u16((src[0] & 0xFF) | (u8(src[1]) << 8));
    }

    static const int ENTER_NAME_WIDTH = 26;
    static const int ENTER_NAME_HEIGHT = 6;
    static const int ENTER_NAME_MAX = 16;

    static const int KEYCODE_EMPTY = '  ';
    static const int KEYCODE_KANA = 'AA';
    static const int KEYCODE_HANDAKU = 'BB';
    static const int KEYCODE_DAKUTEN = 'CC';
    static const int KEYCODE_DELETE = 'DD';

    enum {
        FILE_ARCHIVE,                      // 0
        FILE_XXDN_BG,                      // 1
        FILE_2,                            // 2
        FILE_BG02,                         // 3
        FILE_BG03,                         // 4
        FILE_BG06,                         // 5
        FILE_C00,                          // 6
        FILE_I00,                          // 7
        FILE_I01,                          // 8
        FILE_I02,                          // 9
        FILE_W70,                          // 10
        FILE_B70,                          // 11
        FILE_W10,                          // 12
        FILE_FCODE_BASE,                   // 13
        FILE_FCODE0 = FILE_FCODE_BASE,     // 13
        FILE_FCODE1 = FILE_FCODE_BASE + 1, // 14
        FILE_FCODE2 = FILE_FCODE_BASE + 2, // 15
        FILE_HANDAKU,                      // 16
        FILE_DAKUTEN,                      // 17
        FILE_NGWORD,                       // 18
        FILE_FCODECK,                      // 19
        FILE_MAX,                          // 20
    };

    CScreenManager *manager;
    int state;
    int unkC;
    int unk10;
    u8 unk14;
    u8 tpTouch;
    u8 unk16;
    u8 unk17;
    u32 tpX;
    u32 tpY;
    SKey unk20;
    SKey unk30;
    SKey keyActive;
    int unk50;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    int unk58;
    u8 unk5C;
    u8 unk5D;
    u8 unk5E;
    u8 unk5F;
    u32 unk60;
    u8 *unk64;
    u16 *unk68;
    char unk6C[17];
    char *ngWords;
    char **ngLines;
    size_t ngLineCount;
    size_t ngSize;
    SCanvas canvas;
    SFileData files[FILE_MAX];
    CPhonePassword phonePassword;
};
