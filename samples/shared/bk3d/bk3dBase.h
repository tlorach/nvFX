#pragma once
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
/**
 ** This version is used by each Nodes.
 ** v129 : added different userParams/Ptrs. Flags for Transforms.
 ** v128 : added ...
 ** v127 : added the ability to share index buffers between primgroups. Added the concept of ownership
 ** v131 : added user data slots to be able to temporarily keep data (OpenGL layer...)
 ** v132 : relocation table change to allow file mapping on memory; IK Handle (IK DOF are done : TransformDOF)
 ** v133 : Added Absolute Quaternion transformation + absolute translation + absolute scale
 **        Added IK Handle; Added float array for blendshape weights
 ** v134 : Added Quaternion Keys; made a simplified transform; rigid body + constraints + transformation info stored in tables for GPU
 ** v135 : (TO come) Added offsetConstraints, offsetRigidBodies
 **/
#define RAWMESHVERSIONSTR "134"
#define RAWMESHVERSION 0x134
#define LONG int

#pragma warning(disable: 4505)
#pragma warning(disable: 4311)
#pragma warning(disable: 4996)

#ifndef INLINE
#   define INLINE inline
#endif
#ifndef DBGASSERT
#   if defined(_DEBUG) || defined(DEBUG)
#       include <assert.h>
#       define DBGASSERT(a) assert(a);
#   else
#       define DBGASSERT(a)
#   endif
#endif

#ifndef __NVRAWMESH__
#define __NVRAWMESH__

#include "bk3dDefs.h" // definitions needed when D3D or OpenGL are not included

namespace bk3d
{
#    define NODENAMESZ (4*8)
#    define OTHERSTRINGMINSZ (4*16) // minimum size for other strings such as texture names/paths
/** 
 ** \name Node : for transforms, Meshes...
 ** @{
 **/
#define NODE_HEADER     0 ///< \brief the very first node that you should find in the binary file
#define NODE_MESH       1 ///< \brief the mesh node
#define NODE_RELOCTABLE 8
#define NODE_ATTRIBUTE  5
#define NODE_SLOT        6
#define NODE_PRIMGROUP  7
// extended items
#define NODE_TRANSFORM  2
#define NODE_JOINT      3
#define NODE_TRANSFPOOL 4
#define NODE_MAYACURVE  9
#define NODE_MAYACURVEVECTOR  10
#define NODE_FLOATARRAY 11
#define NODE_MATERIAL   12
#define NODE_QUATCURVE  13
#define NODE_IKHANDLE   14
#define NODE_IKHANDLEROTATEINFLUENCE 15
#define NODE_IKHANDLEROLLINFLUENCE   16
#define NODE_TRANSFORMSIMPLE  17
#define NODE_BONE 18
#define NODE_RIGIDBODY  19
#define NODE_CONSTRAINT 20
//...
#define NODE_END        21
/// @}
//
// Macro to reserve 64 bits in any case : x86 or x64
//
#ifdef NO64BITSCOMPAT // don't do any 32/64 bits compatibility... Doxygen will like it, for example
#define PTR64(a) a
#else
#define PTR64(a)\
    union {\
    a;\
    struct { int : 32; int : 32; };    }
#endif
///
/// \brief Node is a header for each important component.
///
/// The bk3d file is made of a list of nodes of different sizes. I should be possible to easily concatenate them or split a file Node by node.
/// 
struct Node
{
    short               nodeType;         ///< See \ref NODE_HEADER
    short                version;        ///< \brief Node version. Most of the time we expect all to be the same version. NODE_HEADER's version is the most relevant.
    unsigned int        nodeByteSize;     ///< Size in bytes of this node : this includes all what is necessary to make this node consistent.
    char                name[NODENAMESZ];    ///< NOTE: the size of the string is so that Node is either good for 64 and 32 bits
    PTR64(Node            *nextNode); ///< pointer to the next node. 32/64 bits compliant pointer
    // TODO for later: shall we add a pointer to some optional comments ? Could be good to have Node self-documented
};


struct Slot;
struct Attribute;
struct PrimGroup;
struct Mesh;
// The following strutures are the ones available if using bk3dEx.h
struct Bone;
struct TransformSimple;
struct Transform;
struct MaterialAttr;
struct TransformDOF;
struct IKHandlePool;
struct IKHandle;
struct Material;
struct TransformRefs;
struct FloatArray;
struct FloatArrayPool;
struct TransformPool;
struct MayaCurvePool;
struct QuatCurvePool;
struct MaterialPool;
struct IKHandlePool;
struct RigidBodyPool;
struct ConstraintPool;

/// basic Matrix type used to group them in a single contiguous table
  struct MatrixType {
    float     m[16];
    inline float & operator[](int i) { 
        DBGASSERT((i>=0)&&(i<16));
        return m[i]; }
    inline float* pos() { return m + 12; }
    inline operator float* () { return m; }
  };
/// basic vector 3D type
  struct Vec3Type {
    float x, y, z, w;
    inline float & operator[](int i) { 
        DBGASSERT((i>=0)&&(i<3));
        return (&x)[i]; }
    inline operator float* () { return (&x); }
  };
/// basic vector 4D type
  struct Vec4Type {
    float x, y, z, w;
    inline float & operator[](int i) { 
        DBGASSERT((i>=0)&&(i<4));
        return (&x)[i]; }
    inline operator float* () { return (&x); }
  };
/// \brief Ptr64 is a class template used to align pointers to 64 bits even when under 32 bits.
///
/// The issue is that the bk3d file binaries must be compatible with 32 and 64 bits systems. Meaning that we \b need to borrow 64 bits area
/// for each pointer, even if we are in a 32 bit system.
/// this template essentially allows to avoid a level of indirection when writing code
/// \code
/// Ptr64 myPtr;
/// myPtr.p->doSomething
/// //can be written :
/// myPtr->doSomething
/// \endcode
template <class T>
struct Ptr64 
{
    /// overloading '=' for ptr assignment
    inline T* operator =(T * _p) 
    {
        p = _p;
        return p;
    }
    /// overloading the pointer request
    inline operator T*(void) 
    { 
        return p; 
    }
    /// overloading the cast to char
    inline operator char*(void) 
    { 
        return (char*)p;
    }
    /// overloading the field access of the pointer
    inline T* operator->(void) 
    { 
        return p; 
    }
    inline T& operator [](int i) 
    {
        return p[i];
    }
    #ifdef NO64BITSCOMPAT // don't do any 32/64 bits compatibility... Doxygen will like it, for example
        T *p;            ///< the pointer that we are interested in
    #else
    //PTR64(T *p); 
    union {
        T *p;            ///< the pointer that we are interested in
        long long ll;    ///< a long-long to borrow the bits...
    };
    #endif
};
/*--------------------------------
Bounding volumes
----------------------------------*/
/// Bounding Sphere that is supposed to encomparse the vertices of a Mesh
struct BSphere
{
    Vec3Type    pos; ///< center of the sphere
    float       radius; ///< radius
};
/// Axis-aligned bounding box
struct AABBox
{
    Vec3Type    min; // min 3D position
    Vec3Type    max; // max 3D position
};

/// Pool of slots ('stream' in DX10)
struct SlotPool //: public Node
{
    int       n; ///< amount of slots
    int          : 32;
    Ptr64<Slot>        p[1]; ///< array of n slots
};

/// Pool of vertex attributes
struct AttributePool //: public Node // == Layout : contains all the attributes in a list
{
    int       n;             ///< amount of attributes
    int          : 32;
    Ptr64<Attribute> p[1];    ///< array of attributes
};

/// Pool of Primitive groups
struct PrimGroupPool //: public Node
{
    int       n;
    int          : 32;
    Ptr64<PrimGroup> p[1];
};

/// Pool of Meshes
struct MeshPool //: public Node
{
    int       n : 32;
    int            : 32;
    Ptr64<Mesh>        p[1];
};

/*--------------------------------
Mesh related structures
----------------------------------*/

///
/// \brief Attribute of a vertex (NODE_ATTRIBUTE) : data required to setup vertex shader's input attributes
///
/// The idea is to provide an much information as possible so the application can easily setup data
/// (such as Input-Layout in DX; or such as OpenGL string and offsets for AttributePointer...)
///
  struct Attribute : public Node
  {
    D3DDECLTYPE         formatDX9    : 32;   ///< DX9 type for the attribute format
    DXGI_FORMAT         formatDXGI    : 32;   ///< DX10/11 type for the attribute format

    GLType              formatGL    : 32;   ///< OpenGL format. But it doesn't say how many components. See numComp.
    unsigned char       semanticIdx;        ///< semantic index is for example the Idx of the texcoord level
    unsigned char       numComp;            ///< amount of components (since formatGL doesn tell about this)
    unsigned char       strideBytes;        ///< stride in bytes of the attribute
    unsigned char       alignedByteOffset;  ///< DirectX10 needs it in the Input-Layout definition : offset in the layout.

    unsigned int        dataOffsetBytes;    ///< for example, use this for VertexPointer() in VBO or what we do with offsetof(VertexStructure, p)...
    unsigned int        slot;               ///<the Slot # to which it belongs. A Slot can have 1 or more attributes (interleaved)
    /// \todo add the Slot pointer ?
    PTR64(void           *pAttributeBufferData);  ///< This is where the data of the vertex-attribute are located. This pointer \b already has \b dataOffsetBytes added

    unsigned int        userData[2]; ///< free user data. You can add information for Attribute <> OpenGL setup, for example

    Attribute() {init();}
    void init()
    {
      memset((void*)this, 0, sizeof(Attribute));
      nodeType = NODE_ATTRIBUTE;
      version = RAWMESHVERSION;
    }
  };


  /// 
  /// \brief slot/stream (NODE_SLOT) : made of interleaved attributes. In the easiest case it has only one attribute.
  ///
  /// \remark a Slot COULD contain a subset of vertices instead of the whole : if the slot
  /// is being used on a smaller part of the mesh, no need to provide all.
  /// In this case, the Slot may contain
  /// - an attribute with index of the vertices (MESH_VERTEXID)
  /// - other attributes containing the values for these vertices index by the first attribute...
  /// Typical use : Blendshapes/Skinning computed locally onto the mesh. Maya/Blender work this way (Blender uses Vertex Groups)
  /// Can be good if we want to compute these modifiers from CPU; from a SPU or From Compute !
  /// TODO for 0x130: TEST THIS !!!!
  ///
  struct Slot : public Node
  {
    unsigned int        vtxBufferSizeBytes;     ///< size of the vertex buffer of this Slot, in bytes
    unsigned int        vtxBufferStrideBytes;   ///< stride in bytes of the vertex buffer

    unsigned int        vertexCount;            ///< amount of vertices in this slot
    int                 userData;               ///< arbitrary integer user-value
    Ptr64<int>          userPtr;                ///< arbitrary pointer for the user

    /// references to used attributes in this Slot. OpenGL setup essentially works with attributes, while DX10/11 works with Slots.
    /// A Slot can have 1 or more attributes. More than 1 attribute means that they are interleaved
    PTR64(AttributePool *pAttributes);
    PTR64(void          *pVtxBufferData);         ///< array of data : attibutes; interleaved if more than 1

    Slot() {init();}
    void init()
    {
      memset((void*)this, 0, sizeof(Slot));
      nodeType = NODE_SLOT;
      version = RAWMESHVERSION;
    }
  };

/// \brief Primitive group that is in a Mesh
///
/// one primitive group is really related to one \b Drawcall.
///
/// \note PrimGroup isn't a node... so related to a Mesh...
  struct PrimGroup : public Node
  {
    unsigned int                indexCount        : 32; ///< total number of elements. If multi-index: total # of multi-index elements
    unsigned int                indexOffset        : 32; ///< offset: where to start in the index table. interesting when sharing the same index buffer

    unsigned int                indexArrayByteOffset : 32;  ///< offset to reach the element array from &data[1]
    unsigned int                indexArrayByteSize     : 32;  ///< total size in bytes

    unsigned int                primitiveCount    : 32;   ///< DX9 wants it
    unsigned int                indexPerVertex    : 32;   ///< usually 1. If >1 then we are in the case of multi-index mode...

    /// \name index formats for various API's
    /// @{
        D3DFORMAT                   indexFormatDX9    : 32;   ///< index format for DX9
        DXGI_FORMAT                 indexFormatDXGI : 32;   ///< index format for DX10

        GLType                      indexFormatGL    : 32;   ///< index format for openGL
    /// @}
    /// \name topology (primitive type) for various API's
    /// @{
        D3DPRIMITIVETYPE            topologyDX9        : 32;

        /// Note: the DX11 topology can be used in OpenGL to find back the batch vertices. Good enough
        union {
        D3D10_PRIMITIVE_TOPOLOGY    topologyDX10    : 32;
        D3D11_PRIMITIVE_TOPOLOGY    topologyDX11    : 32; ///< just extends DX10...
        OGL_PATCH_VERTICES           glPatchVertices : 32; ///< tells the # of vertices when topo is GL_PATCHES
        };
        GLTopology                  topologyGL        : 32;
    ///@}
    PTR64(void                      *userPtr);            ///< a user pointer to store whatever additional data after load

    /// it is possible that the Index buffer doesn't belong to this structure, but references one from another (so they share it).
    /// useful when creating a OGL/DX buffer : to avoid redundancy
    PTR64(PrimGroup             *pOwnerOfIB);
    /// pIndexBufferData can be shared with other groups (if pOwnerOfIB points to another PrimGroup).
    /// this pointer directly points to the right place (no need to add indexOffset )
    PTR64(void *                pIndexBufferData);
    PTR64(Material              *pMaterial);        ///< Material in used for drawing this group
    /// Transforms used for this drawcall.
    /// it can be NULL is no need. It can have many for the skinning case
    PTR64(TransformRefs         *pTransforms);

    unsigned int                minIndex        : 32; ///< min element index (for OpenGL)
    unsigned int                maxIndex        : 32; ///< max element index (for OpenGL)
    unsigned int                primRestartIndex: 32; ///< primitive Restart Index (added in v130)
    BSphere                     bsphere;              ///< 3+1 floats
    AABBox                      aabbox;                  ///< 3*2 floats
    int : 32; ///< to adjust AABox 
    float visible;// visibility of the primitive group (was int : 32)

    PrimGroup() {init();}
    void init()
    {
      memset((void*)this, 0, sizeof(PrimGroup));
      nodeType = NODE_PRIMGROUP;
      version = RAWMESHVERSION;
    }
  };
  //------------------------------------------------------------------------------------------
  /** \brief Mesh Node.
  
  In the case of instancing : many solutions can be taken
  - Use the same Mesh and aggregate transformations as explained below
  - create a brand new Mesh object *but* share the data asset
   - However we can duplicate what is different between meshes

     for example the transformation references could be totally different from one Mesh instance to another

     another example : skinning weight could be different for one instance... leading to another attribute table for skinning (Slot)
  \todo it is a known fact that rendering same Primitive groups of all instances is better than rendering Meshes one after another
  The question is how to do this...
  - Ok for the case where we aggregated all transforms in the Mesh
  - No Ok for the case where we duplicated the Mesh struct for each instance...
  \note In this design, the skeleton doesn't belong to the Mesh. The skeleton is really part of the transform tree...
  **/
  struct Mesh : public Node
  {
    //
    // SIMPLE ITEMS
    //
    PTR64(SlotPool      *pSlots);                              ///< or called STREAM. Infos + Vertex Buffers for each slot/layer/stream
    PTR64(PrimGroupPool *pPrimGroups);                       ///< primitive group structures is always at the begining of the data chunk.
    //
    // ADDITIONAL ITEMS
    //
    PTR64(AttributePool *pAttributes); ///<all attributes of all Slots together
    PTR64(AttributePool *pBSAttributes); ///<all Blendshapes attributes of all Slots together
    /// N Blendshapes lead to N Blend-shape slots !
    PTR64(SlotPool      *pBSSlots);
    /// v.133: an array of float values for Blendshape weights. 1 float for 1 BS-Slot in pBSSlots
    PTR64(FloatArray    *pBSWeights);
    /// Transform offsets : the ones the mesh is using (many refs for the skinning case, 0 or 1 for the basic case)
    /// Note: if many transforms are regular transforms (not tagged as Bone), then it can mean that you have many instances of this mesh to render
    /// if the list is Regular-Transf + Bones-Transf's...; Regular-Transform + Bones transforms... : then we have many instances with different skeletons
    PTR64(TransformRefs *pTransforms);

    PTR64(FloatArrayPool *pFloatArrays);            ///< array of float coming from curves or anything else derived from FloatArray. For Blendshape animation... for example
    PTR64(void           *userPtr);                 ///< arbitrary data could be pointed by this guy
    BSphere               bsphere;                          ///< for the whole mesh, all primgroup included
    AABBox                aabbox;
    int                   numJointInfluence;                 ///< # of weights for skinning. Weights are passed through attributes. For now only one attrib can be used (4 weights)
    /// v133 : replaced this empty 32 bits with "visibility factor"
    float                 visible; //int : 32; // alignment...
    //
    // Methods
    //
    Mesh() {init();}
    void init()
    {
      memset((void*)this, 0, sizeof(Mesh));
      nodeType = NODE_MESH;
      version = RAWMESHVERSION;
    }
  };

  ///
  /// \brief Pointer relocation table
  ///
  /// When stored as a file, pointers are turned to offsets. later, we use this relocation table to compute pointers.
  ///
  struct RelocationTable : public Node
  {
    LONG            numRelocationOffsets;      ///< data telling where to resolve pointers
    int                : 32;
    /// in the case of file mapping (mmap or windows equivalent), we may change values in the
    /// file at ptr locations and thus lost the offsets. Which is why we should keep them here, too
    struct Offsets {
        unsigned LONG ptrOffset; ///< offset where is located the ptr
        unsigned LONG offset; ///< offset used to recompute the ptr
    };
    PTR64(Offsets     *pRelocationOffsets);      ///< data telling where to resolve pointers

    RelocationTable() {init();}
    void init()
    {
      memset((void*)this, 0, sizeof(RelocationTable));
      nodeType = NODE_RELOCTABLE;
      version = RAWMESHVERSION;
    }
  };
  //------------------------------------------------------------------------------------------
  /// \brief Header (NODE_HEADER) of the Bk3d file. This where the main data are accessible
  ///
  /// \remark Keep the order of items if adding more : so we keep compatibility
  /// 
  //------------------------------------------------------------------------------------------
  struct FileHeader : public Node
  {
    // removed these 2 fields : version is now in the Node and magic is off...
    //unsigned int  magic;                // magic unsigned int  "MESH". Just to make sure this is a correct file. \todo version checking ?
    //unsigned int  version;              // version of this file

    PTR64(MeshPool      *pMeshes);               ///< first Mesh Node
    //
    // ADDITIONAL INFO
    //
    PTR64(TransformPool     *pTransforms);      ///< to be resolved at load time. Contains 0 to N transform offsets
    PTR64(MayaCurvePool     *pMayaCurves);
    PTR64(MaterialPool      *pMaterials);
    PTR64(IKHandlePool      *pIKHandles);
    PTR64(RelocationTable   *pRelocationTable);
    PTR64(QuatCurvePool     *pQuatCurves);
    PTR64(RigidBodyPool     *pRigidBodies);
    PTR64(ConstraintPool     *pConstraints);
    // TODO:
    // Cameras    *pCameras;
    // Modifiers  *pModifiers; // Lattice...

    //----------------------------
                  FileHeader();
    void          init();
    void          resolvePointers(void* pBufferArea);
    void          cleanBufferPointers(void* pBufferArea, bool bPutBackOffsets = false, long long basePtr = 0);
    void          restorePointerOffsets(void* pBufferArea);
  };
} //namespace bk3d

//==========================================================================================
// 
// Inline methods
// 
//==========================================================================================
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS
//
// ptr of the file contain offsets, relative to the class it belongs to
// 
#define RESOLVEPTR(pstruct, ptr, type) ptr = (ptr ? (type*)(((char*)pstruct) + (unsigned int)(ptr)) : NULL)

//
// Unicode and formatting is a bit tricky...
// 
#ifdef  UNICODE
#   define FSTR L"%S"
#    ifndef PRINTF
//#        pragma message("PRINTF undefined, defining it now")
#        define PRINTF(a) wprintf a
#    endif
#    ifndef EPRINTF
//#        pragma message("EPRINTF undefined, defining it now")
#        define EPRINTF(a) wprintf a
#    endif
#   ifndef TEXT
#       define TEXT(t) L t
#   endif
#   ifndef _WINNT_
        typedef const TCHAR * LPCSTR;
#   endif
#else
#   define FSTR "%s"
#    ifndef PRINTF
//#        pragma message("PRINTF undefined, defining it now")
#        define PRINTF(a) printf a
#    endif
#    ifndef EPRINTF
//#        pragma message("EPRINTF undefined, defining it now")
#        define EPRINTF(a) printf a
#    endif
#   ifndef TEXT
#       define TEXT(t) t
#   endif
#   ifndef LPCSTR
#        ifdef BK3DVERBOSE
//#        pragma message("!!! Defining our own LPCSTR !!!")
#        endif
        //typedef const char * LPCSTR;
#   endif
#endif

//
// Optional use of ZLIB
// 
#ifndef GFILE
#ifndef NOGZLIB
//#pragma message("Using zlib to read bk3d models...")
#include "zlib.h"
#define GFILE gzFile
#define GOPEN gzopen
#define GREAD(a,b,c) gzread(a, b, (unsigned int)c)
#define GCLOSE gzclose
#else
#define GFILE FILE *
#define GOPEN fopen
#define GREAD(a,b,c) (int)fread(b, 1, c, a)
#define GCLOSE fclose
#endif
#endif

namespace bk3d
{

INLINE FileHeader::FileHeader() 
{
    init();
}

INLINE void FileHeader::init()
{
  version = RAWMESHVERSION;
  nodeByteSize = sizeof(FileHeader);
  pMeshes = NULL;
  pTransforms = NULL;
  nextNode = NULL;
  nodeType = NODE_HEADER;
  nodeByteSize = sizeof(FileHeader);
}

//------------------------------------------------------------------------------------------
// 
/// global resolution of pointers : this function uses RelocationTable to resolve pointers
// 
//------------------------------------------------------------------------------------------
INLINE void FileHeader::resolvePointers(void* pBufferArea)
{
    RESOLVEPTR(this, pRelocationTable, RelocationTable); // write the correct pointer now we are in memory
    RESOLVEPTR(this, pRelocationTable->pRelocationOffsets, RelocationTable::Offsets); // write the correct pointer now we are in memory
    for(int i=0; i < pRelocationTable->numRelocationOffsets; i++)
    {
        char* ptr = (char*)this;
        unsigned LONG offs = pRelocationTable->pRelocationOffsets[i].ptrOffset;
        if(offs == 0)
            continue;
        if(offs >= nodeByteSize)
            ptr = (char*)pBufferArea + offs - nodeByteSize;
        else
            ptr += offs;
        unsigned long long *ptr2 = (unsigned long long *)ptr;
        if(*ptr2)
        {
            unsigned long long o = pRelocationTable->pRelocationOffsets[i].offset;
            if(o >= nodeByteSize)
                *ptr2 = (unsigned long long)(((char*)pBufferArea) + o - nodeByteSize);
            else
            *ptr2 = (unsigned long long)(((char*)this) + o);
        }
    }
}
// Cleans the pointers related to the Buffer area
// NOTE: it can put back the offsets : for example if this area got
// copied in a VBO, the offsets are their locations in the VBO
// Or we can propose a 64 bits base pointer : if the buffer was copied in VMem
// and we decided to use Bindless graphics to access them
INLINE void FileHeader::cleanBufferPointers(void* pBufferArea, bool bPutBackOffsets, long long basePtr)
{
    for(int i=0; i < pRelocationTable->numRelocationOffsets; i++)
    {
        char* ptr = (char*)this;
        unsigned LONG offs = pRelocationTable->pRelocationOffsets[i].ptrOffset;
        if(offs == 0)
            continue;
        if(offs >= nodeByteSize)
            ptr = (char*)pBufferArea + offs - nodeByteSize;
        else
            ptr += offs;
        unsigned LONG *ptr2 = (unsigned LONG *)ptr;
        if(*ptr2)
        {
            unsigned LONG o = pRelocationTable->pRelocationOffsets[i].offset;
            if(o >= nodeByteSize)
        // TODO: put back offsets + 64 bits ptr value
                *ptr2 = NULL;//o;
        }
    }
}
/// restores the offsets at the pointer locations. Use it when you plan to save the bk3d file

INLINE void FileHeader::restorePointerOffsets(void* pBufferArea)
{
    // cleanup "user data" areas
    if(pMeshes) for(int i=0; i<pMeshes->n; i++) {
        Mesh *pM = pMeshes->p[i];
        pM->userPtr = NULL;
        if(pM->pAttributes) for(int j=0; j<pM->pAttributes->n; j++)
            memset(pM->pAttributes->p[j]->userData, 0, 2*sizeof(unsigned int));
        if(pM->pSlots) for(int j=0; j<pM->pSlots->n; j++) {
            pM->pSlots->p[j]->userData = 0;
            pM->pSlots->p[j]->userPtr.ll = 0; }
        if(pM->pPrimGroups) for(int j=0; j<pM->pPrimGroups->n; j++)
            pM->pPrimGroups->p[j]->userPtr = NULL;
    }
    // put back offsets
    for(int i=0; i < pRelocationTable->numRelocationOffsets; i++)
    {
        char* ptr = (char*)this;
        size_t offs = pRelocationTable->pRelocationOffsets[i].ptrOffset;
        if(offs == 0)
            continue;
        if(offs >= nodeByteSize)
            ptr = (char*)pBufferArea + offs - nodeByteSize;
        else
            ptr += offs;
        unsigned LONG *ptr2 = (unsigned LONG *)ptr;
        if(*ptr2)
        {
            unsigned LONG o = pRelocationTable->pRelocationOffsets[i].offset;
            *ptr2 = o;
        }
    }
    // 2 pointers must be done by hand
    pRelocationTable->pRelocationOffsets = (RelocationTable::Offsets*)((size_t)pRelocationTable->pRelocationOffsets - (size_t)this);
    pRelocationTable = (RelocationTable*)((size_t)pRelocationTable - (size_t)this);
}
//--------------------------------
// 
/// LOAD function
/// 
/// Returns the baked structure of all the data you need to work
// 
//--------------------------------
INLINE static FileHeader * load(const char * fname, void ** pBufferMemory=NULL, unsigned int* bufferMemorySz=NULL)
{
    GFILE fd = NULL;
    if(!fname)
        return NULL;
    fd = GOPEN(fname, "rb");
    if(!fd)
    {
      EPRINTF((TEXT("Error : couldn't load ") FSTR TEXT("\n"), fname));
        return NULL;
    }
	unsigned LONG realsize = 0;
#ifdef NOGZLIB
    fseek(fd, 0, SEEK_END);
	realsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);
#else
	FILE *file = fopen(fname,"rb");
    // http://www.onicos.com/staff/iz/formats/gzip.html header must have 0x1f 0x8b
    unsigned short header;
    fread(&header, 2, 1, file);
    fseek(file, 0, SEEK_END);
	realsize = ftell(file);
    if(header == 0x8b1f) // fetch the real size at the end
    {
        fseek(file, realsize-4, SEEK_SET);
	    fread(&realsize, 4, 1, file);
    }
	fclose(file);
#endif
    // load the Node, first
    int n = 0;
    unsigned int offs = sizeof(Node);
    char * memory = (char*)malloc(offs);
    n= GREAD(fd, memory, offs);
    if(((FileHeader *)memory)->version != RAWMESHVERSION)
    {
      PRINTF((TEXT("Error>> Wrong version in Mesh description\n")));
      PRINTF((TEXT("needed %x and got %x\n"), RAWMESHVERSION, ((FileHeader *)memory)->version));
      free(memory);
      return NULL;
    }
    // This represents the size of the structures defining the Meshes
    unsigned int modelStructSize = ((FileHeader *)memory)->nodeByteSize;
    memory = (char*)realloc(memory, modelStructSize);
    n= GREAD(fd, memory + offs, modelStructSize - offs);
    // Now anything beyond this is Buffer Memory : vertex tables etc.
    char *memory2 = (char*)malloc(realsize - modelStructSize);
    // splitting in 2 at least: huge files (4Gb would fail otherwize when through GZip lib)
    char *memory2b = memory2;
    unsigned LONG sz2 = realsize - modelStructSize;
    unsigned LONG hsz2 = sz2/2; // change the divisor to split more than 2
    unsigned LONG rsz2 = sz2;
    while(rsz2 != 0)
    {
        unsigned LONG sz = rsz2 < hsz2 ? rsz2 : hsz2;
        n= GREAD(fd, memory2b, sz);
        memory2b += sz;
        rsz2 -= sz;
    }

    if(bufferMemorySz)
        *bufferMemorySz = n;
    if(pBufferMemory)
        *pBufferMemory = memory2;
    if(fd)
        GCLOSE(fd);
    ((FileHeader *)memory)->resolvePointers(memory2);
    //PRINTF((TEXT("Loaded ") FSTR TEXT(" (mesh version %x)\n"), fname, ((FileHeader *)memory)->version));
    return (FileHeader *)memory;
}


// level : 0 for brief; 1 for all; 2 for all including attributes and index tables (!)
extern float* FileHeader_findComponentf(FileHeader *pH, const char *compname, bool **pDirty);
extern void FileHeader_debugDumpAll(FileHeader* pH, int level, const char * nodeNameFilter);
extern void Transform_debugDumpLayout(Bone*pT, int l, int level, const char * nodeNameFilter);
extern void Mesh_debugDumpLayout(Mesh *pM, unsigned int &nVtx, unsigned int &nElts, unsigned int &nPrims, int level, const char * nodeNameFilter);


} //namespace bk3d

#endif //__NVRAWMESH__

