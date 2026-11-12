

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
