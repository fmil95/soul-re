#include "Game/OBTABLE.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/REAVER.h"
#include "Game/GLYPH.h"
#include "Game/PHYSOBS.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/FX.h"
#include "Game/SCRIPT2.h"

// this file isn't defined in the SYM file but rather, the remaster's PDB.

ObjectFunc objectFunc[/*8*/] = 
{
    { "raziel__", RazielInit, RazielProcess, RazielCollide, (void *)RazielQuery, RazielPost, RazielAdditionalCollide, NULL, NULL },
    { "sreavr__", SoulReaverInit, SoulReaverProcess, SoulReaverCollide, (void *)SoulReaverQuery, SoulReaverPost, NULL, NULL, NULL },
    { "glphicon", GlyphInit, NULL, GlyphCollide, (void *)GlyphQuery, GlyphPost, NULL, NULL, NULL },
    { "physical", InitPhysicalObject, ProcessPhysicalObject, CollidePhysicalObject, (void *)PhysicalObjectQuery, PhysicalObjectPost, NULL, PhysicalRelocateTune, NULL },
    { "monster_", MonsterInit, MonsterProcess, MonsterCollide, (void *)MonsterQuery, MonsterMessage, MonsterAdditionalCollide, MonsterRelocateTune, MonsterRelocateInstanceObject },
    { "particle", NULL, NULL, NULL, NULL, NULL, NULL, FX_RelocateGeneric, NULL },
    { "litshaft", LitShaftInit, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
};
