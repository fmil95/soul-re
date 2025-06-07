#include "Game/G2/ANMG2ILF.h"

short G2Anim_GetElapsedTime(G2Anim *anim)
{
    G2AnimSection *section;

    section = &anim->section[anim->masterSection];

    return section->elapsedTime;
}

G2AnimKeylist *G2Anim_GetKeylist(G2Anim *anim)
{
    G2AnimSection *section;

    section = &anim->section[anim->masterSection];

    return section->keylist;
}

void G2Anim_GetRootMotionOverInterval(G2Anim *anim, short intervalStart, short intervalEnd, G2SVector3 *motionVector)
{
    short temp; // not from decls.h

    temp = intervalEnd - intervalStart;

    G2Anim_GetRootMotionFromTimeForDuration(anim, intervalStart, temp, motionVector);
}

void G2Anim_InterpToKeylistFrame(G2Anim *anim, G2AnimKeylist *keylist, int keylistID, int targetFrame, short duration)
{
    int i;

    for (i = 0; i < anim->sectionCount; i++)
    {
        G2AnimSection_InterpToKeylistFrame(&anim->section[i], keylist, keylistID, targetFrame, duration);
    }
}

void G2Anim_SetAlphaTable(G2Anim *anim, G2AnimAlphaTable *table)
{
    int i;

    for (i = 0; i < anim->sectionCount; i++)
    {
        G2AnimSection_SetAlphaTable(&anim->section[i], table);
    }
}

void G2Anim_SetCallback(G2Anim *anim, long (*callback)(), void *data)
{
    int i;
    G2AnimSection *section;

    for (i = 0; i < anim->sectionCount; i++)
    {
        section = ((G2Anim *)&((G2AnimSection *)anim)[i])->section;

        section->callback = (void *)callback;
        section->callbackData = data;
    }
}

void G2Anim_SetLooping(G2Anim *anim)
{
    int i;

    for (i = 0; i < anim->sectionCount; i++)
    {
        G2AnimSection_SetLooping(&anim->section[i]);
    }
}

void G2Anim_SetNoLooping(G2Anim *anim)
{
    int i;

    for (i = 0; i < anim->sectionCount; i++)
    {
        G2AnimSection_SetNoLooping(&anim->section[i]);
    }
}

void G2Anim_SetPaused(G2Anim *anim)
{
    int i;

    for (i = 0; i < anim->sectionCount; i++)
    {
        G2AnimSection_SetPaused(&anim->section[i]);
    }
}

void G2Anim_SetSpeedAdjustment(G2Anim *anim, long adjustment)
{
    int i;

    for (i = 0; i < anim->sectionCount; i++)
    {
        anim->section[i].speedAdjustment = adjustment;
    }
}

void G2Anim_SetUnpaused(G2Anim *anim)
{
    int i;

    for (i = 0; i < anim->sectionCount; i++)
    {
        G2AnimSection_SetUnpaused(&anim->section[i]);
    }
}

void G2Anim_SwitchToKeylist(G2Anim *anim, G2AnimKeylist *keylist, int keylistID)
{
    int i;

    for (i = 0; i < anim->sectionCount; i++)
    {
        G2AnimSection_SwitchToKeylist(&anim->section[i], keylist, keylistID);
    }
}

short G2AnimKeylist_GetDuration(G2AnimKeylist *keylist)
{
    return keylist->s0TailTime + (keylist->timePerKey * (keylist->keyCount - 1));
}

int G2AnimKeylist_GetKeyframeCount(G2AnimKeylist *keylist)
{
    return ((keylist->timePerKey * (keylist->keyCount - 1)) + ((keylist->s0TailTime * 2) - 1)) / keylist->s0TailTime;
}

void G2AnimSection_ClearAlarm(G2AnimSection *section, unsigned long flag)
{
    section->alarmFlags &= ~flag;
}

int G2AnimSection_GetKeyframeNumber(G2AnimSection *section)
{
    if (G2AnimSection_IsInInterpolation(section) != G2FALSE)
    {
        return section->interpInfo->targetTime / section->keylist->s0TailTime;
    }
    else
    {
        return section->elapsedTime / section->keylist->s0TailTime;
    }
}

int G2AnimSection_GetStoredKeyframeNumber(G2AnimSection *section)
{
    return section->storedTime / section->keylist->s0TailTime;
}

void G2AnimSection_InterpToKeylistFrame(G2AnimSection *section, G2AnimKeylist *keylist, int keylistID, int targetFrame, short duration)
{
    G2AnimSection_InterpToKeylistAtTime(section, keylist, keylistID, targetFrame * keylist->s0TailTime, duration);
}

G2Bool G2AnimSection_IsInInterpolation(G2AnimSection *section)
{
    G2AnimInterpInfo *interpInfo;

    interpInfo = section->interpInfo;

    if ((interpInfo == NULL) || (interpInfo->stateBlockList == NULL))
    {
        return G2FALSE;
    }

    return !G2FALSE;
}

short G2AnimSection_NextKeyframe(G2AnimSection *section)
{
    if (!(section->flags & 0x1))
    {
        G2AnimSection_SetNotRewinding(section);

        return G2AnimSection_UpdateOverInterval(section, G2Timer_GetFrameTime());
    }

    return 0;
}

void G2AnimSection_SetAlphaTable(G2AnimSection *section, G2AnimAlphaTable *table)
{
    if (section->interpInfo != NULL)
    {
        section->interpInfo->alphaTable = table;
    }
}

void G2AnimSection_SetInterpInfo(G2AnimSection *section, G2AnimInterpInfo *newInfoPtr)
{
    section->interpInfo = newInfoPtr;

    if (section->interpInfo != NULL)
    {
        memset(section->interpInfo, 0, sizeof(G2AnimInterpInfo));
    }
}

void G2AnimSection_SetLooping(G2AnimSection *section)
{
    G2AnimSection_ClearAlarm(section, 0x3);

    G2AnimSection_SetLoopRangeAll(section);

    section->flags |= 0x2;
}

void G2AnimSection_SetLoopRangeAll(G2AnimSection *section)
{
    section->loopStartTime = 0;

    section->loopEndTime = G2AnimKeylist_GetDuration(section->keylist);
}

void G2AnimSection_SetNoLooping(G2AnimSection *section)
{
    section->flags &= ~0x2;
}

void G2AnimSection_SetNotRewinding(G2AnimSection *section)
{
    section->flags &= ~0x4;
}

void G2AnimSection_SetPaused(G2AnimSection *section)
{
    section->flags |= 0x1;
}

void G2AnimSection_SetUnpaused(G2AnimSection *section)
{
    section->flags &= ~0x1;
}

void G2AnimSection_SwitchToKeylist(G2AnimSection *section, G2AnimKeylist *keylist, int keylistID)
{
    G2AnimSection_SwitchToKeylistAtTime(section, keylist, keylistID, 0);
}
