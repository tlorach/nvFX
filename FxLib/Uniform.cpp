/*
    Copyright (c) 2013, NVIDIA CORPORATION. All rights reserved.
    Copyright (c) 2013, Tristan Lorach. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Neither the name of NVIDIA CORPORATION nor the names of its
       contributors may be used to endorse or promote products derived
       from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
    OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    Please direct any questions to tlorach@nvidia.com (Tristan Lorach)
*/
#include <fstream>
#include <assert.h>
#include "Fx.h"
#pragma warning(disable:4996)
using namespace nvFX;

/*************************************************************************/ /**
 ** NOTE: for now the conversion is straightforward. But later we might want
 ** a more subtle conversion : for example the tex is 2D and the resource is 3D :
 ** maybe the resource view is a slice of the volume (and so on for CubeMap...)
 ** returns true if it could convert (when ttype is != UNKNOWN)
 **/ /*************************************************************************/ 

bool typeRes2Tex(TextureType &ttype, ResourceType rtype)
{
    //TODO: more complex handling
    ttype = (TextureType)rtype;
    return true;
}
bool typeRes2Uniform(IUniform::Type &utype, ResourceType rtype)
{
    switch(rtype)
    {
    case RESOURCE_UNKNOWN: utype = IUniform::TUndefined; break;
    case RESTEX_1D: utype = IUniform::TTexture1D; break;
    case RESTEX_2D: utype = IUniform::TTexture2D; break;
    case RESTEX_2DRECT: utype = IUniform::TTexture2DRect; break;
    case RESTEX_3D: utype = IUniform::TTexture3D; break;
    case RESTEX_CUBE_MAP: utype = IUniform::TTextureCube; break;
    default:
    CHECK_TRUE_MSG(false,"missing convertion. TODO or to CHECK");
        return false;
    }
    return true;
}

/***************************************************************************/
/* Uniform Uniform Uniform Uniform Uniform Uniform Uniform Uniform         */
/***************************************************************************/
bool Uniform::ShadowedData::tooSmall(IUniform::Type type, size_t nitems, size_t* pSz)
{
    size_t sz;
    if(nitems < 1) nitems = 1;
    switch(type)
    {
    case TInt:
    case TInt2:
    case TInt3:
    case TInt4:
        sz = (sizeof(int)*nitems*(1+(int)type - (int)TInt));
        break;
    //case UInt:
    case TBool:
    case TBool2:
    case TBool3:
    case TBool4:
        sz = (sizeof(bool)*nitems*(1+(int)type - (int)TBool));
        break;
    case TFloat:
    case TVec2:
    case TVec3:
    case TVec4:
        sz = (sizeof(float)*nitems*(1+(int)type - (int)TFloat));
        break;
    case TMat2:
    case TMat3:
    case TMat4:
        sz = (sizeof(float)*nitems*(2+(int)type - (int)TMat2)*(2+(int)type - (int)TMat2));
        break;
    case TUBO:
        sz = (sizeof(int)*nitems);
        break;
    case TCB:
        sz = (sizeof(int)*nitems);
        break;
    case TUniform:
        sz = (sizeof(Uniform *)*nitems);
        break;
    case TTexture:
    case TTexture1D:
    case TTexture2D:
    case TTexture2DRect:
    case TTexture3D:
    case TTextureCube:
        sz = (sizeof(Uniform::ShadowedData::Tex)*nitems);
// Solved... but let's keep the code here if ever something I didn't test gets wrong
//#pragma message("!!!!!!!! MEMORY ISSUE: sz seems not sufficient. x2 to avoid the problem... COULD NOT FIND THE PROBLEM, YET")
//sz*=2;
        break;
    case TTexUnit:
        sz = (sizeof(int)*nitems);
        break;
#ifndef OGLES2
    case TSubroutineUniform:
        sz = (sizeof(char*)*nitems);
        break;
    case TSubroutineUniformByID:
        sz = (sizeof(int)*nitems);
        break;
#endif
    default:
        CHECK_TRUE_MSG(false, "unhandled Type ! TODO");
        sz = 0;
        break;
    }
    if(pSz) *pSz = sz;
    //if(this) { // to debug
    //    if(sz > size)
    //        return true;
    //    else if(sz != size)
    //        return false;
    //    else
    //        return false;
    //} else
    //    return true;
    return (this ? sz > size : true); 
}
Uniform::ShadowedData*  Uniform::ShadowedData::allocate(IUniform::Type type, size_t nitems)
{
    size_t sz;
    if(!tooSmall(type, nitems, &sz))
        return this;
    if(this)
        free();
    void* p = malloc(sz + sizeof(size) /* the size item from ShadowedData*/);
    memset(p,0,sz + sizeof(size));
    ((Uniform::ShadowedData*)p)->size = sz;
    return (Uniform::ShadowedData*)p;
}
Uniform::ShadowedData* Uniform::ShadowedData::copy(const ShadowedData* pSrc)
{
    if(this)
        free();
    void* p = malloc(pSrc->size + sizeof(size) /* the size item from ShadowedData*/);
    memcpy(p,pSrc, pSrc->size + sizeof(size));
    return (Uniform::ShadowedData*)p;
}
void                    Uniform::ShadowedData::free() { assert(this); ::free(this); }
//void	*Uniform::ShadowedData::operator new( size_t stAllocateBlock, IUniform::Type type/*, size_t nitems*/) ){ return NULL; }
//void	*Uniform::ShadowedData::operator new[] (size_t, IUniform::Type type) { return NULL; }
//void    Uniform::ShadowedData::operator delete (void*) {}
//void    Uniform::ShadowedData::operator delete[] (void*) {}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
Uniform::~Uniform()
{
    if(m_data) 
    {
        if(m_type == TSubroutineUniform)
            for(int i=0; i<16; i++)
                if(m_data->subroutineNames[i])
                    delete [] m_data->subroutineNames[i];
        m_data->free();
        m_data = NULL;
    }
}
Uniform::Uniform(const char* name, const char *groupname, const char *semantic) : 
    IUniformEx()
{
	m_activeTarget = -1;
    m_precision = PHighP;
    m_idx = 0;
    m_arraySz = 1;
    m_data = NULL;
    m_type = TUndefined;
    m_name.set(name, groupname);
    std::string aaa = m_name;
    if(semantic)
        m_semantic = std::string(semantic);
}

/*************************************************************************/ /**
 ** \brief Update the dirty flag of a target.
 **
 ** Special case : when the target is a CstBuffer, we must propagate the dirty
 ** flag when 'true' to this owner
 **
 ** \todo we should make a better Job at setting 'dirty' the CstBuffer for 
 ** TCSTBUFFER case. See comments in nvFX::CstBuffer::setDirty
 **/ /*************************************************************************/ 
void Uniform::setTargetDirtyFlag(STarget &t, bool bDirty)
{
    t.dirty = bDirty;
#pragma MESSAGE(__FILE__ "(66) : TODO TODO TODO TODO : we should do a better management of CstBuffer 'dirty' when >0 children got dirty...")
    if(bDirty && (t.ttype == TCSTBUFFER))
        t.pBuffer->setDirty(true);
}

/*************************************************************************/ /**
 ** \brief Update Uniform data in the targets in which it is used
 **
 ** \arg pass != NULL means that we target a specific pass for the update. Only the ones
 ** used in this pass will be updated. If not available for this pass, it will try to locate the uniform
 ** TODO: shall we also filter according to CUDA, Cg... ? I guess... see later
 ** 
 **/ /*************************************************************************/ 
IUniform*    Uniform::update(IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    if(m_data)
        update(m_data, p, p->getActiveProgramLayer(), false);
    return this;
}
/*************************************************************************/ /**
 ** \brief remove the target for pass
 **
 ** 
 **/ /*************************************************************************/ 
IUniform*    Uniform::deleteTarget(IPass *pass, int layerID)
{
    std::vector<STarget>::iterator it = m_targets.begin();
    std::vector<STarget>::iterator it2;
    while(it != m_targets.end())
    {
        it2 = it;
        ++it2;
        if((it->pass == pass)&&(it->passLayerId == layerID))
            m_targets.erase(it);
        it = it2;
    }
    return this;
}

/*************************************************************************/ /**
 ** \brief remove the target for pass
 **
 ** 
 **/ /*************************************************************************/ 
IUniform*    Uniform::invalidateTarget(IPass *pass, int layerID)
{
    std::vector<STarget>::iterator it = m_targets.begin();
    while(it != m_targets.end())
    {
        if((it->pass == pass)&&(it->passLayerId == layerID))
            it->valid = false;
        ++it;
    }
    return this;
}

/*************************************************************************/ /**
 **
 ** 
 **/ /*************************************************************************/ 
int Uniform::getNumTargets(TargetType t)
{
    int tsz = (int)m_targets.size();
    if(t == TANY)
        return tsz;
    int n = 0;
    for(int i=0; i<tsz; i++)
        if(m_targets[i].ttype == t)
            n++;
    return n;
}

/*************************************************************************/ /**
 **
 ** 
 **/ /*************************************************************************/ 

// debug assertion if not assigning the right type. Cast to do asap
#define SETTYPE(a,b)\
    CHECK_TRUE_MSG((a == b)||(a == Uniform::TUndefined), "TODO ASAP: perform a cast of values when the current uniform expected a different type");\
    a = b;
IUniform*    Uniform::setValue1f(float f)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TFloat, 1, NULL)))
        m_data = m_data->allocate(IUniform::TFloat, 1);
    else if((m_data->fvals[0] == f)&&(m_type == Uniform::TFloat))
        return this;
    SETTYPE(m_type, Uniform::TFloat);
    m_data->fvals[0] = f;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue2f(float f1, float f2)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TVec2, 1, NULL)))
        m_data = m_data->allocate(IUniform::TVec2, 1);
    else if((m_data->fvals[0] == f1)&&(m_data->fvals[1] == f2)&&(m_type == Uniform::TVec2))
        return this;
    SETTYPE(m_type, Uniform::TVec2);
    m_data->fvals[0] = f1; m_data->fvals[1] = f2;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue3f(float f1, float f2, float f3)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TVec3, 1, NULL)))
        m_data = m_data->allocate(IUniform::TVec3, 1);
    else if((m_data->fvals[0] == f1)&&(m_data->fvals[1] == f2)&&(m_data->fvals[2] == f3)&&(m_type == Uniform::TVec3))
        return this;
    SETTYPE(m_type, Uniform::TVec3);
    m_data->fvals[0] = f1; m_data->fvals[1] = f2; m_data->fvals[2] = f3;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue4f(float f1, float f2, float f3, float f4)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TVec4, 1, NULL)))
        m_data = m_data->allocate(IUniform::TVec4, 1);
    else if((m_data->fvals[0] == f1)&&(m_data->fvals[1] == f2)&&(m_data->fvals[2] == f3)&&(m_data->fvals[3] == f4)
        &&(m_type == Uniform::TVec4) )
        return this;
    SETTYPE(m_type, Uniform::TVec4);
    m_data->fvals[0] = f1; m_data->fvals[1] = f2; m_data->fvals[2] = f3; m_data->fvals[3] = f4; 
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue1fv(float *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TFloat, 1, NULL)))
        m_data = m_data->allocate(IUniform::TFloat, 1);
    else if((memcmp(m_data->fvals, pf, sizeof(float))==0)&&(m_type == Uniform::TFloat))
        return this;
    SETTYPE(m_type, Uniform::TFloat);
    memcpy(m_data->fvals, pf, sizeof(float));
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue2fv(float *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TVec2, 1, NULL)))
        m_data = m_data->allocate(IUniform::TVec2, 1);
    else if((memcmp(m_data->fvals, pf, sizeof(float)*2)==0)&&(m_type == Uniform::TVec2))
        return this;
    SETTYPE(m_type, Uniform::TVec2);
    memcpy(m_data->fvals, pf, sizeof(float)*2);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue3fv(float *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TVec3, 1, NULL)))
        m_data = m_data->allocate(IUniform::TVec3, 1);
    else if((memcmp(m_data->fvals, pf, sizeof(float)*3)==0)&&(m_type == Uniform::TVec3))
        return this;
    SETTYPE(m_type, Uniform::TVec3);
    memcpy(m_data->fvals, pf, sizeof(float)*3);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue4fv(float *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TVec4, 1, NULL)))
        m_data = m_data->allocate(IUniform::TVec4, 1);
    else if((memcmp(m_data->fvals, pf, sizeof(float)*4)==0)&&(m_type == Uniform::TVec4))
        return this;
    SETTYPE(m_type, Uniform::TVec4);
    memcpy(m_data->fvals, pf, sizeof(float)*4);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValuefv(float *pf, int dim, int nVecs, int offsetVec)
{
    if((dim > 4)||(dim < 1)||(nVecs<=0))
        return NULL;
    // set the type
    m_type = (Uniform::Type)((int)Uniform::TFloat + dim - 1);
    if((m_data == NULL)||(m_data->tooSmall(m_type, nVecs, NULL)))
        m_data = m_data->allocate(m_type, nVecs);
    else if(!memcmp(m_data->fvals + (dim*offsetVec), pf, sizeof(float)*nVecs * dim))
        return this;
    // set the amount of items
    m_arraySz = nVecs + offsetVec; // let's take the whole as numItems : we need this to send correctly data to GLSL via setuniform()
    memcpy(m_data->fvals + (dim*offsetVec), pf, sizeof(float)*dim*nVecs);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue1i(int f)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TInt, 1, NULL)))
        m_data = m_data->allocate(IUniform::TInt, 1);
    else if((m_data->ivals[0] == f)&&(m_type == Uniform::TInt))
        return this;
    SETTYPE(m_type, Uniform::TInt);
    m_data->ivals[0] = f;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue2i(int f1, int f2)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TInt2, 1, NULL)))
        m_data = m_data->allocate(IUniform::TInt2, 1);
    else if((m_data->ivals[0] == f1)&&(m_data->ivals[1] == f2)&&(m_type == Uniform::TInt2))
        return this;
    SETTYPE(m_type, Uniform::TInt2);
    m_data->ivals[0] = f1; m_data->ivals[1] = f2;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue3i(int f1, int f2, int f3)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TInt3, 1, NULL)))
        m_data = m_data->allocate(IUniform::TInt3, 1);
    else if((m_data->ivals[0] == f1)&&(m_data->ivals[1] == f2)&&(m_data->ivals[2] == f3)&&(m_type == Uniform::TInt3))
        return this;
    SETTYPE(m_type, Uniform::TInt3);
    m_data->ivals[0] = f1; m_data->ivals[1] = f2; m_data->ivals[2] = f3;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue4i(int f1, int f2, int f3, int f4)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TInt4, 1, NULL)))
        m_data = m_data->allocate(IUniform::TInt4, 1);
    else if((m_data->ivals[0] == f1)&&(m_data->ivals[1] == f2)&&(m_data->ivals[2] == f3)&&(m_data->ivals[3] == f4)&&(m_type == Uniform::TInt4))
        return this;
    SETTYPE(m_type, Uniform::TInt4);
    m_data->ivals[0] = f1; m_data->ivals[1] = f2; m_data->ivals[2] = f3; m_data->ivals[3] = f4;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue1iv(int *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TInt, 1, NULL)))
        m_data = m_data->allocate(IUniform::TInt, 1);
    else if((memcmp(m_data->fvals, pf, sizeof(float))==0)&&(m_type == Uniform::TInt))
        return this;
    SETTYPE(m_type, Uniform::TInt);
    memcpy(m_data->fvals, pf, sizeof(int)*1);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue2iv(int *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TInt2, 1, NULL)))
        m_data = m_data->allocate(IUniform::TInt2, 1);
    else if((memcmp(m_data->fvals, pf, sizeof(float)*2)==0)&&(m_type == Uniform::TInt2))
        return this;
    SETTYPE(m_type, Uniform::TInt2);
    memcpy(m_data->fvals, pf, sizeof(int)*2);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue3iv(int *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TInt3, 1, NULL)))
        m_data = m_data->allocate(IUniform::TInt3, 1);
    else if((memcmp(m_data->fvals, pf, sizeof(float)*3)==0)&&(m_type == Uniform::TInt3))
        return this;
    SETTYPE(m_type, Uniform::TInt3);
    memcpy(m_data->fvals, pf, sizeof(int)*3);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue4iv(int *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TInt4, 1, NULL)))
        m_data = m_data->allocate(IUniform::TInt4, 1);
    else if((memcmp(m_data->fvals, pf, sizeof(float)*4)==0)&&(m_type == Uniform::TInt4))
        return this;
    SETTYPE(m_type, Uniform::TInt4);
    memcpy(m_data->fvals, pf, sizeof(int)*4);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValueiv(int *pf, int dim, int nVecs, int offsetVec)
{
    if((dim > 4)||(dim < 1)||(nVecs<=0))
        return NULL;
    // type of what we store
    m_type = (Uniform::Type)((int)Uniform::TInt + dim - 1);
    if((m_data == NULL)||(m_data->tooSmall(m_type, nVecs, NULL)))
        m_data = m_data->allocate(m_type, nVecs);
    else if(!memcmp(m_data->fvals + (dim*offsetVec), pf, sizeof(int)*dim*nVecs))
        return this;
    // set the amount of items
    m_arraySz = nVecs + offsetVec;
    memcpy(m_data->fvals + (dim*offsetVec), pf, sizeof(int)*dim*nVecs);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}


IUniform*    Uniform::setValue1b(bool f)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TBool, 1, NULL)))
        m_data = m_data->allocate(IUniform::TBool, 1);
    else if((m_data->bvals[0] == f) && (m_type == Uniform::TBool))
        return this;
    SETTYPE(m_type, Uniform::TBool);
    m_data->bvals[0] = f;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue2b(bool f1, bool f2)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TBool2, 1, NULL)))
        m_data = m_data->allocate(IUniform::TBool2, 1);
    else if((m_data->bvals[0] == f1)&&(m_data->bvals[1] == f2) && (m_type == Uniform::TBool2))
        return this;
    SETTYPE(m_type, Uniform::TBool2);
    m_data->bvals[0] = f1; m_data->bvals[1] = f2;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue3b(bool f1, bool f2, bool f3)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TBool3, 1, NULL)))
        m_data = m_data->allocate(IUniform::TBool3, 1);
    else if((m_data->bvals[0] == f1)&&(m_data->bvals[1] == f2)&&(m_data->bvals[2] == f3) && (m_type == Uniform::TBool3))
        return this;
    SETTYPE(m_type, Uniform::TBool3);
    m_data->bvals[0] = f1; m_data->bvals[1] = f2; m_data->bvals[2] = f3;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue4b(bool f1, bool f2, bool f3, bool f4)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TBool4, 1, NULL)))
        m_data = m_data->allocate(IUniform::TBool4, 1);
    else if((m_data->bvals[0] == f1)&&(m_data->bvals[1] == f2)&&(m_data->bvals[2] == f3)&&(m_data->bvals[3] == f4) && (m_type == Uniform::TBool4))
        return this;
    SETTYPE(m_type, Uniform::TBool4);
    m_data->bvals[0] = f1; m_data->bvals[1] = f2; m_data->bvals[2] = f3; m_data->bvals[3] = f4;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue1bv(bool *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TBool, 1, NULL)))
        m_data = m_data->allocate(IUniform::TBool, 1);
    else if((!memcmp(m_data->bvals, pf, sizeof(bool))) && (m_type == Uniform::TBool))
        return this;
    SETTYPE(m_type, Uniform::TBool);
    memcpy(m_data->bvals, pf, sizeof(bool)*1);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue2bv(bool *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TBool2, 1, NULL)))
        m_data = m_data->allocate(IUniform::TBool2, 1);
    else if((!memcmp(m_data->bvals, pf, sizeof(bool))) && (m_type == Uniform::TBool2))
        return this;
    SETTYPE(m_type, Uniform::TBool2);
    memcpy(m_data->bvals, pf, sizeof(bool)*2);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue3bv(bool *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TBool3, 1, NULL)))
        m_data = m_data->allocate(IUniform::TBool3, 1);
    else if((!memcmp(m_data->bvals, pf, sizeof(bool))) && (m_type == Uniform::TBool3))
        return this;
    SETTYPE(m_type, Uniform::TBool3);
    memcpy(m_data->bvals, pf, sizeof(bool)*3);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValue4bv(bool *pf)
{
    if((m_data == NULL)||(m_data->tooSmall(IUniform::TBool4, 1, NULL)))
        m_data = m_data->allocate(IUniform::TBool4, 1);
    else if((!memcmp(m_data->bvals, pf, sizeof(bool))) && (m_type == Uniform::TBool4))
        return this;
    SETTYPE(m_type, Uniform::TBool4);
    memcpy(m_data->bvals, pf, sizeof(bool)*4);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setValuebv(bool *pf, int dim, int nVecs, int offsetVec)
{
    if((dim > 4)||(dim < 1)||(nVecs<=0))
        return NULL;
    // type of what we store
    m_type = (Uniform::Type)((int)Uniform::TBool + dim - 1);
    if((m_data == NULL)||(m_data->tooSmall(m_type, nVecs, NULL)))
        m_data = m_data->allocate(m_type, nVecs);
    else if(!memcmp(m_data->bvals + (dim*offsetVec), pf, sizeof(bool)*dim*nVecs))
        return this;
    // set the amount of items
    m_arraySz = nVecs + offsetVec;
    memcpy(m_data->bvals + (dim*offsetVec), pf, sizeof(bool)*dim*nVecs);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}

bool Uniform::getValuefv(float *pf, int dim, int nVecs)
{
    int d = dim;
    int n = (nVecs > m_arraySz) ? m_arraySz : nVecs;
    if((m_data==NULL)||(pf==NULL))
        return false;
    switch(m_type)
    {
    case Uniform::TFloat:
    case Uniform::TVec2:
    case Uniform::TVec3:
    case Uniform::TVec4:
    {
        if(dim > (1+(int)Uniform::TVec4 - (int)Uniform::TFloat))
            d = (1+(int)Uniform::TVec4 - (int)Uniform::TFloat);
        for(int j=0; j<n; j++)
          for(int i=0; i<d; i++)
            pf[i+j*dim] = m_data->fvals[i+j*(1+(int)Uniform::TVec4 - (int)Uniform::TFloat)];
        return true;
    }
    case Uniform::TInt:
    case Uniform::TInt2:
    case Uniform::TInt3:
    case Uniform::TInt4:
    {
        if(dim > (1+(int)Uniform::TInt4 - (int)Uniform::TInt))
            d = (1+(int)Uniform::TInt4 - (int)Uniform::TInt);
        for(int j=0; j<n; j++)
          for(int i=0; i<d; i++)
            pf[i+j*dim] = (float)m_data->ivals[i+j*(1+(int)Uniform::TInt4 - (int)Uniform::TInt)];
        return true;
    }
    case Uniform::TBool:
    case Uniform::TBool2:
    case Uniform::TBool3:
    case Uniform::TBool4:
    {
        if(dim > (1+(int)Uniform::TBool4 - (int)Uniform::TBool))
            d = (1+(int)Uniform::TBool4 - (int)Uniform::TBool);
        for(int j=0; j<n; j++)
          for(int i=0; i<d; i++)
              pf[i+j*dim] = m_data->bvals[i+j*(1+(int)Uniform::TBool4 - (int)Uniform::TBool)] ? 1.0f : 0.0f;
        return true;
    }
  }
    return false;
}

bool Uniform::getValueiv(int *pf, int dim, int nVecs)
{
    int d = dim;
    int n = (nVecs > m_arraySz) ? m_arraySz : nVecs;
    if((m_data==NULL)||(pf==NULL))
        return false;
    switch(m_type)
    {
    case Uniform::TFloat:
    case Uniform::TVec2:
    case Uniform::TVec3:
    case Uniform::TVec4:
    {
        if(dim > (1+(int)Uniform::TVec4 - (int)Uniform::TFloat))
            d = (1+(int)Uniform::TVec4 - (int)Uniform::TFloat);
        for(int j=0; j<n; j++)
          for(int i=0; i<d; i++)
            pf[i+j*dim] = (int)m_data->fvals[i+j*(1+(int)Uniform::TVec4 - (int)Uniform::TFloat)];
        return true;
    }
    case Uniform::TInt:
    case Uniform::TInt2:
    case Uniform::TInt3:
    case Uniform::TInt4:
    {
        if(dim > (1+(int)Uniform::TInt4 - (int)Uniform::TInt))
            d = (1+(int)Uniform::TInt4 - (int)Uniform::TInt);
        for(int j=0; j<n; j++)
          for(int i=0; i<d; i++)
            pf[i+j*dim] = (int)m_data->ivals[i+j*(1+(int)Uniform::TInt4 - (int)Uniform::TInt)];
        return true;
    }
    case Uniform::TBool:
    case Uniform::TBool2:
    case Uniform::TBool3:
    case Uniform::TBool4:
    {
        if(dim > (1+(int)Uniform::TBool4 - (int)Uniform::TBool))
            d = (1+(int)Uniform::TBool4 - (int)Uniform::TBool);
        for(int j=0; j<n; j++)
          for(int i=0; i<d; i++)
              pf[i+j*dim] = m_data->bvals[i+j*(1+(int)Uniform::TBool4 - (int)Uniform::TBool)] ? 1 : 0;
        return true;
    }
  }
    return false;
}
bool Uniform::getValuebv(bool *pf, int dim, int nVecs)
{
    int d = dim;
    int n = (nVecs > m_arraySz) ? m_arraySz : nVecs;
    if((m_data==NULL)||(pf==NULL))
        return false;
    switch(m_type)
    {
    case Uniform::TFloat:
    case Uniform::TVec2:
    case Uniform::TVec3:
    case Uniform::TVec4:
    {
        if(dim > (1+(int)Uniform::TVec4 - (int)Uniform::TFloat))
            d = (1+(int)Uniform::TVec4 - (int)Uniform::TFloat);
        for(int j=0; j<n; j++)
          for(int i=0; i<d; i++)
            pf[i+j*dim] = m_data->fvals[i+j*(1+(int)Uniform::TVec4 - (int)Uniform::TFloat)] == 0.0 ? false : true;
        return true;
    }
    case Uniform::TInt:
    case Uniform::TInt2:
    case Uniform::TInt3:
    case Uniform::TInt4:
    {
        if(dim > (1+(int)Uniform::TInt4 - (int)Uniform::TInt))
            d = (1+(int)Uniform::TInt4 - (int)Uniform::TInt);
        for(int j=0; j<n; j++)
          for(int i=0; i<d; i++)
            pf[i+j*dim] = m_data->ivals[i+j*(1+(int)Uniform::TInt4 - (int)Uniform::TInt)] == 0 ? false : true;
        return true;
    }
    case Uniform::TBool:
    case Uniform::TBool2:
    case Uniform::TBool3:
    case Uniform::TBool4:
    {
        if(dim > (1+(int)Uniform::TBool4 - (int)Uniform::TBool))
            d = (1+(int)Uniform::TBool4 - (int)Uniform::TBool);
        for(int j=0; j<n; j++)
          for(int i=0; i<d; i++)
              pf[i+j*dim] = m_data->bvals[i+j*(1+(int)Uniform::TBool4 - (int)Uniform::TBool)];
        return true;
    }
  }
    return false;
}



IUniform*    Uniform::setBuffer(int buffer)
{
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TUBO, 1, NULL)))
        m_data = m_data->allocate(Uniform::TUBO, 1);
    else if(buffer == m_data->ivals[0])
        return this;
    SETTYPE(m_type, Uniform::TUBO);
    m_data->ivals[0] = buffer;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}

IUniform*    Uniform::setBuffer(void* buffer)
{
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TCB, 1, NULL)))
        m_data = m_data->allocate(Uniform::TCB, 1);
    else if(m_data->pVoid == buffer)
        return this;
    SETTYPE(m_type, Uniform::TCB);
    m_data->pVoid = buffer;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}

IUniform*    Uniform::setMatrix4f(float *pm)
{
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TMat4, 1, NULL)))
        m_data = m_data->allocate(Uniform::TMat4, 1);
    else if(!memcmp(m_data->fvals, pm, sizeof(float)*16))
        return this;
    SETTYPE(m_type, Uniform::TMat4);
    memcpy(m_data->fvals, pm, sizeof(float)*16);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}

#ifndef OGLES2
IUniform*    Uniform::setSubroutine(const char *funcName, int idx)
{
    if(idx >= 16)
        return NULL;
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TSubroutineUniform, 16, NULL)))
    {
        m_data = m_data->allocate(Uniform::TSubroutineUniform, 16);
        SETTYPE(m_type, Uniform::TSubroutineUniform);
    } else { 
    CHECK_TRUE(m_type == Uniform::TSubroutineUniform);
    CHECK_TRUE_MSG(idx < 16, "Arbitrary max for sub-routines. We might want to use OpenGL max instead!");
        if(m_data->subroutineNames[idx] && (!strcmp(m_data->subroutineNames[idx], funcName)))
            return this;
    }
    if(m_data->subroutineNames[idx])
        delete [] m_data->subroutineNames[idx];
    m_data->subroutineNames[idx] = new char[strlen(funcName)+1];
    strcpy(m_data->subroutineNames[idx], funcName);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
IUniform*    Uniform::setSubroutines(const char **funcNames, int numFuncNames)
{
    if(m_data && (m_type == TSubroutineUniform))
        for(int i=0; i<16; i++)
            if(m_data->subroutineNames[i])
                delete [] m_data->subroutineNames[i];
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TSubroutineUniform, 16, NULL)))
        m_data = m_data->allocate(Uniform::TSubroutineUniform, 16);
    SETTYPE(m_type, Uniform::TSubroutineUniform);
    memset(m_data->subroutineNames, 0, sizeof(char*)*16);
    int i=0;
    while(i<numFuncNames /*&& (i < 16)*/)
    {
    CHECK_TRUE(i < 16);
        m_data->subroutineNames[i] = new char[strlen(funcNames[i])+1];
        strcpy(m_data->subroutineNames[i], funcNames[i]);
        ++i;
    }
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}
#endif

IUniform*    Uniform::setSamplerResource(ResourceType resType, int oglTexID, int texUnit, ISamplerState *pSState)
{
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TTexture, 1, NULL)))
    {
        m_data = m_data->allocate(Uniform::TTexture, 1);
        SETTYPE(m_type, Uniform::TUndefined);
    }
    bool bChanged = false;
    IUniform::Type t;
    typeRes2Uniform(t, resType);
    if(t != m_type)
    {
        m_type = t; bChanged = true;
    }
    TextureType tt;
    typeRes2Tex(tt, resType);
    if(m_data->tex.textureType != tt)
    {
        m_data->tex.textureType = tt; bChanged = true;
    }
    if((oglTexID >0)&&(oglTexID != m_data->tex.textureID))
    {
        m_data->tex.textureID = oglTexID; bChanged = true;
    }
    if((texUnit>=0) && (m_data->tex.textureUnit != texUnit))
    {
        m_data->tex.textureUnit = texUnit < 0 ? -1 : texUnit; bChanged = true;
    }
    if(pSState && (m_data->tex.pSState != pSState))
    {
        m_data->tex.pSState = pSState; bChanged = true;
    }
    m_data->tex.access = ACCESS_UNDEFINED; // ACCESS_UNDEFINED => we tell that we are dealing with a regular texture
    m_data->tex.bLayered = false;
    m_data->tex.layer = 0;
    if(!bChanged)
        return this;
    //update(NULL, true);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    return this;
}

IUniform*    Uniform::setTextureResource(IResource* pRes)
{
    if(pRes == NULL)
        return this;
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TTexture, 1, NULL)))
        m_data = m_data->allocate(Uniform::TTexture, 1);
    {
    bool bChanged = false;
    IUniform::Type t;
    typeRes2Uniform(t, pRes->getType());
    if(t != m_type)
    {
        m_type = t; bChanged = true;
    }
    if((pRes)&&(m_data->tex.pRes != static_cast<Resource*>(pRes)))
    { 
        m_data->tex.pRes = static_cast<Resource*>(pRes); bChanged = true;
    }
    if(!bChanged)
        return this;
    }
    if(m_data->tex.access == 0)
    {
        m_data->tex.access = ACCESS_UNDEFINED; // ACCESS_UNDEFINED => we tell that we are dealing with a regular texture
        m_data->tex.bLayered = false;
        m_data->tex.layer = 0;
    }
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    return this;
}
IUniform*    Uniform::setSamplerResource(IResource* pRes, int texUnit, ISamplerState *pSState)
{
    if(pRes == NULL)
        return this;
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TTexture, 1, NULL)))
        m_data = m_data->allocate(Uniform::TTexture, 1);
    bool bChanged = false;
    IUniform::Type t;
    typeRes2Uniform(t, pRes->getType());
    if(t != m_type)
    {
        m_type = t; bChanged = true;
    }
    if((pRes)&&(m_data->tex.pRes != static_cast<Resource*>(pRes)))
    { 
        m_data->tex.pRes = static_cast<Resource*>(pRes); bChanged = true;
    }
    if((texUnit>=0)&&(m_data->tex.textureUnit != texUnit))
    { 
        m_data->tex.textureUnit = texUnit < 0 ? -1 : texUnit; bChanged = true;
    }
    if((pSState)&&(m_data->tex.pSState != pSState))
    { 
        m_data->tex.pSState = pSState; bChanged = true; 
    }
    if(m_data->tex.access == 0)
    {
        m_data->tex.access = ACCESS_UNDEFINED; // ACCESS_UNDEFINED => we tell that we are dealing with a regular texture
        m_data->tex.bLayered = false;
        m_data->tex.layer = 0;
    }
    if(!bChanged)
        return this;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    return this;
}
//
// TODO: shall we create a Uniform for texture ID, too ?
// - storing the texture type
// - texture ID
// - texture unit
//
IUniform*    Uniform::setSamplerUnit(int i)
{
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TTexture, 1, NULL)))
        m_data = m_data->allocate(Uniform::TTexture, 1);
    else if(m_data->tex.textureUnit == i)
        return this;
    switch(m_type)
    {
    case IUniform::TTexture:
    case IUniform::TTexture1D:
    case IUniform::TTexture2D:
    case IUniform::TTexture2DRect:
    case IUniform::TTexture3D:
    case IUniform::TTextureCube:
        m_data->tex.textureUnit = i < 0 ? -1 : i;
        break;
    default:
        SETTYPE(m_type, Uniform::TTexture);
        typeRes2Tex(m_data->tex.textureType, nvFX::RESOURCE_UNKNOWN);
        m_data->tex.pSState = NULL;
        m_data->tex.textureID = -1;
        m_data->tex.textureUnit = i < 0 ? -1 : i;
        m_data->tex.access = ACCESS_UNDEFINED; // ACCESS_UNDEFINED => we tell that we are dealing with a regular texture
        m_data->tex.bLayered = false;
        m_data->tex.layer = 0;
        return this;
    }
    int tsz = (int)m_targets.size();
  for(int j=0; j<tsz;j++)
    setTargetDirtyFlag(m_targets[j], true);
    //update(NULL, true);
    return this;
}

IUniform*    Uniform::setSamplerState(ISamplerState *pSState)
{
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TTexture, 1, NULL)))
        m_data = m_data->allocate(Uniform::TTexture, 1);
    else if(m_data->tex.pSState == pSState)
        return this;
    switch(m_type)
    {
    case IUniform::TTexture:
    case IUniform::TTexture1D:
    case IUniform::TTexture2D:
    case IUniform::TTexture2DRect:
    case IUniform::TTexture3D:
    case IUniform::TTextureCube:
        m_data->tex.pSState = pSState;
        break;
    default:
        SETTYPE(m_type, Uniform::TTexture);
        typeRes2Tex(m_data->tex.textureType, nvFX::RESOURCE_UNKNOWN);
        m_data->tex.textureID = 0;
        m_data->tex.textureUnit = -1;
        m_data->tex.pSState = pSState;
        m_data->tex.access = ACCESS_UNDEFINED; // ACCESS_UNDEFINED => we tell that we are dealing with a regular texture
        m_data->tex.bLayered = false;
        m_data->tex.layer = 0;
        return this;
    }
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    //update(NULL, true);
    return this;
}

IUniform*    Uniform::setImageResource(ResourceType resType, int oglTexID, int texUnit)
{
    bool bChanged = false;
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TTexture, 1, NULL)))
    {
        m_data = m_data->allocate(Uniform::TTexture, 1);
        SETTYPE(m_type, Uniform::TUndefined);
    }
    TextureType tt;
    typeRes2Tex(tt, resType);
    if(m_data->tex.textureType != tt)
    {
        m_data->tex.textureType = tt; bChanged = true;
    }
    IUniform::Type t;
    typeRes2Uniform(t, resType);
    if(t != m_type)
    {
        m_type = t; bChanged = true;
    }
    if((oglTexID >= 0)&&(m_data->tex.textureID != oglTexID))
    {
        m_data->tex.textureID = oglTexID; bChanged = true;
    }
    if((texUnit >= 0)&&(m_data->tex.textureUnit != texUnit))
    {
        m_data->tex.textureUnit = texUnit < 0 ? -1 : texUnit; bChanged = true;
    }
    m_data->tex.pSState = NULL;
    if(m_data->tex.access == 0)
    {
        m_data->tex.access = ACCESS_READ_WRITE; // by default
        m_data->tex.bLayered = false;
        m_data->tex.layer = 0;
    }
    if(!bChanged)
        return this;
    //update(NULL, true);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    return this;
}

IUniform*    Uniform::setImageResource(IResource* pRes, int texUnit)
{
	if(!pRes)
		return this;
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TTexture, 1, NULL)))
        m_data = m_data->allocate(Uniform::TTexture, 1);
    bool bChanged = false;
    IUniform::Type t;
    typeRes2Uniform(t, pRes->getType());
    if(t != m_type)
    {
        m_type = t; bChanged = true;
    }
    if((pRes)&&(m_data->tex.pRes != static_cast<Resource*>(pRes)))
    { 
        m_data->tex.pRes = static_cast<Resource*>(pRes); bChanged = true; 
    }
    if((texUnit>=0)&&(m_data->tex.textureUnit != texUnit))
    { 
        m_data->tex.textureUnit = texUnit; bChanged = true; 
    }
    m_data->tex.pSState = NULL;
    if(m_data->tex.access == 0)
    {
        m_data->tex.access = ACCESS_READ_WRITE; // by default
        m_data->tex.bLayered = false;
        m_data->tex.layer = 0;
    }
    if(!bChanged)
        return this;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    return this;
}
//
// TODO: shall we create a Uniform for texture ID, too ?
// - storing the texture type
// - texture ID
// - texture unit
//
IUniform*    Uniform::setImageUnit(int i)
{
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TTexture, 1, NULL)))
        m_data = m_data->allocate(Uniform::TTexture, 1);
    else if(m_data->tex.textureUnit == i)
        return this;
    switch(m_type)
    {
    case IUniform::TTexture:
    case IUniform::TTexture1D:
    case IUniform::TTexture2D:
    case IUniform::TTexture2DRect:
    case IUniform::TTexture3D:
    case IUniform::TTextureCube:
        if(i >= 0) m_data->tex.textureUnit = i < 0 ? -1 : i;
        break;
    default:
        SETTYPE(m_type, Uniform::TTexture);
        m_data->tex.textureType = nvFX::TEXTURE_UNKNOWN;
        m_data->tex.textureID = -1;
        m_data->tex.textureUnit = i < 0 ? -1 : i;
        return this;
    }
    int tsz = (int)m_targets.size();
  for(int j=0; j<tsz;j++)
    setTargetDirtyFlag(m_targets[j], true);
    //update(NULL, true);
    return this;
}

IUniform*    Uniform::setUniform(IUniform* pU)
{
    if(!pU)
        return this;
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TUniform, 1, NULL)))
        m_data = m_data->allocate(Uniform::TUniform, 1);
    else if(m_data->pUniform == pU)
        return this;
    SETTYPE(m_type, Uniform::TUniform);
    m_data->pUniform = static_cast<Uniform*>(pU);
    //update(NULL, true);
    return this;
}

IUniform*    Uniform::setFromUniform(IUniform* pU_)
{
    //NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_GREEN, 3);
    Uniform *pU = static_cast<Uniform*>(pU_);
    int isTex = 0;
    if((m_type == Uniform::TTexture)
        ||(m_type == IUniform::TTexture1D)
        ||(m_type == IUniform::TTexture2D)
        ||(m_type == IUniform::TTexture2DRect)
        ||(m_type == IUniform::TTexture3D)
        ||(m_type == IUniform::TTextureCube) )
        isTex++;
    if((pU->m_type == Uniform::TTexture)
        ||(pU->m_type == IUniform::TTexture1D)
        ||(pU->m_type == IUniform::TTexture2D)
        ||(pU->m_type == IUniform::TTexture2DRect)
        ||(pU->m_type == IUniform::TTexture3D)
        ||(pU->m_type == IUniform::TTextureCube) )
        isTex++;
    if((isTex < 2)&&(m_type != Uniform::TUndefined) && (m_type != pU->m_type))
    {
        NXPROFILEFUNCCOL2("Oops", COLOR_RED, 3);
        LOGI("Uniform set from another one should be of the same type !\n");
    CHECK_TRUE(false);
        return this;
    }
    m_type = pU->m_type;
    if((m_data == NULL)||(m_data->tooSmall(Uniform::TTexture, 1, NULL)))
    {
        m_data = m_data->copy(pU->m_data);
    }
    else if(!memcmp(m_data, pU->m_data, sizeof(ShadowedData)))
        return this;
    // special case of textures : we don't want to overwrite fields that are 'undefined'
    // (Sorry... maybe bits to tell which fields are valid could be better)
    else if((pU->m_type == Uniform::TTexture)
        ||(pU->m_type == IUniform::TTexture1D)
        ||(pU->m_type == IUniform::TTexture2D)
        ||(pU->m_type == IUniform::TTexture2DRect)
        ||(pU->m_type == IUniform::TTexture3D)
        ||(pU->m_type == IUniform::TTextureCube) )
    {
        ShadowedData::Tex &tex = pU->m_data->tex;
        m_data->tex.access = tex.access;
        if(tex.textureUnit >= 0)
            m_data->tex.textureUnit = tex.textureUnit;
        if(tex.pSState)
            m_data->tex.pSState = tex.pSState;
        if(tex.textureType != nvFX::TEXTURE_UNKNOWN)
            m_data->tex.textureType = tex.textureType;
        if(tex.textureID)
            m_data->tex.textureID = tex.textureID;
        if(tex.pRes)
            m_data->tex.pRes = tex.pRes;
        m_data->tex.bLayered = tex.bLayered;
        m_data->tex.layer = tex.layer;
        if(tex.access)
            m_data->tex.access = tex.access;
    } else
        memcpy(m_data, pU->m_data, sizeof(ShadowedData));
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    return this;
}

IUniform*    Uniform::updateFromUniform(IUniform* pU_, IPass *pass)
{
    //NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_GREEN, 3);
    Pass*		p = static_cast<Pass*>(pass);
    Uniform *	pU = static_cast<Uniform*>(pU_);
    if((m_type != Uniform::TUndefined) && (m_type != pU->m_type))
    {
        NXPROFILEFUNCCOL2("Oops", COLOR_RED, 3);
        LOGI("Uniform set from another one should be of the same type !\n");
    CHECK_TRUE(false);
        return this;
    }
    m_type = pU->m_type;
	//LOGI("updateFromUniform : sampler %s in unit %d\n", pU->m_name.c_str(), pU->m_data->tex.textureUnit);
	update(pU->m_data, p, p->getActiveProgramLayer(), false);
    return this;
}
/**************************************************************************************************
 **
 ** Versions of methods for direct update without use shadowing
 **
 **************************************************************************************************/
IUniform*    Uniform::updateValue1f(float f, IPass *pass)
{
    SETTYPE(m_type, Uniform::TFloat);
    Pass* p = static_cast<Pass*>(pass);
    //if(m_data)
    //    setValue1f(f);
    update(WILDCAST_AS_SHADOWEDDATA(&f), p, p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue2f(float f1, float f2, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TVec2);
#pragma MESSAGE(__FILE__ "(835) : TODO TODO TODO TODO TODO: change delete with set in Uniform::update...()")
    //if(m_data) delete m_data; m_data = NULL;
    ShadowedData v;
    v.fvals[0] = f1; v.fvals[1] = f2;
    update(&v, p, p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue3f(float f1, float f2, float f3, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TVec3);
    //if(m_data) delete m_data; m_data = NULL;
    ShadowedData v;
    v.fvals[0] = f1; v.fvals[1] = f2; v.fvals[2] = f3;
    update(&v, p, p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue4f(float f1, float f2, float f3, float f4, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TVec4);
    //if(m_data) delete m_data; m_data = NULL;
    ShadowedData v;
    v.fvals[0] = f1; v.fvals[1] = f2; v.fvals[2] = f3; v.fvals[3] = f4;
    update(&v, p, p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue1fv(float *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TFloat);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p, p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue2fv(float *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TVec2);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p, p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue3fv(float *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TVec3);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue4fv(float *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TVec4);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValuefv(float *pf, int dim, IPass *pass)
{
    if((dim > 4)||(dim < 1))
        return NULL;
    Pass* p = static_cast<Pass*>(pass);
    //if(m_data) delete m_data; m_data = NULL;
    m_type = (Uniform::Type)((int)Uniform::TFloat + dim - 1);
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue1i(int f, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TInt);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(&f), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue2i(int f1, int f2, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TInt2);
    //if(m_data) delete m_data; m_data = NULL;
    ShadowedData v;
    int vv[1];
    v.ivals[0] = f1; v.ivals[1] = f2;
    update(&v, p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue3i(int f1, int f2, int f3, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TInt3);
    //if(m_data) delete m_data; m_data = NULL;
    ShadowedData v;
    int vv[2];
    v.ivals[0] = f1; v.ivals[1] = f2; v.ivals[2] = f3;
    update(&v, p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue4i(int f1, int f2, int f3, int f4, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TInt4);
    //if(m_data) delete m_data; m_data = NULL;
    ShadowedData v;
    int vv[3];
    v.ivals[0] = f1; v.ivals[1] = f2; v.ivals[2] = f3; v.ivals[3] = f4;
    update(&v, p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue1iv(int *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TInt);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue2iv(int *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TInt2);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue3iv(int *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TInt3);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue4iv(int *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TInt4);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValueiv(int *pf, int dim, IPass *pass)
{
    if((dim > 4)||(dim < 1))
        return NULL;
    Pass* p = static_cast<Pass*>(pass);
    //if(m_data) delete m_data; m_data = NULL;
    m_type = (Uniform::Type)((int)Uniform::TInt + dim - 1);
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}


IUniform*    Uniform::updateValue1b(bool f, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TBool);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(&f), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue2b(bool f1, bool f2, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TBool2);
    //if(m_data) delete m_data; m_data = NULL;
    ShadowedData v;
    bool vv[1];
    v.bvals[0] = f1; v.bvals[1] = f2;
    update(&v, p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue3b(bool f1, bool f2, bool f3, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TBool3);
    //if(m_data) delete m_data; m_data = NULL;
    ShadowedData v;
    bool vv[2];
    v.bvals[0] = f1; v.bvals[1] = f2; v.bvals[2] = f3;
    update(&v, p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue4b(bool f1, bool f2, bool f3, bool f4, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TBool4);
    //if(m_data) delete m_data; m_data = NULL;
    ShadowedData v;
    bool vv[3];
    v.bvals[0] = f1; v.bvals[1] = f2; v.bvals[2] = f3; v.bvals[3] = f4;
    update(&v, p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue1bv(bool *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TBool);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue2bv(bool *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TBool2);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue3bv(bool *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TBool3);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValue4bv(bool *pf, IPass *pass)
{
    Pass* p = static_cast<Pass*>(pass);
    SETTYPE(m_type, Uniform::TBool4);
    //if(m_data) delete m_data; m_data = NULL;
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateValuebv(bool *pf, int dim, IPass *pass)
{
    if((dim > 4)||(dim < 1))
        return NULL;
    Pass* p = static_cast<Pass*>(pass);
    //if(m_data) delete m_data; m_data = NULL;
    m_type = (Uniform::Type)((int)Uniform::TBool + dim - 1);
    update(WILDCAST_AS_SHADOWEDDATA(pf), p,p->getActiveProgramLayer(), false);
    return this;
}

IUniform*    Uniform::updateSamplerResource(ResourceType resType, int oglTexID, int texUnit, ISamplerState *pSState, IPass *pass)
{
    ShadowedData data;
    typeRes2Uniform(m_type, resType);
    typeRes2Tex(data.tex.textureType, resType);
    data.tex.textureID = oglTexID;
    if(texUnit >= 0)
        data.tex.textureUnit = texUnit;
    else if(m_data)
        data.tex.textureUnit = m_data->tex.textureUnit;
    else
        data.tex.textureUnit = -1;

    if(pSState)
        data.tex.pSState = pSState;
    else if(m_data)
        data.tex.pSState = m_data->tex.pSState;
    else 
        data.tex.pSState = NULL;
    data.tex.pRes = NULL; // in any case, we release the possible resource attachment : this call means we rely on external texture ID
    data.tex.access = ACCESS_UNDEFINED;
    data.tex.bLayered = false;
    data.tex.layer = 0;
    Pass* p = static_cast<Pass*>(pass);
    update(&data, p,p->getActiveProgramLayer(), false);
    return this;
}

IUniform*    Uniform::updateSamplerResource(IResource* pRes, int texUnit, ISamplerState *pSState, IPass *pass)
{
    if(!pRes)
        return this;
    ShadowedData data;

    typeRes2Uniform(m_type, pRes->getType());
    typeRes2Tex(data.tex.textureType, pRes->getType());

    data.tex.pRes = static_cast<Resource*>(pRes);

    data.tex.textureID = 0; // specifying a resource means there is no use
    if(texUnit >= 0)
        data.tex.textureUnit = texUnit;
    else if(m_data)
        data.tex.textureUnit = m_data->tex.textureUnit;
    else 
        data.tex.textureUnit = -1;

    if(pSState)
        data.tex.pSState = pSState;
    else if(m_data)
        data.tex.pSState = m_data->tex.pSState;
    else 
        data.tex.pSState = NULL;
    data.tex.access = ACCESS_UNDEFINED;
    data.tex.bLayered = false;
    data.tex.layer = 0;
    Pass* p = static_cast<Pass*>(pass);
    update(&data, p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateSamplerUnit(int i, IPass *pass)
{
    ShadowedData data;

    if((m_type != Uniform::TTexture)
        &&(m_type != IUniform::TTexture1D)
        &&(m_type != IUniform::TTexture2D)
        &&(m_type != IUniform::TTexture2DRect)
        &&(m_type != IUniform::TTexture3D)
        &&(m_type != IUniform::TTextureCube) )
    {
        SETTYPE(m_type, Uniform::TTexture);
        data.tex.textureType = nvFX::TEXTURE_UNKNOWN;
        data.tex.textureID = 0;
        data.tex.access = ACCESS_UNDEFINED;
        data.tex.bLayered = false;
        data.tex.layer = 0;
    }
    data.tex.textureUnit = i < 0 ? -1 : i;
    Pass* p = static_cast<Pass*>(pass);
    update(&data, p,p->getActiveProgramLayer(), false);
    return this;
}

IUniform*    Uniform::updateImageResource(ResourceType resType, int oglTexID, int texUnit, IPass *pass)
{
    ShadowedData data;

    typeRes2Uniform(m_type, resType);
    typeRes2Tex(data.tex.textureType, resType);

    data.tex.textureID = oglTexID;
    if(texUnit >= 0)
        data.tex.textureUnit = texUnit;
    else if(m_data)
        data.tex.textureUnit = m_data->tex.textureUnit;
    else 
        data.tex.textureUnit = -1;
    data.tex.access = ACCESS_READ_WRITE; // by default
    data.tex.bLayered = false;
    data.tex.layer = 0;
    data.tex.pSState = NULL;
    Pass* p = static_cast<Pass*>(pass);
    update(&data, p,p->getActiveProgramLayer(), false);
    return this;
}

IUniform*    Uniform::updateImageResource(IResource* pRes, int texUnit, IPass *pass)
{
    if(!pRes)
        return this;
    ShadowedData data;

    typeRes2Uniform(m_type, pRes->getType() );
    typeRes2Tex(data.tex.textureType, pRes->getType() );

    data.tex.pRes = static_cast<Resource*>(pRes);
    data.tex.pSState = NULL;
    if(texUnit >= 0)
        data.tex.textureUnit = texUnit;
    else if(m_data)
        data.tex.textureUnit = m_data->tex.textureUnit;
    else 
        data.tex.textureUnit = -1;
    data.tex.access = ACCESS_READ_WRITE; // by default
    data.tex.bLayered = false;
    data.tex.layer = 0;

    Pass* p = static_cast<Pass*>(pass);
    update(&data, p,p->getActiveProgramLayer(), false);
    return this;
}
IUniform*    Uniform::updateImageUnit(int i, IPass *pass)
{
    ShadowedData data;
    if((m_type != Uniform::TTexture)
        &&(m_type != IUniform::TTexture1D)
        &&(m_type != IUniform::TTexture2D)
        &&(m_type != IUniform::TTexture2DRect)
        &&(m_type != IUniform::TTexture3D)
        &&(m_type != IUniform::TTextureCube) )
    {
        SETTYPE(m_type, Uniform::TTexture);
        data.tex.textureType = nvFX::TEXTURE_UNKNOWN;
        data.tex.textureID = 0;
        data.tex.access = ACCESS_READ_WRITE; // by default
        data.tex.bLayered = false;
        data.tex.layer = 0;
    }
    data.tex.textureUnit = i < 0 ? -1 : i;
    Pass* p = static_cast<Pass*>(pass);
    update(&data, p,p->getActiveProgramLayer(), false);
    return this;
}

IUniform*    Uniform::updateSamplerState(ISamplerState *pSState, IPass *pass)
{
    ShadowedData data;

    if((m_type != Uniform::TTexture)
        &&(m_type != IUniform::TTexture1D)
        &&(m_type != IUniform::TTexture2D)
        &&(m_type != IUniform::TTexture2DRect)
        &&(m_type != IUniform::TTexture3D)
        &&(m_type != IUniform::TTextureCube) )
    {
        SETTYPE(m_type, Uniform::TTexture);
        data.tex.textureType = nvFX::TEXTURE_UNKNOWN;
        data.tex.textureID = 0;
        data.tex.textureUnit = -1;
        data.tex.access = ACCESS_UNDEFINED;
        data.tex.bLayered = false;
        data.tex.layer = 0;
    }
    data.tex.pSState = pSState;
    Pass* p = static_cast<Pass*>(pass);
    update(&data, p,p->getActiveProgramLayer(), false);
    return this;
}

IUniform*    Uniform::updateBuffer(int buffer, IPass *pass)
{
    //if(m_data) delete m_data; m_data = NULL;
    SETTYPE(m_type, Uniform::TUBO);
    Pass* p = static_cast<Pass*>(pass);
    update(WILDCAST_AS_SHADOWEDDATA(&buffer), p,p->getActiveProgramLayer(), false);
    return this;
}

IUniform*    Uniform::updateBuffer(void* buffer, IPass *pass)
{
    //if(m_data) delete m_data; m_data = NULL;
    SETTYPE(m_type, Uniform::TCB);
    Pass* p = static_cast<Pass*>(pass);
    update(WILDCAST_AS_SHADOWEDDATA(&buffer), p,p->getActiveProgramLayer(), false);
    return this;
}

IUniform*    Uniform::updateMatrix4f(float *pm, IPass *pass)
{
    //if(m_data) delete m_data; m_data = NULL;
    SETTYPE(m_type, Uniform::TMat4);
    Pass* p = static_cast<Pass*>(pass);
    update(WILDCAST_AS_SHADOWEDDATA(pm), p,p->getActiveProgramLayer(), false);
    return this;
}

IUniform*    Uniform::updateUniform(IUniform* pU, IPass *pass)
{
    //if(m_data) delete m_data; m_data = NULL;
    SETTYPE(m_type, Uniform::TUniform);
    Pass* p = static_cast<Pass*>(pass);
    update(WILDCAST_AS_SHADOWEDDATA(&pU), p,p->getActiveProgramLayer(), false);
    return this;
}

#ifndef OGLES2
IUniform*    Uniform::updateSubroutine(const char *funcName, int idx, IPass *pass)
{
    if(idx >= 16)
        return NULL;
    //if(m_data) delete m_data; m_data = NULL;
    ShadowedData data;
    if(m_type != Uniform::TSubroutineUniform)
    {
        memset(data.subroutineNames, 0, sizeof(char*)*16);
        SETTYPE(m_type, Uniform::TSubroutineUniform);
    }
    if(data.subroutineNames[idx])
        delete [] data.subroutineNames[idx];
    data.subroutineNames[idx] = new char[strlen(funcName)+1];
    strcpy(data.subroutineNames[idx], funcName);
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    Pass* p = static_cast<Pass*>(pass);
    update(&data, p,p->getActiveProgramLayer(), true);
    return this;
}

IUniform*    Uniform::updateSubroutines(const char **funcNames, IPass *pass)
{
    if(m_data && (m_type == TSubroutineUniform))
        for(int i=0; i<16; i++)
            if(m_data->subroutineNames[i])
                delete [] m_data->subroutineNames[i];
    //if(m_data) delete m_data; m_data = NULL;
    SETTYPE(m_type, Uniform::TSubroutineUniform);
    ShadowedData data;
    memset(data.subroutineNames, 0, sizeof(char*)*16);
    int i=0;
    while(*funcNames /*&& (i < 16)*/)
    {
    CHECK_TRUE(i < 16);
        data.subroutineNames[i] = new char[strlen(*funcNames)+1];
        strcpy(data.subroutineNames[i], *funcNames);
        ++funcNames;
        ++i;
    }
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    Pass* p = static_cast<Pass*>(pass);
    update(&data, p,p->getActiveProgramLayer(), true);
    return this;
}

IUniform* Uniform::updateSubroutines(int *funcIDs, int numIDs, IPass *pass)
{
    //if(m_data) delete m_data; m_data = NULL;
    SETTYPE(m_type, Uniform::TSubroutineUniformByID);
    ShadowedData data;
    memset(data.ivals, 0, sizeof(int)*16);
    int i=0;
    for(int i=0; i<16; i++)
        if(i<numIDs)
            data.ivals[i] = funcIDs[i];
        else data.ivals[i] = -1;
        int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        setTargetDirtyFlag(m_targets[i], true);
    Pass* p = static_cast<Pass*>(pass);
    update(&data, p,p->getActiveProgramLayer(), true);
    return this;
}
#endif

/**************************************************************************************************
 **
 ** set size of array in advance
 **
 **************************************************************************************************/
void Uniform::setArraySz(int sz)
{
    m_arraySz = sz;
    #pragma MESSAGE("TODO: check this case : maybe not enough to just set the size...")
}

/**************************************************************************************************
 **
 ** \brief returns the total Bytes used by this uniform.
 ** \note this is only good for the uniforms that can be part of a constant buffer
 **
 **************************************************************************************************/
int Uniform::getSizeOfType()
{
    switch(m_type)
    {
    case Uniform::TFloat:
    case Uniform::TVec2:
    case Uniform::TVec3:
    case Uniform::TVec4:
        return 4 * ((int)m_type - (int)Uniform::TFloat + 1);
    case Uniform::TInt:
    case Uniform::TInt2:
    case Uniform::TInt3:
    case Uniform::TInt4:
        return 4 * ((int)m_type - (int)Uniform::TInt + 1);

    case Uniform::TBool:
    case Uniform::TBool2:
    case Uniform::TBool3:
    case Uniform::TBool4:
        return 4 * ((int)m_type - (int)Uniform::TBool + 1);

    case TMat2:
    case TMat3:
    case TMat4:
        return 4 * ((int)m_type - (int)Uniform::TMat2 + 2)*((int)Uniform::TMat4 - (int)Uniform::TMat2 + 2);
        break;
    case TUBO: // Would need to return the size of the buffer ?
    case TCB:  // Would need to return the size of the buffer ?
    case TUndefined:
    case TUniform:
    case TTexture:
    case TTexture1D:
    case TTexture2D:
    case TTexture2DRect:
    case TTexture3D:
    case TTextureCube:
    case TTexUnit:
#ifndef OGLES2
    case TSubroutineUniform:
    case TSubroutineUniformByID:
#endif
    default:
        break;
    }
    return 0;
}

/**************************************************************************************************
 **
 ** \brief copies raw values of the shadowed data of this uniform
 **
 **************************************************************************************************/
bool Uniform::getValueRaw(void *pDst, int szMax, int bytesPadding)
{
    if(!m_data)
        return false;
    int sz = getSizeOfType();
    if(sz == 0)
        return false;
    if(bytesPadding == 0)
    {
        sz *= m_arraySz;
        if((szMax > 0)&&(sz > szMax))
        {
            sz = szMax;
            memcpy(pDst, &(m_data->pVoid), sz);
            return false;
        }
        memcpy(pDst, &(m_data->pVoid), sz);
    } else {
        char* p = (char *)pDst;
        char* s = (char *)&(m_data->pVoid);
        int offssrc = 0;
        int offsdst = 0;
        for(int i=0; i<m_arraySz; i++)
        {
            if((szMax > 0)&&(sz > szMax))
            {
                sz = szMax;
                memcpy(p, s, sz);
            } else {
                memcpy(p+offsdst, s+offssrc, sz);
            }
            offsdst += sz + bytesPadding;
            offssrc += sz;
        }
    }
    return true;
}

/**************************************************************************************************
 **
 ** 
 **
 **************************************************************************************************/
bool Uniform::getDirty(Pass *pass)
{
    bool bDirty = false;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz; i++)
    {
        STarget &t = m_targets[i];
        if((pass && (t.pass == pass)) || (pass == NULL))
            if(t.dirty)
                return true;
    }
    return false;
}
void Uniform::setDirty(bool bYes, Pass *pass)
{
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz; i++)
    {
        STarget &t = m_targets[i];
        if((pass && (t.pass == pass)) || (pass == NULL))
            t.dirty = bYes;
    }
}

/**************************************************************************************************
 **
 **************************************************************************************************/
void Uniform::copyFrom(Uniform* pU)
{
  //CHECK_TRUE_RET(pU);
  if (!pU)
    return;
    m_annotations = pU->m_annotations;
    m_name = pU->m_name;
    m_groupname = pU->m_groupname;
    m_semantic = pU->m_semantic;
    m_targets.clear();
    m_type = pU->m_type;
    m_precision = pU->m_precision;
    m_idx = pU->m_idx;
    m_arraySz = pU->m_arraySz;
    if(pU->m_data)
    {
        m_data = m_data->copy(pU->m_data);
    }
}

/**************************************************************************************************
 **
 ** 
 **
 **************************************************************************************************/
bool UniformRepository::remove(Uniform *p)
{
    ItemMap::iterator iR = m_Items.begin();
    for(; iR != m_Items.end(); iR++)
    {
        if(iR->second.p == p)
        {
            int c = --iR->second.refCnt;
            if(c == 0) {
                LOGD("Resource not used anymore, removing from singleton repository\n");
                m_Items.erase(iR);
            }
            return true;
        }
    }
    return false;
}
UniformRepository::UniformRepository()
{
}
UniformRepository::~UniformRepository()
{
    ItemMap::iterator iR = m_Items.begin();
    for(; iR != m_Items.end(); iR++)
    {
        int cnt = 0;
        cnt = --iR->second.refCnt;
    }
    m_Items.clear();
}
IUniform*   UniformRepository::createUniform(const char *name, const char *groupname, const char *semantic)
{
    Uniform* p = NULL;
    ItemMap::iterator iR = m_Items.find(name);
    if(iR != m_Items.end())
    {
        LOGD("UniformRepository::createUniform : WARNING Uniform %s already declared\n", name);
        iR->second.refCnt++;
        return iR->second.p;
    }
    p = new_Uniform(name, groupname, semantic);
    m_Items[name] = Item(p,1);
    return p;
}
int         UniformRepository::addUniform(IUniform* p)
{
  if (NULL == p)
    return NULL;

    ItemMap::iterator iR = m_Items.find(p->getName());
    if(iR != m_Items.end())
    {
        if(iR->second.p != p)
        {
            LOGD("Uniform %s already declared. failing...\n", p->getName());
            return -1;
        } else {
            return ++iR->second.refCnt;
        }
    }
    m_Items[p->getName()] = Item(static_cast<Uniform*>(p),1);
    return 1;
}
int         UniformRepository::releaseUniform(IUniform* p)
{
  if (NULL == p)
    return 0;
    Uniform* pp = static_cast<Uniform*>(p);
    ItemMap::iterator iR = m_Items.begin();
    while(iR != m_Items.end())
    {
        if(iR->second.p == pp)
        {
            iR->second.refCnt--;
            if(iR->second.refCnt == 0)
            {
                LOGD("Delete Uniform %s and remove it from repository\n", pp->getName());
                delete_Uniform(pp);
                m_Items.erase(iR);
                return 0;
            } else {
                LOGD("Released Uniform %s to refCnt=%d\n", pp->getName(), iR->second.refCnt);
                return iR->second.refCnt;
            }
        }
        ++iR;
    }
    return -1;
}

IUniform*  UniformRepository::find(const char * Name)
{
    ItemMap::iterator iR = m_Items.find(Name);
    if(iR != m_Items.end())
        return iR->second.p;
    return NULL;
}
IUniform*  UniformRepository::find(IUniform * p)
{
    ItemMap::iterator iR = m_Items.begin();
    for(; iR != m_Items.end(); iR++)
    {
        if(iR->second.p == p)
        {
            return iR->second.p;
        }
    }
    return NULL;
}
IUniform*  UniformRepository::findFromSemantic(const char * sem)
{
    ItemMap::iterator iR = m_Items.begin();
    for(; iR != m_Items.end(); iR++)
    {
        if(!strcmp(iR->second.p->getSemantic(), sem))
        {
            return iR->second.p;
        }
    }
    return NULL;
}
IUniform*  UniformRepository::get(int i)
{
    if(i >= (int)m_Items.size())
        return NULL;
    ItemMap::iterator iR = m_Items.begin();
    while(i > 0)
    {
        ++iR;
        --i;
    }
    return iR->second.p;
}

static IUniform* uArray[100];

extern bool findMatchingAnnotation(IAnnotation* pAnnot, const char *annotationName, const char* value);

int UniformRepository::gatherFromAnnotation(IUniform** &pp, const char *annotationName, const char* str, int *ival, float *fval)
{
    memset(uArray, 0, 100*sizeof(IShader*));
    pp = (IUniform**)uArray;
    int n = 0;
    if(!annotationName)
        return 0;
    ItemMap::iterator i2 = m_Items.begin();
    while(i2 != m_Items.end())
    {
        if(i2->second.p)
        {
            IAnnotation* pAnnot = i2->second.p->annotations();
            if((str && findMatchingAnnotation(pAnnot, annotationName, str))
             ||(ival && (pAnnot->getAnnotationInt(annotationName)==ival[0]))
             ||(fval && (pAnnot->getAnnotationFloat(annotationName)==fval[0])))
            {
                if(n >= 100)
                {
                    nvFX::printf(">>Error : gathering of uniforms exceeded 100 !");
                    return n;
                }
                uArray[n++] = i2->second.p;
            }
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return static_cast<UniformRepository*>(getUniformRepositorySingleton())->gatherFromAnnotation(pp, annotationName, str, ival, fval);
    }
    return n;
}
int UniformRepository::gatherFromAnnotation(IUniform** &pp, const char *annotationName, const char* value)
{
    return gatherFromAnnotation(pp, annotationName, value, NULL, NULL);
}
int UniformRepository::gatherFromAnnotation(IUniform** &pp, const char *annotationName, int value)
{
    return gatherFromAnnotation(pp, annotationName, NULL, &value, NULL);
}
int UniformRepository::gatherFromAnnotation(IUniform** &pp, const char *annotationName, float value)
{
    return gatherFromAnnotation(pp, annotationName, NULL, NULL, &value);
}
int UniformRepository::gatherFromNamespace(IUniform** &pp, const char *ns)
{
    memset(uArray, 0, 100*sizeof(IShader*));
    pp = (IUniform**)uArray;
    int n = 0;
    if(!ns)
        return 0;
    std::string strNS(ns);
    ItemMap::iterator i2 = m_Items.begin();
    while(i2 != m_Items.end())
    {
        Uniform* p = i2->second.p;
        if(p && (p->m_name.ns() == strNS))
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of uniforms exceeded 100 !");
                return n;
            }
            uArray[n++] = p;
        }
        ++i2;
    }
    return n;
}

