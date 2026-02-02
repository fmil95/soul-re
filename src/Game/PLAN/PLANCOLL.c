#include "Game/PLAN/PLANCOLL.h"
#include "Game/MATH3D.h"
#include "Game/GAMELOOP.h"
#include "Game/MEMPACK.h"
#include "Game/STREAM.h"
#include "Game/COLLIDE.h"
#include "Game/PSX/MAIN.h"

static FindTerrainHit directionList[6] = {
    {
        0,
        0,
        1,
        0,
    },
    {
        1,
        0,
        0,
        1,
    },
    {
        0,
        1,
        0,
        1,
    },
    {
        0xFF,
        0,
        0,
        1,
    },
    {
        0,
        0xFF,
        0,
        1,
    },
    {
        0,
        0,
        0xFF,
        2,
    },
};

int PLANCOLL_DoesLOSExistFinal(Position *startPos, Position *endPos, int collideType, int passThroughHit, int zoffset)
{
    PCollideInfo pcollideinfo;
    SVECTOR startPt;
    SVECTOR endPt;
    Level *temp;       // not from decls.h
    TFace *temp2;      // not from decls.h
    TextureFT3 *temp3; // not from decls.h

    (void)collideType;

    COPY_SVEC(SVector, (SVector *)&startPt, Position, startPos);
    COPY_SVEC(SVector, (SVector *)&endPt, Position, endPos);

    pcollideinfo.newPoint = &endPt;
    pcollideinfo.oldPoint = &startPt;

    pcollideinfo.collideType = 0x127;

    pcollideinfo.inst = NULL;
    pcollideinfo.instance = NULL;

    startPt.vz += zoffset;
    endPt.vz += zoffset;

    COLLIDE_PointAndWorld(&pcollideinfo, NULL);

    if ((passThroughHit != 0) && (pcollideinfo.type == 3) && (((TFace *)pcollideinfo.prim)->textoff != 0xFFFF))
    {
        temp = (Level *)pcollideinfo.inst;

        temp2 = (TFace *)pcollideinfo.prim;

        temp3 = (TextureFT3 *)((char *)temp->terrain->StartTextureList + temp2->textoff);

        if ((temp3->attr & 0x1000))
        {
            return 1;
        }
    }

    return pcollideinfo.type == 0;
}

int PLANCOLL_CheckUnderwaterPoint(Position *position)
{
    StreamUnit *streamUnit;
    Level *level;
    BSPTree *tree;
    Sphere_noSq *sphere;
    int i;
    Position center;

    streamUnit = &StreamTracker.StreamList[0];

    for (i = 16; i != 0; i--, streamUnit++)
    {
        if ((streamUnit->used == 2) && (MEMPACK_MemoryValidFunc((char *)streamUnit->level) != 0))
        {
            level = streamUnit->level;

            tree = level->terrain->BSPTreeArray;

            sphere = &tree->bspRoot->sphere;

            COPY_SVEC(Position, &center, Position, &sphere->position);

            {
                Position *_v0;

                _v0 = &tree->globalOffset;

                center.x += _v0->x;
                center.y += _v0->y;
                center.z += _v0->z;
            }

            if ((MATH3D_LengthXYZ(position->x - center.x, position->y - center.y, position->z - center.z) < sphere->radius) && (position->z < level->waterZLevel))
            {
                return streamUnit->StreamUnitID;
            }
        }
    }

    return -1;
}

int PLANCOLL_FindTerrainHitFinal(PlanCollideInfo *pci, int *placement, int distBefore, int distAfter, int start, int end)
{
    PCollideInfo pcollideinfo;
    FindTerrainHit *current;
    FindTerrainHit *last;
    SVECTOR startPt;
    SVECTOR endPt;
    SVector normal;

    pcollideinfo.oldPoint = &startPt;
    pcollideinfo.newPoint = &endPt;

    pcollideinfo.collideType = 1;

    current = &directionList[start];
    last = &directionList[end + 1];

    pcollideinfo.inst = NULL;
    pcollideinfo.instance = NULL;

    for (; current < last; current++)
    {
        // TODO: Replace with COPY_SVEC macro
        short _x1;
        short _y1;
        short _z1;
        Position *_v0;
        Position *_v1;

        _v0 = (Position *)&startPt;
        _v1 = (Position *)&pci->collidePos;

        _x1 = _v1->x;
        _y1 = _v1->y;
        _z1 = _v1->z;

        _v0->x = _x1;
        _v0->y = _y1;
        _v0->z = _z1;

        COPY_SVEC(SVector, (SVector *)&endPt, Position, &pci->collidePos);

        startPt.vx += (signed char)current->x * distBefore;
        startPt.vy += (signed char)current->y * distBefore;
        startPt.vz += (signed char)current->z * distBefore;

        endPt.vx += (signed char)current->x * distAfter;
        endPt.vy += (signed char)current->y * distAfter;
        endPt.vz += (signed char)current->z * distAfter;

        COLLIDE_PointAndWorld(&pcollideinfo, NULL);

        if (pcollideinfo.type == 3)
        {
            if (placement != 0)
            {
                *placement = (signed char)current->nodeType;
            }

            pci->StreamUnitID = ((Level *)pcollideinfo.inst)->streamUnitID;

            pci->tFace = (TFace *)pcollideinfo.prim;

            COLLIDE_GetNormal(((TFace *)pcollideinfo.prim)->normal, (short *)((Level *)(&pcollideinfo.inst->node))->terrain->normalList, &normal);

            endPt.vx += (normal.x * 10) / 4096;
            endPt.vy += (normal.y * 10) / 4096;
            endPt.vz += (normal.z * 10) / 4096;

            COPY_SVEC(Position, &pci->collidePos, SVector, (SVector *)&endPt);

            return 1;
        }
    }

    return 0;
}

int PLANCOLL_DoesStraightLinePathExist(Position *startPos, Position *endPos, int collideType)
{
    int temp, temp2; // not from decls.h

    temp = 0;

    temp2 = gameTrackerX.gameFlags < temp;

    if (PLANCOLL_DoesLOSExistFinal(startPos, endPos, collideType, temp2, 256) != 0)
    {
        temp = PLANCOLL_DoesLOSExistFinal(startPos, endPos, collideType, temp2, -256) != 0;
    }

    return temp;
}

int PLANCOLL_DoesWaterPathUpExist(Position *startPos, Position *endPos, int collideType, Position *peakPos, int passThroughHit)
{
    long time_ftop;
    long time_ptow;
    long time_tot;
    Position diff;

    time_ftop = MATH3D_FastSqrt(327680);

    peakPos->z = (startPos->z - endPos->z) + 640;

    time_ptow = MATH3D_FastSqrt((peakPos->z * 131072) / 3);

    time_tot = time_ptow / 16;

    diff.x = startPos->x - endPos->x;
    diff.y = startPos->y - endPos->y;

    peakPos->x = (short)(endPos->x + ((diff.x * time_tot) / (time_ftop + time_tot)));
    peakPos->y = (short)(endPos->y + ((diff.y * time_tot) / (time_ftop + time_tot)));

    return PLANCOLL_DoesLOSExistFinal(startPos, peakPos, collideType, passThroughHit, 0);
}
