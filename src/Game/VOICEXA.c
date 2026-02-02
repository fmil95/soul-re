#include "Game/VOICEXA.h"
#include "Game/GAMELOOP.h"
#include "Game/PSX/AADLIB.h"
#include "Game/PSX/MAIN.h"

typedef void (*voiceCmd)(XAVoiceTracker *vt, short voiceIndex);

XAVoiceTracker voiceTracker;

XAVoiceListEntry *voiceList;

void VOICEXA_Init()
{
    int i;
    CdlFILE fp;
    XAVoiceTracker *vt;
    char fileName[32];

    vt = &voiceTracker;

    if ((gameTrackerX.debugFlags & 0x80000))
    {
        vt->voiceStatus = 0;
        vt->cdStatus = 0;

        vt->reqIn = 0;
        vt->reqOut = 0;
        vt->reqsQueued = 0;

        vt->cdCmdIn = 0;
        vt->cdCmdOut = 0;
        vt->cdCmdsQueued = 0;

        vt->voiceCmdIn = 0;
        vt->voiceCmdOut = 0;
        vt->voiceCmdsQueued = 0;

        for (i = 0; i < 30; i++)
        {
            sprintf(&fileName[0], "\\VOICE\\VOICE%02d.XA;1", i);

            if (CdSearchFile(&fp, &fileName[0]) == 0)
            {
                vt->xaFileInfo[i].startPos = 0;
            }
            else
            {
                vt->xaFileInfo[i].startPos = CdPosToInt(&fp.pos);
            }
        }
    }
}

void putCdCommand(XAVoiceTracker *vt, unsigned char cdCommand, int numParams, unsigned char *params)
{
    int i;

    vt->cdCmdQueue[vt->cdCmdIn].cdCommand = cdCommand;

    for (i = 0; i < numParams; i++)
    {
        vt->cdCmdQueue[vt->cdCmdIn].cdCmdParam[i] = params[i];
    }

    if (vt->cdCmdsQueued < 7)
    {
        vt->cdCmdsQueued++;
        vt->cdCmdIn++;

        if (vt->cdCmdIn == 8)
        {
            vt->cdCmdIn = 0;
        }
    }
}

void VOICEXA_CdSyncCallback(unsigned char status, unsigned char *result)
{
    XAVoiceTracker *vt;

    (void)result;

    vt = &voiceTracker;

    if (status == 2)
    {
        vt->cdStatus = 0;

        if (++vt->cdCmdOut == 8)
        {
            vt->cdCmdOut = 0;
        }

        vt->cdCmdsQueued--;

        CdSyncCallback((CdlCB)vt->prevCallback);
    }
    else
    {
        vt->cdStatus = 2;
    }
}

void processCdCommands(XAVoiceTracker *vt)
{
    CdCommand *cmd;

    if (vt->cdStatus == 2)
    {
        vt->cdStatus = 1;

        cmd = &vt->cdCmdQueue[vt->cdCmdOut];

        CdControl(cmd->cdCommand, cmd->cdCmdParam, vt->cdResult);
    }
    else if ((vt->cdCmdsQueued != 0) && (vt->cdStatus != 1))
    {
        vt->cdStatus = 1;

        cmd = &vt->cdCmdQueue[vt->cdCmdOut];

        vt->prevCallback = (void *)CdSyncCallback((void *)VOICEXA_CdSyncCallback);

        CdControl(cmd->cdCommand, cmd->cdCmdParam, vt->cdResult);
    }
}

void putVoiceCommand(XAVoiceTracker *vt, unsigned char voiceCmd, unsigned char nextVoiceStatus, int voiceCmdParam)
{
    vt->voiceCmdQueue[vt->voiceCmdIn].voiceCmd = voiceCmd;

    vt->voiceCmdQueue[vt->voiceCmdIn].nextVoiceStatus = nextVoiceStatus;

    vt->voiceCmdQueue[vt->voiceCmdIn].voiceCmdParam = voiceCmdParam;

    if (vt->voiceCmdsQueued < 15)
    {
        vt->voiceCmdsQueued++;
        vt->voiceCmdIn++;

        if (vt->voiceCmdIn == 16)
        {
            vt->voiceCmdIn = 0;
        }
    }
}

void processVoiceCommands(XAVoiceTracker *vt)
{
    VoiceCommand *cmd;
    static voiceCmd voiceCmdTbl[5] = {voiceCmdPlay, voiceCmdStop, voiceCmdPause, voiceCmdResume, voiceCmdNull};

    if (vt->voiceCmdsQueued != 0)
    {
        cmd = &vt->voiceCmdQueue[vt->voiceCmdOut];

        vt->voiceCmdsQueued--;

        if (++vt->voiceCmdOut == 16)
        {
            vt->voiceCmdOut = 0;
        }

        if (cmd->voiceCmd < 5)
        {
            voiceCmdTbl[cmd->voiceCmd](vt, (short)cmd->voiceCmdParam);

            vt->voiceStatus = cmd->nextVoiceStatus;
        }
    }
}

void voiceCmdPlay(XAVoiceTracker *vt, short voiceIndex)
{
    CdlFILTER filter;
    CdlLOC pos;
    unsigned char mode;
    SpuCommonAttr spuattr;
    XAVoiceListEntry *voice;
    XAFileInfo *file;

    if (voiceList != NULL)
    {
        voice = &voiceList[voiceIndex];

        vt->fileNum = voiceIndex >> 4;

        file = &vt->xaFileInfo[vt->fileNum];

        putCdCommand(vt, 9, 0, NULL);

        filter.file = 1;

        filter.chan = voiceIndex & 0xF;

        putCdCommand(vt, 13, 4, (unsigned char *)&filter);

        mode = 200;

        putCdCommand(vt, 14, 1, &mode);

        CdIntToPos(file->startPos, &vt->currentPos);

        vt->endSector = (file->startPos - 150) + voice->length;

        CdIntToPos(file->startPos, &pos);

        putCdCommand(vt, 27, 4, (unsigned char *)&pos);

        spuattr.mask = 0x3FCF;

        spuattr.mvol.left = 16383;
        spuattr.mvol.right = 16383;

        spuattr.mvolmode.left = 0;
        spuattr.mvolmode.right = 0;

        spuattr.cd.reverb = 0;

        spuattr.cd.mix = 1;

        spuattr.ext.volume.left = 32767;
        spuattr.ext.volume.right = 32767;

        spuattr.ext.reverb = 0;

        spuattr.ext.mix = 1;

        spuattr.cd.volume.left = (short)(gameTrackerX.sound.gVoiceVol << 8);
        spuattr.cd.volume.right = (short)(gameTrackerX.sound.gVoiceVol << 8);

        SpuSetCommonAttr(&spuattr);

        if (gameTrackerX.sound.gMusicVol > 60)
        {
            aadStartMusicMasterVolFade(60, -1, NULL);
        }
    }
}

void voiceCmdStop(XAVoiceTracker *vt, short cmdParam)
{
    SpuCommonAttr spuattr;

    (void)cmdParam;

    if (vt->voiceStatus != 0)
    {
        putCdCommand(vt, 9, 0, NULL);

        spuattr.mask = 0x2200;

        spuattr.cd.mix = 0;
        spuattr.ext.mix = 0;

        SpuSetCommonAttr(&spuattr);

        aadStartMusicMasterVolFade(gameTrackerX.sound.gMusicVol, 1, NULL);
    }
}

void voiceCmdPause(XAVoiceTracker *vt, short cmdParam)
{
    (void)cmdParam;

    if ((vt->voiceStatus == 1) || (vt->voiceStatus == 2))
    {
        putCdCommand(vt, 9, 0, NULL);
    }
}

void voiceCmdResume(XAVoiceTracker *vt, short cmdParam)
{
    (void)cmdParam;

    if (vt->voiceStatus == 3)
    {
        putCdCommand(vt, 27, 4, (unsigned char *)&vt->currentPos);
    }
}

void voiceCmdNull(XAVoiceTracker *vt, short cmdParam)
{
    (void)vt;
    (void)cmdParam;
}

void VOICEXA_Play(int voiceIndex, int queueRequests)
{
    XAVoiceTracker *vt;
    XAFileInfo *file;

    vt = &voiceTracker;

    file = &vt->xaFileInfo[voiceIndex >> 4];

    if (((gameTrackerX.debugFlags & 0x80000)) && (file->startPos != 0) && ((unsigned)gameTrackerX.sound.gVoiceOn != 0))
    {
        if (queueRequests != 0)
        {
            vt->requestQueue[vt->reqIn] = voiceIndex;

            if (vt->reqsQueued < 3)
            {
                vt->reqsQueued++;
                vt->reqIn++;

                if ((vt->reqIn & 0xFF) == 4)
                {
                    vt->reqIn = 0;
                }
            }
        }
        else
        {
            putVoiceCommand(vt, 0, 1, voiceIndex);
        }
    }
}

int VOICEXA_FinalStatus(XAVoiceTracker *vt)
{
    int tailIndex;

    if (vt->voiceCmdsQueued == 0)
    {
        return vt->voiceStatus;
    }

    tailIndex = vt->voiceCmdIn;

    if (tailIndex == 0)
    {
        tailIndex = 15;
    }
    else
    {
        tailIndex--;
    }

    return vt->voiceCmdQueue[tailIndex].nextVoiceStatus;
}

void VOICEXA_Pause()
{
    XAVoiceTracker *vt;
    int finalStatus;

    vt = &voiceTracker;

    finalStatus = VOICEXA_FinalStatus(vt);

    if ((gameTrackerX.debugFlags & 0x80000))
    {
        if ((finalStatus == 1) || (finalStatus == 2))
        {
            putVoiceCommand(vt, 2, 3, 0);
        }
        else if (finalStatus == 0)
        {
            putVoiceCommand(vt, 4, 4, 0);
        }
    }
}

void VOICEXA_Resume()
{
    XAVoiceTracker *vt;
    int finalStatus;

    vt = &voiceTracker;

    finalStatus = VOICEXA_FinalStatus(vt);

    if ((gameTrackerX.debugFlags & 0x80000))
    {
        if (finalStatus == 3)
        {
            putVoiceCommand(vt, 3, 1, 0);
        }
        else if (finalStatus == 4)
        {
            putVoiceCommand(vt, 4, 0, 0);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/VOICEXA", VOICEXA_Tick);

int VOICEXA_IsPlaying()
{
    XAVoiceTracker *vt;

    vt = &voiceTracker;

    if (vt->voiceStatus == 2)
    {
        return 2;
    }

    if ((vt->voiceStatus == 1) || (vt->cdStatus != 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
