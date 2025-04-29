#include "Game/G2/ANIMG2.h"
#include "Game/G2/POOLMMG2.h"
#include "Game/HASM.h"
#include "Game/PSX/PSX_G2/QUATVM.h"
#include "Game/G2/TIMERG2.h"
#include "Game/MATH3D.h"
#include "Game/G2/QUATG2.h"
#include "Game/G2/ANMINTRP.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/G2/ANMG2ILF.h"
#include "Game/G2/ANMDECMP.h"

static int _indexTable[] = {-1, -2, -1, -1, 1, 2, 4, 7, -1, -2, -1, -1, 1, 2, 4, 7};

static int _stepSizeTable[] = {
    7,
    8,
    9,
    0xA,
    0xB,
    0xC,
    0xD,
    0xE,
    0x10,
    0x12,
    0x14,
    0x16,
    0x18,
    0x1A,
    0x1D,
    0x20,
    0x23,
    0x26,
    0x29,
    0x2D,
    0x31,
    0x35,
    0x39,
    0x3E,
    0x43,
    0x48,
    0x4F,
    0x55,
    0x5B,
    0x61,
    0x68,
    0x6F,
    0x76,
    0x7E,
    0x6B,
    0x73,
    0x7C,
    0x85,
    0x8E,
    0x98,
    0xA2,
    0xAD,
    0xB8,
    0xC4,
    0xD0,
    0xDD,
    0xEA,
    0xF8,
    0x106,
    0x115,
    0x125,
    0x135,
    0x146,
    0x157,
    0x169,
    0x17C,
    0x190,
    0x1A5,
    0x1BB,
    0x1D2,
    0x1EB,
    0x206,
    0x223,
    0x243,
};

static G2AnimChanStatusBlockPool _chanStatusBlockPool;

static G2AnimSegValue _segValues[80];

static G2AnimControllerPool _controllerPool;

static G2AnimInterpStateBlockPool _interpStateBlockPool;

#include "Game/G2/ANMG2ILF.c"
#include "Game/G2/ANMDECMP.c"
#include "Game/G2/ANMCTRLR.c"
#include "Game/G2/ANMINTRP.c"

G2Bool G2Anim_Install()
{
    G2AnimController *dummyController;

    G2PoolMem_InitPool(&_chanStatusBlockPool, 180, sizeof(G2AnimChanStatusBlock));
    G2PoolMem_InitPool(&_interpStateBlockPool, 96, sizeof(G2AnimInterpStateBlock));
    G2PoolMem_InitPool(&_controllerPool, 122, sizeof(G2AnimController));

    dummyController = (G2AnimController *)G2PoolMem_Allocate(&_controllerPool);

    dummyController->next = 0;

    dummyController->segNumber = 255;

    dummyController->type = 0;

    return !G2FALSE;
}

void G2Anim_ResetInternalState()
{
    G2AnimController *dummyController;

    G2PoolMem_ResetPool(&_chanStatusBlockPool);
    G2PoolMem_ResetPool(&_interpStateBlockPool);
    G2PoolMem_ResetPool(&_controllerPool);

    dummyController = (G2AnimController *)G2PoolMem_Allocate(&_controllerPool);

    dummyController->next = 0;

    dummyController->segNumber = 255;

    dummyController->type = 0;
}

void G2Anim_Init(G2Anim *anim, Model *modelData)
{
    G2AnimSection *section;
    int sectionID;

    anim->sectionCount = 1;

    anim->masterSection = 0;

    anim->controllerList = 0;

    anim->disabledControllerList = 0;

    anim->segMatrices = NULL;

    anim->modelData = modelData;

    memset(&anim->disabledBits, 0, sizeof(anim->disabledBits) + sizeof(anim->section));

    for (sectionID = 0; sectionID < 3; sectionID++)
    {
        section = &anim->section[sectionID];

        section->storedTime = -1;

        section->swAlarmTable = NULL;

        section->speedAdjustment = 4096;

        section++;
    }

    anim->section[0].segCount = (unsigned char)modelData->numSegments;
}

G2AnimSection *G2Anim_AddSection(G2Anim *anim, int firstSegID, int segCount)
{
    G2AnimSection *section;

    section = &anim->section[anim->sectionCount];

    memset(section, 0, sizeof(G2AnimSection));

    section->sectionID = anim->sectionCount;

    section->storedTime = -1;

    section->firstSeg = firstSegID;

    section->segCount = segCount;

    section->swAlarmTable = NULL;

    section->speedAdjustment = 4096;

    anim->sectionCount++;

    return section;
}

void G2Anim_Free(G2Anim *anim)
{
    G2AnimSection *animSection;
    int sectionID;
    G2AnimInterpInfo *interpInfo;

    for (sectionID = 0; sectionID < anim->sectionCount; sectionID++)
    {
        animSection = &anim->section[sectionID];

        if (G2AnimSection_IsInInterpolation(animSection) != G2FALSE)
        {
            animSection->elapsedTime = animSection->interpInfo->targetTime;
        }

        _G2Anim_FreeChanStatusBlockList(animSection->chanStatusBlockList);

        interpInfo = animSection->interpInfo;

        animSection->chanStatusBlockList = NULL;

        if (interpInfo != NULL)
        {
            _G2Anim_FreeInterpStateBlockList(interpInfo->stateBlockList);

            interpInfo->stateBlockList = NULL;
        }
    }
}

void G2Anim_Restore(G2Anim *anim)
{
    G2AnimSection *section;
    int sectionID;

    for (sectionID = 0; sectionID < anim->sectionCount; sectionID++)
    {
        section = &anim->section[sectionID];

        if (section->keylist != NULL)
        {
            section->storedTime = -section->keylist->timePerKey;

            G2AnimSection_JumpToTime(section, section->elapsedTime);
        }
    }
}

void G2Anim_BuildTransforms(G2Anim *anim)
{
    unsigned short z;
    unsigned long xy;

    G2Anim_UpdateStoredFrame(anim);

    if ((anim->section[0].flags & 0x88) != 0x80)
    {
        ((int *)&anim->rootTrans.x)[0] = 0;
        anim->rootTrans.z = 0;
    }

    if ((anim->section[0].flags & 0x4))
    {
        anim->rootTrans.x = -anim->rootTrans.x;
        anim->rootTrans.y = -anim->rootTrans.y;
        anim->rootTrans.z = -anim->rootTrans.z;
    }

    z = anim->rootTrans.z;
    xy = ((unsigned long *)&anim->rootTrans.x)[0];

    _segValues[0].trans.z = z;
    ((unsigned long *)&_segValues[0].trans.x)[0] = xy;

    if (anim->controllerList != 0)
    {
        _G2Anim_BuildTransformsWithControllers(anim);
    }
    else
    {
        _G2Anim_BuildTransformsNoControllers(anim);
    }

    ((int *)&anim->rootTrans.x)[0] = 0;
    anim->rootTrans.z = 0;

    anim->section[0].flags &= 0x7F;
    anim->flags &= ~0x1;
}

void G2Anim_UpdateStoredFrame(G2Anim *anim)
{
    G2AnimSection *section;
    short storedTime;
    short elapsedTime;
    G2SVector3 motionVector;
    int sectionCount;
    G2AnimInterpInfo *interpInfo;
    G2SVector3 *vector;

    section = &anim->section[0];

    sectionCount = anim->sectionCount;

    storedTime = section->storedTime;

    elapsedTime = section->elapsedTime;

    for (; sectionCount > 0; sectionCount--)
    {
        interpInfo = section->interpInfo;

        if ((interpInfo != NULL) && (interpInfo->stateBlockList != NULL))
        {
            _G2AnimSection_UpdateStoredFrameFromQuat(section);
        }
        else
        {
            _G2AnimSection_UpdateStoredFrameFromData(section, anim);
        }

        section++;
    }

    vector = &motionVector;

    if (storedTime < 0)
    {
        storedTime = 0;
    }

    ((int *)&vector->x)[0] = 0;
    vector->z = 0;

    if (elapsedTime > storedTime)
    {
        G2Anim_GetRootMotionOverInterval(anim, storedTime, elapsedTime, vector);
    }
    else if (elapsedTime < storedTime)
    {
        G2Anim_GetRootMotionOverInterval(anim, elapsedTime, storedTime, vector);
    }

    if ((anim->section[0].flags & 0x4))
    {
        anim->rootTrans.x -= motionVector.x;
        anim->rootTrans.y -= motionVector.y;
        anim->rootTrans.z -= motionVector.z;
    }
    else
    {
        anim->rootTrans.x += motionVector.x;
        anim->rootTrans.y += motionVector.y;
        anim->rootTrans.z += motionVector.z;
    }
}

G2AnimSection *G2Anim_GetSectionWithSeg(G2Anim *anim, int segNumber)
{
    G2AnimSection *section;
    G2AnimSection *tempSection;
    G2AnimSection *endSection;
    int firstSeg;
    int lastSeg;

    section = NULL;

    endSection = &anim->section[anim->sectionCount];

    for (tempSection = &anim->section[0]; tempSection < endSection; tempSection++)
    {
        firstSeg = tempSection->firstSeg;

        lastSeg = firstSeg + tempSection->segCount;

        if ((segNumber >= firstSeg) && (segNumber < lastSeg))
        {
            section = tempSection;
            break;
        }
    }

    return section;
}

G2Bool G2Anim_SegmentHasActiveChannels(G2Anim *anim, int segNumber, unsigned short chanMask)
{
    G2AnimSection *section;
    unsigned char *segChanFlagStream;
    unsigned char activeChanBits;
    unsigned short dataFlagBits;
    unsigned short segFlagBits;
    int flagBytesPerSeg;

    section = G2Anim_GetSectionWithSeg(anim, segNumber);

    flagBytesPerSeg = ((anim->modelData->numSegments * 3) + 7) / 8;

    segNumber += segNumber * 2;

    segChanFlagStream = &((unsigned char *)&section->keylist->sectionData[section->keylist->sectionCount])[segNumber / 8] + 1;

    segNumber -= (segNumber / 8) * 8;

    activeChanBits = ((unsigned char *)&section->keylist->sectionData[section->keylist->sectionCount])[0];

    segFlagBits = 0;

    if ((activeChanBits & 0x1))
    {
        dataFlagBits = segChanFlagStream[0];
        dataFlagBits = (segChanFlagStream[1] << 8) | dataFlagBits;

        segFlagBits |= (dataFlagBits >> segNumber) & 0x7;

        segChanFlagStream += flagBytesPerSeg;
    }

    if ((activeChanBits & 0x2))
    {
        dataFlagBits = segChanFlagStream[0];
        dataFlagBits = (segChanFlagStream[1] << 8) | dataFlagBits;

        segFlagBits |= ((dataFlagBits >> segNumber) & 0x7) << 4;

        segChanFlagStream += flagBytesPerSeg;
    }

    if ((activeChanBits & 0x4))
    {
        dataFlagBits = segChanFlagStream[0];
        dataFlagBits = (segChanFlagStream[1] << 8) | dataFlagBits;

        segFlagBits |= ((dataFlagBits >> segNumber) & 0x7) << 8;
    }

    return (G2Bool)((segFlagBits & chanMask) > 0);
}

void G2Anim_GetSegChannelValue(G2Anim *anim, int segIndex, unsigned short *valueTable, unsigned short channelMask)
{
    unsigned short *chanFinalValue;
    unsigned short z;
    unsigned long xy;

    G2Anim_UpdateStoredFrame(anim);

    xy = ((unsigned long *)&anim->rootTrans.x)[0];
    z = anim->rootTrans.z;

    ((unsigned long *)&_segValues[0].trans.x)[0] = xy;
    _segValues[0].trans.z = z;

    _G2Anim_ApplyControllersToStoredFrame(anim);

    chanFinalValue = (unsigned short *)&_segValues[segIndex];

    while (channelMask != 0)
    {
        if ((channelMask & 0x1))
        {
            *valueTable++ = *chanFinalValue;
        }

        channelMask >>= 1;

        chanFinalValue++;
    }
}

void G2Anim_GetRootMotionFromTimeForDuration(G2Anim *anim, short durationStart, short duration, G2SVector3 *motionVector)
{
    G2Anim dummyAnim;
    G2AnimSection *section;
    G2AnimKeylist *keylist;
    short storedKeyEndTime;
    short timePerKey;
    short keyTime;
    long alpha;
    G2AnimInterpInfo *interpInfo;

    section = &anim->section[0];

    interpInfo = section->interpInfo;

    if ((interpInfo != NULL) && (interpInfo->stateBlockList != NULL))
    {
        // G2SVector3 *dest; // unused   
        G2SVector3 *base;
        G2SVector3 *offset;
        // long alpha; // unused
        G2AnimInterpStateBlock *stateBlockList;

        timePerKey = interpInfo->duration;

        stateBlockList = interpInfo->stateBlockList;

        alpha = (durationStart << 12) / timePerKey;
        alpha = _G2AnimAlphaTable_GetValue(interpInfo->alphaTable, alpha);

        base = &stateBlockList->quatInfo->srcTrans;
        offset = &stateBlockList->quatInfo->destTrans;

        gte_ldlvnlsv(base);

        gte_ldsv(offset);

        gte_lddp(alpha);

        gte_ngpl12();

        gte_stlvnlsv(motionVector);

        keylist = section->keylist;

        alpha = (duration << 12) / keylist->timePerKey;

        motionVector->x = (motionVector->x * alpha) >> 12;
        motionVector->y = (motionVector->y * alpha) >> 12;
        motionVector->z = (motionVector->z * alpha) >> 12;
    }
    else
    {
        G2SVector3 *vector;
        short temp; // not from decls.h

        keylist = section->keylist;

        timePerKey = keylist->timePerKey;

        storedKeyEndTime = (durationStart / keylist->timePerKey) + 1;
        storedKeyEndTime = keylist->timePerKey * storedKeyEndTime;

        temp = keylist->timePerKey * (keylist->keyCount - 1);

        dummyAnim.sectionCount = 1;

        dummyAnim.modelData = anim->modelData;

        dummyAnim.section->sectionID = 0;

        dummyAnim.section->firstSeg = 0;

        dummyAnim.section->segCount = 1;

        dummyAnim.section->keylist = keylist;

        dummyAnim.section->chanStatusBlockList = NULL;

        dummyAnim.section->storedTime = -timePerKey;

        ((int *)&motionVector->x)[0] = 0;
        motionVector->z = 0;

        while (duration != 0)
        {
            // G2SVector3 *dest; // unused
            // G2SVector3 *base; // unused
            // long alpha; // unused      

            if (durationStart >= temp)
            {
                timePerKey = keylist->s0TailTime;
            }

            dummyAnim.section->elapsedTime = durationStart;

            _G2AnimSection_UpdateStoredFrameFromData(&dummyAnim.section[0], &dummyAnim);

            keyTime = storedKeyEndTime - durationStart;
            keyTime = (duration < keyTime) ? duration : keyTime;

            if (keyTime < timePerKey)
            {
                alpha = (keyTime << 12) / timePerKey;
            }
            else
            {
                alpha = 4096;
            }

            gte_ldlvnlsv(motionVector);

            vector = &_segValues->trans;

            gte_ldsv(vector);

            gte_lddp(alpha);

            gte_ngpl12();

            gte_stlvnlsv(motionVector);

            durationStart = storedKeyEndTime;

            duration -= keyTime;

            storedKeyEndTime += timePerKey;
        }

        _G2Anim_FreeChanStatusBlockList(dummyAnim.section->chanStatusBlockList);
    }
}

void G2AnimSection_SwitchToKeylistAtTime(G2AnimSection *section, G2AnimKeylist *keylist, int keylistID, short targetTime)
{
    G2Anim *anim;
    G2SVector3 rootMotion;
    G2AnimInterpInfo *interpInfo;
    unsigned short z;
    unsigned long xy;

    anim = _G2AnimSection_GetAnim(section);

    if (section->firstSeg == 0)
    {
        anim->flags |= 0x1;

        if ((section->keylist != NULL) && (section->storedTime >= 0))
        {
            G2Anim_GetRootMotionOverInterval(anim, section->storedTime, section->elapsedTime, &rootMotion);
        }
        else
        {
            *(unsigned long *)&rootMotion.x = 0;
            rootMotion.z = 0;
        }

        rootMotion.x += anim->rootTrans.x;
        rootMotion.y += anim->rootTrans.y;
        rootMotion.z += anim->rootTrans.z;
    }

    interpInfo = section->interpInfo;

    if ((interpInfo != NULL) && (interpInfo->stateBlockList != NULL))
    {
        _G2Anim_FreeInterpStateBlockList(interpInfo->stateBlockList);

        interpInfo->stateBlockList = NULL;
    }

    G2AnimSection_ClearAlarm(section, 0x3);

    if (keylist != section->keylist)
    {
        section->keylist = keylist;
        section->keylistID = keylistID;

        section->storedTime = -section->keylist->timePerKey;
    }

    G2AnimSection_JumpToTime(section, targetTime);

    if (section->firstSeg == 0)
    {
        section->flags |= 0x80;

        xy = *(unsigned long *)&rootMotion.x;
        z = rootMotion.z;

        *(unsigned long *)&anim->rootTrans.x = xy;
        anim->rootTrans.z = z;
    }

    if ((section->flags & 0x2))
    {
        G2AnimSection_SetLoopRangeAll(section);
    }

    G2AnimSection_SetUnpaused(section);

    section->swAlarmTable = NULL;
}

void G2AnimSection_JumpToTime(G2AnimSection *section, short targetTime)
{
    G2Anim *anim;

    anim = _G2AnimSection_GetAnim(section);

    if (targetTime < section->elapsedTime)
    {
        section->storedTime = -section->keylist->timePerKey;
    }

    section->elapsedTime = targetTime;

    _G2AnimSection_UpdateStoredFrameFromData(section, anim);

    G2AnimSection_ClearAlarm(section, 0x3);

    section->flags &= 0x7F;

    if (section->firstSeg == 0)
    {
        *(unsigned int *)&anim->rootTrans.x = 0;
        anim->rootTrans.z = 0;
    }
}

short G2AnimSection_UpdateOverInterval(G2AnimSection *section, short interval)
{
    G2Anim *anim;
    G2SVector3 motionVector;
    G2AnimInterpInfo *interpInfo;
    short elapsedTime;
    unsigned short z;
    unsigned long xy;
    short temp; // not from decls.h

    if ((section->flags & 0x1))
    {
        return 0;
    }

    interpInfo = section->interpInfo;

    if ((interpInfo != NULL) && (interpInfo->stateBlockList != NULL))
    {
        anim = _G2AnimSection_GetAnim(section);

        anim->flags |= 0x1;

        elapsedTime = section->elapsedTime;

        temp = (elapsedTime + (short)((interval * section->speedAdjustment) >> 12)) - interpInfo->duration;

        if (temp >= 0)
        {
            section->storedTime = -section->keylist->timePerKey;

            G2AnimSection_JumpToTime(section, interpInfo->targetTime);

            if (section->firstSeg == 0)
            {
                G2Anim_GetRootMotionOverInterval(anim, elapsedTime, interpInfo->duration, &motionVector);

                xy = *(unsigned long *)&motionVector.x;
                z = motionVector.z;

                *(unsigned long *)&anim->rootTrans.x = xy;
                anim->rootTrans.z = z;

                section->flags |= 0x80;
            }

            _G2Anim_FreeInterpStateBlockList(interpInfo->stateBlockList);

            interpInfo->stateBlockList = NULL;

            if ((section->flags & 0x2))
            {
                G2AnimSection_SetLoopRangeAll(section);
            }

            section->alarmFlags |= 0x10;

            if (section->callback != NULL)
            {
                section->callback(anim, section->sectionID, G2ANIM_MSG_SECTION_INTERPDONE, 0, 0, (Instance *)section->callbackData);
            }

            return temp;
        }
        else
        {
            section->elapsedTime = elapsedTime + (short)((interval * section->speedAdjustment) >> 12);
            return 0;
        }
    }

    if (!(section->flags & 0x4))
    {
        return G2AnimSection_AdvanceOverInterval(section, interval);
    }
    else
    {
        return G2AnimSection_RewindOverInterval(section, interval);
    }
}

short G2AnimSection_AdvanceOverInterval(G2AnimSection *section, short interval)
{
    G2AnimKeylist *keylist;
    short newTime;
    short extraTime;
    short elapsedTime;
    short endTime;
    short loopExtraTime;
    short *swAlarmTable;
    short swAlarmTime;
    G2Anim *anim;
    G2SVector3 motionVector;
    unsigned long message;

    loopExtraTime = 0;

    if (((section->flags & 0x1)) || ((section->alarmFlags & 0x1)))
    {
        return 0;
    }

    anim = _G2AnimSection_GetAnim(section);

    anim->flags |= 0x1;

    G2AnimSection_ClearAlarm(section, 0x3);

    keylist = section->keylist;

    elapsedTime = section->elapsedTime;

    section->flags &= ~0x4;

    if ((section->flags & 0x2))
    {
        endTime = section->loopEndTime;
    }
    else
    {
        endTime = G2AnimKeylist_GetDuration(keylist);
    }

    newTime = elapsedTime + ((interval * section->speedAdjustment) >> 12);

    if (section->swAlarmTable != NULL)
    {
        swAlarmTable = section->swAlarmTable;

        while ((swAlarmTime = *swAlarmTable) != -1)
        {
            do // this do while is fake, only the code within is valid
            {
                if (((elapsedTime < swAlarmTime) && (newTime >= swAlarmTime)) || ((section->storedTime <= 0) && (elapsedTime == swAlarmTime)))
                {
                    section->alarmFlags |= 0x20;

                    if (section->callback != NULL)
                    {
                        section->callback(anim, section->sectionID, G2ANIM_MSG_SWALARMSET, elapsedTime, newTime, section->callbackData);
                    }
                }
            } while (0);

            swAlarmTable++;
        }
    }
    else
    {
        G2AnimSection_ClearAlarm(section, 0x20);
    }

    _G2AnimSection_TriggerEffects(section, elapsedTime, newTime);

    extraTime = newTime - endTime;

    while (1)
    {
        if (extraTime >= 0)
        {
            if ((section->flags & 0x2))
            {
                message = 2;

                section->alarmFlags |= 0x4;

                G2AnimSection_JumpToTime(section, section->loopStartTime);

                newTime = section->loopStartTime + extraTime;

                loopExtraTime = newTime - endTime;

                _G2AnimSection_TriggerEffects(section, section->loopStartTime - 1, newTime);

                if (newTime >= endTime)
                {
                    newTime = endTime - 1;
                }
            }
            else
            {
                message = 1;

                newTime = endTime - 1;

                section->alarmFlags |= 0x1;
            }

            if (section->firstSeg == 0)
            {
                anim = _G2AnimSection_GetAnim(section);

                G2Anim_GetRootMotionOverInterval(anim, elapsedTime, endTime, &motionVector);
            }

            if (section->callback != NULL)
            {
                swAlarmTime = section->callback(_G2AnimSection_GetAnim(section), section->sectionID, message, newTime, extraTime, section->callbackData);

                if (swAlarmTime != newTime)
                {
                    newTime = swAlarmTime;

                    G2AnimSection_JumpToTime(section, swAlarmTime);
                }
                else if ((section->flags & 0x2))
                {
                    G2AnimSection_JumpToTime(section, swAlarmTime);

                    section->storedTime = section->loopStartTime;
                }
                else
                {
                    *(unsigned int *)&motionVector.x = 0;
                    motionVector.z = 0;
                }
            }

            if (section->firstSeg == 0)
            {
                unsigned short z;
                unsigned long xy;

                xy = *(int *)&motionVector.x;
                z = motionVector.z;

                *(int *)&anim->rootTrans.x = xy;
                anim->rootTrans.z = z;

                section->flags |= 0x80;
            }

            if (!(section->flags & 0x2))
            {
                break;
            }

            endTime = section->loopEndTime;
            extraTime = loopExtraTime;
        }
        else
        {
            extraTime = 0;
            break;
        }
    }

    section->elapsedTime = newTime;

    return extraTime;
}

short G2AnimSection_RewindOverInterval(G2AnimSection *section, short interval)
{
    short newTime;
    short extraTime;
    short elapsedTime;
    short endTime;
    G2Anim *anim;
    G2SVector3 motionVector;
    unsigned long message;
    short temp; // not from decls.h

    if (((section->flags & 0x1)) || ((section->alarmFlags & 0x2)))
    {
        return 0;
    }

    anim = _G2AnimSection_GetAnim(section);

    anim->flags |= 0x1;

    G2AnimSection_ClearAlarm(section, 3);

    endTime = 0;

    elapsedTime = section->elapsedTime;

    section->flags |= 0x4;

    if ((section->flags & 0x2))
    {
        endTime = section->loopStartTime;
    }

    newTime = elapsedTime - ((interval * section->speedAdjustment) >> 12);

    extraTime = endTime - newTime;

    if (extraTime > 0)
    {
        if ((section->flags & 0x2))
        {
            section->alarmFlags |= 0x4;

            G2AnimSection_JumpToTime(section, section->loopEndTime);

            message = 2;

            newTime = section->loopEndTime - extraTime;
        }
        else
        {
            message = 1;

            newTime = endTime;

            section->alarmFlags |= 0x2;
        }

        if (section->firstSeg == 0)
        {
            G2Anim_GetRootMotionOverInterval(anim, elapsedTime, endTime, &motionVector);
        }

        if (section->callback != NULL)
        {
            temp = section->callback(_G2AnimSection_GetAnim(section), section->sectionID, message, newTime, extraTime, section->callbackData);

            if (((section->flags & 0x2)) || (temp != newTime))
            {
                newTime = temp;

                G2AnimSection_JumpToTime(section, temp);
            }
            else
            {
                *(unsigned long *)&motionVector.x = 0;
                motionVector.z = 0;
            }
        }

        if (section->firstSeg == 0)
        {
            unsigned short z;
            unsigned long xy;

            xy = *(unsigned long *)&motionVector.x;
            z = motionVector.z;

            *(unsigned long *)&anim->rootTrans.x = xy;
            anim->rootTrans.z = z;

            section->flags |= 0x80;
        }
    }
    else
    {
        extraTime = 0;
    }

    section->elapsedTime = newTime;

    return extraTime;
}

void _G2Anim_BuildTransformsNoControllers(G2Anim *anim)
{
    Segment *segment;
    G2Matrix *segMatrix;
    G2Bool bRootTransUpdated;
    int segIndex;
    int segCount;
    unsigned long disabledBits[3];
    unsigned long disabledMask;
    unsigned long parentMask;
    unsigned long parentIndex;
    unsigned char flags; // not from decls.h

    flags = anim->section->flags;

    segment = anim->modelData->segmentList;

    segMatrix = anim->segMatrices;

    disabledMask = 0x1;

    bRootTransUpdated = ((flags & 0x80)) && (!(flags & 0x8));

    disabledBits[0] = anim->disabledBits[0];
    disabledBits[1] = anim->disabledBits[1];
    disabledBits[2] = anim->disabledBits[2];

    segCount = anim->modelData->numSegments;

    segIndex = 0;
    parentIndex = 0;

    while (segIndex < segCount)
    {
        if (segment->parent != -1)
        {
            parentMask = 1 << (segment->parent & 0x1F);

            if ((disabledBits[segment->parent >> 5] & parentMask))
            {
                disabledBits[parentIndex] |= disabledMask;
            }
        }

        if (!(disabledBits[parentIndex] & disabledMask))
        {
            _G2Anim_BuildSegTransformNoControllers(segMatrix, &anim->segMatrices[segment->parent], bRootTransUpdated, segIndex);
        }

        bRootTransUpdated = G2FALSE;

        segment++;

        segMatrix++;
        segIndex++;

        disabledMask <<= 1;

        if (disabledMask == 0)
        {
            parentIndex++;

            disabledMask = 0x1;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/G2/ANIMG2", _G2Anim_BuildSegTransformNoControllers);

void _G2Anim_BuildSegLocalRotMatrix(G2AnimSegValue *segValue, G2Matrix *segMatrix)
{
    G2SVector3 rot;
    G2SVector3 *source;
    G2SVector3 *dest;
    unsigned long mask;
    unsigned short z;
    unsigned long xy;

    source = &segValue->rotQuat.rot;

    if (segValue->bIsQuat != 0)
    {
        G2Quat_ToMatrix_S(&segValue->rotQuat.quat, segMatrix);
    }
    else
    {
        mask = 0xFFF0FFF;

        dest = &rot;

        z = source->z & 0xFFF;
        xy = *(unsigned long *)&source->x & mask;

        dest->z = z;
        *(unsigned long *)&rot.x = xy;

        RotMatrixZYX((SVECTOR *)dest, (MATRIX *)segMatrix);
    }
}

void wombat(unsigned char *segKeyList, int flagBitOffset, G2AnimSegKeyflagInfo *kfInfo)
{
    int flagDWordOffset;
    int flagBitShift;

    flagDWordOffset = flagBitOffset >> 5;

    segKeyList = &segKeyList[flagDWordOffset << 2];

    flagBitShift = flagBitOffset - (flagDWordOffset << 5);

    kfInfo->stream = (unsigned long *)segKeyList;

    kfInfo->flags = *kfInfo->stream >> flagBitShift;

    kfInfo->bitCount = 32 - (flagBitOffset & 0x1F);
}

unsigned long kangaroo(G2AnimSegKeyflagInfo *kfInfo)
{
    unsigned long keyflags;
    unsigned long tempFlags;

    keyflags = 0;

    if (kfInfo->stream != NULL)
    {
        keyflags = kfInfo->flags & 0x7;

        kfInfo->flags >>= 3;

        kfInfo->bitCount -= 3;

        if (kfInfo->bitCount <= 0)
        {
            kfInfo->stream = &kfInfo->stream[1];

            kfInfo->flags = kfInfo->stream[0];

            if (kfInfo->bitCount < 0)
            {
                tempFlags = (kfInfo->flags << (kfInfo->bitCount + 3)) & 0x7;

                keyflags |= tempFlags;

                kfInfo->flags >>= -kfInfo->bitCount;
            }

            kfInfo->bitCount += 32;
        }
    }

    return keyflags;
}

void _G2Anim_InitializeSegValue(G2Anim *anim, G2AnimSegValue *segValue, int segIndex)
{
    Segment *segment;
    // G2Quat *quat; // unused
    // unsigned long zpad; // unused
    // unsigned long xy; // unused

    SET_QUAT_FAST(&segValue->rotQuat.quat, 0, 0, 0, 4096);
    SET_SVEC3_FAST(&segValue->scale, 4096, 4096, 4096);

    segment = &anim->modelData->segmentList[segIndex];

    COPY_SVEC3_FAST((G2SVector3 *)&segValue->trans, (G2SVector3 *)&segment->px);
}

void _G2AnimSection_InitStatus(G2AnimSection *section, G2Anim *anim)
{
    G2AnimDecompressChannelInfo dcInfo;
    G2AnimSegValue *segValue;
    G2AnimChanStatusBlock **chanStatusNextBlockPtr;
    G2AnimChanStatusBlock *chanStatusBlock;
    G2AnimChanStatus *chanStatus;
    G2AnimSegKeyflagInfo rotKfInfo;
    G2AnimSegKeyflagInfo scaleKfInfo;
    G2AnimSegKeyflagInfo transKfInfo;
    int type;
    unsigned long segChanFlags;
    int segIndex;
    int lastSeg;
    int flagBitOffset;
    unsigned long activeChanBits;
    unsigned char *segKeyList;
    int bitsPerFlagType;
    // int chanStatusChunkCount; // unused

    flagBitOffset = ((section->firstSeg << 1) + section->firstSeg) + 8;

    bitsPerFlagType = ((anim->modelData->numSegments << 1) + anim->modelData->numSegments) + 7;

    segKeyList = (unsigned char *)&section->keylist->sectionData[section->keylist->sectionCount];

    activeChanBits = *segKeyList;

    bitsPerFlagType &= ~0x7;

    rotKfInfo.stream = NULL;
    scaleKfInfo.stream = NULL;
    transKfInfo.stream = NULL;

    if ((activeChanBits & 0x1))
    {
        wombat(segKeyList, flagBitOffset, &rotKfInfo);

        flagBitOffset += bitsPerFlagType;
    }

    if ((activeChanBits & 0x2))
    {
        wombat(segKeyList, flagBitOffset, &scaleKfInfo);

        flagBitOffset += bitsPerFlagType;
    }

    if ((activeChanBits & 0x4))
    {
        wombat(segKeyList, flagBitOffset, &transKfInfo);
    }

    chanStatus = NULL;

    _G2Anim_FreeChanStatusBlockList(section->chanStatusBlockList);

    section->chanStatusBlockList = NULL;

    activeChanBits = 0;

    dcInfo.keylist = section->keylist;

    dcInfo.chanData = section->keylist->sectionData[section->sectionID];

    segIndex = section->firstSeg;

    lastSeg = segIndex + section->segCount;

    segValue = &_segValues[segIndex];

    chanStatusNextBlockPtr = &section->chanStatusBlockList;

    for (; segIndex < lastSeg; segIndex++, segValue++)
    {
        segChanFlags = kangaroo(&rotKfInfo);

        segChanFlags |= kangaroo(&scaleKfInfo) << 4;
        segChanFlags |= kangaroo(&transKfInfo) << 8;

        _G2Anim_InitializeSegValue(anim, segValue, segIndex);

        while (segChanFlags != 0)
        {
            if ((segChanFlags & 0x1))
            {
                type = ((unsigned char *)dcInfo.chanData)[1] & 0xE0; // TODO: ensure that this cast doesn't cause issues

                if (type == 0xE0)
                {
                    type = 0;
                }

                switch (type)
                {
                case 0:
                    dcInfo.chanData = &dcInfo.chanData[dcInfo.keylist->keyCount];
                    break;
                case 0x20:
                    dcInfo.chanData = &dcInfo.chanData[2];
                    break;
                default:
                    if (activeChanBits == 0)
                    {
                        activeChanBits = 8;

                        chanStatusBlock = (G2AnimChanStatusBlock *)G2PoolMem_Allocate(&_chanStatusBlockPool);

                        chanStatusBlock->next = NULL;

                        *chanStatusNextBlockPtr = chanStatusBlock;

                        chanStatusNextBlockPtr = (G2AnimChanStatusBlock **)chanStatusBlock;

                        chanStatus = ((G2AnimChanStatusBlock *)chanStatusNextBlockPtr)->chunks;
                    }

                    switch (type)
                    {
                    case 0x40:
                        _G2Anim_InitializeChannel_AdaptiveDelta(&dcInfo, chanStatus);
                        break;
                    case 0x60:
                        _G2Anim_InitializeChannel_Linear(&dcInfo, chanStatus);
                        break;
                    }

                    chanStatus++;

                    activeChanBits--;
                    break;
                }
            }

            segChanFlags >>= 1;
        }
    }

    section->storedTime = -section->keylist->timePerKey;
}

void FooBar(G2AnimSection *section, G2Anim *anim, int decompressedKey, int targetKey, long timeOffset)
{
    G2AnimDecompressChannelInfo dcInfo;
    G2AnimSegValue *segValue;
    short *chanValue;
    G2AnimChanStatusBlock *chanStatusBlock;
    G2AnimChanStatus *chanStatus;
    int chanStatusChunkCount;
    G2AnimSegKeyflagInfo rotKfInfo;
    G2AnimSegKeyflagInfo scaleKfInfo;
    G2AnimSegKeyflagInfo transKfInfo;
    int type;
    unsigned long segChanFlags;
    int segIndex;
    int lastSeg;
    G2AnimDecompressChannelInfo nextDCInfo;
    G2AnimDecompressChannelInfo initDCInfo;
    G2AnimChanStatus nextChanStatus;
    // int bitsPerFlagType; // unused
    int flagBitOffset;
    unsigned long activeChanBits;
    unsigned char *segKeyList;

    flagBitOffset = ((section->firstSeg << 1) + section->firstSeg) + 8;

    segChanFlags = ((anim->modelData->numSegments << 1) + anim->modelData->numSegments) + 7;

    segKeyList = (unsigned char *)&section->keylist->sectionData[section->keylist->sectionCount];

    activeChanBits = *segKeyList;

    segChanFlags &= ~0x7;

    rotKfInfo.stream = NULL;
    scaleKfInfo.stream = NULL;
    transKfInfo.stream = NULL;

    if ((activeChanBits & 0x1))
    {
        wombat(segKeyList, flagBitOffset, &rotKfInfo);

        flagBitOffset += segChanFlags;
    }

    if ((activeChanBits & 0x2))
    {
        wombat(segKeyList, flagBitOffset, &scaleKfInfo);

        flagBitOffset += segChanFlags;
    }

    if ((activeChanBits & 0x4))
    {
        wombat(segKeyList, flagBitOffset, &transKfInfo);
    }

    chanStatusBlock = section->chanStatusBlockList;

    chanStatusChunkCount = 8;

    chanStatus = &chanStatusBlock->chunks[0];

    dcInfo.keylist = section->keylist;

    dcInfo.chanData = section->keylist->sectionData[section->sectionID];

    dcInfo.storedKey = decompressedKey;
    dcInfo.targetKey = targetKey;

    if (timeOffset != 0)
    {
        nextDCInfo.keylist = dcInfo.keylist;

        nextDCInfo.chanData = dcInfo.chanData;

        nextDCInfo.storedKey = targetKey;
        nextDCInfo.targetKey = targetKey + 1;

        if (nextDCInfo.targetKey >= section->keylist->keyCount)
        {
            if ((section->flags & 0x2))
            {
                initDCInfo.keylist = dcInfo.keylist;

                initDCInfo.chanData = dcInfo.chanData;

                nextDCInfo.storedKey = -1;
                nextDCInfo.targetKey = 0;
            }
            else
            {
                timeOffset = 0;
            }
        }
    }

    segIndex = section->firstSeg;

    segValue = &_segValues[segIndex];

    lastSeg = segIndex + section->segCount;

    for (; segIndex < lastSeg; segValue++, segIndex++)
    {
        _G2Anim_InitializeSegValue(anim, segValue, segIndex);

        segChanFlags = kangaroo(&rotKfInfo);

        segChanFlags |= kangaroo(&scaleKfInfo) << 4;
        segChanFlags |= kangaroo(&transKfInfo) << 8;

        chanValue = (short *)segValue;

        while (segChanFlags != 0)
        {
            if ((segChanFlags & 0x1))
            {
                type = ((unsigned char *)dcInfo.chanData)[1] & 0xE0; // TODO: ensure that this cast doesn't cause issues

                if (type == 0xE0)
                {
                    type = 0;
                }

                switch (type)
                {
                case 0:
                    *chanValue = dcInfo.chanData[targetKey];

                    dcInfo.chanData += dcInfo.keylist->keyCount;
                    break;
                case 0x20:
                    *chanValue = dcInfo.chanData[1];

                    dcInfo.chanData += 2;
                    break;
                default:
                    switch (type)
                    {
                    case 0x40:
                        _G2Anim_DecompressChannel_AdaptiveDelta(&dcInfo, chanStatus);
                        break;
                    case 0x60:
                        _G2Anim_DecompressChannel_Linear(&dcInfo, chanStatus);
                        break;
                    }

                    chanStatusChunkCount--;

                    *chanValue = chanStatus->keyData;

                    nextChanStatus = chanStatus[0];

                    chanStatus++;

                    if (chanStatusChunkCount == 0)
                    {
                        chanStatusBlock = chanStatusBlock->next;

                        chanStatusChunkCount = 8;

                        chanStatus = chanStatusBlock->chunks;
                    }

                    break;
                }

                if (timeOffset != 0)
                {
                    switch (type)
                    {
                    case 0:
                        nextChanStatus.keyData = nextDCInfo.chanData[nextDCInfo.targetKey];

                        nextDCInfo.chanData = dcInfo.chanData;
                        initDCInfo.chanData = dcInfo.chanData;
                        break;
                    case 0x20:
                        nextChanStatus.keyData = *chanValue;

                        nextDCInfo.chanData = dcInfo.chanData;
                        initDCInfo.chanData = dcInfo.chanData;
                        break;
                    case 0x40:
                        if (nextDCInfo.storedKey == -1)
                        {
                            _G2Anim_InitializeChannel_AdaptiveDelta(&initDCInfo, &nextChanStatus);
                        }

                        _G2Anim_DecompressChannel_AdaptiveDelta(&nextDCInfo, &nextChanStatus);
                        break;
                    case 0x60:
                        if (nextDCInfo.storedKey == -1)
                        {
                            _G2Anim_InitializeChannel_Linear(&initDCInfo, &nextChanStatus);
                        }

                        _G2Anim_DecompressChannel_Linear(&nextDCInfo, &nextChanStatus);
                        break;
                    }

                    nextChanStatus.keyData -= *chanValue;

                    if (nextChanStatus.keyData >= 2048)
                    {
                        nextChanStatus.keyData -= 4096;
                    }

                    if (nextChanStatus.keyData <= -2048)
                    {
                        nextChanStatus.keyData += 4096;
                    }

                    if (segIndex != 0)
                    {
                        *chanValue += (short)((nextChanStatus.keyData * timeOffset) >> 12);
                    }
                }
            }

            segChanFlags >>= 1;

            chanValue++;
        }
    }
}

void _G2AnimSection_UpdateStoredFrameFromData(G2AnimSection *section, G2Anim *anim)
{
    short timePerKey;
    long storedKey;
    long targetKey;
    long timeOffset;

    storedKey = section->storedTime / section->keylist->timePerKey;

    timePerKey = section->keylist->timePerKey;

    targetKey = section->elapsedTime / timePerKey;

    if ((storedKey < 0) || (targetKey < storedKey))
    {
        _G2AnimSection_InitStatus(section, anim);

        storedKey = -1;
    }

    timeOffset = ((section->elapsedTime - (targetKey * timePerKey)) << 12) / timePerKey;

    FooBar(section, anim, storedKey, targetKey, timeOffset);

    section->storedTime = section->elapsedTime;

    section->flags |= 0x80;
}

G2Anim *_G2AnimSection_GetAnim(G2AnimSection *section)
{
    return (G2Anim *)((char *)section - ((section->sectionID * sizeof(G2AnimSection)) + 36));
}

void _G2AnimSection_TriggerEffects(G2AnimSection *section, short startTime, short endTime)
{
    G2AnimKeylist *keylist;
    G2AnimFxHeader *fxHeader;
    int fxSectionID;
    unsigned long fxSize;
    G2Anim *Anim;

    keylist = section->keylist;

    if (keylist->fxList != NULL)
    {
        fxHeader = keylist->fxList;

        while (fxHeader->type != -1)
        {
            fxSectionID = fxHeader->sizeAndSection & 0xF;
            fxSize = (fxHeader->sizeAndSection & 0xF0) >> 2;

            if (fxSectionID == section->sectionID)
            {
                short time;

                time = fxHeader->keyframeID * keylist->s0TailTime;

                if (((startTime < time) || ((time == 0) && (time >= startTime))) && (endTime >= time) && (section->callback != NULL))
                {
                    Anim = _G2AnimSection_GetAnim(section);

                    section->callback(Anim, section->sectionID, G2ANIM_MSG_PLAYEFFECT, fxHeader->type, &fxHeader[1], (Instance *)section->callbackData);
                }
            }

            fxHeader = (G2AnimFxHeader *)((unsigned char *)fxHeader + fxSize);
        }
    }
}

void _G2Anim_FreeChanStatusBlockList(G2AnimChanStatusBlock *block)
{
    G2AnimChanStatusBlock *nextBlock;

    while (block != NULL)
    {
        nextBlock = block->next;

        G2PoolMem_Free(&_chanStatusBlockPool, block);

        block = nextBlock;
    }
}

long _G2AnimAlphaTable_GetValue(G2AnimAlphaTable *table, long trueAlpha)
{
    long position;
    long positionInt;
    long positionFrac;
    long value;

    if (table == NULL)
    {
        return trueAlpha;
    }

    value = trueAlpha;

    position = (table->size - 1) * value;

    positionInt = position >> 12;

    value = table->data[positionInt];

    positionFrac = position & 0xFFF;

    value += ((table->data[positionInt + 1] - value) * positionFrac) >> 12;

    return value;
}
