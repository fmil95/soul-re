

// April 14th
void COLLIDE_NearestPointOnPlane_C(SVector *planePoint, SVector *normal, long constant, SVector *point) 
{
    Vector *dpv; 

    dpv = (Vector*)getScratchAddr(0);

    gte_SetRotMatrix(point);   
    gte_ldv0(normal);
    gte_nrtv0();
    gte_stlvnl(dpv);
    
    dpv->x = constant - dpv->x;
    
    planePoint->x = (short)(point->x + ((normal->x * dpv->x) >> 12));
    planePoint->y = (short)(point->y + ((normal->y * dpv->x) >> 12));
    planePoint->z = (short)(point->z + ((normal->z * dpv->x) >> 12));
} 

// February 16th
long BSP_SphereIntersectsViewVolume_C(Sphere *sphere, Camera *camera) 
{
    VECTOR dpv[2]; 

    gte_SetRotMatrix(&camera->core.vvNormalWorVecMat[0]);
    gte_ldv0(&sphere->position);
    gte_nrtv0();
    gte_stlvnl(&dpv[0]);
    
    dpv[0].vx -= camera->core.vvPlaneConsts[0];
    
    if (-sphere->radius < dpv[0].vx) 
    {
        if (dpv[0].vx < (camera->core.farPlane + sphere->radius)) // mismatching struct field offset. farPlane is just a guess.
        { 
            if ((-sphere->radius < (dpv[0].vy - camera->core.vvPlaneConsts[1])) && ((-sphere->radius < (dpv[0].vz - camera->core.vvPlaneConsts[2])))) 
            {
                gte_SetRotMatrix(&camera->core.vvNormalWorVecMat[1]);
                gte_ldv0(&sphere->position);
                gte_nrtv0();
                gte_stlvnl(&dpv[1]);
                
                if (((dpv[1].vx - camera->core.vvPlaneConsts[3]) > -sphere->radius) && ((dpv[1].vy - camera->core.vvPlaneConsts[4]) > -sphere->radius))
                {
                    return 1;
                }
            }
        }
    }
    
    return 0;
}

// April 14th
short MATH3D_racos_C(short cos_ang_dest_act)
{
    short test_angle; 
    short cos_ang; 
    short step; 
    short cos_ang_dest; 

    test_angle = 512;
    
    cos_ang = 0;
    
    step = 256;
    
    if (cos_ang_dest_act >= 0) 
    {
        cos_ang_dest = cos_ang_dest_act;
    }
    else 
    {
        cos_ang_dest = -cos_ang_dest_act;
    }
    
    for ( ; (cos_ang != cos_ang_dest) && (step != 0); step >>= 1)
    {
        cos_ang = rcos(test_angle);
        
        if (cos_ang < cos_ang_dest)
        {
            test_angle -= step;
        }
        else 
        {
            test_angle += step;
        }
    }
    
    if (cos_ang_dest_act < 0) 
    {
        test_angle = 2048 - test_angle;
    }
    
    return test_angle;
}
