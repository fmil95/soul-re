#include "Game/SPLINE.h"
#include "Game/MATH3D.h"
#include "Game/PSX/PSX_G2/QUATVM.h"
#include "Game/G2/QUATG2.h"

long FRAC_BITS = 15;

unsigned long SplineGetData(Spline *spline, SplineDef *def, void *p);
unsigned long SplineGetNext(Spline *spline, SplineDef *def);
unsigned long SplineGetPrev(Spline *spline, SplineDef *def);

void _SplineS2Pos(vecS *p, long s, SplineKey *key, SplineKey *key2)
{
    long s2;
    long s3;
    long h0;
    long h1;
    long h2;
    long h3;
    int temp, temp2; // not from decls.h

    if (s != 0)
    {
        s2 = (s * s) >> 15;

        temp = s2 * 2;

        s3 = (s2 * s) >> 15;

        temp2 = s3 * 2;

        h0 = (temp2 - (s2 * 3)) + 32768;

        h1 = (temp - temp2) + s2;

        h2 = (s3 - temp) + s;

        h3 = s3 - s2;

        p->x = (short)(((key->point.x * h0) + (key2->point.x * h1) + (key->dd.x * h2) + (key2->ds.x * h3)) >> 15);
        p->y = (short)(((key->point.y * h0) + (key2->point.y * h1) + (key->dd.y * h2) + (key2->ds.y * h3)) >> 15);
        p->z = (short)(((key->point.z * h0) + (key2->point.z * h1) + (key->dd.z * h2) + (key2->ds.z * h3)) >> 15);
    }
    else
    {
        p->x = key->point.x;
        p->y = key->point.y;
        p->z = key->point.z;
    }
}

void SplineSetDefDenom(Spline *spline, SplineDef *def, int denomFlag)
{
    unsigned long denom;

    if (def->denomFlag != denomFlag)
    {
        if (def->fracCurr != 0)
        {
            if (spline->type == 1)
            {
                denom = ((RSpline *)spline)->key[def->currkey].count;
            }
            else
            {
                denom = spline->key[def->currkey].count;
            }

            if (denom == 0)
            {
                denom = 1;
            }

            if (denomFlag != 0)
            {
                def->fracCurr = (def->fracCurr * denom) >> 15;
            }
            else
            {
                def->fracCurr = (def->fracCurr << 15) / denom;
            }
        }

        def->denomFlag = denomFlag;
    }
}

unsigned short SplineGetFrameNumber(Spline *spline, SplineDef *def)
{
    unsigned short frame;
    unsigned int i;
    SplineKey *key;
    SplineDef temp;

    if ((spline == NULL) || (def == NULL))
    {
        frame = 0xFFFF;
    }
    else
    {
        key = spline->key;

        temp = *def;

        frame = 0;

        for (i = 0; i < (unsigned)def->currkey; i++, key++)
        {
            frame += key->count;
        }

        temp.currkey = i;

        SplineSetDefDenom(spline, &temp, 0);

        frame += (unsigned short)(temp.fracCurr >> 12);
    }

    return frame & 0xFFFF;
}

short SplineSetDef2FrameNumber(Spline *spline, SplineDef *def, unsigned short frame_number)
{
    unsigned long isRot;
    short status;
    SplineKey *key;
    SplineRotKey *rkey;
    unsigned short frame;
    short deltaFrame;
    int temp; // not from decls.h

    status = 0;

    frame = 0;

    def->currkey = 0;

    def->fracCurr = 0;

    if ((spline == NULL) || (def == NULL))
    {
        status = -1;
    }
    else
    {
        rkey = (SplineRotKey *)spline->key;

        isRot = spline->type == 1;

        key = (SplineKey *)rkey;

        while (frame < frame_number)
        {
            if (def->currkey < spline->numkeys)
            {
                deltaFrame = frame_number - frame;

                if (deltaFrame < ((isRot != 0) ? rkey->count : key->count))
                {
                    def->fracCurr = deltaFrame << 12;
                    break;
                }
                else
                {
                    temp = frame;

                    if (isRot != 0)
                    {
                        temp += rkey->count;

                        frame = temp;
                    }
                    else
                    {
                        temp += key->count;

                        frame = temp;
                    }

                    def->currkey = (def->currkey + 1) % spline->numkeys;

                    key += 1;

                    if (def->currkey != 0)
                    {
                        rkey += 1;
                    }
                    else
                    {
                        rkey = (SplineRotKey *)spline->key;

                        key = (SplineKey *)rkey;
                    }
                }
            }
            else
            {
                break;
            }
        }

        def->denomFlag = 0;
    }

    return status;
}

short SplineIsWhere(Spline *spline, SplineDef *def)
{
    short curr;

    curr = (short)(def->fracCurr >> 12);

    if ((curr == 0) && (def->currkey == 0))
    {
        return -1;
    }

    if ((curr == (spline->numkeys - 1)) && (def->currkey == 0))
    {
        return 1;
    }

    return 0;
}

short SplineMultiIsWhere(MultiSpline *multi)
{
    short where;

    where = 0;

    if (multi->positional != NULL)
    {
        where = SplineIsWhere(multi->positional, &multi->curPositional);
    }

    if ((multi->rotational != NULL) && (where == 0))
    {
        where = SplineIsWhere((Spline *)multi->rotational, &multi->curRotational);
    }

    if ((multi->scaling != NULL) && (where == 0))
    {
        where = SplineIsWhere(multi->scaling, &multi->curScaling);
    }

    return where;
}

G2Quat *SplineGetFirstRot(RSpline *rspline, SplineDef *def)
{
    if (rspline == NULL)
    {
        return NULL;
    }

    def->fracCurr = 0;

    def->currkey = 0;

    def->denomFlag = 0;

    return &rspline->key->q;
}

SVector *SplineGetFirstPoint(Spline *spline, SplineDef *def)
{
    if (spline == NULL)
    {
        return NULL;
    }

    def->fracCurr = 0;

    def->currkey = 0;

    def->denomFlag = 0;

    return (SVector *)&spline->key->point;
}

SVector *SplineGetNextPoint(Spline *spline, SplineDef *def)
{
    static SVector nextrot; /*Unused*/
    static SVector point;

    (void)nextrot;

    if ((SplineGetNext(spline, def) != 0) && (SplineGetData(spline, def, &point) != 0))
    {
        return &point;
    }

    return NULL;
}

SVector *SplineGetPreviousPoint(Spline *spline, SplineDef *def)
{
    static SVector point;

    if ((SplineGetPrev(spline, def) != 0) && (SplineGetData(spline, def, &point) != 0))
    {
        return &point;
    }

    return NULL;
}

SVector *SplineGetNearestPoint(Spline *spline, SVector *point, SplineDef *def)
{
    static SVector dpoint;
    static SVector p; /*Unused*/
    long dist;
    long closest_dist;
    int closest_keyframe;
    int adjacent_keyframe;
    int closest_ib;
    int saved_closest_key_dist;
    int i;
    int frame_count;
    int swapped_keys;
    SplineKey *key;
    Vector d;
    SplineDef tempdef;

    (void)p;

    closest_dist = 0x7FFFFFFF;

    closest_keyframe = 0;

    key = spline->key;

    for (i = 0; i < spline->numkeys; key++, i++)
    {
        d.x = point->x - key->point.x;
        d.y = point->y - key->point.y;
        d.z = point->z - key->point.z;

        gte_ldlvl(&d);
        gte_nsqr0();
        gte_stlvnl(&d);

        dist = d.x + d.y + d.z;

        if (dist < closest_dist)
        {
            closest_dist = dist;

            closest_keyframe = i;
        }
    }

    saved_closest_key_dist = closest_dist;

    if (closest_keyframe == 0)
    {
        adjacent_keyframe = 1;
    }
    else if (closest_keyframe == (spline->numkeys - 1))
    {
        adjacent_keyframe = spline->numkeys - 2;
    }
    else
    {
        key = &spline->key[closest_keyframe] - 1;

        d.x = point->x - key->point.x;
        d.y = point->y - key->point.y;
        d.z = point->z - key->point.z;

        gte_ldlvl(&d);
        gte_nsqr0();
        gte_stlvnl(&d);

        key = &spline->key[closest_keyframe] + 1;

        dist = d.x + d.y + d.z;

        d.x = point->x - key->point.x;
        d.y = point->y - key->point.y;
        d.z = point->z - key->point.z;

        gte_ldlvl(&d);
        gte_nsqr0();
        gte_stlvnl(&d);

        if (d.x + d.y + d.z >= dist)
        {
            adjacent_keyframe = closest_keyframe - 1;
        }
        else
        {
            adjacent_keyframe = closest_keyframe + 1;
        }
    }

    if (adjacent_keyframe < closest_keyframe)
    {
        tempdef.currkey = adjacent_keyframe;

        swapped_keys = 1;
    }
    else
    {
        tempdef.currkey = closest_keyframe;

        swapped_keys = 0;
    }

    tempdef.fracCurr = 0;

    frame_count = spline->key[tempdef.currkey].count;

    closest_ib = 0;

    tempdef.denomFlag = 0;

    for (i = 0; i < frame_count; i++)
    {
        SplineGetData(spline, (SplineDef *)&tempdef.currkey, &dpoint);

        tempdef.fracCurr += 4096;

        d.x = point->x - dpoint.x;
        d.y = point->y - dpoint.y;
        d.z = point->z - dpoint.z;

        gte_ldlvl(&d);
        gte_nsqr0();
        gte_stlvnl(&d);

        dist = d.x + d.y + d.z;

        if (dist < closest_dist)
        {
            closest_dist = dist;

            closest_ib = i;
        }
    }

    if (swapped_keys != 0)
    {
        if (closest_dist >= saved_closest_key_dist)
        {
            def->currkey = closest_keyframe;

            def->fracCurr = 0;
        }
        else
        {
            def->currkey = adjacent_keyframe;

            def->fracCurr = closest_ib << 12;
        }
    }
    else
    {
        def->currkey = closest_keyframe;

        def->fracCurr = closest_ib << 12;
    }

    def->denomFlag = 0;

    SplineGetData(spline, def, &dpoint);

    return &dpoint;
}

unsigned long SplineGetData(Spline *spline, SplineDef *def, void *p)
{
    unsigned long gotDataOk;
    unsigned long isRot;
    int count;
    G2Quat quat;
    G2EulerAngles ea;

    gotDataOk = 0;

    if ((spline != NULL) && (def != NULL))
    {
        isRot = spline->type == 1;

        if ((def->currkey < spline->numkeys) && (def->currkey >= 0))
        {
            SplineSetDefDenom(spline, def, 0);

            gotDataOk = 1;

            if (spline->type == 1)
            {
                count = ((RSpline *)spline)->key[def->currkey].count;
            }
            else
            {
                count = spline->key[def->currkey].count;
            }

            if ((count != 0) && (def->fracCurr != 0))
            {
                if (isRot != 0)
                {
                    G2Quat_Slerp_VM(def->fracCurr / count, &((RSpline *)spline)->key[def->currkey].q, &((RSpline *)spline)->key[def->currkey + 1].q, &quat, 0);
                }
                else
                {
                    _SplineS2Pos((vecS *)p, (def->fracCurr * 8) / count, &spline->key[def->currkey], &spline->key[def->currkey] + 1);
                }
            }
            else if (isRot != 0)
            {
                quat = ((RSpline *)spline)->key[def->currkey].q;
            }
            else
            {
                COPY_SVEC(vecS, p, vecS, &spline->key[def->currkey].point);
            }

            if (isRot != 0)
            {
                G2Quat_ToEuler(&quat, &ea, 21);

                *(G2EulerAngles *)p = ea;
            }
        }
    }

    return gotDataOk;
}

unsigned long SplineGetQuatData(Spline *spline, SplineDef *def, void *p)
{
    unsigned long gotDataOk;
    int count;
    G2Quat quat;

    gotDataOk = 0;

    if ((spline != NULL) && (def != NULL))
    {
        if ((def->currkey < spline->numkeys) && (def->currkey >= 0))
        {
            SplineSetDefDenom(spline, def, 0);

            count = ((RSpline *)spline)->key[def->currkey].count;

            gotDataOk = 1;

            if ((count != 0) && (def->fracCurr != 0))
            {
                // TODO: third parameter is fake, second parameter might be different as well
                G2Quat_Slerp_VM(def->fracCurr / count, &((RSpline *)spline)->key[def->currkey].q, (G2Quat *)(&((RSpline *)spline)->key[def->currkey].q.z + 3), &quat, 0);
            }
            else
            {
                memcpy(&quat, &((RSpline *)spline)->key[def->currkey].q, 8);
            }

            memcpy(p, &quat, 8);
        }
    }

    return gotDataOk;
}

unsigned long SplineGetNext(Spline *spline, SplineDef *def)
{
    unsigned long movedSplineOk;
    int count;
    int temp; // not from decls.h

    movedSplineOk = 0;

    if ((spline != NULL) && (def != NULL))
    {
        temp = spline->type == 1;

        if (def->currkey < spline->numkeys)
        {
            movedSplineOk = 1;

            if (temp != 0)
            {
                count = ((RSpline *)spline)->key[def->currkey].count;
            }
            else
            {
                count = spline->key[def->currkey].count;
            }

            SplineSetDefDenom(spline, def, 0);

            def->fracCurr += 4096;

            if ((short)(def->fracCurr >> 12) >= count)
            {
                def->currkey++;

                def->fracCurr = 0;

                if ((spline->numkeys - 1) < def->currkey)
                {
                    movedSplineOk = 0;

                    if (((spline->flags & 0x4)) || ((spline->flags & 0x2)))
                    {
                        def->currkey = 0;
                    }
                    else
                    {
                        def->currkey = spline->numkeys - 1;
                    }
                }
            }
        }
    }

    return movedSplineOk;
}

INCLUDE_ASM("asm/nonmatchings/Game/SPLINE", SplineGetPrev);

unsigned long SplineGetOffsetNext(Spline *spline, SplineDef *def, long fracOffset)
{
    unsigned long movedSplineOk;
    unsigned long isRot;
    int count;

    movedSplineOk = 0;

    if ((spline != NULL) && (def != NULL))
    {
        isRot = spline->type == 1;

        if (def->currkey < spline->numkeys)
        {
            movedSplineOk = 1;

            if (isRot != 0)
            {
                count = ((RSpline *)spline)->key[def->currkey].count;
            }
            else
            {
                count = spline->key[def->currkey].count;
            }

            if (count <= 0)
            {
                count = 1;
            }

            SplineSetDefDenom(spline, def, 0);

            def->fracCurr += fracOffset;

            while (count < (def->fracCurr >> 12))
            {
                def->currkey++;

                def->fracCurr -= count << 12;

                if ((spline->numkeys - 1) < def->currkey)
                {
                    if (((spline->flags & 0x4)) || ((spline->flags & 0x2)))
                    {
                        def->currkey = 0;
                    }
                    else
                    {
                        def->currkey = spline->numkeys - 1;

                        movedSplineOk = 0;
                    }
                }

                if ((spline->numkeys - 1) >= def->currkey)
                {
                    if (isRot != 0)
                    {
                        count = ((RSpline *)spline)->key[def->currkey].count;
                    }
                    else
                    {
                        count = spline->key[def->currkey].count;
                    }

                    if (count <= 0)
                    {
                        count = 1;
                    }
                }
            }
        }
        else
        {
            printf("def->currkey=%d spline->numkeys=%d\n", def->currkey, spline->numkeys);
        }
    }

    return movedSplineOk;
}

unsigned long SplineGetOffsetPrev(Spline *spline, SplineDef *def, long fracOffset)
{
    unsigned long movedSplineOk;
    unsigned long isRot;
    int count;

    movedSplineOk = 0;

    if ((spline != NULL) && (def != NULL))
    {
        SplineSetDefDenom(spline, def, 0);

        isRot = spline->type == 1;

        if (def->currkey < spline->numkeys)
        {
            movedSplineOk = 1;

            if (isRot != 0)
            {
                count = ((RSpline *)spline)->key[def->currkey].count;
            }
            else
            {
                count = spline->key[def->currkey].count;
            }

            if (count <= 0)
            {
                count = 1;
            }

            def->fracCurr -= fracOffset;

            while (def->fracCurr < 0)
            {
                if (def->currkey <= 0)
                {
                    if (((spline->flags & 0x4)) || ((spline->flags & 0x2)))
                    {
                        def->currkey = spline->numkeys - 1;

                        if (isRot != 0)
                        {
                            count = ((RSpline *)spline)->key[def->currkey].count;
                        }
                        else
                        {
                            count = spline->key[def->currkey].count;
                        }

                        if (count <= 0)
                        {
                            count = 1;
                        }

                        def->fracCurr += count << 12;
                    }
                    else
                    {
                        movedSplineOk = 0;

                        def->currkey = 0;

                        def->fracCurr = 0;
                    }
                }
                else
                {
                    def->currkey--;

                    if (isRot != 0)
                    {
                        count = ((RSpline *)spline)->key[def->currkey].count;
                    }
                    else
                    {
                        count = spline->key[def->currkey].count;
                    }

                    if (count <= 0)
                    {
                        count = 1;
                    }

                    def->fracCurr += count << 12;
                }
            }
        }
    }

    return movedSplineOk;
}

SVector *SplineGetOffsetNextPoint(Spline *spline, SplineDef *def, long offset)
{
    static SVector point;

    if ((SplineGetOffsetNext(spline, def, offset) != 0) && (SplineGetData(spline, def, &point) != 0))
    {
        return &point;
    }

    return NULL;
}

SVector *SplineGetOffsetPreviousPoint(Spline *spline, SplineDef *def, long offset)
{
    static SVector point;

    if ((SplineGetOffsetPrev(spline, def, offset) != 0) && (SplineGetData(spline, def, &point) != 0))
    {
        return &point;
    }

    return NULL;
}
