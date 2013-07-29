#include <assert.h>
#include <vector>
#include <string>

#include "bk3dEx.h"
#include "nv_math.h"
//-----------------------------------------------------------------------------
// Transformation
// Compute or update all the components of the transformation
// If necessary, it will call the parent's update prior to finalizing the work
// If possible, it will use Quaternions but still maintain the matrices
// Note that the Quaternion case may not be 100% compatible with Maya complex transformations (Pivots...)
//-----------------------------------------------------------------------------
bool ComputeMatrix(bk3d::Transform * tr, bool bComputeBindPose, bool forceDirty, int parentDirty)
{
    bool bParentChanged = false;
    mat4 m_matrix, m_abs_matrix, m_bindpose_matrix;
    bool bChanged = false;

    // copy the original matrix, if ever this matrix is already available and bDirty is false
    memcpy(m_matrix.mat_array, tr->matrix, sizeof(float)*16);
    if(forceDirty)
    {
        tr->validComps &= ~(TRANSFCOMP_matrix_ready);
        tr->bDirty = true;
    }
    if((tr->bDirty || forceDirty))
    {
        // case where the matrix is really the main source of transformation
        if(tr->validComps & TRANSFCOMP_matrix)
        {
            if(!(tr->validComps & TRANSFCOMP_matrix_ready))
            {
                bChanged = true;
                tr->validComps |= TRANSFCOMP_matrix_ready;
                tr->validComps &= ~(TRANSFCOMP_abs_matrix_ready|TRANSFCOMP_abs_rotationQuat_ready);
            }
        } else
        {
            quat Q(0.0f, 0.0f, 0.0f, 1.0f);
            // because some components are now used, turn the matrix to Identity and re-compute it from scratch
            m_matrix.identity();
            // Joint transformation order (OpenGL order) :
            // P2 = Mtranslate * MparentScaleInverse * MjointOrient * Mrotation * Mrotorientation * Mscale * PosVec
            // P2 = [T]        * [IS]                * [JO]         * [R]       * [RO]            * [S]    * P
            // from maya doc : matrix = [S] * [RO] * [R] * [JO] * [IS] * [T]
            // basic Transformation order :
            // P2 = Mtranslate * MrotPivotTransl * 
            //      MrotPivot * Mrotation * MrotOrient * MrotPivotInv * 
            //      MscalePivotTransl * MscalePivot * Mscale * MscalePivotInv * PosVec
            // In maya doc (DX matrix orientation) :
            //             -1                      -1
            // p' = p x [Sp]x[S]x[Sh]x[Sp]x[St]x[Rp]x[Ro]x[R]x[Rp]x[Rt]x[T]


            //translation (T)
            if(tr->validComps & TRANSFCOMP_pos)
            {
                bChanged = true;
                m_matrix.translate(tr->pos);
            }
            // parent scale inverse for JOINTs: TODO

            //rotation pivot translate (Rt)
            if(tr->validComps & TRANSFCOMP_rotationPivotTranslate)
            {
                bChanged = true;
                m_matrix.translate(tr->rotationPivotTranslate);
            }
            //JOINT ONLY : orientation
            if(tr->validComps & TRANSFCOMP_jointOrientation)
            {
                bChanged = true;
                Q = quat(tr->jointOrientation);
                m_matrix.rotate(Q);
            }
	        //Rotation
            // rotation-pivot (Rp)
            if(tr->validComps & TRANSFCOMP_rotationPivot)
            {
                bChanged = true;
                m_matrix.translate(vec3(tr->rotationPivot[0], tr->rotationPivot[1], tr->rotationPivot[2]));
            }
            {
                // if you want Quaternion at any cost, Euler must be canceled
	            //rotation from a Quaternion (==R)
                // NOTE: if we force the dirty flag, we'll give priority to Euler
                if((tr->validComps & TRANSFCOMP_rotationQuat)
                    ||((tr->validComps & TRANSFCOMP_rotationQuat_ready)&&(forceDirty == false)))
                {
                    bChanged = true;
                    quat Qr(tr->rotationQuat);
                    Q *= Qr;
	                m_matrix.rotate(Qr);
                    tr->validComps |= TRANSFCOMP_rotationQuat_ready;
                } 
	            // TODO: take the order into account (R)
                // Euler rotation was higher priority over Quaternion, but I changed it so that IK can work
                // when quaternions get updated without Euler...
                // Warning : when an anim curve is connected to Euler, we will have to cancel Quaternion every-time
                else if(tr->validComps & TRANSFCOMP_rotation)
                {
                    bChanged = true;
                    // No need : use internal method from the quat
                    //quat qz(vec3(0,0,1), tr->rotation[2] * nv_to_rad);
                    //quat qy(vec3(0,1,0), tr->rotation[1] * nv_to_rad);
                    //quat qx(vec3(1,0,0), tr->rotation[0] * nv_to_rad);
                    //qx *= qy;
                    //qx *= qz;
                    //m_matrix.rotate(qx);
                    vec3 e(tr->rotation);
                    e *= nv_to_rad;
                    quat qz(e);
                    m_matrix.rotate(qz);
                    // make a copy of this rotation in the quaternion and now the quaternion part is valid
                    memcpy(tr->rotationQuat, qz.comp, sizeof(nv_scalar)*4);
                    tr->validComps |= TRANSFCOMP_rotationQuat_ready;
                    Q *= qz;
                } else {
                    // if no rotation in this transformation, just update the quaternion
                    tr->rotationQuat[0] = tr->rotationQuat[1] = tr->rotationQuat[2] = 0.0f;
                    tr->rotationQuat[3] = 1.0f;
                    tr->validComps |= TRANSFCOMP_rotationQuat_ready;
                }
	            //rotation orientation (Ro)
                if(tr->validComps & TRANSFCOMP_rotationOrientation)
                {
                    bChanged = true;
                    quat Qro(tr->rotationOrientation);
                    Q *= Qro;
	                m_matrix.rotate(Qro);
                }
            }
            // inverse rotation-pivot Inv(Rp)
            if(tr->validComps & TRANSFCOMP_rotationPivot)
            {
                mat4 mrp, mrpI;
                mrp.identity();
                mrp.set_translation(vec3(tr->rotationPivot[0], tr->rotationPivot[1], tr->rotationPivot[2]));
                invert(mrpI, mrp);
                m_matrix *= mrpI;
            }
            // scale-pivot translation (St) introduced to preserve existing scale transformations when moving pivot. 
            // This is used to prevent the object from moving when the objects pivot point is not at the origin 
            // and a non-unit scale is applied to the object 
            if(tr->validComps & TRANSFCOMP_scalePivotTranslate)
            {
                bChanged = true;
                m_matrix.translate(vec3(tr->scalePivotTranslate[0], tr->scalePivotTranslate[1], tr->scalePivotTranslate[2]));
            }

            // scale-pivot (Sp)
            if(tr->validComps & TRANSFCOMP_scalePivot)
            {
                bChanged = true;
                m_matrix.translate(vec3(tr->scalePivot[0], tr->scalePivot[1], tr->scalePivot[2]));
            }
            {
                // According to maya doc, there should be a "Shearing" matrix, here... TODO?
	            //scale (S)
                if(tr->validComps & TRANSFCOMP_scale)
                {
                    bChanged = true;
	                m_matrix.scale(tr->scale);
                }
            }
            // inverse scale-pivot Inv(Sp)
            if(tr->validComps & TRANSFCOMP_scalePivot)
            {
                mat4 mscp, mscpI;
                mscp.identity();
                mscp.set_translation(vec3(tr->scalePivot[0], tr->scalePivot[1], tr->scalePivot[2]));
                invert(mscpI, mscp);
                m_matrix *= mscpI;
            }
        } // else (if it has components ON)
    } //if(tr->bDirty || forceDirty)
    // -----------------------------
    // Now work on matrices
    if(parentDirty < 0)
    {
        if(tr->pParent)
            bParentChanged = ComputeMatrix(tr->pParent, bComputeBindPose, forceDirty, parentDirty);
    } else {
        bParentChanged = parentDirty == 0 ? false : true;
    }
    // re-compute absolute values (and bindpose if necessary)
    if(bParentChanged || bChanged || (!(tr->validComps & TRANSFCOMP_abs_matrix_ready)))
    {
        bChanged = true;
        // if we have the Quaternion ON, let's update the abs values, too
        if(tr->validComps & (TRANSFCOMP_rotationQuat|TRANSFCOMP_rotationQuat_ready))
        {
            tr->validComps |= TRANSFCOMP_abs_rotationQuat_ready;
            //if(tr->validComps & TRANSFCOMP_rotation) // If Euler is still there... careful !
            //{
            //    // Shall we re-compute the local quaternion from Euler ?
            //    assert(!"Warning : local quaternion overridden by Euler");
            //}
            // NOTE: the Quaternion case certainly missed details from the complex matrix chain from Maya
            // TODO: We totally ignore pivots in this case :-( Need to check what we can do
            //if((!(tr->validComps & (TRANSFCOMP_scalePivot|TRANSFCOMP_scalePivotTranslate|TRANSFCOMP_rotationPivot|TRANSFCOMP_rotationPivotTranslate))))
            //{
            //  PRINTF(("Warning with tr->validComps"));
            //}
            vec3 VQpos(tr->pos);
            vec3 VQscale(tr->scale);
            // we assume the whole chain of transformations are consistent : parent has valid Quaternions
            quat Qabs;
            quat Q(0.0f, 0.0f, 0.0f, 1.0f);
            if(tr->validComps & TRANSFCOMP_jointOrientation)
            {
                Q = quat(tr->jointOrientation);
                if(tr->validComps & (TRANSFCOMP_rotationQuat|TRANSFCOMP_rotationQuat_ready))
                    Q *= quat(tr->rotationQuat);
            } else if(tr->validComps & (TRANSFCOMP_rotationQuat|TRANSFCOMP_rotationQuat_ready))
                Q = quat(tr->rotationQuat);
            if(tr->validComps & TRANSFCOMP_rotationOrientation)
                Q *= quat(tr->rotationOrientation);
            if(tr->pParent)
            {
                assert(tr->validComps & TRANSFCOMP_abs_rotationQuat_ready);
                quat Qparent(tr->pParent->abs_QuatTransf.Q);
                Qabs = Qparent * Q;
                //Qabs.Normalize(); // maybe we could this only fewer times
                // correct the scale and pos according to parents
                VQpos.rotateBy(Qparent);
                VQpos += vec3(tr->pParent->abs_QuatTransf.pos);
                VQscale *= vec3(tr->pParent->abs_QuatTransf.scale);
            } else {
                Qabs = Q;
            }
            memcpy(tr->abs_QuatTransf.Q, Qabs.comp, sizeof(nv_scalar)*4);
            memcpy(tr->abs_QuatTransf.pos, VQpos.vec_array, sizeof(nv_scalar)*3);
            memcpy(tr->abs_QuatTransf.scale, VQscale.vec_array, sizeof(nv_scalar)*3);
        }
#if 1
        // OROGINAL VERSION
        if(tr->pParent)
            m_abs_matrix = mat4(tr->pParent->abs_matrix) * m_matrix;
        else
            m_abs_matrix = m_matrix;
#else
// =====================================================================
// TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY
// Temporary code to debug the Quaternions
        mat4 M;
#if 0
        quat Q(tr->rotationQuat);
        vec3 P(tr->pos);
        vec3 S(tr->scale);
        struct DODO {
            static void calc(bk3d::Transform *PT, quat & Q, vec3 & P, vec3 & S)
            {
                quat Q2;
                vec3 P2, S2;
                if(PT->pParent)
                {
                    DODO::calc(PT->pParent, Q2, P2, S2);
                    Q = Q2 * quat(PT->rotationQuat);
                    memcpy(PT->abs_QuatTransf.Q, Q.comp, sizeof(float)*4);
                    P = vec3(PT->pos);
                    P.rotateBy(Q2);
                    P += P2;
                    memcpy(PT->abs_QuatTransf.pos, P.vec_array, sizeof(float)*3);
                    S = vec3(PT->scale);
                    S *= S2;
                    memcpy(PT->abs_QuatTransf.scale, S.vec_array, sizeof(float)*3);
                } else {
                    Q = quat(PT->rotationQuat);
                    P = vec3(PT->pos);
                    S = vec3(PT->scale);
                }
            }
        };
        DODO::calc(tr, Q, P, S);
#endif
        // create the abs matrix from the quaternions
        quat Qabs(tr->abs_QuatTransf.Q);
        vec3 Pabs(tr->abs_QuatTransf.pos);
        vec3 Sabs(tr->abs_QuatTransf.scale);
        M = mat4(array16_id);
        M.as_translation(Pabs);
        M.rotate(Qabs);
        M.scale(Sabs);

        m_abs_matrix = M;
#endif
// TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY TEMPORARY
// =====================================================================
    }
    // recompute the bindpose only if needed (i.e. the first time)
    if((tr->bindpose_matrix[15] == 0.0)||(bComputeBindPose))
    {
        tr->validComps |= TRANSFCOMP_bindpose_matrix;
        invert(m_bindpose_matrix, m_abs_matrix);
        memcpy(tr->bindpose_matrix, m_bindpose_matrix.mat_array, sizeof(float)*16);
    }
    if(bChanged)
    {
        tr->validComps |= TRANSFCOMP_matrix_ready;
        tr->validComps |= TRANSFCOMP_abs_matrix_ready;
        // Add the Quaternion data... if possible
        // Warning
        tr->validComps |= TRANSFCOMP_abs_rotationQuat_ready;
        memcpy(tr->abs_matrix, m_abs_matrix.mat_array, sizeof(float)*16);
        memcpy(tr->abs_matrix, m_abs_matrix.mat_array, sizeof(float)*16);
        memcpy(tr->matrix, m_matrix.mat_array, sizeof(float)*16);
    }
    return bChanged;
}
void recTransfUpdate(bk3d::Transform *tr, bool bComputeBindPose, bool forceDirty, int parentDirty=false)
{
    ComputeMatrix(tr, bComputeBindPose, forceDirty, parentDirty);
    //
    // Children
    //
    if(tr && tr->pChildren)
        for(int i=0; i<tr->pChildren->n; i++)
        {
            recTransfUpdate(tr->pChildren->p[i], bComputeBindPose, forceDirty, tr->bDirty||forceDirty ? 1 : parentDirty);
        }
    // now we can set the dirty flag to false...
    tr->bDirty = false;
}
void updateTransforms(bk3d::FileHeader * bk3dHeader, bool bComputeBindPose=false, bool forceDirty=false)
{
    if(bk3dHeader && bk3dHeader->pTransforms)
        for(int i=0; i<bk3dHeader->pTransforms->n; i++)
        {
            recTransfUpdate(bk3dHeader->pTransforms->p[i], bComputeBindPose, forceDirty);
        }
}

