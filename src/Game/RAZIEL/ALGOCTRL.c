#include "Game/RAZIEL/ALGOCTRL.h"

static int AlgoControlFlag = 0;

void InitAlgorithmicWings(Instance *instance)
{
    G2EulerAngles Rot;
    unsigned char i;

    if (!(AlgoControlFlag & 0x1))
    {
        if (instance->matrix == NULL)
        {
            INSTANCE_Post(instance, 0x100006, 0);
        }
        else
        {
            for (i = 51; i < 54; i++)
            {
                if (instance->matrix == NULL)
                {
                    Rot.z = 0;
                    Rot.y = 0;
                    Rot.x = 0;
                }
                else
                {
                    G2EulerAngles_FromMatrix(&Rot, &instance->anim.segMatrices[i - 1], 21);
                }

                G2Anim_EnableController(&instance->anim, i, 8);

                G2EmulationSetInterpController_Vector(instance, i, 8, (G2SVector3 *)&Rot, (i - 50) * 3, 2);
            }

            for (i = 59; i < 62; i++)
            {
                if (instance->matrix == NULL)
                {
                    Rot.z = 0;
                    Rot.y = 0;
                    Rot.x = 0;
                }
                else
                {
                    G2EulerAngles_FromMatrix(&Rot, &instance->anim.segMatrices[i - 1], 21);
                }

                G2Anim_EnableController(&instance->anim, i, 8);

                G2EmulationSetInterpController_Vector(instance, i, 8, (G2SVector3 *)&Rot, (i - 58) * 3, 2);
            }

            AlgoControlFlag |= 0x1;
        }
    }
}

void DeInitAlgorithmicWings(Instance *instance)
{
    unsigned char i;

    if ((AlgoControlFlag & 0x1))
    {
        for (i = 51; i < 54; i++)
        {
            G2Anim_DisableController(&instance->anim, i, 8);
        }

        for (i = 59; i < 62; i++)
        {
            G2Anim_DisableController(&instance->anim, i, 8);
        }

        AlgoControlFlag &= ~0x1;
    }
}

void AlgorithmicWings(Instance *instance, evAnimationControllerDoneData *ControllerData)
{
    G2EulerAngles Rot;
    int temp; // not from decls.h

    temp = ControllerData->segment;

    do
    {
        instance++;
        instance--;
        // this do while is fake, only the following two function calls are valid code
        G2EulerAngles_FromMatrix(&Rot, &instance->anim.segMatrices[temp - 1], 21);
        G2EmulationSetInterpController_Vector(instance, ControllerData->segment, ControllerData->type, (G2SVector3 *)&Rot, 5, 2);
    } while (0);
}

void AlgorithmicNeck(Instance *Player, Instance *Target)
{
    Position From;
    Position To;
    Rotation Rot1;
    int Diff;
    MATRIX matrix;

    Raziel.Senses.Flags &= ~0x8;

    if ((Raziel.Senses.Flags & 0x10))
    {
        evCollideInstanceStatsData data;

        TransposeMatrix(Player->oldMatrix, &matrix);

        if ((INSTANCE_SetStatsData(Player, NULL, &Raziel.Senses.lookAtPoint, &data, &matrix) != 0) && (data.distance < 3200) && (MATH3D_ConeDetect(&data.relativePosition, 967, 967) != 0))
        {
            Raziel.Senses.Flags |= 0x8;
        }
    }

    if ((Target == NULL) && (!(Raziel.Senses.Flags & 0x8)))
    {
        if (G2Anim_IsControllerActive(&Player->anim, 17, 8) != G2FALSE)
        {
            G2Anim_InterpDisableController(&Player->anim, 17, 8, 900);
        }

        return;
    }

    if (G2Anim_IsControllerActive(&Player->anim, 17, 8) == G2FALSE)
    {
        G2Anim_EnableController(&Player->anim, 17, 8);
    }

    if ((Raziel.Senses.Flags & 0x8))
    {
        To.x = (short)Raziel.Senses.lookAtPoint.x;
        To.y = (short)Raziel.Senses.lookAtPoint.y;
        To.z = (short)Raziel.Senses.lookAtPoint.z;
    }
    else
    {
        MATRIX *matrix;

        matrix = (MATRIX *)INSTANCE_Query(Target, queryLookatMatrix);

        if (matrix == NULL)
        {
            return;
        }

        To.x = (short)matrix->t[0];
        To.y = (short)matrix->t[1];
        To.z = (short)matrix->t[2];
    }

    From.x = (short)Player->matrix[17].t[0];
    From.y = (short)Player->matrix[17].t[1];
    From.z = (short)Player->matrix[17].t[2];

    MATH3D_RotationFromPosToPos(&From, &To, &Rot1);

    Diff = AngleDiff(Player->rotation.z, Rot1.z);

    if (Diff > 512)
    {
        Rot1.z = Player->rotation.z + 512;
    }

    if (Diff < -512)
    {
        Rot1.z = Player->rotation.z - 512;
    }

    if ((Rot1.x > 512) && (Rot1.x < 2048))
    {
        Rot1.x = 512;
    }

    if ((Rot1.x > 2048) && (Rot1.x < 3438))
    {
        Rot1.x = 3438;
    }

    MATH3D_ZYXtoXYZ(&Rot1);

    G2EmulationSetInterpController_Vector(Player, 17, 8, (G2SVector3 *)&Rot1, 3, 0);
}
