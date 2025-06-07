#include "Game/MATH3D.h"
#include "Game/G2/QUATG2.h"
#include "Game/HASM.h"

void MATH3D_Sort3VectorCoords(long *a, long *b, long *c)
{
    long a1;
    long b1;
    long c1;

    a1 = *a;
    b1 = *b;
    c1 = *c;

    if (a1 < b1)
    {
        if (c1 < a1)
        {
            *c = b1;
            *b = a1;
            *a = c1;
        }
        else if (c1 < b1)
        {
            *c = b1;
            *b = c1;
        }
    }
    else if (c1 < b1)
    {
        *a = c1;
        *c = a1;
    }
    else if (c1 < a1)
    {
        *a = b1;
        *b = c1;
        *c = a1;
    }
    else
    {
        *a = b1;
        *b = a1;
    }
}

long MATH3D_LengthXYZ(long x, long y, long z)
{
    long t;

    x = abs(x);
    y = abs(y);
    z = abs(z);

    if (x < y)
    {
        if (z < x)
        {
            t = x;
            x = z;
            z = y;
            y = t;
        }
        else if (z < y)
        {
            t = y;
            y = z;
            z = t;
        }
    }
    else if (z < y)
    {
        t = x;
        x = z;
        z = t;
    }
    else if (z < x)
    {
        t = x;
        x = y;
        y = z;
        z = t;
    }
    else
    {
        t = x;
        x = y;
        y = t;
    }

    t = (z * 30) + (y * 12) + (x * 9);

    return t / 32;
}

long MATH3D_LengthXY(long x, long y)
{
    long temp; // not from decls.h

    x = abs(x);
    y = abs(y);

    temp = y * 16;

    if (y < x)
    {
        x = x ^ y;
        y = x ^ y;
        x = x ^ y;

        temp = y * 16;
    }

    return (((temp - y) * 2) + (12 * x)) / 32;
}

void MATH3D_Normalize(Normal *normal)
{
    long length;

    length = MATH3D_LengthXYZ(normal->x * 4, normal->y * 4, normal->z * 4);

    if (length != 0)
    {
        normal->x = ((normal->x << 14) / length);
        normal->y = ((normal->y << 14) / length);
        normal->z = ((normal->z << 14) / length);
    }
}

short MATH3D_FastAtan2(long y, long x)
{
    long ax;
    long ay;

    if (x == 0)
    {
        x = 1;
    }

    if (y == 0)
    {
        return (x < 1) * 2048;
    }

    ax = abs(x);
    ay = abs(y);

    if (x > 0)
    {
        if (y > 0)
        {
            if (ax < ay)
            {
                return (1024 - ((ax * 512) / ay));
            }
            else
            {
                return ((ay * 512) / ax);
            }
        }
        else
        {
            if (ay < ax)
            {
                return (4096 - ((ay * 512) / ax));
            }
            else
            {
                return (((ax * 512) / ay) + 3072);
            }
        }
    }

    if (y > 0)
    {
        if (ax < ay)
        {
            return (((ax * 512) / ay) + 1024);
        }
        else
        {
            return (2048 - ((ay * 512) / ax));
        }
    }

    if (ay < ax)
    {
        return (((ay * 512) / ax) + 2048);
    }
    else
    {
        return (3072 - ((ax * 512) / ay));
    }
}

long MATH3D_FastSqrt(long square)
{
    unsigned long result;
    long remainder;
    long mask;
    long shift;
    long mask_squared;
    long result_shift;

    shift = 31;

    if (square != 0)
    {
        mask = 0x80000000;

        if (square >= 0)
        {
            do
            {
                mask >>= 1;

                shift--;
            } while (!(mask & square));
        }

        shift >>= 1;

        result = 1 << (shift + 6);

        mask = result;

        result_shift = 1 << (shift << 1);

        mask_squared = result_shift;

        shift--;

        square -= result_shift;

        while (shift != -1)
        {
            mask_squared >>= 2;

            remainder = result_shift + mask_squared;
            remainder = square - remainder;

            mask >>= 1;

            if (remainder < 0)
            {
                result_shift >>= 1;
            }
            else
            {
                square = remainder;

                remainder = result_shift >> 1;

                result_shift = remainder + mask_squared;

                result |= mask;
            }

            shift--;
        }

        mask_squared >>= 2;

        square <<= 12;

        result_shift <<= 12;

        mask_squared = 4096;

        mask >>= 1;

        while (mask != 0)
        {
            mask_squared >>= 2;

            remainder = result_shift + mask_squared;
            remainder = square - remainder;

            if (remainder < 0)
            {
                result_shift >>= 1;
            }
            else
            {
                square = remainder;

                remainder = result_shift >> 1;

                result_shift = remainder + mask_squared;

                result |= mask;
            }

            mask >>= 1;
        }

        return result;
    }

    return 0;
}

long MATH3D_FastSqrt0(long square)
{
    unsigned long result;
    long remainder;
    long mask;
    long shift;
    long mask_squared;
    long result_shift;

    shift = 31;

    if (square != 0)
    {
        mask = 0x80000000;

        if (square >= 0)
        {
            do
            {
                mask >>= 1;

                remainder = mask & square;

                shift--;
            } while (remainder == 0);
        }

        shift >>= 1;

        result = 1 << shift;

        mask = result;

        result_shift = 1 << (shift << 1);

        mask_squared = result_shift;

        square -= result_shift;

        while (--shift != -1)
        {
            mask >>= 1;

            mask_squared >>= 2;

            remainder = square - result_shift;

            remainder -= mask_squared;

            result_shift >>= 1;

            if (remainder >= 0)
            {
                square = remainder;

                result_shift += mask_squared;

                result |= mask;
            }
        }

        return result;
    }

    return 0;
}

long MATH3D_DistanceBetweenPositions(Position *pos1, Position *pos2)
{
    return MATH3D_FastSqrt0(MATH3D_SquareLength((pos2->x - pos1->x), (pos2->y - pos1->y), (pos2->z - pos1->z)));
}

short MATH3D_AngleBetweenVectors(SVector *vector1, SVector *vector2)
{
    long projection_length;

    if ((vector1->x == vector2->x) && (vector1->y == vector2->y) && (vector1->z == vector2->z))
    {
        return 0;
    }

    projection_length = (((vector1->x * vector2->x) + (vector1->y * vector2->y) + (vector1->z * vector2->z)) + 2048) >> 12;

    if (projection_length >= 4097)
    {
        projection_length = 4096;
    }
    else if (projection_length < -4096)
    {
        projection_length = -4096;
    }

    return ratan2(MATH3D_FastSqrt0(16777216 - projection_length * projection_length), projection_length);
}

void MATH3D_RotMatAboutVec(SVector *vec, MATRIX *mat, short angle)
{
    long length;
    SVECTOR rot_angs;
    MATRIX mat1;
    MATRIX mat2;

    if (angle != 0)
    {
        length = MATH3D_FastSqrt0(MATH3D_SquareLength(0, vec->y, vec->z) + 2048);

        rot_angs.vx = -ratan2(vec->y, vec->z);
        rot_angs.vy = ratan2(vec->x, length);
        rot_angs.vz = 0;

        RotMatrix(&rot_angs, &mat1);

        TransposeMatrix(&mat1, &mat2);

        MulMatrix2(&mat2, mat);

        RotMatrixZ(angle, mat);

        MulMatrix2(&mat1, mat);
    }
}

void MATH3D_SetUnityMatrix(MATRIX *mat)
{
    ((unsigned int *)&mat->m[0][0])[0] = 4096;
    ((unsigned int *)&mat->m[0][2])[0] = 0;

    ((unsigned int *)&mat->m[1][1])[0] = 4096;
    ((unsigned int *)&mat->m[2][0])[0] = 0;

    mat->m[2][2] = 4096;
}

void AngleMoveToward(short *current_ptr, short destination, short step)
{
    long diff;
    short current;

    current = *current_ptr;

    diff = AngleDiff(current, destination);

    if ((diff == 0) || (abs(diff) < step))
    {
        *current_ptr = destination;
    }
    else
    {
        if (diff > 0)
        {
            current += step;
        }
        else if (diff < 0)
        {
            current -= step;
        }

        *current_ptr = current & 0xFFF;
    }
}

short AngleDiff(short current, short destination)
{
    current = (destination - current) & 0xFFF;

    if (current >= 2049)
    {
        current |= 0xF000;
    }

    return current;
}

short MATH3D_AngleFromPosToPos(Position *from, Position *to)
{
    return (ratan2(from->y - to->y, from->x - to->x) + 3072) & 0xFFF;
}

void MATH3D_ZYXtoXYZ(Rotation *rot)
{
    MATRIX tempMat;
    G2EulerAngles ea;

    RotMatrixZYX((SVECTOR *)rot, &tempMat);

    G2EulerAngles_FromMatrix(&ea, (G2Matrix *)&tempMat, 21);

    rot->x = ea.x;
    rot->y = ea.y;
    rot->z = ea.z;
}

short MATH3D_ElevationFromPosToPos(Position *from, Position *to)
{
    int dx;
    int dy;

    dx = from->x - to->x;
    dy = from->y - to->y;

    return -(ratan2(to->z - from->z, (short)MATH3D_FastSqrt0((dx * dx) + (dy * dy)))) & 0xFFF;
}

void MATH3D_RotationFromPosToPos(Position *from, Position *to, Rotation *rot)
{
    rot->x = MATH3D_ElevationFromPosToPos(from, to);
    rot->y = 0;
    rot->z = MATH3D_AngleFromPosToPos(from, to);
}

int MATH3D_veclen2(int ix, int iy)
{
    int t;
    int temp; // not from decls.h

    if (ix < 0)
    {
        ix = -ix;
    }

    if (iy < 0)
    {
        iy = -iy;
    }

    t = iy >> 1;

    if (ix < iy)
    {
        ix = ix ^ iy;
        iy = ix ^ iy;
        ix = ix ^ iy;

        t = iy >> 1;
    }

    temp = iy + t;

    return (((ix - (ix >> 5)) - (ix >> 7)) + (temp >> 2)) + (temp >> 6);
}

void MATH3D_RotateAxisToVector(MATRIX *dest, MATRIX *src, SVector *vec, enum MATH3D_AXIS axis)
{
    MATRIX xform;
    G2Quat rot;
    long len;
    int theta;
    int sintheta;
    int px;
    int py;
    int pz;

    if ((unsigned int)axis >= AXIS_NEG_X)
    {
        axis = (enum MATH3D_AXIS)((unsigned int)axis - AXIS_NEG_X);

        px = -src->m[0][(unsigned int)axis];
        py = -src->m[1][(unsigned int)axis];
        pz = -src->m[2][(unsigned int)axis];
    }
    else
    {
        px = src->m[0][(unsigned int)axis];
        py = src->m[1][(unsigned int)axis];
        pz = src->m[2][(unsigned int)axis];
    }

    rot.x = ((py * vec->z) - (pz * vec->y)) / 4096;
    rot.y = ((pz * vec->x) - (px * vec->z)) / 4096;
    rot.z = ((px * vec->y) - (py * vec->x)) / 4096;

    sintheta = MATH3D_racos_S(((px * vec->x) + (py * vec->y) + (pz * vec->z)) / 4096) / 2;

    len = MATH3D_SquareLength(rot.x, rot.y, rot.z);

    if (len <= 0)
    {
        len = 4096;
    }
    else
    {
        len = MATH3D_FastSqrt0(len);
    }

    theta = rsin(sintheta);

    rot.x = (short)(rot.x * theta / len);
    rot.y = (short)(rot.y * theta / len);
    rot.z = (short)(rot.z * theta / len);
    rot.w = rcos(sintheta);

    G2Quat_ToMatrix_S(&rot, (G2Matrix *)&xform);

    MulMatrix0(src, &xform, dest);
}

int MATH3D_ConeDetect(SVector *pos, int arc, int elevation)
{
    long x;
    long y;

    x = pos->x;
    y = -pos->y;

    if ((MATH3D_FastAtan2(abs(x), y) < arc) && (MATH3D_FastAtan2(abs(pos->z), MATH3D_LengthXY(x, y)) < elevation))
    {
        return 1;
    }

    return 0;
}

void MATH3D_CrossProduct(SVector *t, SVector *r, SVector *s)
{
    t->x = ((r->y * s->z) - (r->z * s->y)) >> 12;
    t->y = -(((r->x * s->z) - (r->z * s->x)) >> 12);
    t->z = ((r->x * s->y) - (r->y * s->x)) >> 12;
}

unsigned long MATH3D_SquareLength(long x, long y, long z)
{
    hasm_sqrlen1(x, y, z);
    gte_sqr0();
    hasm_sqrlen2(x, y, z);

    return x;
}
