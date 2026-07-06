#include "CSceneScriptManager.hpp"

CSceneScriptThread::CSceneScriptThread()
{
    this->manager = NULL;
    this->unk148 = FALSE;
    this->unk290 = 0;
}

void CSceneScriptThread::init(CSceneScriptManager *manager)
{
    this->manager = manager;
    this->events = NULL;
    this->unk148 = FALSE;
}

BOOL CSceneScriptThread::FUN_ov16_020fe530(int idx)
{
    if (!this->manager) {
        return FALSE;
    }
    if (this->unk148) {
        return FALSE;
    }
    
    this->fileIdx = idx;
    this->fileContext = this->manager->getFileContext(idx);

    return TRUE;
}

void CSceneScriptThread::reset()
{
    this->resetEvents();
    this->fileContext = NULL;
}

BOOL CSceneScriptThread::checkFunctionCondition(ScriptInstruction *start, s32 *argv)
{
    int num = 0;
    
    if (!start) {
        return FALSE;
    }

    this->scopeStart = start;
    this->unk148 = TRUE;

    this->getFunctionArguments(start, argv);

    s32 temp_r3 = start->argsCount + 7;
    s32 argIdx = (s32) (temp_r3 + ((u32) (temp_r3 >> 2) >> 0x1D)) >> 3;
    ScriptInstruction *v0 = this->next(start);
    if (v0->id != 0x6012) {
        this->unk148 = FALSE;
        return *(BOOL *)(((u32)start + argIdx * 4) + 8);
    }

    start = this->next(start);
    while (start) {
        if (start->id == 0x6001) {
            break;
        }
        num++;
        start = this->next(start);
    }
    if (!start) {
        this->unk148 = FALSE;
        return FALSE;
    }

    this->evCount = num;
    this->events = static_cast<SScriptEvent *>(this->callocate(sizeof(*this->events) * this->evCount));
    if (!this->events) {
        return FALSE;
    }
    
    for (int i = 0; i < this->evCount; i++) {
        this->events[i].data = v0;
        v0 = this->next(v0);
    }

    u32 result;
    this->processEvent(this->events, &result);

    gAllocator.deallocate(this->events);
    this->events = NULL;
    this->evCount = 0;
    this->unk148 = FALSE;

    if (result) {
        return *(BOOL *)(((u32)this->scopeStart + argIdx * 4) + 8);;
    }

    return FALSE;
}

void CSceneScriptThread::executeScope(CSceneDirection *direction, ScriptInstruction *start, s32 *argv)
{
    this->getFunctionArguments(start, argv);
    MI_CpuClear8(&this->unkEvent, sizeof(this->unkEvent));
    if (start->id == 0x3001) {
        s32 temp_r1 = start->argsCount + 7;
        this->functionID = *(int *) (((s32)start + (((s32) (temp_r1 + ((u32) (temp_r1 >> 2) >> 0x1D)) >> 3) * 4)) + 8);
    }

    start = this->next(start);
    while (start) {
        if (start->id == 0x6001) {
            break;
        }

        start = this->next(start);
    }
    if (!start) {
        return;
    }

    this->scopeStart = start;
    
    int num = 1;
    ScriptInstruction *cur = this->next(start);
    while (cur) {
        num++;
        if ((cur->id == 0x6002) && (*(s32 *)((s32)cur + 0xC) == 0)) {
            break;
        }
        cur = this->next(cur);
    }
    if (!cur) {
        return;
    }

    s32 type, code;
    this->fileContext->getTypeAndCode(&type, &code);

    this->evCount = num;
    this->events = static_cast<SScriptEvent *>(this->callocate(sizeof(*this->events) * this->evCount));
    if (!this->events) {
        return;
    }

    cur = this->scopeStart;
    for (int i = 0; i < this->evCount; i++) {
        this->events[i].data = cur;
        this->events[i].unk9 = cur->unk;

        cur = this->next(cur);
    }

    this->direction = direction;
    this->firstEvent = this->events;
    this->unk180 = 0;
    this->unk168 = 0;
    this->unk14C = 0;
    this->unk150 = 0;
    this->unk154 = 0;
    this->unk158 = 0;
    this->unk15C = 0;
    this->unk148 = TRUE;
    this->returnVal = 0;
    
}

u32 CSceneScriptThread::resetEvents(void)
{
    this->unk148 = FALSE;

    if (this->events) {
        s32 type, code;
        this->fileContext->getTypeAndCode(&type, &code);
        
        this->free(this->events);
    }

    this->events = NULL;
    this->evCount = 0;

    return this->returnVal;
}

void CSceneScriptThread::FUN_ov16_020fe904(void)
{
    if (this->unk158 == 0) {
        this->unk14C = 1;
    }
}

void CSceneScriptThread::FUN_ov16_020fe918(void)
{
    if (this->unk158 == 0) {
        this->unk150 = 1;
    }
}

BOOL CSceneScriptThread::FUN_ov16_020fe92c(void)
{
    if (this->unk148 == FALSE) {
        return FALSE;
    }

    u32 v0 = this->unk168 + gDeltaTime;
    this->unk178 = gDeltaTime;
    this->unk168 = v0;

    SScriptEvent *ev = this->executeRange(this->firstEvent, &this->events[this->evCount - 1]);
    this->firstEvent = ev;
    if (ev >= &this->events[this->evCount - 1]) {
        this->unk148 = FALSE;
        return FALSE;
    }

    return TRUE;
}

SScriptEvent *CSceneScriptThread::getEvent(void)
{
    return this->events;
}

int CSceneScriptThread::getEventCount(void)
{
    return this->evCount;
}

void CSceneScriptThread::setFirstEvent(SScriptEvent *event)
{
    this->firstEvent = event;
}

void CSceneScriptThread::FUN_ov16_020fe9bc(BOOL param1)
{
    this->unk148 = param1;
}

void *CSceneScriptThread::callocate(size_t size)
{
    return this->manager->callocate(size);
}

void CSceneScriptThread::free(void *ptr)
{
    this->manager->free(ptr);
}

ScriptInstruction *CSceneScriptThread::next(ScriptInstruction *cur)
{
    return this->fileContext->next(cur);
}

void CSceneScriptThread::getFunctionArguments(ScriptInstruction *inst, s32 *argv)
{
    MI_CpuClear8(&this->variable, sizeof(this->variable));

    if (!argv) {
        return;
    }

    int temp_r1 = inst->argsCount + 7;
    s32 v1 = (((s32) (temp_r1 + ((u32) (temp_r1 >> 2) >> 0x1D)) >> 3) * 4);
    for (int i = 2; i < inst->argsCount; i++) {
        int v0 = *(int *)(((s32)inst + v1 + (i * 4)) + 8);
        this->variable[v0] = argv[i - 2];
    }

    this->argc = inst->argsCount - 2;
}

SScriptEvent *CSceneScriptThread::findEvent(SScriptEvent *start, u16 id)
{
    if (!start) {
        start = this->events;
    }

    int idx = ((int)start - (int)this->events) / sizeof(*start);

    SScriptEvent *cur = start;
    for (int i = idx; i < this->evCount; i++) {
        if (*static_cast<u16 *>(cur->data) == id) {
            return cur;
        }

        cur++;
    }

    cur = start;
    cur--;
    while (idx > 0) {
        if (*static_cast<u16 *>(cur->data) == id) {
            return cur;
        }

        cur--;
        idx--;
    }

    return NULL;
}

SScriptEvent *CSceneScriptThread::findEventReverse(SScriptEvent *start, u16 id)
{
    if (!start) {
        start = &this->events[this->evCount - 1];
    }

    int idx = ((int)start - (int)this->events) / sizeof(*start);

    SScriptEvent *cur = start;
    for (int i = idx; i >= 0; i--) {
        if (*static_cast<u16 *>(cur->data) == id) {
            return cur;
        }

        cur--;
    }

    cur = start;
    idx--;
    cur++;
    while (idx < this->evCount) {
        if (*static_cast<u16 *>(cur->data) == id) {
            return cur;
        }

        idx++;
        cur++;
    }

    return NULL;
}

SScriptEvent *CSceneScriptThread::findPreviousEventByID(SScriptEvent *start, u16 id)
{
    if (!start) {
        start = &this->events[this->evCount - 1];
    }

    int idx = ((int)start - (int)this->events) / sizeof(*start);

    while (idx >= 0) {
        if (static_cast<ScriptInstruction *>(start->data)->id == id) {
            return start;
        }

        start--;
        idx--;
    }

    return NULL;
}

SScriptEvent *CSceneScriptThread::getArguments(SScriptEvent *event, s32 *argv)
{
    SScriptEvent *next = event + 1;

    ScriptInstruction *inst = static_cast<ScriptInstruction *>(event->data);
    int temp_r1 = inst->argsCount + 7;
    int temp_r5 = (s32)inst + (((s32) (temp_r1 + ((u32) (temp_r1 >> 2) >> 0x1D)) >> 3) * 4);
    for (int i = 0; i < inst->argsCount; i++)
    {
        u32 result;
        SScriptEvent *ev;
        u32 temp_r2;
        int idx;
        u32 temp_r2_2;
        switch (temp_r5) {
            case 1:
            case 2:
            case 3:
            case 7:
                argv[i] = *(s32 *)((temp_r5 + (i * 4)) + 8);
                break;
            case 4:
                ev = this->findEvent(next, *(s32 *)((temp_r5 + (i * 4)) + 8));
                this->processEvent(ev, &result);
                next = ev->next;
                if (!next) {
                    next = ev + 1;
                }
                argv[i] = result;
                break;
            case 5:
                temp_r2 = *(u32 *)((temp_r5 + (i * 4)) + 8);
                idx = temp_r2 & 0xFFFF;
                if (temp_r2 & 0x10000) {
                    argv[i] = *this->manager->getVariable(idx);
                } else if (temp_r2 & 0x20000) {
                    argv[i] = *this->fileContext->getVariable(idx);
                } else {
                    argv[i] = this->variable[idx];
                }
                break;
            case 6:
                temp_r2_2 = *(u32 *)((temp_r5 + (i * 4)) + 8);
                idx = temp_r2_2 & 0xFFFF;
                if (temp_r2 & 0x10000) {
                    argv[i] = *this->manager->getVariable(idx);
                } else if (temp_r2 & 0x20000) {
                    argv[i] = *this->fileContext->getVariable(idx);
                } else {
                    argv[i] = this->variable[idx];
                }
                break;
        }
    }

    return next;
}

SScriptEvent *CSceneScriptThread::getArgumentsClear(SScriptEvent *event, s32 *argv)
{
    MI_CpuClearFast(argv, sizeof(argv) * 32);
    event->next = this->getArguments(event, argv);
    return event->next;
}

void CSceneScriptThread::FUN_ov16_020feda4(SScriptEvent *event)
{
    int v0 = *(int *)((int)event->variable[0].i + 0xC);
    this->unk17C = v0;
    this->unkEvent[v0] = event;
    this->unk17C++;
}

/*SScriptEvent *CSceneScriptThread::executeRange(SScriptEvent *event, SScriptEvent *last)
{
    SScriptEvent *next;
    SScriptEvent *first = event;
    while (first < last) {
        if (event->complete) {
            next = event->next;
            if (!next) {
                next = event + 1;
            }
        } else {
            
        }
    }
}*/
