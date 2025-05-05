#include "Game/G2/ANMDECMP.h"

void _G2Anim_DecompressChannel_AdaptiveDelta(G2AnimDecompressChannelInfo *dcInfo, G2AnimChanStatus *status)
{
    unsigned short *chanData;
    int index;
    int keyData;
    unsigned short step;
    unsigned short predictedDelta;
    int targetKey;
    int storedKey;
    int keyCount;

    chanData = &dcInfo->chanData[2];

    storedKey = dcInfo->storedKey;
    targetKey = dcInfo->targetKey;

    keyCount = dcInfo->keylist->keyCount + 3;

    keyData = status->keyData;

    index = status->index;

    while (storedKey < targetKey)
    {
        int temp, temp2, temp3; // not from decls.h

        storedKey++;

        temp = chanData[storedKey >> 2];

        temp2 = temp >> ((storedKey & 0x3) << 2);

        step = _stepSizeTable[index];

        index += _indexTable[temp2 & 0xF];

        if (index < 0)
        {
            index = 0;
        }

        if (index >= 64)
        {
            index = 63;
        }

        predictedDelta = step >> 3;

        temp3 = step & 0x1;

        if ((temp2 & 0x4))
        {
            predictedDelta += step;
        }

        step >>= 1;

        if ((temp2 & 0x2))
        {
            predictedDelta += step;
        }

        step >>= 1;

        if ((temp2 & 0x1))
        {
            predictedDelta += step + temp3;
        }

        if ((temp2 & 0x8))
        {
            keyData -= predictedDelta;
        }
        else
        {
            keyData += predictedDelta;
        }
    }

    status->index = index;

    status->keyData = keyData;

    dcInfo->chanData = &chanData[keyCount >> 2];
}

void _G2Anim_DecompressChannel_Linear(G2AnimDecompressChannelInfo *dcInfo, G2AnimChanStatus *status)
{
    unsigned short *chanData;
    short rangeBase;
    short rangeInfo;
    int rangeLength;
    int rangeOffset;
    int targetKey;

    chanData = dcInfo->chanData;

    targetKey = dcInfo->targetKey;

    dcInfo->chanData = &chanData[((chanData[0] & 0xFFF)) + 1];

    rangeBase = chanData[1];

    rangeInfo = chanData[2];

    chanData++;

    rangeLength = (rangeInfo & 0xF800) >> 11;

    rangeOffset = (rangeInfo << 21) >> 21;

    while (rangeLength < targetKey)
    {
        rangeInfo = chanData[2];

        targetKey -= rangeLength;

        rangeBase += rangeOffset;

        chanData++;

        rangeLength = (rangeInfo & 0xF800) >> 11;

        rangeOffset = (rangeInfo << 21) >> 21;
    }

    rangeOffset = (short)((rangeOffset * targetKey) / rangeLength);

    status->keyData = rangeBase + rangeOffset;
}

void _G2Anim_InitializeChannel_AdaptiveDelta(G2AnimDecompressChannelInfo *dcInfo, G2AnimChanStatus *status)
{
    unsigned short *chanData;
    int keyCount;

    keyCount = dcInfo->keylist->keyCount;

    chanData = dcInfo->chanData;

    status->index = ((unsigned char *)chanData)[0];

    status->keyData = chanData[1];

    dcInfo->chanData = &chanData[((keyCount + 3) >> 2)] + 2;
}

void _G2Anim_InitializeChannel_Linear(G2AnimDecompressChannelInfo *dcInfo, G2AnimChanStatus *status)
{
    unsigned short *chanData;
    int chanLength;

    chanData = dcInfo->chanData;

    chanLength = (chanData[0] & 0xFFF) + 1;

    status->keyData = chanData[1];

    dcInfo->chanData = &chanData[chanLength];
}
