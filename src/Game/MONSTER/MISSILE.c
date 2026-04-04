#include "Game/MONSTER/MISSILE.h"
#include "Game/GAMELOOP.h"
#include "Game/PHYSOBS.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/STATE.h"
#include "Game/OBTABLE.h"
#include "Game/INSTANCE.h"

static MissileGraphics MISSILE_objectTable[] = {{0xD, 0x1}, {0x13, 0}, {0xB, 0}, {0x13, 0}};

void MISSILE_Process(Instance *instance, GameTracker *gameTracker)
{
    ProcessPhysicalObject(instance, gameTracker);

    if ((instance->LinkParent == NULL) && ((unsigned)instance->work2 < MON_GetTime(instance)))
    {
        INSTANCE_KillInstance(instance);
    }
}

void MISSILE_Collide(Instance *instance, GameTracker *gameTracker)
{
    CollidePhysicalObject(instance, gameTracker);

    if (instance->LinkParent == NULL)
    {
        INSTANCE_KillInstance(instance);
    }
}

Instance *MISSILE_Find(Instance *instance, MonsterMissile *missiledef)
{
    Object *ob;
    Instance *missile;

    missile = instance->LinkChild;

    ob = objectAccess[MISSILE_objectTable[missiledef->graphic].object].object;

    while (missile != NULL)
    {
        if ((missile->ParentLinkNode == missiledef->segment) && (missile->object == ob))
        {
            return missile;
        }

        missile = missile->LinkSibling;
    }

    return NULL;
}

Instance *MISSILE_Birth(Instance *instance, MonsterMissile *missiledef)
{
    Instance *missile;

    if (missiledef->type == 3)
    {
        missile = MISSILE_Find(instance, missiledef);

        if (missile != NULL)
        {
            missile->processFunc = MISSILE_Process;
            missile->collideFunc = MISSILE_Collide;

            goto end;
        }
    }

    missile = PHYSOB_BirthProjectile(instance, missiledef->segment, missiledef->graphic)->birthInstance;

    if (missile != NULL)
    {
        missile->processFunc = MISSILE_Process;

        if (instance->matrix == NULL)
        {
            return missile;
        }
        else
        {
            MATRIX *matrix;

            matrix = &instance->matrix[missiledef->segment];

            missile->position.x = matrix->t[0];
            missile->position.y = matrix->t[1];
            missile->position.z = matrix->t[2];
        }
    }

end:
    return missile;
}

Instance *MISSILE_Fire(Instance *instance, MonsterMissile *missiledef, void *target, int type)
{
    Instance *miss;

    miss = MISSILE_Birth(instance, missiledef);

    if (miss != NULL)
    {
        int spin;
        SVector rotVel;

        spin = THROW_SPIN_TYPE_SELF_ADJUSTING;

        if (missiledef->type == 3)
        {
            spin = THROW_SPIN_TYPE_VELOCITY;

            rotVel.x = 273;
            rotVel.y = 0;
            rotVel.z = 0;
        }

        INSTANCE_Post(miss, 0x800010, SetObjectThrowData(target, &rotVel, type & 0xFFFF, spin, missiledef->speed, 0, 0, 0));

        miss->work2 = MON_GetTime(miss) + 5000;
    }

    return miss;
}

Instance *MISSILE_FireAtInstance(Instance *instance, MonsterMissile *missiledef, Instance *target)
{
    return MISSILE_Fire(instance, missiledef, target, THROW_TYPE_TARGET);
}
