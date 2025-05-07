#ifndef _SLUAGH_H_
#define _SLUAGH_H_

#include "common.h"

void SLUAGH_Attack(Instance *instance);
void SLUAGH_Death(Instance *instance);
void SLUAGH_AttackEntry(Instance *instance);
void SLUAGH_DeathEntry(Instance *instance);
void SLUAGH_DamageEffect(Instance *instance, evFXHitData *data);
uintptr_t SLUAGH_Query(Instance *instance, unsigned long query);

extern MonsterFunctionTable SLUAGH_FunctionTable;
extern MonsterStateChoice SOUL_StateChoiceTable[];

#endif
