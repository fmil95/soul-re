#ifndef _LOCALSTR_H_
#define _LOCALSTR_H_

#include "common.h"

language_t localstr_get_language();
char *localstr_get(localstr_t id);
void localstr_set_language(language_t lang);

#endif
