/*-----------------------------------------------------------------------
    Copyright (c) 2013, Tristan Lorach. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Neither the name of its contributors may be used to endorse 
       or promote products derived from this software without specific
       prior written permission.

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

    feedback to lorachnroll@gmail.com (Tristan Lorach)
*/ //--------------------------------------------------------------------
#include "bk3dEx.h"
#ifndef NOCURVES
#   include "CurveReader.h"
#endif

#pragma warning(disable: 4312)

//using namespace nv;
using namespace bk3d;

#define POSITION 0
#define BLENDWEIGHT 1
#define NORMAL      2
#define COLOR0      3
#define DIFFUSE     3
#define COLOR1      4
#define SPECULAR    4
#define FOGCOORD    5
#define PSIZE       6
#define TEXCOORD0   8
#define TEXCOORD1   9
#define TEXCOORD2   10
#define TEXCOORD3   11
#define TEXCOORD4   12
#define TEXCOORD5   13
#define TEXCOORD6   14
#define TEXCOORD7   15

#ifndef PRINTF
#define PRINTF printf
#endif
#ifndef EPRINTF
#define EPRINTF printf
#endif
#ifndef NXPROFILEFUNC
#define NXPROFILEFUNC(f)
#endif

class COGLSceneHelper;
//
// additional data that we will attach to Primitive groups in a Mesh
//
struct AttrMapping
{
    int Sz;
    struct Pair { Pair(int a, int b) {glattr=a; bk3dattr=b;} int glattr; int bk3dattr; };
    Pair *p;
};
struct OGLPrimGroupData
{
    AttrMapping attrMapping;
    GLuint      m_primGroupsBO;  // Element buffer (triangle indices)
    GLuint64EXT m_primGroupsBOAddrs;  // VBO addresses for VBUM
    bool        fakeImmediateMode;
};
//
// wrapper of a mesh to work with the OpenGL part
//
class OGLMeshWrapper
{
public:
    Mesh *    m_pMesh;

    OGLMeshWrapper(Mesh *pMesh, COGLSceneHelper* pOwner=NULL) : m_pMesh(pMesh) {}
    int     getNumJointInfluence() {    return m_pMesh->numJointInfluence; }
    int     getNumTransforms() {        return m_pMesh->pTransforms ? m_pMesh->pTransforms->n : 0; }
    float*  getTransformArray(float *matArrayToFill);
    void    initBuffers(COGLSceneHelper *pOwner, bool bUseVBUM = false, bool fakeImmediateMode = false, bool reserved=false);
    void    destroyBuffers(COGLSceneHelper *pOGLSceneHelper);
    bool    bindBuffers(COGLSceneHelper *pOGLSceneHelper, int primGroup=0, bool fakeImmediateMode = false);
    void    unbindBuffers(int primGroup=0);// { for(int i=0; i < 15; i++) glDisableVertexAttribArray(i); }
    bool    computeWorldspaceVertices(bool bYes=true);
    void    draw(COGLSceneHelper *pOGLSceneHelper, int primGroupStart=0, int primGroupEnd=-1, bool bFakeImmediateMode = false);
    void    drawAsPatch(int primGroupStart=0, int primGroupEnd=-1);
    // Select the attributes of the mesh that need to be sent to the shader
    //void    mapInputLayout(LPCSTR vtxShaderName, int primGroup=-1);
    //void    mapInputLayout(CGeffect effect, LPCSTR techniqueName, LPCSTR passName, int primGroup=-1);
    void    resetAttributeMapping() { }
    bool    setAttribute(int a2vAttrib, LPCSTR meshAttribName, int primGroup=-1);
    //
    // helpers to fetch user parameters in the mesh
    //
    inline  OGLPrimGroupData*   getOGLPrimGroupData(int pg) { 
        if(pg >= m_pMesh->pPrimGroups->n) return NULL;
        return (OGLPrimGroupData *)m_pMesh->pPrimGroups->p[pg]->userPtr; 
    }
    inline  OGLPrimGroupData*   createOGLPrimGroupData(int pg, OGLPrimGroupData* p=NULL) {
        if(pg < m_pMesh->pPrimGroups->n) {
            m_pMesh->pPrimGroups->p[pg]->userPtr = p ? p : malloc(sizeof(OGLPrimGroupData));
            if(m_pMesh->pPrimGroups->p[pg]->userPtr) ZeroMemory(m_pMesh->pPrimGroups->p[pg]->userPtr, sizeof(OGLPrimGroupData));
            return (OGLPrimGroupData *)m_pMesh->pPrimGroups->p[pg]->userPtr; 
        }
        return NULL;
    }
    //
    // Attribute mapping
    //
    inline  AttrMapping*        getAttrMapping()
    { 
        return (AttrMapping*) m_pMesh->userPtr;
    }
    __declspec(property(get = getAttrMapping, put = setSlotVBO)) AttrMapping* propAttrMapping;
    inline  AttrMapping*        createAttrMapping(AttrMapping* p=NULL)
    { 
        m_pMesh->userPtr = (p ? p : malloc(sizeof(AttrMapping)));
        if(m_pMesh->userPtr) ZeroMemory(m_pMesh->userPtr, sizeof(AttrMapping));
        return (AttrMapping*) m_pMesh->userPtr;
    }
    //
    // VBOs for slots
    //
    inline  GLuint              getSlotVBO(int slot) { return slot < m_pMesh->pSlots->n ? m_pMesh->pSlots->p[slot]->userData : 0; }
    inline  void                setSlotVBO(int slot, GLuint v) { if(slot < m_pMesh->pSlots->n) m_pMesh->pSlots->p[slot]->userData = v; }
    __declspec(property(get = getSlotVBO, put = setSlotVBO)) GLuint propVBO[];
    //
    // VBUM for slots
    //
    inline  GLuint64EXT         getSlotVBUMAdd(int slot) { return slot < m_pMesh->pSlots->n ? (GLuint64EXT)m_pMesh->pSlots->p[slot]->userPtr.p : NULL; }
    inline  void                setSlotVBUMAdd(int slot, GLuint64EXT v) { if(slot < m_pMesh->pSlots->n) m_pMesh->pSlots->p[slot]->userPtr.ll = v; }
    __declspec(property(get = getSlotVBUMAdd, put = setSlotVBUMAdd)) GLuint64EXT propSlotVBUMAdd[];
};
//
// helper wrapping FileHeader and CurvePool
// contains also the animations
//
class COGLSceneHelper
{
public:
//TODO: handle multiple headers if it happens
    FileHeader*         m_pHeader;
    void *              m_pBufferMemory; // the second section of the binary data where we have vtx buffers etc.
#    ifndef NOCURVES
    CurvePool                   m_cvPool;
#    endif

    FileHeader*            load(LPCSTR name);
    FileHeader*         assign(FileHeader* data, void* bufferMemory=NULL, bool bakeNow=true);
    void                unload();
    void                unloadBufferSection();
    //float*              findComponentf(const char *compname, bool **pDirty);
    void                updateTransforms();
#    ifndef NOCURVES
    void                bindAnimationFile(LPCSTR animfile);
    void                bindAnimationFromBk3d();
    void                updateAnim(float time) {    m_cvPool.updateAll(time); }
#    endif
    OGLMeshWrapper getMesh(int meshNum = 0) 
    { 
        if(m_pHeader && (meshNum < m_pHeader->pMeshes->n))
            return OGLMeshWrapper(m_pHeader->pMeshes->p[meshNum].p);
        return OGLMeshWrapper(NULL);
    }
    int getNumMeshes() 
    { 
        if(!(m_pHeader && m_pHeader->pMeshes)) return 0;
        return m_pHeader->pMeshes->n; 
    }
	int     getNumTransforms() { return m_pHeader->pTransforms ? m_pHeader->pTransforms->nBones : 0; }
    float*  getTransformArray(float *matArrayToFill);
    void begin() {}
protected:
    void                    recTransfUpdate(Transform *ptr);
};
////////////////////////////////////////////////////////////////////
float* COGLSceneHelper::getTransformArray(float *matArrayToFill)
{
    NXPROFILEFUNC(__FUNCTION__);
    if(!m_pHeader->pTransforms)
        return NULL;
    for(int i=0; i<m_pHeader->pTransforms->nBones; i++)
    {
        //assert(m_pMesh->pTransforms->p[i].p);
        mat4 &bpM = (mat4)m_pHeader->pTransforms->pBones[i]->MatrixInvBindpose();
        mat4 &M = (mat4)m_pHeader->pTransforms->pBones[i]->MatrixAbs();
        //mat4 bpMM = mat4(array16_id);
        mat4 bpMM = M * bpM;
        memcpy(&(matArrayToFill[16*i]), bpMM.mat_array , sizeof(float)*16);
    }
    return matArrayToFill;
}
void COGLSceneHelper::unload()
{
    NXPROFILEFUNC(__FUNCTION__);
    if(!m_pHeader)
        return;
    for(int i=0; i<m_pHeader->pMeshes->n; i++)
    {
        OGLMeshWrapper oglmesh(m_pHeader->pMeshes->p[i]);
        oglmesh.destroyBuffers(this);
    }
    //
    // Free the baked data
    //
    free(m_pHeader);
    if(m_pBufferMemory)
        free(m_pBufferMemory);
    m_pBufferMemory = NULL;
    m_pHeader = NULL;
}
void COGLSceneHelper::unloadBufferSection()
{
    m_pHeader->cleanBufferPointers(m_pBufferMemory);
    if(m_pBufferMemory)
        free(m_pBufferMemory);
    m_pBufferMemory = NULL;
}
////////////////////////////////////////////////////////////////////
FileHeader*   COGLSceneHelper::load(LPCSTR name)
{
    NXPROFILEFUNC(__FUNCTION__);
    m_pHeader = bk3d::load(name, &m_pBufferMemory);
    if(!m_pHeader)
        return NULL;
    return m_pHeader;
}

FileHeader*   COGLSceneHelper::assign(FileHeader* data, void* bufferMemory, bool bakeNow)
{
    NXPROFILEFUNC(__FUNCTION__);
    m_pHeader = data;
    m_pBufferMemory = bufferMemory;
    return m_pHeader;
}

#ifndef NOCURVES
void COGLSceneHelper::bindAnimationFile(LPCSTR animfile)
{
    NXPROFILEFUNC(__FUNCTION__);
    m_cvPool.newCVFromFile(animfile);
    for(int i=0; i<m_cvPool.getNumCV(); i++)
    {
        CurveVector *pcv = m_cvPool.getCVByIndex(i);
        bool *pdirty;
		float *pf = bk3d::FileHeader_findComponentf(m_pHeader, pcv->getName(), &pdirty);
        if(pf)
            pcv->bindPtrs(  pf, pf+1, pf+2, pf+3, pdirty);
        else
        {
            const char *n = pcv->getName();
            printf("Component not found : %s\n", n);
        }
    }
}
void COGLSceneHelper::bindAnimationFromBk3d()
{
    NXPROFILEFUNC(__FUNCTION__);
    if(!m_pHeader->pMayaCurves)
        return;
    for(int i=0; i<m_pHeader->pMayaCurves->n; i++)
    {
        bk3d::MayaCurveVector *pMCV = m_pHeader->pMayaCurves->p[i];
        // First, see if the curve is of any interest
        bool *pdirty;
        float *pf = bk3d::FileHeader_findComponentf(m_pHeader,pMCV->name, &pdirty);
        if(!pf)
            continue;
        if(pMCV->nCurves == 0)
            continue;
        CurveVector *pcv = m_cvPool.newCV(pMCV->name, pMCV->nCurves);
        for(int j=0; j<pMCV->nCurves; j++)
        {
            CurveReader *pCurve = pcv->getCurve(j);
            bk3d::MayaCurve *pMC = pMCV->pCurve[j];
            pCurve->startKeySetup(
                pMC->inputIsTime,
                pMC->outputIsAngular,
                pMC->isWeighted,
                (::EtInfinityType)pMC->preInfinity,
                (::EtInfinityType)pMC->postInfinity);
            for(int k=0; k<pMC->nKeys; k++)
            {
                pCurve->addKey(
                    pMC->key[k].time,
                    pMC->key[k].value,
                    (::EtTangentType)pMC->key[k].inTangentType,
                    (::EtTangentType)pMC->key[k].outTangentType,
                    pMC->key[k].inAngle,
                    pMC->key[k].inWeight,
                    pMC->key[k].outAngle,
                    pMC->key[k].outWeight);
            }
            pCurve->endKeySetup();
        }
        pcv->bindPtrs(  pf, pf+1, pf+2, pf+3, pdirty);
    }
}
#endif
// Joint transformation order (OpenGL order) :
// P2 = Mtranslate * MjointOrient * Mrotation * Mrotorientation * Mscale * P
// basic Transformation order :
// P2 = MrotPivotTransl * MrotPivot * Mrotation * MrotOrient * MrotPivotInv * MscalePivotTransl 
//      * MscalePivot * Mscale * MscalePivotInv * P
// TODO: add basic transform (P2)
void COGLSceneHelper::recTransfUpdate(Transform *ptr)
{
    NXPROFILEFUNC(__FUNCTION__);
#if 1
    //assert(!"TODO");
#else
        if(ptr->bDirty)
        {
            //translation
            glTranslatef(   ptr->pos[0],
                            ptr->pos[1],
                            ptr->pos[2]);
            //joint orientation
            nv::matrix4f jorient;
            nv::quaternionf qjorient(ptr->jointOrientation);
            qjorient.get_value(jorient);
            glMultMatrixf(jorient._array);
            //Rotation
            // TODO: take the order into account
            glRotatef(ptr->rotation[2] , 0, 0, 1); //Z
            glRotatef(ptr->rotation[1] , 0, 1, 0); //Y
            glRotatef(ptr->rotation[0] , 1, 0, 0); //X
            //joint orientation
            nv::matrix4f rorient;
            nv::quaternionf qrorient(ptr->rotationOrientation);
            qrorient.get_value(rorient);
            glMultMatrixf(rorient._array);
            //scale
            glScalef(   ptr->scale[0],
                        ptr->scale[1],
                        ptr->scale[2]);

            glPushMatrix();
                glMultMatrixf(ptr->bindpose_matrix);
                glGetFloatv(GL_MODELVIEW_MATRIX, ptr->MatrixAbs() );
            glPopMatrix();
        } else {
            glMultMatrixf(ptr->matrix);
            glPushMatrix();
                glMultMatrixf(ptr->bindpose_matrix);
                glGetFloatv(GL_MODELVIEW_MATRIX, ptr->MatrixAbs() );
            glPopMatrix();
        }
        //
        // Children
        //
        if(ptr->pChildren)
            for(int c=0; c<ptr->pChildren->n; c++)
            {
                glPushMatrix();
                recTransfUpdate(ptr->pChildren->p[c]);
                glPopMatrix();
            }
#endif
}
void COGLSceneHelper::updateTransforms()
{
    NXPROFILEFUNC(__FUNCTION__);
    if(!m_pHeader->pTransforms)
        return;
    glPushMatrix();
    glLoadIdentity();
    for(int i=0; i<m_pHeader->pTransforms->nBones; i++)
    {
        Transform *pT = m_pHeader->pTransforms->pBones[i]->asTransf();
        if(pT->pParent == NULL)
            recTransfUpdate(pT);
    }
    glPopMatrix();
}
//
// Compute the vertices in Worldspace.
// NOTE and TODO: this can be done via vertex-buffer + stream-out... faster
//
bool OGLMeshWrapper::computeWorldspaceVertices(bool bYes)
{
    if(m_pMesh->pTransforms->n == 0)
        return false;
    if(m_pMesh->pPrimGroups->p[0]->pTransforms && m_pMesh->pPrimGroups->p[0]->pTransforms->n > 0)
    {
        PRINTF(("Warning>> This mesh %s has local prim-group transforms. Skipping the World-space computation\n", m_pMesh->name));
        return false;
    }
    // Mark the transformation as invalid since the vertices are already transformed
    m_pMesh->pTransforms->p[0]->ValidComps() |= TRANSFCOMP_invalidMatrix;
    for(int a=0; a< m_pMesh->pAttributes->n; a++)
    {
        Attribute   *pA = m_pMesh->pAttributes->p[a];
        Slot        *pS = m_pMesh->pSlots->p[pA->slot];
        GLuint      bo = getSlotVBO(pA->slot);
        if(!bo)
            continue;
        if(strcmp(pA->name, "position"))
            //if(strcmp(pA->name, "normal")) //TODO: tan/binorm
                continue;
        GLuint64EXT gpuAddr = getSlotVBUMAdd(pA->slot);
        if(gpuAddr)
        {
            glMakeNamedBufferNonResidentNV(bo);
            glDisableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
        } 
        //glBindBuffer(GL_ARRAY_BUFFER, bo);
        //float *ptDst = (float *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        float *ptDst = (float *)pA->pAttributeBufferData;
        float *ptSrc = (float *)pA->pAttributeBufferData;
        for(int i = 0; i<(int)pS->vertexCount;i++)
        {
            const float *mw = m_pMesh->pTransforms->p[0]->MatrixAbs();
            float v[4];
            v[0] = mw[0]*ptSrc[0] + mw[4]*ptSrc[1] + mw[8]*ptSrc[2] + mw[12];
            v[1] = mw[1]*ptSrc[0] + mw[5]*ptSrc[1] + mw[9]*ptSrc[2] + mw[13];
            v[2] = mw[2]*ptSrc[0] + mw[6]*ptSrc[1] + mw[10]*ptSrc[2] + mw[14];
            memcpy(ptDst, v, sizeof(float)*3);
            ptSrc = (float*)(((char*)ptSrc)+pA->strideBytes);
            ptDst = (float*)(((char*)ptDst)+pA->strideBytes);
        }
        //glUnmapBuffer(GL_ARRAY_BUFFER);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    //
    // re-setup the VBUM if needed
    //
    for(int s=0; s< m_pMesh->pSlots->n; s++)
    {
        Slot        *pS = m_pMesh->pSlots->p[s];
        GLuint      bo = getSlotVBO(s);
        if(bo == 0)
            continue;
        glBindBuffer(GL_ARRAY_BUFFER, bo);
        glBufferData(   GL_ARRAY_BUFFER, 
                        pS->vtxBufferSizeBytes, 
                        pS->pVtxBufferData, 
                        GL_STATIC_DRAW);
        GLuint64EXT gpuAddr = getSlotVBUMAdd(s);
        if(gpuAddr)
        {
            // get the address of this buffer and make it resident.
            GLuint64EXT boAddr;
            glGetBufferParameterui64vNV(GL_ARRAY_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &boAddr); 
            setSlotVBUMAdd(s,boAddr);
            glMakeBufferResidentNV(GL_ARRAY_BUFFER, GL_READ_ONLY);
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return true;
}
bool OGLMeshWrapper::bindBuffers(COGLSceneHelper *pOGLSceneHelper, int primGroup, bool fakeImmediateMode)
{
    NXPROFILEFUNC(__FUNCTION__);
    // take the custom data that we attached to the original mesh
    AttrMapping* attrMapping = getAttrMapping();
    if(primGroup >= 0)
    {
        OGLPrimGroupData *pOGLData = getOGLPrimGroupData(primGroup);
        attrMapping = pOGLData && (pOGLData->attrMapping.Sz > 0) ? &pOGLData->attrMapping : attrMapping;
    }
    // Do simple assignment from attrib0 to attribN
    if((!attrMapping)||(attrMapping->Sz == 0))
    {
        int i=0;
        for(int s=0; s< m_pMesh->pSlots->n; s++)
        {
            Slot *pS = m_pMesh->pSlots->p[s];
            GLuint      bo = getSlotVBO(s);
            GLuint64EXT gpuAddr = getSlotVBUMAdd(s);
            // No VBO or we want to emulate non-VBO case
            if((bo == 0)||((m_pMesh->name[0]=='A')&&(m_pMesh->name[1]=='R')))
            {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                for(int j=0; j < pS->pAttributes->n; j++)
                {
                    Attribute *pA = pS->pAttributes->p[j];
                    glEnableVertexAttribArray(i);
                    glVertexAttribPointer(i, pA->numComp, pA->formatGL, GL_FALSE, pA->strideBytes, (const GLvoid *)pA->pAttributeBufferData);
                    i++;
                }
            } 
            else if(gpuAddr == 0) // Use VBOs: one VBO for one slot
            {
                glBindBuffer(GL_ARRAY_BUFFER, bo);
                for(int j=0; j < pS->pAttributes->n; j++)
                {
                    Attribute *pA = pS->pAttributes->p[j];
                    glEnableVertexAttribArray(i);
                    glVertexAttribPointer(i++, pA->numComp, pA->formatGL, GL_FALSE, pA->strideBytes, (const GLvoid *)pA->dataOffsetBytes);
                }
            }
            else // Use VBUM: one VBO for one slot
            {
                glEnableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
                for(int j=0; j < pS->pAttributes->n; j++, i++)
                {
                    Attribute *pA = pS->pAttributes->p[j];
                    glEnableVertexAttribArray(i);
                    glVertexAttribFormatNV(i, pA->numComp, pA->formatGL, /*normalized*/GL_FALSE, pA->strideBytes);
                    glBufferAddressRangeNV(GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, i, gpuAddr + pA->dataOffsetBytes, pS->vtxBufferSizeBytes - pA->dataOffsetBytes);
                    i++;
                }
            }
        }

    } else { // Or do map the attributes as requested in m_primGroupAttributeMapper
        GLuint boundBO = 0;
        for(int i=0; i < attrMapping->Sz; i++)
        {
            AttrMapping::Pair &attribMapper = attrMapping->p[i];
            Attribute   *attrib = m_pMesh->pAttributes->p[attribMapper.bk3dattr];
            Slot        *pS     = m_pMesh->pSlots->p[attrib->slot]; // we need the slot in order to know about the whole size of the attribute array
            GLuint      bo      = getSlotVBO(attrib->slot);
            GLuint64EXT gpuAddr = getSlotVBUMAdd(attrib->slot);
            glEnableVertexAttribArray(attribMapper.glattr);
            // cancel BO if we are in immediate mode fake
            if(fakeImmediateMode && ((m_pMesh->name[0]=='A')&&(m_pMesh->name[1]=='R')) )
                bo = 0;
            if(gpuAddr) // if we have VBUM, let's use them
            {
                glEnableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
                glVertexAttribFormatNV(attribMapper.glattr, attrib->numComp, attrib->formatGL, /*normalized*/GL_FALSE, attrib->strideBytes);
                glBufferAddressRangeNV(GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, attribMapper.glattr, gpuAddr + attrib->dataOffsetBytes, pS->vtxBufferSizeBytes - attrib->dataOffsetBytes);
                //glVertexAttribPointer(attribMapper.glattr, attrib->numComp, attrib->formatGL, GL_FALSE, attrib->strideBytes, (const GLvoid *)attrib->dataOffsetBytes);
            } 
            else if(bo) // if we have VBOs, let's use them
            {
                if(bo != boundBO)
                {
                    boundBO = bo;
                    glBindBuffer(GL_ARRAY_BUFFER, bo);
                }
                // here we only tell the stride since we are in a VBO
                glVertexAttribPointer(attribMapper.glattr, attrib->numComp, attrib->formatGL, GL_FALSE, attrib->strideBytes, (const GLvoid *)attrib->dataOffsetBytes);
            } 
            else // No VBO at all : simple OpenGL stuff
            {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glVertexAttribPointer(attribMapper.glattr, attrib->numComp, attrib->formatGL, GL_FALSE, attrib->strideBytes, attrib->pAttributeBufferData);
            }
        }
    }
    return true;
}
void OGLMeshWrapper::unbindBuffers(int primGroup)
{
    NXPROFILEFUNC(__FUNCTION__);
    // take the custom data that we attached to the original mesh
    OGLPrimGroupData *pOGLData = getOGLPrimGroupData(primGroup);
    // let disable VBUM, provided that further uses would enable them again
    // We assume at least one Slot is available
    if(getSlotVBUMAdd(0))
        glDisableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
    if(pOGLData && pOGLData->m_primGroupsBOAddrs)
        glDisableClientState(GL_ELEMENT_ARRAY_UNIFIED_NV);
    // Do simple assignment from attrib0 to attribN
    if((!pOGLData)||(pOGLData->attrMapping.Sz == 0))
    {
        GLuint boundBO = 0;
        for(int i=0; i<m_pMesh->pSlots->n; i++)
        {
            bk3d::Slot *pS = m_pMesh->pSlots->p[i];
            for(int j=0; j<pS->pAttributes->n; j++)
            {
                Attribute *pA = pS->pAttributes->p[j];
                // TODO : Check if this is useful (glEnableVertexAttribArray(i); glBindBuffer(GL_ARRAY_BUFFER, 0);)")
                // I think these 2 calls are useless
                //glEnableVertexAttribArray(i);
                //glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDisableVertexAttribArray(i);
                i++;
            }
        }
    } else {
        GLuint boundBO = 0;
        for(int i=0; i < pOGLData->attrMapping.Sz; i++)
        {
            AttrMapping::Pair &attribMapper = pOGLData->attrMapping.p[i];
            // TODO : Check if this is useful (glEnableVertexAttribArray(i); glBindBuffer(GL_ARRAY_BUFFER, 0);)")
            // I think these 2 calls are useless
            //glEnableVertexAttribArray(attribMapper.glattr);
            //glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDisableVertexAttribArray(attribMapper.glattr);
        }
    }
}
void OGLMeshWrapper::draw(COGLSceneHelper *pOGLSceneHelper, int primGroupStart, int primGroupEnd, bool bFakeImmediateMode)
{
    NXPROFILEFUNC(__FUNCTION__);
    if((primGroupEnd < 0)||(primGroupEnd >= m_pMesh->pPrimGroups->n))
        primGroupEnd = m_pMesh->pPrimGroups->n-1;
    for(int i=primGroupStart; i <= primGroupEnd; i++)
    {
        bk3d::PrimGroup  *pPG = m_pMesh->pPrimGroups->p[i];
        OGLPrimGroupData *pOGLData = getOGLPrimGroupData(i);
        GLenum topologyGL = pPG->topologyGL;
        // For now we don't manage Patches. At least let's draw Quads
        if(pPG->topologyGL == GL_PATCHES)
            topologyGL = GL_QUADS;
        if(pPG->primRestartIndex > 0)
        {
            glPrimitiveRestartIndex(pPG->primRestartIndex);
            glEnableClientState(GL_PRIMITIVE_RESTART_NV);//GL_PRIMITIVE_RESTART);
        } else {
            glDisableClientState(GL_PRIMITIVE_RESTART_NV);//GL_PRIMITIVE_RESTART);
        }
        if(pPG->indexCount == 0)
            continue;
        if(pOGLData && pOGLData->m_primGroupsBOAddrs) // if we have VBUM, let's use them
        {
            glEnableClientState(GL_ELEMENT_ARRAY_UNIFIED_NV);
            glBufferAddressRangeNV(GL_ELEMENT_ARRAY_ADDRESS_NV, 0, 
                pOGLData->m_primGroupsBOAddrs, 
                pPG->indexArrayByteSize);
            glDrawElements(
                topologyGL,
                pPG->indexCount, 
                pPG->indexFormatGL,
                (BYTE*)pPG->indexArrayByteOffset);
        } 
        else if(pOGLData && pOGLData->m_primGroupsBO) // if we have EBOs, let's use them
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pOGLData->m_primGroupsBO);
            glDrawElements(
                topologyGL,
                pPG->indexCount, 
                pPG->indexFormatGL,
                (BYTE*)pPG->indexArrayByteOffset);
            //CHECKGLERRORS();
        } 
        else if (pPG->pIndexBufferData) // If we still have some index but no VBO
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDrawElements(
                topologyGL,
                pPG->indexCount, 
                pPG->indexFormatGL,
                ((char*)pPG->pIndexBufferData)+pPG->indexArrayByteOffset);
            //CHECKGLERRORS();
        }
        // Lame Immediate mode emulation...
        else if(pOGLData->fakeImmediateMode && bFakeImmediateMode)
        {
            AttrMapping* attrMapping = getAttrMapping();
            attrMapping = pOGLData && (pOGLData->attrMapping.Sz > 0) ? &pOGLData->attrMapping : attrMapping;
            glBegin(topologyGL);
            for(int e=0; e<(int)pPG->indexCount; e++)
            {
                for(int i=attrMapping->Sz-1; i>=0; i--)
                {
                    AttrMapping::Pair &attribMapper = attrMapping->p[i];
                    Attribute   *attrib = m_pMesh->pAttributes->p[attribMapper.bk3dattr];
                    glVertexAttrib3fv(attribMapper.glattr, (float*)(((char*)attrib->pAttributeBufferData)+ (attrib->strideBytes*e)));
                }
            }
            glEnd();
            //CHECKGLERRORS();
        } else { // DrawArrays
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDrawArrays(
                topologyGL,
                pPG->indexOffset,
                pPG->indexCount);
            //CHECKGLERRORS();
        }
    } //for(int i=primGroupStart; i <= primGroupEnd; i++)
}

void OGLMeshWrapper::drawAsPatch(int primGroupStart, int primGroupEnd)
{
    NXPROFILEFUNC(__FUNCTION__);
    if((primGroupEnd < 0)||(primGroupEnd >= m_pMesh->pPrimGroups->n))
        primGroupEnd = m_pMesh->pPrimGroups->n-1;
    for(int i=primGroupStart; i <= primGroupEnd; i++)
    {
        OGLPrimGroupData *pOGLData = getOGLPrimGroupData(i);
        GLenum topology = GL_PATCHES;
        switch(m_pMesh->pPrimGroups->p[i]->topologyGL)
        {
        case GL_TRIANGLES:
            glPatchParameteri(GL_PATCH_VERTICES, 3);
            break;
        case GL_QUADS:
            glPatchParameteri(GL_PATCH_VERTICES, 4);
            break;
        default:
            return;
        }
        // TODO : write the VBUM case"
        if(pOGLData->m_primGroupsBO) // if we have EBOs, let's use them
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pOGLData->m_primGroupsBO);
            glDrawElements(
                topology,
                m_pMesh->pPrimGroups->p[i]->indexCount, 
                m_pMesh->pPrimGroups->p[i]->indexFormatGL,
                NULL);
        } else {
            glDrawElements(
                topology,
                m_pMesh->pPrimGroups->p[i]->indexCount, 
                m_pMesh->pPrimGroups->p[i]->indexFormatGL,
                m_pMesh->pPrimGroups->p[i]->pIndexBufferData);
        }
    }
}

float* OGLMeshWrapper::getTransformArray(float *matArrayToFill)
{
    NXPROFILEFUNC(__FUNCTION__);
    if(!m_pMesh->pTransforms)
        return NULL;
    for(int i=0; i<m_pMesh->pTransforms->n; i++)
    {
        //assert(m_pMesh->pTransforms->p[i].p);
        memcpy(&(matArrayToFill[16*i]), m_pMesh->pTransforms->p[i]->MatrixAbs() , sizeof(float)*16);
    }
    return matArrayToFill;
}
//
// simple attribute mapping. Only to make match the attribute # and the items exported in the mesh
//POSITION 0
//BLENDWEIGHT 1
//NORMAL      2
//COLOR0      3
//DIFFUSE     3
//COLOR1      4
//SPECULAR    4
//FOGCOORD    5
//PSIZE       6
//TEXCOORD0   8
//TEXCOORD1   9
//TEXCOORD2   10
//TEXCOORD3   11
//TEXCOORD4   12
//TEXCOORD5   13
//TEXCOORD6   14
//TEXCOORD7   15
//
bool    OGLMeshWrapper::setAttribute(int a2vAttrib, LPCSTR meshAttribName, int primGroup)
{
    NXPROFILEFUNC(__FUNCTION__);
    AttrMapping* attrMapping;
    if(primGroup<0)
    {
        attrMapping = getAttrMapping();
        if(!attrMapping)
            attrMapping = createAttrMapping();
    } else {
        OGLPrimGroupData* oglPrimGroupData = getOGLPrimGroupData(primGroup);
        if(!oglPrimGroupData)
            return false;
        attrMapping = &oglPrimGroupData->attrMapping;
    }
    for(int j=0; j<m_pMesh->pAttributes->n; j++)
    {
        if(!strcmp(m_pMesh->pAttributes->p[j]->name, meshAttribName))
        {
            //PRINTF(("OGLMeshWrapper: %s set to attrib[%d]\n", meshAttribName, a2vAttrib));
            attrMapping->Sz++;
            attrMapping->p = (AttrMapping::Pair*)realloc(attrMapping->p, sizeof(AttrMapping::Pair)*attrMapping->Sz);
            attrMapping->p[attrMapping->Sz-1] = AttrMapping::Pair(a2vAttrib,j);
            return true;
        }
    }
    //assert(!"OGLMeshWrapper : couldn't find the attribute");
#if 0
    PRINTF(("Warning>> OGLMeshWrapper : couldn't find attribute %s\n", meshAttribName));
    PRINTF(("attributes available in the model are :\n"));
    for(int j=0; j<m_pMesh->pAttributes->n; j++)
    {
        PRINTF(("Event>> %s\n", m_pMesh->pAttributes->p[j]->name));
    }
#endif
    return false;
}
//
// VBO creation
//
void OGLMeshWrapper::initBuffers(COGLSceneHelper *pOwner, bool bUseVBUM, bool fakeImmediateMode, bool reserved)
{
    NXPROFILEFUNC(__FUNCTION__);
    for(int i=0; i<(int)m_pMesh->pSlots->n; i++)
    {
        if((fakeImmediateMode)&&(m_pMesh->name[0] == 'I')&&(m_pMesh->name[1] == 'M'))
            continue;
        if((fakeImmediateMode)&&(m_pMesh->name[0] == 'A')&&(m_pMesh->name[1] == 'R'))
            continue;
        Slot *pS = m_pMesh->pSlots->p[i];
        GLuint bo;
        glGenBuffers(1, &bo);
        setSlotVBO(i, bo);
        glBindBuffer(GL_ARRAY_BUFFER, bo);
        glBufferData(   GL_ARRAY_BUFFER, 
                        pS->vtxBufferSizeBytes, 
                        pS->pVtxBufferData, 
                        GL_STATIC_DRAW);
        if(bUseVBUM)
        {
            // get the address of this buffer and make it resident.
            GLuint64EXT boAddr;
            glGetBufferParameterui64vNV(GL_ARRAY_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &boAddr); 
            setSlotVBUMAdd(i,boAddr);
            glMakeBufferResidentNV(GL_ARRAY_BUFFER, GL_READ_ONLY);
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // TODO : implement the case where one IBO is used for many prim groups")
    for(int i=0; i<m_pMesh->pPrimGroups->n; i++)
    {
        PrimGroup *pPG = m_pMesh->pPrimGroups->p[i];
        OGLPrimGroupData *pOGLPG = createOGLPrimGroupData(i);
        //
        // This buffer belongs to this primgroup. Create it
        //
        // Fake non-creation of EBO if requested
        if((fakeImmediateMode)&&(pPG->name[0] == 'V')&&(pPG->name[1] == 'E'))
        {
            pOGLPG->m_primGroupsBO = 0;
            continue;
        }
        if((pPG->name[0] == 'I')&&(pPG->name[1] == 'M'))
        {
            pOGLPG->fakeImmediateMode = true;
        }
        if((fakeImmediateMode)&&(pPG->name[0] == 'I')&&(pPG->name[1] == 'M'))
        {
            pOGLPG->m_primGroupsBO = 0;
            continue;
        }
        //
        // Create the EBO
        //
        if((pPG->pIndexBufferData)&&((pPG->pOwnerOfIB == NULL)||(pPG->pOwnerOfIB == pPG)))
        {
            glGenBuffers(1, &pOGLPG->m_primGroupsBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pOGLPG->m_primGroupsBO);
            glBufferData(   GL_ELEMENT_ARRAY_BUFFER, 
                            pPG->indexArrayByteSize, 
                            pPG->pIndexBufferData, 
                            GL_STATIC_DRAW);
            if(bUseVBUM)
            {
                // get the address of this buffer and make it resident.
                glGetBufferParameterui64vNV(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &pOGLPG->m_primGroupsBOAddrs); 
                glMakeBufferResidentNV(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
            }
        } else {
            pOGLPG->m_primGroupsBO = 0;
        }
    }
    //
    // Second pass to resolve the primgroups using other's buffers
    //
    for(int i=0; i<m_pMesh->pPrimGroups->n; i++)
    {
        PrimGroup *pPG = m_pMesh->pPrimGroups->p[i];
        OGLPrimGroupData *pOGLPG = getOGLPrimGroupData(i);
        if(pPG->pOwnerOfIB)
        {
            pOGLPG->m_primGroupsBO = ((OGLPrimGroupData *)pPG->pOwnerOfIB->userPtr)->m_primGroupsBO;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//
// VBO destruction
//
void OGLMeshWrapper::destroyBuffers(COGLSceneHelper *pOGLSceneHelper)
{
    NXPROFILEFUNC(__FUNCTION__);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    for(int i=0; i<(int)m_pMesh->pSlots->n; i++)
    {
        GLuint bo = getSlotVBO(i);
        if(bo > 0) glDeleteBuffers(1, &bo);
    }
    for(int i=0; i<(int)m_pMesh->pPrimGroups->n; i++)
    {
        OGLPrimGroupData* p = getOGLPrimGroupData(i);
        if(p) glDeleteBuffers(1, &(p->m_primGroupsBO));
    }
    //
    // Release allocations made for OGL data
    //
    AttrMapping* attrMapping = getAttrMapping();
    if(attrMapping && attrMapping->p) free(attrMapping->p);
    if(attrMapping) free(attrMapping);
    createAttrMapping(NULL);
    for(int i=0; i<m_pMesh->pPrimGroups->n; i++)
    {
        OGLPrimGroupData* p = getOGLPrimGroupData(i);
        if(!p) 
            continue;
        if(p->attrMapping.p)
            free(p->attrMapping.p);
        free(p);
        createOGLPrimGroupData(i, NULL);
    }
}

