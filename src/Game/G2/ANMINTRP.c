#include "Game/G2/ANMINTRP.h"

void G2AnimSection_InterpToKeylistAtTime(G2AnimSection *section, G2AnimKeylist *keylist, int keylistID, short targetTime, short duration)
{
    G2Anim *anim;
    G2AnimInterpInfo *interpInfo;
    G2AnimInterpStateBlock *stateBlockList;
    G2AnimQuatInfo *quatInfo;
    unsigned long alarmFlags;
    short elapsedTime;
    int quatInfoChunkCount;
    int segCount;

    if (duration == 0)
    {
        G2AnimSection_SwitchToKeylistAtTime(section, keylist, keylistID, targetTime);
    }
    else
    {
        anim = _G2AnimSection_GetAnim(section);

        interpInfo = section->interpInfo;

        segCount = section->segCount;

        if (interpInfo->stateBlockList != NULL)
        {
            _G2AnimSection_InterpStateToQuat(section);
        }
        else if (_G2Anim_AllocateInterpStateBlockList(section) == NULL)
        {
            G2AnimSection_SwitchToKeylistAtTime(section, keylist, keylistID, targetTime);
            return;
        }
        else
        {
            section->interpInfo = NULL;

            _G2AnimSection_UpdateStoredFrameFromData(section, anim);

            section->interpInfo = interpInfo;

            _G2AnimSection_SegValueToQuat(section, 0);
        }

        elapsedTime = section->elapsedTime;

        alarmFlags = section->alarmFlags;

        section->interpInfo = NULL;

        G2AnimSection_SwitchToKeylistAtTime(section, keylist, keylistID, targetTime);

        _G2AnimSection_UpdateStoredFrameFromData(section, anim);

        section->interpInfo = interpInfo;

        section->alarmFlags = alarmFlags;

        section->elapsedTime = elapsedTime;

        _G2AnimSection_SegValueToQuat(section, 1);

        stateBlockList = interpInfo->stateBlockList;

        quatInfoChunkCount = 4;

        quatInfo = &stateBlockList->quatInfo[0];

        if ((quatInfo->destTrans.x | quatInfo->destTrans.y | quatInfo->destTrans.z) == 0)
        {
            quatInfo->srcTrans.x = 0;
            quatInfo->srcTrans.y = 0;
            quatInfo->srcTrans.z = 0;
        }

        while (segCount > 0)
        {
            segCount--;

            quatInfo->destScale.x -= quatInfo->srcScale.x;
            quatInfo->destScale.y -= quatInfo->srcScale.y;
            quatInfo->destScale.z -= quatInfo->srcScale.z;

            quatInfo->destTrans.x -= quatInfo->srcTrans.x;
            quatInfo->destTrans.y -= quatInfo->srcTrans.y;
            quatInfo->destTrans.z -= quatInfo->srcTrans.z;

            quatInfo++;

            if (--quatInfoChunkCount == 0)
            {
                stateBlockList = stateBlockList->next;

                quatInfoChunkCount = 4;

                quatInfo = &stateBlockList->quatInfo[0];
            }
        }

        interpInfo->targetTime = targetTime;

        interpInfo->duration = duration;

        if ((!(section->flags & 0x2)) && ((section->alarmFlags & 0x3)))
        {
            if (duration < ((elapsedTime % section->keylist->s0TailTime) + 1))
            {
                elapsedTime = duration;
            }
            else
            {
                elapsedTime = (elapsedTime % section->keylist->s0TailTime) + 1;
            }

            section->elapsedTime = elapsedTime;
        }
        else
        {
            section->elapsedTime = 0;
        }

        section->keylist = keylist;
        section->keylistID = keylistID;

        section->storedTime = -section->keylist->timePerKey;

        if ((section->flags & 0x2))
        {
            G2AnimSection_SetLoopRangeAll(section);
        }

        G2AnimSection_ClearAlarm(section, 0x3);

        section->flags &= 0x7F;

        G2AnimSection_SetUnpaused(section);

        section->swAlarmTable = NULL;

        section->interpInfo = interpInfo;

        anim->flags |= 0x1;
    }
}

void _G2AnimSection_UpdateStoredFrameFromQuat(G2AnimSection *section)
{
    G2AnimSegValue *segValue;
    G2AnimInterpInfo *interpInfo;
    G2AnimInterpStateBlock *stateBlockList;
    G2AnimQuatInfo *quatInfo;
    long alpha;
    G2Quat newQuat;
    int quatInfoChunkCount;
    int segCount;

    interpInfo = section->interpInfo;

    alpha = (section->elapsedTime * 4096) / interpInfo->duration;

    quatInfoChunkCount = 4;

    segCount = section->segCount;

    alpha = _G2AnimAlphaTable_GetValue(interpInfo->alphaTable, alpha);

    stateBlockList = interpInfo->stateBlockList;

    segValue = &_segValues[section->firstSeg];

    quatInfo = stateBlockList->quatInfo;

    while (segCount > 0)
    {
        G2Quat *source;
        unsigned long zw;
        unsigned long xy;

        G2Quat_Slerp_VM(alpha, &quatInfo->srcQuat, &quatInfo->destQuat, &newQuat, 0);

        source = &newQuat;

        xy = GETP_XY(source);
        zw = GETP_ZW(source);

        SET_XY(segValue->rotQuat.quat, xy);
        SET_ZW(segValue->rotQuat.quat, zw);

        gte_process((G2Quat *)&segValue->scale, &quatInfo->srcScale, &quatInfo->destScale, alpha);
        gte_process((G2Quat *)&segValue->trans, &quatInfo->srcTrans, &quatInfo->destTrans, alpha);

        segCount--;

        quatInfoChunkCount--;

        quatInfo++;

        if (quatInfoChunkCount == 0)
        {
            stateBlockList = stateBlockList->next;

            quatInfoChunkCount = 4;

            quatInfo = stateBlockList->quatInfo;
        }

        segValue->bIsQuat = 1;

        segValue++;
    }

    section->storedTime = section->elapsedTime;

    section->flags |= 0x80;
}

void _G2AnimSection_InterpStateToQuat(G2AnimSection *section)
{
    G2AnimInterpInfo *interpInfo;
    G2AnimInterpStateBlock *stateBlockList;
    G2AnimQuatInfo *quatInfo;
    long alpha;
    G2Quat newQuat;
    int quatInfoChunkCount;
    int segCount;

    interpInfo = section->interpInfo;

    alpha = (section->elapsedTime * 4096) / interpInfo->duration;

    quatInfoChunkCount = 4;

    segCount = section->segCount;

    alpha = _G2AnimAlphaTable_GetValue(interpInfo->alphaTable, alpha);

    stateBlockList = interpInfo->stateBlockList;

    quatInfo = &stateBlockList->quatInfo[0];

    while (segCount > 0)
    {
        G2Quat *source;
        unsigned long zw;
        unsigned long xy;

        G2Quat_Slerp_VM(alpha, &quatInfo->srcQuat, &quatInfo->destQuat, &newQuat, 0);

        source = &newQuat;

        xy = GET_XY(newQuat);
        zw = GETP_ZW(source);

        SET_XY(quatInfo->srcQuat, xy);
        SET_ZW(quatInfo->srcQuat, zw);

        gte_process((G2Quat *)&quatInfo->srcScale, &quatInfo->srcScale, &quatInfo->destScale, alpha);
        gte_process((G2Quat *)&quatInfo->srcTrans, &quatInfo->srcTrans, &quatInfo->destTrans, alpha);

        segCount--;

        quatInfoChunkCount--;

        quatInfo++;

        if (quatInfoChunkCount == 0)
        {
            stateBlockList = stateBlockList->next;

            quatInfoChunkCount = 4;

            quatInfo = stateBlockList->quatInfo;
        }
    }
}

void _G2AnimSection_SegValueToQuat(G2AnimSection *section, int zeroOne)
{
    G2AnimSegValue *segValue;
    G2AnimInterpInfo *interpInfo;
    G2AnimInterpStateBlock *stateBlockList;
    G2AnimQuatInfo *quatInfo;
    G2EulerAngles preQuat;
    int quatInfoChunkCount;
    int segCount;

    quatInfoChunkCount = 4;

    segCount = section->segCount;

    interpInfo = section->interpInfo;

    stateBlockList = interpInfo->stateBlockList;

    segValue = &_segValues[section->firstSeg];

    quatInfo = stateBlockList->quatInfo;

    while (segCount > 0)
    {
        preQuat.x = segValue->rotQuat.quat.x & 0xFFF;
        preQuat.y = segValue->rotQuat.quat.y & 0xFFF;
        preQuat.z = segValue->rotQuat.quat.z & 0xFFF;

        preQuat.order = 0;

        if (zeroOne == 0)
        {
            G2Quat_FromEuler_S(&quatInfo->srcQuat, &preQuat);

            quatInfo->srcScale.x = segValue->scale.x;
            quatInfo->srcScale.y = segValue->scale.y;
            quatInfo->srcScale.z = segValue->scale.z;

            quatInfo->srcTrans.x = segValue->trans.x;
            quatInfo->srcTrans.y = segValue->trans.y;
            quatInfo->srcTrans.z = segValue->trans.z;
        }
        else
        {
            G2Quat_FromEuler_S(&quatInfo->destQuat, &preQuat);

            quatInfo->destScale.x = segValue->scale.x;
            quatInfo->destScale.y = segValue->scale.y;
            quatInfo->destScale.z = segValue->scale.z;

            quatInfo->destTrans.x = segValue->trans.x;
            quatInfo->destTrans.y = segValue->trans.y;
            quatInfo->destTrans.z = segValue->trans.z;
        }

        segValue++;
        segCount--;

        quatInfo++;

        if (--quatInfoChunkCount == 0)
        {
            stateBlockList = stateBlockList->next;

            quatInfoChunkCount = 4;

            quatInfo = stateBlockList->quatInfo;
        }
    }
}

G2AnimInterpStateBlock *_G2Anim_AllocateInterpStateBlockList(G2AnimSection *section)
{
    G2AnimInterpInfo *interpInfo;
    G2AnimInterpStateBlock *newBlock;
    int segCount;

    segCount = section->segCount;

    interpInfo = section->interpInfo;

    interpInfo->stateBlockList = (G2AnimInterpStateBlock *)G2PoolMem_Allocate(&_interpStateBlockPool);

    if (interpInfo->stateBlockList != NULL)
    {
        newBlock = interpInfo->stateBlockList;

        while ((segCount -= 4) > 0)
        {
            newBlock->next = (G2AnimInterpStateBlock *)G2PoolMem_Allocate(&_interpStateBlockPool);

            if (newBlock->next == NULL)
            {
                _G2Anim_FreeInterpStateBlockList(interpInfo->stateBlockList);

                interpInfo->stateBlockList = NULL;

                return NULL;
            }

            newBlock = newBlock->next;
        }

        newBlock->next = NULL;
    }

    return interpInfo->stateBlockList;
}

void _G2Anim_FreeInterpStateBlockList(G2AnimInterpStateBlock *block)
{
    G2AnimInterpStateBlock *nextBlock;

    while (block != NULL)
    {
        nextBlock = block->next;

        G2PoolMem_Free(&_interpStateBlockPool, block);

        block = nextBlock;
    }
}
