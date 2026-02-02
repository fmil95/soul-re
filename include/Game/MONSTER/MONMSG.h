#ifndef _MONMSG_H_
#define _MONMSG_H_

#include "common.h"

void MON_IdleMessageHandler(Instance *instance, Message *message);
void MON_DefaultMessageHandler(Instance *instance, Message *message);
void MON_DefaultQueueHandler(Instance *instance);
void MON_PupateMessageHandler(Instance *instance, Message *message);
void MON_IdleQueueHandler(Instance *instance);
int MON_GroundMoveQueueHandler(Instance *instance);
void MON_PupateQueueHandler(Instance *instance);

#endif
