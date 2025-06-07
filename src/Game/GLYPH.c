#include "Game/GLYPH.h"
#include "Game/CAMERA.h"
#include "Game/GAMELOOP.h"
#include "Game/STATE.h"
#include "Game/DEBUG.h"
#include "Game/SOUND.h"
#include "Game/FX.h"
#include "Game/MEMPACK.h"
#include "Game/MATH3D.h"
#include "Game/DRAW.h"
#include "Game/INSTANCE.h"
#include "Game/OBTABLE.h"
#include "Game/FONT.h"

static short MANNA_Position;

static short MANNA_Pos_vel;

static short glyph_time;

static short glyph_trigger;

static short glyph_cost;

static short HUD_Count;

static short HUD_Count_Overall;

static int blast_range;

static int fx_radius_old;

static int MANNA_Pickup_Time;

static short HUD_Rotation;

static short HUD_Rot_vel;

static short HUD_Position;

static short HUD_Pos_vel;

static short HUD_State;

static short HUD_Wait;

static SVector HUD_Cap_Pos;

static SVector HUD_Cap_Vel;

static int warpDraw;

static int glowdeg;

int hud_warp_arrow_flash;

static short fx_going = 0;

FXBlastringEffect *fx_blastring = NULL;

static short HUD_Captured = 0;

void GlyphInit(Instance *instance, GameTracker *gameTracker)
{
    GlyphData *data;

    (void)gameTracker;

    if ((instance->flags & 0x20000))
    {
        data = (GlyphData *)instance->extraData;

        MEMPACK_Free((char *)data);
    }
    else
    {
        data = (GlyphData *)MEMPACK_Malloc(sizeof(GlyphData), 29);

        instance->extraData = data;

        InitMessageQueue(&data->messages);

        EnMessageQueueData(&data->messages, 0x100001, 0);

        data->process = &_GlyphOffProcess;

        data->selectedGlyph = 7;

        data->target_glyph_rotation = 3510;

        data->glyph_time = 0;

        glyph_time = 0;

        data->glyph_radius = 0;
        data->glyph_scale = 0;
        data->glyph_movement = 1;
        data->glyph_open = 0;

        glyph_trigger = 0;

        fx_blastring = NULL;

        glyph_cost = -1;

        fx_going = 0;

        data->glyph_rotation = (data->selectedGlyph - 1) * 585;

        instance->flags |= 0x10800;
    }

    HUD_Init();

    MANNA_Pickup_Time = 0;

    MANNA_Position = -64;

    MANNA_Pos_vel = 0;
}

void GlyphCollide()
{
}

void GlyphProcess(Instance *instance, GameTracker *gameTracker)
{
    GlyphData *data;

    (void)gameTracker;

    data = (GlyphData *)instance->extraData;

    data->process(instance, 0, 0);

    instance->oldPos = instance->parent->position;

    instance->position = instance->oldPos;

    instance->currentStreamUnitID = instance->parent->currentStreamUnitID;

    instance->flags |= 0xC00;
}

unsigned long GlyphQuery(Instance *instance, unsigned long query)
{
    (void)instance;

    switch (query)
    {
    case 24:
        return 0;
    case 1:
        break;
    }

    return 0;
}

void GlyphPost(Instance *instance, unsigned long message, unsigned long messageData)
{
    GlyphData *data;

    data = (GlyphData *)instance->extraData;

    if (message != 0x100007)
    {
        EnMessageQueueData(&data->messages, message, messageData);
    }
}

void _GlyphSwitchProcess(Instance *instance, void (*process)())
{
    GlyphData *data;

    data = (GlyphData *)instance->extraData;

    PurgeMessageQueue(&data->messages);

    EnMessageQueueData(&data->messages, 0x100004, 0);

    data->process(instance, 0, 0);

    EnMessageQueueData(&data->messages, 0x100001, 0);

    data->process = process;

    data->process(instance, 0, 0);
}

int GlyphIsGlyphOpen(Instance *instance)
{
    GlyphData *data;

    data = (GlyphData *)instance->extraData;

    return data->glyph_open;
}

int _GlyphIsGlyphSet(int glyph)
{
    unsigned long abilities;

    abilities = INSTANCE_Query(gameTrackerX.playerInstance, 36) | debugRazielFlags3;

    return (1 << (glyph + 17)) & abilities;
}

int _GlyphIsGlyphUsable(int glyph)
{
    return (1 << (glyph + 17)) & INSTANCE_Query(gameTrackerX.playerInstance, 19);
}

int _GlyphIsAnyGlyphSet()
{
    unsigned long abilities;

    abilities = INSTANCE_Query(gameTrackerX.playerInstance, 36);

    abilities |= debugRazielFlags3;

    return abilities & 0x1FC0000;
}

int _GlyphCost(GlyphTuneData *glyphtunedata, int glyphNum)
{
    return (unsigned char)glyphtunedata->glyph_costs[glyphNum - 1];
}

void _GlyphDefaultProcess(Instance *instance, int data1, int data2)
{
    Message *Ptr;
    GlyphData *data;

    (void)data1;
    (void)data2;

    data = (GlyphData *)instance->extraData;

    while ((Ptr = PeekMessageQueue(&data->messages)))
    {
        if (Ptr != NULL)
        {
            switch (Ptr->ID)
            {
            case 0x80000010:
                _GlyphSwitchProcess(instance, &_GlyphSelectProcess);

                SndPlayVolPan(17, 127, 64, 0);
                break;
            }

            DeMessageQueue(&data->messages);
        }
    }
}

void HUD_GetPlayerScreenPt(DVECTOR *center)
{
    Position position;

    PushMatrix();

    SetRotMatrix(theCamera.core.wcTransform);
    SetTransMatrix(theCamera.core.wcTransform);

    position = gameTrackerX.playerInstance->position;

    position.z += 448;

    gte_ldv0(&position);
    gte_nrtps();
    gte_stsxy(center);

    PopMatrix();
}

void GlyphDrawMenu(Instance *instance)
{
    Position place;
    DVECTOR center;
    int n;
    int rot;
    int radius;
    int glyph_rotation_speed;
    int MANNA_Count;
    GlyphData *data;
    GlyphTuneData *glyphtunedata;

    glyph_rotation_speed = (gameTrackerX.timeMult * 64) >> 12;

    data = (GlyphData *)instance->extraData;

    glyphtunedata = (GlyphTuneData *)instance->object->data;

    MANNA_Count = INSTANCE_Query(gameTrackerX.playerInstance, 32);

    if (data->target_glyph_rotation != data->glyph_rotation)
    {
        int diff;

        diff = AngleDiff(data->target_glyph_rotation, data->glyph_rotation);

        if (glyph_rotation_speed >= ABS(diff))
        {
            data->glyph_rotation = data->target_glyph_rotation;
        }
        else if (data->glyph_movement > 0)
        {
            data->glyph_rotation = (data->glyph_rotation + glyph_rotation_speed) & 0xFFF;
        }
        else
        {
            data->glyph_rotation = (data->glyph_rotation - glyph_rotation_speed) & 0xFFF;
        }
    }

    HUD_GetPlayerScreenPt(&center);

    if (center.vy < 72)
    {
        center.vy = 72;
    }

    radius = data->glyph_radius;

    rot = data->glyph_rotation + 3072;

    for (n = 0; n < 7; rot -= 585, n++)
    {
        int enabled;
        int scale_modify;
        int num;
        int temp; // not from decls.h

        rot &= 0xFFF;

        if (AngleDiff(rot, 3072) >= 0)
        {
            temp = 4096;

            scale_modify = temp - (AngleDiff(rot, 3072) << 1);
        }
        else
        {
            scale_modify = temp + (AngleDiff(rot, 3072) << 1); // bug in the original code?
        }

        if (scale_modify < 1536)
        {
            scale_modify = 1536;
        }

        scale_modify = (data->glyph_scale * scale_modify) / 4096;

        place.x = center.vx + ((rcos(rot) * radius) >> 12);
        place.y = center.vy - (((rsin(rot) * radius) >> 12) / 8);

        if (_GlyphIsGlyphSet(n + 1) != 0)
        {
            num = n;

            if (_GlyphIsGlyphUsable(n + 1) != 0)
            {
                if (_GlyphCost(glyphtunedata, n + 1) <= MANNA_Count)
                {
                    enabled = 1;
                }
                else
                {
                    enabled = 0;
                }
            }
            else
            {
                enabled = 0;
            }
        }
        else if ((n + 1) != 7)
        {
            num = 7;

            enabled = 1;
        }
        else
        {
            num = n;

            enabled = 0;
        }

        FX_MakeGlyphIcon(&place, instance->object, (glyphtunedata->glyph_size * scale_modify) / 8192, num, enabled);
    }

    if ((data->glyph_time == 4096) && (data->target_glyph_rotation == data->glyph_rotation) && (MANNA_Count >= _GlyphCost(glyphtunedata, data->selectedGlyph)))
    {
        Vector f1;

        f1.x = center.vx;
        f1.y = center.vy + (radius / 8);

        DRAW_CreateAGlowingCircle(&f1, 320, gameTrackerX.primPool, gameTrackerX.drawOT, 5, 0x1404040, 20, 24, 0);
    }

    FX_DrawScreenPoly(2, (data->glyph_time / glyphtunedata->glyph_darkness) | (((data->glyph_time / glyphtunedata->glyph_darkness) << 16) | ((data->glyph_time / glyphtunedata->glyph_darkness) << 8)), 32);
}

long GlyphTime(int time)
{
    time *= time;

    return time / 4096;
}

void ShrinkGlyphMenu(Instance *instance)
{
    GlyphData *data;
    int time;
    short accl;

    data = (GlyphData *)instance->extraData;

    gameTrackerX.streamFlags &= ~0x100000;

    if (data->glyph_time > 0)
    {
        data->glyph_time -= (short)((gameTrackerX.timeMult * 512) >> 12);

        if (data->glyph_time < 0)
        {
            data->glyph_time = 0;
        }

        glyph_time = data->glyph_time;

        time = GlyphTime(data->glyph_time);

        data->glyph_radius = (time * 150) / 4096;

        data->glyph_scale = time;

        if (data->glyph_time > 0)
        {
            GlyphDrawMenu(instance);
        }
    }
    else
    {
        glyph_cost = -1;
    }

    if (MANNA_Pickup_Time <= 0)
    {
        CriticalDampValue(5, &MANNA_Position, -64, &MANNA_Pos_vel, &accl, 12);
    }
    else
    {
        MANNA_Pickup_Time -= gameTrackerX.timeMult;
    }

    data->glyph_open = 0;
}

void EnlargeGlyphMenu(Instance *instance)
{
    GlyphData *data;
    int time;
    short accl;

    data = (GlyphData *)instance->extraData;

    gameTrackerX.streamFlags |= 0x100000;

    if (data->glyph_time < 4096)
    {
        data->glyph_time += (short)((gameTrackerX.timeMult * 512) >> 12);

        if (data->glyph_time >= 4097)
        {
            data->glyph_time = 4096;
        }

        glyph_time = data->glyph_time;
    }

    time = GlyphTime(data->glyph_time);

    data->glyph_radius = (time * 150) / 4096;

    data->glyph_scale = time;

    GlyphDrawMenu(instance);

    MANNA_Pickup_Time = 0;

    CriticalDampValue(5, &MANNA_Position, 24, &MANNA_Pos_vel, &accl, 12);

    data->glyph_open = 1;
}

void _GlyphOffProcess(Instance *instance, int data1, int data2)
{
    Message *Ptr;
    GlyphData *data;

    data = (GlyphData *)instance->extraData;

    ShrinkGlyphMenu(instance);

    while ((Ptr = PeekMessageQueue(&data->messages)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100004:
        case 0x100001:
            break;
        case 0x80000010:
            if (data->process == &_GlyphSelectProcess)
            {
                _GlyphSwitchProcess(instance, _GlyphOffProcess);
            }
            else
            {
                _GlyphSwitchProcess(instance, _GlyphSelectProcess);
            }

            break;
        default:
            _GlyphDefaultProcess(instance, data1, data2);
        }

        DeMessageQueue(&data->messages);
    }

    if (data->process == &_GlyphSelectProcess)
    {
        SndPlayVolPan(17, 127, 64, 0);
    }

    Glyph_DoFX(instance);
}

INCLUDE_ASM("asm/nonmatchings/Game/GLYPH", _GlyphSelectProcess);

void Glyph_StartSpell(Instance *instance, int glyphnum)
{
    int message;

    (void)instance;

    message = 0;

    switch (glyphnum)
    {
    case 6:
        message = 0x80006;
        break;
    case 4:
        message = 0x80004;
        break;
    case 5:
        message = 0x80005;
        break;
    case 2:
        message = 0x80002;
        break;
    case 1:
        message = 0x80001;
        break;
    case 3:
        message = 0x80003;
        break;
    case 7:
        message = 0x80007;
        break;
    }

    INSTANCE_Post(gameTrackerX.playerInstance, message, 0);
}

INCLUDE_ASM("asm/nonmatchings/Game/GLYPH", Glyph_Broadcast);

void Glyph_DoSpell(Instance *instance, int glyphnum)
{
    int fx_radius;
    // int fx_accl; unused
    int fx_vel;
    long fx_color;
    int fx_height1;
    int fx_height2;
    int fx_height3;
    int fx_rad2;
    int fx_rad3;
    int pred_offset;
    int color_change_radius;
    MATRIX mat;
    GlyphTuneData *glyphtunedata;
    int temp; // not from decls.h

    fx_radius = 0;

    fx_vel = 0;

    fx_color = 0;

    fx_height1 = 0;
    fx_height2 = 0;
    fx_height3 = 0;

    fx_rad2 = 0;
    fx_rad3 = 0;

    glyphtunedata = (GlyphTuneData *)instance->object->data;

    fx_going = 0;

    MATH3D_SetUnityMatrix(&mat);

    pred_offset = 0;

    fx_radius_old = 0;

    color_change_radius = glyphtunedata->glyph_range[glyphnum - 1];

    blast_range = color_change_radius;

    temp = color_change_radius - (color_change_radius / 4);

    switch (glyphnum)
    {
    case 6:
        fx_radius = 1;

        fx_color = 0xFFFFFF;

        fx_height1 = -512;
        fx_height2 = 0;
        fx_height3 = 512;

        fx_vel = 1048576;

        fx_rad2 = 640;
        fx_rad3 = 832;

        fx_going = 6;
        break;
    case 4:
        fx_radius = 1;

        fx_color = 0xFF0000;

        fx_height1 = -512;
        fx_height2 = 0;
        fx_height3 = 512;

        fx_vel = 1048576;

        fx_rad2 = 640;
        fx_rad3 = 832;

        fx_going = 4;
        break;
    case 5:
        fx_radius = 1;

        fx_vel = 1048576;

        fx_color = 0xFF;

        fx_height1 = -512;
        fx_height2 = 0;
        fx_height3 = 512;

        fx_rad2 = 640;
        fx_rad3 = 832;

        fx_going = 5;
        break;
    case 2:
        CAMERA_SetShake(&theCamera, 60, 2048);

        fx_radius = 640;

        fx_vel = 1310720;

        fx_color = 0xA0A0A0;

        fx_height3 = 0;
        fx_height2 = 0;
        fx_height1 = 0;

        fx_rad2 = 640;
        fx_rad3 = 832;

        fx_going = 2;
        break;
    case 1:
        fx_radius = 1;

        fx_color = 0xC0C0C0;

        fx_height1 = -256;
        fx_height2 = 256;
        fx_height3 = 768;

        fx_vel = 2097152;

        fx_rad2 = -640;
        fx_rad3 = 0;

        fx_going = 1;
        break;
    case 3:
        fx_radius = 1;

        fx_color = 0xFFFFFF;

        fx_height1 = 0;
        fx_height2 = 64;
        fx_height3 = 128;

        fx_vel = 1048576;

        fx_rad2 = 640;
        fx_rad3 = 832;

        fx_going = 3;

        pred_offset = 5;
        break;
    case 7:
        break;
    }

    if (fx_going != 0)
    {
        fx_blastring = FX_DoBlastRing(NULL, (SVector *)&instance->position, &mat, -1, fx_radius, blast_range, temp, fx_rad2, fx_rad3, fx_vel, -2048, fx_height1, fx_height2, fx_height3, fx_color, 0, pred_offset, -99, 0);

        if (fx_blastring == NULL)
        {
            fx_going = 0;
        }
    }

    blast_range <<= 12;
}

void Glyph_EndFX()
{
    fx_blastring->lifeTime = 0;

    fx_going = 0;

    fx_blastring = NULL;
}

void Glyph_DoFX(Instance *instance)
{
    if ((gameTrackerX.gameMode != 6) && (!(gameTrackerX.streamFlags & 0x100000)) && (fx_going != 0))
    {
        Glyph_Broadcast(instance, fx_going);

        fx_radius_old = fx_blastring->radius;

        if (fx_blastring->radius > blast_range)
        {
            Glyph_EndFX();
        }

        if (fx_going == 5)
        {
            fx_blastring->height1 -= 60;
            fx_blastring->height3 += 60;
        }
    }
}

void _GlyphGenericProcess(Instance *instance, int data1, int data2)
{
    Message *Ptr;
    GlyphData *data;

    data = (GlyphData *)instance->extraData;

    ShrinkGlyphMenu(instance);

    while (Ptr = PeekMessageQueue(&data->messages), Ptr != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            Glyph_StartSpell(instance, data->selectedGlyph);
            break;
        case 0x100004:
            break;
        case 0x80000000:
            _GlyphSwitchProcess(instance, &_GlyphOffProcess);
            break;
        default:
            _GlyphDefaultProcess(instance, data1, data2);
        }

        DeMessageQueue(&data->messages);
    }

    if (glyph_trigger != 0)
    {
        GlyphTuneData *glyphtunedata;

        glyphtunedata = (GlyphTuneData *)instance->object->data;

        Glyph_DoSpell(instance, data->selectedGlyph);

        glyph_trigger = 0;

        INSTANCE_Post(gameTrackerX.playerInstance, 0x40008, _GlyphCost(glyphtunedata, data->selectedGlyph));
    }

    Glyph_DoFX(instance);
}

void GlyphTrigger()
{
    if (fx_going == 0)
    {
        glyph_trigger = 1;
    }
}

void MANNA_Pickup()
{
    MANNA_Pickup_Time = 122880;

    MANNA_Position = 24;
}

void HEALTHU_Pickup(Instance *instance)
{
    ApplyMatrixSV(theCamera.core.wcTransform2, (SVECTOR *)&instance->position, (SVECTOR *)&HUD_Cap_Pos);

    HUD_Cap_Pos.x += (short)theCamera.core.wcTransform2->t[0];
    HUD_Cap_Pos.y += (short)theCamera.core.wcTransform2->t[1];
    HUD_Cap_Pos.z += (short)theCamera.core.wcTransform2->t[2];

    HUD_Cap_Vel.z = 0;
    HUD_Cap_Vel.y = 0;
    HUD_Cap_Vel.x = 0;

    INSTANCE_KillInstance(instance);

    HUD_Captured = 1;

    HUD_State = 1;
}

void HUD_Damp(short *val, short target, short *vel, short max)
{
    short accl;

    CriticalDampValue(1, val, target, vel, &accl, max);
}

void HUD_Init()
{
    HUD_Rotation = 0;

    HUD_Rot_vel = 0;
    HUD_Pos_vel = 0;

    HUD_State = 0;

    HUD_Wait = 0;

    HUD_Position = -1000;

    HUD_Count = 0;
    HUD_Count_Overall = 0;

    warpDraw = 0;

    glowdeg = 0;

    hud_warp_arrow_flash = 0;
}

void HUD_Setup_Chit_Count(int chits)
{
    HUD_Count_Overall = chits;

    HUD_Count = chits % 5;
}

void HUD_Update()
{
    short accl;

    if ((gameTrackerX.gameMode == 6) && (HUD_State == 0))
    {
        HUD_State = 10;
    }
    else if (gameTrackerX.gameMode != 6)
    {
        if ((HUD_State == 10) || (HUD_State == 11))
        {
            HUD_State = 12;
        }
    }
    else if (HUD_State > 1)
    {
        if (HUD_State < 7)
        {
            return;
        }
    }

    if (HUD_Wait > 0)
    {
        HUD_Wait--;
        return;
    }

    switch (HUD_State)
    {
    default:
    case 0:
    case 7:
    case 8:
    case 9:
        HUD_Position = -1000;
        HUD_Rotation = 0;

        HUD_Rot_vel = 0;
        HUD_Pos_vel = 0;

        if (HUD_Count == 5)
        {
            HUD_Count = 0;
        }

        break;
    case 3:
    case 4:
    case 11:
        break;
    case 1:
    case 10:
        HUD_Damp(&HUD_Position, 0, &HUD_Pos_vel, 96);

        if (HUD_Position >= 0)
        {
            HUD_Position = 0;

            if (HUD_State != 10)
            {
                if (HUD_Count <= 0)
                {
                    HUD_State = 3;
                }
                else
                {
                    HUD_State = 2;
                }
            }
            else
            {
                HUD_State = 11;
            }
        }

        if (HUD_State == 10)
        {
            MANNA_Pickup_Time = 0;

            CriticalDampValue(5, &MANNA_Position, 24, &MANNA_Pos_vel, &accl, 12);
        }

        break;
    case 2:
        HUD_Damp(&HUD_Rotation, 819, &HUD_Rot_vel, 80);

        if (HUD_Rotation >= 819)
        {
            HUD_Rotation = 819;

            HUD_State = 3;
        }

        break;
    case 5:
        HUD_State = 6;

        HUD_Wait = 10;

        HUD_Rotation = 0;

        HUD_Captured = 0;

        HUD_Count++;
        HUD_Count_Overall++;
        break;
    case 6:
    case 12:
        HUD_Damp(&HUD_Position, -1000, &HUD_Pos_vel, 96);

        if (HUD_Position < -999)
        {
            HUD_State = 0;
        }

        break;
    }
}

extern char D_800D1A94[];
extern char D_800D1A98[];
void HUD_Draw()
{
    SVector Rotation;
    SVector Pos;
    SVector offset;
    int n;
    ObjectAccess *temp; // not from decls.h

    if (((theCamera.instance_mode & 0x80000000)) || (warpDraw != 0))
    {
        DVECTOR center;
        int glow;
        int left;
        int right;

        if ((theCamera.instance_mode & 0x80000000))
        {
            warpDraw += gameTrackerX.timeMult / 16;

            if (warpDraw > 4096)
            {
                warpDraw = 4096;
            }
        }
        else
        {
            warpDraw -= gameTrackerX.timeMult / 16;

            if (warpDraw < 0)
            {
                warpDraw = 0;
            }
        }

        HUD_GetPlayerScreenPt(&center);

        glow = (warpDraw / 3) + (((warpDraw / 10) * rcos(glowdeg)) / 4096);

        if (glow < 0)
        {
            glow = 0;
        }

        glowdeg += gameTrackerX.timeMult / 32;

        if (hud_warp_arrow_flash > 0)
        {
            right = glow + hud_warp_arrow_flash;

            hud_warp_arrow_flash -= gameTrackerX.timeMult / 8;

            if (hud_warp_arrow_flash < 0)
            {
                hud_warp_arrow_flash = 0;
            }
        }
        else
        {
            right = glow;
        }

        if (hud_warp_arrow_flash < 0)
        {
            left = glow - hud_warp_arrow_flash;

            hud_warp_arrow_flash += gameTrackerX.timeMult / 8;

            if (hud_warp_arrow_flash > 0)
            {
                hud_warp_arrow_flash = 0;
            }
        }
        else
        {
            left = glow;
        }

        FX_MakeWarpArrow(center.vx - 64, center.vy, -64, 32, left);
        FX_MakeWarpArrow(center.vx + 64, center.vy, 64, 32, right);
    }

    HUD_Update();

    offset.x = 0;
    offset.y = 0;
    offset.z = 135;

    if (MANNA_Position > -64)
    {
        int oldx;
        int oldy;
        int MANNA_Count;
        int MANNA_Max;

        oldx = fontTracker.font_xpos;
        oldy = fontTracker.font_ypos;

        MANNA_Count = INSTANCE_Query(gameTrackerX.playerInstance, 32);
        MANNA_Max = INSTANCE_Query(gameTrackerX.playerInstance, 45);

        FX_MakeMannaIcon(MANNA_Position, 23, 51, 32);

        FONT_Flush();

        FONT_SetCursor(MANNA_Position + 58, 32);

        if (glyph_cost != -1)
        {
            FONT_Print("%d/", glyph_cost);
        }

        if (MANNA_Count >= MANNA_Max)
        {
            FONT_SetColorIndex(2);
        }

        FONT_Print("%d", MANNA_Count);

        FONT_SetColorIndex(0);
        FONT_SetCursor(oldx, oldy);

        FONT_Flush();
    }

    if (HUD_Position > -1000)
    {
        SVector targetPos;
        SVector accl;
        SVector HUD_Cap_Rot;

        if ((HUD_Captured != 0) && (gameTrackerX.gameMode != 6))
        {
            HUD_Cap_Rot.x = 1024;
            HUD_Cap_Rot.y = 0;
            HUD_Cap_Rot.z = 0;

            targetPos.x = -1536;
            targetPos.z = 2560;

            if (HUD_State < 4)
            {
                targetPos.y = 288;
            }
            else
            {
                targetPos.y = 608;
            }

            CriticalDampPosition(1, (Position *)&HUD_Cap_Pos, (Position *)&targetPos, &HUD_Cap_Vel, &accl, 128);

            if ((HUD_Cap_Vel.x == 0) && (HUD_Cap_Vel.y == 0) && (HUD_Cap_Vel.z == 0))
            {
                if (HUD_State == 3)
                {
                    HUD_State = 4;
                }
                else if (HUD_State == 4)
                {
                    HUD_State = 5;
                }
            }

            FX_DrawModel((Object *)objectAccess[21].object, 0, &HUD_Cap_Rot, &HUD_Cap_Pos, &offset, 0);
        }

        Rotation.x = 1024;
        Rotation.y = 0;
        Rotation.z = HUD_Rotation;

        Pos.x = HUD_Position - 1536;
        Pos.y = 608;
        Pos.z = 2560;

        offset.z = 0;

        if ((HUD_Count != 0) || (HUD_Count_Overall < 15))
        {
            for (n = 0; n < 5; n++)
            {
                temp = objectAccess;

                switch (n)
                {
                case 0:
                    Rotation.z = 0;
                    break;
                case 1:
                    Rotation.z = 3277;
                    break;
                case 2:
                    Rotation.z = 2458;
                    break;
                case 3:
                    Rotation.z = 1638;
                    break;
                case 4:
                    Rotation.z = 819;
                    break;
                }

                Rotation.z = (Rotation.z - HUD_Rotation) & 0xFFF;

                FX_DrawModel((Object *)temp[24].object, 0, &Rotation, &Pos, &offset, (n < HUD_Count) == 0);
            }
        }
    }
}
