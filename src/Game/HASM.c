
/* This is just a raw file containing C versions of code handwritten in MIPS. These functions have been extracted from earlier prototypes of the game. */

// February 16th
void BSP_MarkVisibleLeaves_C(Terrain *terrain, Camera *camera, PolytopeList *polytopeList, int curTree) 
{
    BSPScratch *Spad; 
    BSPNode **stack; 
    BSPNode *bspNode; 
    long farPlane; 
    int numPolytopes; 
    int radius; 
    BSPNode *temp; // not from SYM
    char temp2;    // not from SYM
    
    temp2 = 0;
    
    farPlane = depthQFogFar;
    
    stack = (BSPNode**)getScratchAddr(51);
    
    Spad = (BSPScratch*)getScratchAddr(0);

    if (camera->farPlane < depthQFogFar)
    {
        farPlane = camera->farPlane;
    }

    stack[0] = (BSPNode*)stack;
    
    numPolytopes = 0;
    
    if (gameTrackerX.MorphTime != 1000)
    {
        Spad->in_spectral = 2;
    } 
    else if (gameTrackerX.MorphType == 1)
    {
        Spad->in_spectral = 1;
    }
    else
    {
        Spad->in_spectral = 0;
    }
    
    Spad->vv[0] = camera->vvNormalWorVecMat[0];
    Spad->vv[1] = camera->vvNormalWorVecMat[1];

    gte_SetRotMatrix(&Spad->vv[0]);
    gte_ldv0(&camera->position); 
    gte_nrtv0();
    gte_stlvnl(&Spad->d);

    gte_SetRotMatrix(&Spad->vv[1]);
    gte_ldv0(&camera->position);
    gte_nrtv0();
    gte_stlvnl(&Spad->d[3]);
    
    Spad->camMatrix.m[0][0] = camera->position.x;
    Spad->camMatrix.m[0][1] = camera->position.y;
    Spad->camMatrix.m[0][2] = camera->position.z;

    {
    SVECTOR *tmpvect; 
    SVECTOR *campoints_rot; 
        
    tmpvect = (SVECTOR*)&Spad->dpv[0];
        
    tmpvect->vx = (camera->halfwidth_per_z  * farPlane) >> 15;
    tmpvect->vy = (camera->halfheight_per_z * farPlane) >> 15;
    tmpvect->vz = farPlane;

    tmpvect       = (SVECTOR*)&Spad->dpv[0];
    campoints_rot = (SVECTOR*)&Spad->dpv[1];
        
    SetRotMatrix(camera->cwTransform2);
    gte_ldv0(tmpvect);
    gte_nrtv0();
    gte_stsv(campoints_rot);
    
    Spad->camMatrix.m[1][0] = campoints_rot->vx;
    Spad->camMatrix.m[1][1] = campoints_rot->vy;
    Spad->camMatrix.m[1][2] = campoints_rot->vz;
        
    tmpvect->vy = -tmpvect->vy;
        
    gte_ldv0(tmpvect);
    gte_nrtv0();
    gte_stsv(&Spad->camMatrix.m[2]);
    
    tmpvect->vx = -tmpvect->vx;
        
    gte_ldv0(tmpvect);
    gte_nrtv0();
    gte_stsv(&Spad->boundMatrix);
    
    tmpvect->vy = -tmpvect->vy;
        
    gte_ldv0(tmpvect);
    gte_nrtv0();
    gte_stsv(campoints_rot);

    tmpvect = (SVECTOR*)&Spad->dpv[1];
        
    Spad->boundMatrix.m[1][0] = tmpvect->vx;
    Spad->boundMatrix.m[1][1] = tmpvect->vy;
    Spad->boundMatrix.m[1][2] = tmpvect->vz;
    }
    
    SetRotMatrix(&Spad->vv[0]);
    
    *stack++ = (BSPNode*)stack;
    *stack   = terrain->BSPTreeArray[curTree].bspRoot;
    
    while ((int)*stack != (int)stack) 
    {
        if (stack != NULL) 
        {
            temp2 = -temp2;
        }
        
        bspNode = *stack--;
        
        gte_ldv0(&bspNode->sphere.position);
        gte_nrtv0();
        
        radius = -bspNode->sphere.radius;
        
        gte_stlvnl(&Spad->dpv[0]);
        
        Spad->dpv[0].vx -= Spad->d[0];
        
        if (((radius < Spad->dpv[0].vx) && (Spad->dpv[0].vx < (farPlane - radius))) && (radius < (Spad->dpv[0].vy - Spad->d[1])) && (radius < (Spad->dpv[0].vz - Spad->d[2]))) 
        {
            gte_SetRotMatrix(&Spad->vv[1]);
            gte_nrtv0();
            gte_stlvnl(&Spad->dpv[1]);
            
            if ((radius < (Spad->dpv[1].vx - Spad->d[3])) && (radius < (Spad->dpv[1].vy - Spad->d[4]))) 
            {
                if (bspNode->flags & 0x1) 
                {
                    int plane_front_error; 
                    int plane_back_error; 
                    
                    if (Spad->in_spectral != 0) 
                    {
                        plane_front_error = bspNode->front_spectral_error;
                        plane_back_error  = bspNode->back_spectral_error;
                    } 
                    else 
                    {
                        plane_front_error = bspNode->front_material_error;
                        plane_back_error  = bspNode->back_material_error;
                    }
                    
                    gte_SetRotMatrix(&Spad->camMatrix);
                    gte_ldv0(&bspNode->a);
                    gte_nrtv0();
                    gte_stlvnl(&Spad->dpv[2]);

                    Spad->dpv[2].vx -= bspNode->d;
                    
                    if (Spad->dpv[2].vx >= 0) 
                    {
                        if (bspNode->back != NULL)
                        {
                            long tmpval; 
                             
                            tmpval = -Spad->dpv[2].vx;
                            
                            if ((plane_front_error < Spad->dpv[2].vx) && (plane_front_error < (Spad->dpv[2].vy - tmpval)) && (plane_front_error < (Spad->dpv[2].vz - tmpval))) 
                            {
                                gte_SetRotMatrix(&Spad->boundMatrix);
                                gte_nrtv0();
                                gte_stlvnl(&Spad->dpv[1]);
                                
                                if ((plane_front_error >= (Spad->dpv[1].vx - tmpval)) || (plane_front_error >= (Spad->dpv[1].vy - tmpval))) 
                                { 
                                    *++stack = bspNode->back;
                                }
                             } 
                            else
                            {
                                *++stack = bspNode->back;
                            }
                        }
                        
                        temp = bspNode->front;
                    }
                    else
                    {
                        if (bspNode->front != NULL) 
                        {
                            long tmpval; 
                            
                            tmpval = -Spad->dpv[2].vx;
                            
                            if ((Spad->dpv[2].vx < plane_back_error) && ((Spad->dpv[2].vy - tmpval) < plane_back_error) && ((Spad->dpv[2].vz - tmpval) < plane_back_error)) 
                            {
                                gte_SetRotMatrix(&Spad->boundMatrix);
                                gte_nrtv0();
                                gte_stlvnl(&Spad->dpv[1]);
                                
                                if (((Spad->dpv[1].vx - tmpval) >= plane_back_error) || ((Spad->dpv[1].vy - tmpval) >= plane_back_error)) 
                                { 
                                    *++stack = bspNode->front;
                                }
                            }
                            else 
                            {
                                *++stack = bspNode->front;
                            }
                        } 
                        
                        temp = bspNode->back;
                    }

                    if (temp != NULL) 
                    {
                        *++stack = temp;
                    }
                } 
                else if (numPolytopes < 1024)
                {
                    polytopeList->polytope[numPolytopes++] = bspNode;
                }
            }
            
            gte_SetRotMatrix(&Spad->vv[0]);
        }
    }
    
    polytopeList->numPolytopes = numPolytopes;
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
