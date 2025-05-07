#include "Game/MONSTER/WORSHIP.h"
#include "Game/MONSTER/HUMAN.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/INSTANCE.h"
#include "Game/COLLIDE.h"

MonsterStateChoice WORSHIP_StateChoiceTable[5] = {
    {MONSTER_STATE_STUNNED, {HUMAN_StunnedEntry, HUMAN_Stunned}},
    {MONSTER_STATE_DEAD, {HUMAN_DeadEntry, HUMAN_Dead}},
    {MONSTER_STATE_EMBRACE, {HUMAN_EmbraceEntry, HUMAN_Embrace}},
    {MONSTER_STATE_COMBAT, {WORSHIP_CombatEntry, MON_Combat}},
    {-1, {NULL, NULL}}
};

void WORSHIP_Init(Instance *instance)
{
    MonsterAttributes *ma;
    MonsterVars *mv;
    Instance *weapon;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    if (mv->age == 0)
    {
        weapon = HUMAN_CreateWeapon(instance, 12, ma->rightWeaponSegment);
    }
    else
    {
        HUMAN_CreateWeapon(instance, 11, ma->leftWeaponSegment);

        weapon = HUMAN_CreateWeapon(instance, 11, ma->rightWeaponSegment);
    }

    if (weapon != NULL)
    {
        HUMAN_Init(instance);
    }

    if (mv->age == 0)
    {
        mv->soulJuice = 20480;
    }
    else
    {
        mv->soulJuice = 24576;
    }
}

void WORSHIP_CombatEntry(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    Instance *temp; // not from decls.h

    mv = (MonsterVars *)instance->extraData;

    if ((mv->enemy != NULL) && (mv->enemy->distance < mv->subAttr->combatAttributes->combatRange) && (mv->age == 1))
    {
        ma = (MonsterAttributes *)instance->data;

        temp = instance->LinkChild;

        if (temp != NULL)
        {
            if ((temp->LinkSibling == NULL) && (temp->ParentLinkNode == ma->rightWeaponSegment))
            {
                INSTANCE_UnlinkFromParent(temp);

                INSTANCE_LinkToParent(temp, instance, ma->leftWeaponSegment);

                mv->mvFlags |= 0x20;
            }
        }
        else
        {
            HUMAN_CreateWeapon(instance, 11, ma->leftWeaponSegment);

            mv->mvFlags |= 0x20;
        }
    }

    MON_CombatEntry(instance);
}
