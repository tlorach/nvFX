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

 ** This version is used by each Nodes.
 ** v129 : added different userParams/Ptrs. Flags for Transforms.
 ** v128 : added ...
 ** v127 : added the ability to share index buffers between primgroups. Added the concept of ownership
 ** v131 : added user data slots to be able to temporarily keep data (OpenGL layer...)
 ** v132 : relocation table change to allow file mapping on memory; IK Handle (IK DOF are done : TransformDOF)
 ** v133 : Added Absolute Quaternion transformation + absolute translation + absolute scale
 **        Added IK Handle; Added float array for blendshape weights
 **/
#ifndef __BK3DEX__
#define __BK3DEX__
#include "bk3dBase.h"
#ifndef DBGASSERT
#   pragma message("DBGASSERT wasn't defined. Defining it as NULL")
#   define DBGASSERT(a)
#endif
namespace bk3d
{

///
/// \brief connector NODE_FLOATARRAY for vectors/matrices/scalars of Floats.
///
/// This structure will be referenced by Transforms or Meshes or others.
/// this is used when for example a curve is supposed to change something
/// on the receiver. IncomingValue is generic. Can be a curveVector or other...
///
struct FloatArray : public Node
{
	int		dim; ///< amount of components in f
	float	f[1]; ///< array of dim floats. f[1] will be in fact f[dim]
	/// Constructor of this Node
	FloatArray() {		
		memset((void*)this, 0, sizeof(FloatArray));
		nodeType = NODE_FLOATARRAY;
		version = RAWMESHVERSION;
		strcpy_s(name, 31, "FloatArray");
		}
};

/// \brief Pool of FloatArray's
///
/// \remark this structure doesn't require to be a Node.
///
/// This pool would gather for example all the input data that would also be connected to a curve.
///
/// in a Mesh : each Blendshape would have a curve...
struct FloatArrayPool
{
	FloatArrayPool() {		
		memset((void*)this, 0, sizeof(FloatArrayPool));
		}
	int			n; ///< amount of FloatArray
	int			: 32;
	struct Connection ///< associates the source and destination
	{
        char destName[32];		///< name of the destination. QUESTION: we should assume first char is the TYPE
		PTR64(FloatArray *p);  ///<pointer to a FloatArray source
        // Note (v133) : so far I don't give a type info on what is the target type... we can know it from the destName
	    union {
	    float *pfTarget; ///<ptr to the destination where to write the data from FloatArray. Can be NULL if we want to rely only on destName...
        int   *piTarget; ///<(v133) ptr to an integer destination, instead
        int   ival[2]; ///<(v133) some cases could just be some offset with not real target (for Blendshape #, for example)
        struct { int : 32; int : 32; };// maybe ival[2] is enough an no more need for this struct (64 bits compliance)...
        };
	};
	Connection p[1]; ///< serie of 'n' connections
};

struct Slot;
struct Attribute;
struct PrimGroup;
struct Mesh;
struct Bone;
struct TransformSimple;
struct Transform;
struct MaterialAttr;
struct TransformDOF;
struct FloatPool;
struct IKHandlePool;
struct IKHandle;

struct RigidBody;
struct Constraint;
struct RigidBodyPool;
struct ConstraintPool;

/*--------------------------------
MAYA Animation Curves. We define them through
- vector of curve
	- contains curves
		- contains keys
----------------------------------*/
/// type of behavior for the curve at infinity
enum EtInfinityType 
{
	kInfinityConstant=0,
	kInfinityLinear=1,
	kInfinityCycle=3,
	kInfinityCycleRelative=4,
	kInfinityOscillate=5,
	kInfinityEnd = -1
};
/// tangent type of the curve
enum EtTangentType 
{
	kTangentGlobal = 0,
	kTangentFixed,
	kTangentLinear,
	kTangentFlat,
	kTangentSmooth,
	kTangentStep,
	kTangentSlow,
	kTangentFast,
	kTangentClamped,
	kTangentPlateau,
    kTangentStepNext,
	kTangentEnd = -1
};

typedef enum EtTangentType EtTangentType;

/// maya curve key. You can refer to Maya documentation for details
struct MayaReadKey
{
	float			time;
	float			value;

	EtTangentType	inTangentType	: 32;
	EtTangentType	outTangentType	: 32;

	float			inAngle;
	float			inWeight;

	float			outAngle;
	float			outWeight;
};

/// maya curve object. This is also related to Maya documentation
struct MayaCurve : public Node
{
	MayaCurve()
	{
		memset((void*)this, 0, sizeof(MayaCurve));
		nodeType = NODE_MAYACURVE;
		version = RAWMESHVERSION;
		inputIsTime = true;
		outputIsAngular = false;
		isWeighted = false;
		preInfinity = kInfinityConstant;
		postInfinity = kInfinityConstant;
	}
	// Curve Settings
	EtInfinityType preInfinity	: 8;	///< how to evaluate pre-infinity			
	EtInfinityType postInfinity	: 8;	///< how to evaluate post-infinity		
	bool	inputIsTime			: 1;	///< if true, the input do not need Plugs to increase
	bool	outputIsAngular		: 1;
	bool	isWeighted			: 1;	///< whether or not this curve has weighted tangents 
	int							: 13;
	// IO
	float	fIn;		///< input value

	float	fOut;		///< result
	// KEYS
	int		nKeys;

	MayaReadKey key[1]; ///< array of keys
};

/// vector of maya curve. Most of the time we use vector curves (to connect to any 3D component...)
/// 
/// Note that we use FloatArray in order to allow these curve-vectors to be connected to something else (see FloatArrayPool )
struct MayaCurveVector : public Node
{
	int		nCurves	: 32;
	int				: 32;
	PTR64(FloatArray	*pFloatArray); ///< place to write the final result. It's a pointer because FloatArray's size can vary
	Ptr64<MayaCurve>	pCurve[1]; ///< nCurves that are used to define the curve-vector
	//-----------------------
	MayaCurveVector()
	{
		memset((void*)this, 0, sizeof(MayaCurveVector));
		nodeType = NODE_MAYACURVEVECTOR;
		version = RAWMESHVERSION;
	}
};

/// Quaternion curve key
struct QuatReadKey
{
	float			time;
	float			value[4];
    int             :32;
};
/// Curve for quaternions
struct QuatCurve : public Node
{
	QuatCurve()
	{
		memset((void*)this, 0, sizeof(QuatCurve));
		nodeType = NODE_QUATCURVE;
		version = RAWMESHVERSION;
	}
	PTR64(FloatArray	*pFloatArray); ///< place to write the final result. It's a pointer because FloatArray's size can vary
	// IO
	float	fIn;		///< input value
	float	fOut;		///< result
	// KEYS
	int		nKeys   : 32;
	int		userData: 32;

	QuatReadKey key[1]; ///< array of keys
};

/// pool of quaternion curves
struct QuatCurvePool //: public Node
{
	int				n; ///< amount of curves
	int				: 32;
	Ptr64<QuatCurve> p[1]; ///< array of n curves
};

/// pool of curves
struct MayaCurvePool //: public Node
{
	int				n; ///< amount of curves
	int				: 32;
	Ptr64<MayaCurveVector> p[1]; ///< array of n curves
};

/// Pool of float values
struct FloatPool
{
	int       n : 32;
	int			: 32;
	float     f[1];
};

/// Pool of materials
struct MaterialAttrPool
{
	int			n;
	int		  : 32;
	Ptr64<MaterialAttr>	p[1];
};
/*--------------------------------
Transform, Transform Pool and references
This contains all the tranformations and children/parent infos
----------------------------------*/
  #define TRANSFCOMP_pos                    (1<<0)
  #define TRANSFCOMP_scale                  (1<<1)
  #define TRANSFCOMP_rotation               (1<<2)
  #define TRANSFCOMP_Quat                   (1<<3)
  #define TRANSFCOMP_rotationOrder          (1<<4)
  #define TRANSFCOMP_scalePivot             (1<<5)
  #define TRANSFCOMP_scalePivotTranslate    (1<<6)
  #define TRANSFCOMP_rotationPivot          (1<<7)
  #define TRANSFCOMP_rotationPivotTranslate (1<<8)
  #define TRANSFCOMP_rotationOrientation    (1<<9)
  #define TRANSFCOMP_jointOrientation       (1<<10)
  //#define TRANSFCOMP_bindPose               (1<<11) // TODO: remove or recycle...
  #define TRANSFCOMP_isBone                 (1<<12)
  #define TRANSFCOMP_matrix                 (1<<13) // tells the matrix is the origin of the transformation and is not calculated from other params (pos, scale)
  #define TRANSFCOMP_abs_matrix             (1<<14) // tells the matrix is the origin of the transformation and is not calculated from other params (pos, scale)
  #define TRANSFCOMP_bindpose_matrix        (1<<15)
  #define TRANSFCOMP_abs_Quat               (1<<16) // 0x133
  #define TRANSFCOMP_abs_Quat_ready         (1<<16) // 0x133 : let's keep the same bit for now : abs Quat is never defined as the origin
  #define TRANSFCOMP_matrix_ready           (1<<17) // 0x133 : tells the matrix is ready but wasn't natively defined in the object
  #define TRANSFCOMP_abs_matrix_ready       (1<<18) // 0x133 : tells the abs_matrix is ready but wasn't natively defined in the object
  #define TRANSFCOMP_Quat_ready             (1<<19) // 0x133 : tells the quaternion is ready but wasn't natively defined in the object
  #define TRANSFCOMP_has_DOF                (1<<20)
  #define TRANSFCOMP_invalidMatrix          (1<<31)

  #define TRANSFCOMP_MAYATRANSF (TRANSFCOMP_scale|TRANSFCOMP_pos|TRANSFCOMP_rotation|TRANSFCOMP_scalePivot|TRANSFCOMP_scalePivotTranslate\
                                |TRANSFCOMP_rotationPivot|TRANSFCOMP_rotationPivotTranslate|TRANSFCOMP_rotationOrientation)
  #define TRANSFCOMP_MAYAJOINT (TRANSFCOMP_scale|TRANSFCOMP_rotationOrientation|TRANSFCOMP_rotation|TRANSFCOMP_jointOrientation|TRANSFCOMP_pos\
                               |TRANSFCOMP_isBone/*|TRANSFCOMP_bindpose_matrix|TRANSFCOMP_bindPose*/)
/// data that are put together for the Bone: there is no need to separate them:
/// they make sense to stay together. Used for updating the Bone transformation data
/// \remark validComps: contains TRANSFCOMP_ flags. These bits are important to know which field is valid.
/// \note   TRANSFCOMP_matrix, TRANSFCOMP_abs_matrix, TRANSFCOMP_bindpose_matrix mean that
/// the matrices are there and ready to use from the baked export. This means that it is not needed to recompute
/// them prior to use them. Unless something changed (say, pos etc.).
/// If no other flags are valid, this means that nothing but the matrices are available.
///
/// \note Inversely, if these matrix bits aren't set but only separate components are, then this means that
/// you must compte the matrices before using them.
/// 
/// \note Flag TRANSFCOMP_isBone : set to 1 when the transformation is considered as part of a skeleton
/// this flag is useful for IK computation and to draw a line between a "scene-graph" hierarchy and a skeletong.
/// The choice of mixing all together like a big hierarchy was made to simplify the tree of transformations.
///
/// \note Flag TRANSFCOMP_invalidMatrix : Just tells if the resulting matrices are valid or not
///

  struct BoneDataType {
    // 8 bytes:
    unsigned short    parentID;
    unsigned short    childrenListID; // offset where in tableChildrenLists to find the list of children: [NChildren, child0, child1,...]
    unsigned short    DOFID;           ///< ref to TransformDOF in tableTransformDOF if this transform should be limited in its movement...
    /// when bDirty is true, you should recompute the matrices (relative, absolute, skin). The consequence is that
    /// some children of the dirty transformations should either re-compute their absolute matrices
    /// when bDirty is false, all the matrices are ready to use.
    unsigned char     bDirty;
    unsigned char     :8;
    //unsigned short    IKHandlesListID; ///< ref to the of IKHandles in tableIKHandlesLists to which the tranform can be influenced by
    // 8x8 bytes:
    MatrixType      matrix;
    // 8x2 bytes:
    Vec3Type        posBoneTail;
    unsigned int    validComps;
    // 8x4 bytes
    Vec4Type        quat;
    Vec4Type        quatAbs;
    BoneDataType() {init();}
    void init()
    {
        memset((void*)this, 0, sizeof(BoneDataType));
    }
  };

  // Maya-style transforms. We don't need structure of array for them: we don't expect them to be used separately
  struct MayaTransformData {
    Vec3Type    rotation;           	///< Euler Rotation in degres (TRANSFCOMP_rotation)
    char        rotationOrder[3];      	///< 3 chars for "xyz" or any other (TRANSFCOMP_rotationOrder)
    // Note: Do we really need all of them ? Overkill ?
    Vec3Type    scalePivot;			///< TRANSFCOMP_scalePivot
    Vec3Type    scalePivotTranslate;	///< TRANSFCOMP_scalePivotTranslate
    Vec3Type    rotationPivot;		///< TRANSFCOMP_rotationPivot
    Vec3Type    rotationPivotTranslate;///< TRANSFCOMP_rotationPivotTranslate

    Vec4Type    rotationOrientation; ///< TRANSFCOMP_rotationOrientation, Quaternion
    Vec4Type    jointOrientation;    ///< TRANSFCOMP_jointOrientation, Quaternion
    MayaTransformData() {init();}
    void init()
    {
        memset((void*)this, 0, sizeof(MayaTransformData));
    }
  };

  /// \brief data for IK Handle transformations
  /// not all the transformations need this kind of data. In fact, IK Handles aren't needed
  /// for rendering. So they should even be out of the table to send to the GPU
  /// thus, IKHandle transformations should be put always at the end of the list of transfomations in TransformPool
  /// and an offset should be used to
  struct IKHandleData
  {
    int priority;
    int maxIter;
    float weight;

    // Removed: instead we use the posBoneTail of the first transformation in the IK chain
    //float effectorPos[3]; ///< position relative to the first transform : handle position
    // Removed: now IKHandle inherits from Transform
    //float handlePos[3]; ///< target position relative to the optional transform

    /// pool of transforms for pos : has all from pos's parent tranform to Transform's parents...
    /// used to resolve the IK : if the transform isn't here, it won't be used for IK
    /// the first pair of (transformID, weight) contains the number for items following in 'transformID' field
    /// the next pair is the tranform to which we attached the handle (pos) : the end-effector's transform
    /// the others are the active parents of this first transform : leaf to root order
    unsigned int    effectorWeightAndTransformListID; ///< offset in tableEffectorWeights where to find the list of weights and transformIDs
    unsigned int    numEffectors; ///< redundant with the first pair. But we had 32 bits available for this...
  };
  struct EffectorTransformAndWeight {
      unsigned int  transformID;
      float         weight;
  };
  /**
  \brief Modes for TransformDOF
   **/
  enum TransformDOFMode
  {
    DOF_CONE = 0,
    DOF_SINGLE_AXIS_X = 1,
    //DOF_SINGLE_AXIS_Y = 2, Not necessary: X is enough
    //DOF_SINGLE_AXIS_Z = 3,
    DOF_TWIST_ALONG_BONE = 4,
    DOF_UNDEF = 0xFFFFFFFF
  };
  /**
  \brief Degree Of Freedom object : allows to restrict the freedom of a transformation

  The following comes from the DOF Locator plugin
  - "DOFAlpha", "dofa" :  angle limit. Used to dot product...
  - "SingleDOF", "sdof": boolean  to tell the DOF is just one axis along Z
  - "theColor", "tc"
  - "drawLast", "dL"
  - "OxAxisLimit", "oxlim" : boolean
  - "aOxLimitStart", "oxstart" : float
  - "aOxLimitRange", "oxrange" : float
  **/
  struct TransformDOF
  {
	TransformDOFMode mode;
	float	DOFAlpha;///< angle of the cone

	float	AxisLimitStart;///< start for the limitation along one axis
	float	AxisLimitRange;///< range for the limitation along one axis

    Vec4Type   quat;
    Vec4Type   abs_Quat;
	//float	theColor[3];///< "tc" : Color to display the DOF...
	//bool	drawLast;///< "dL" : if we want the DOF to be rendered after the 3D objects...
    TransformDOF() {init();}
    void init()
    {
        memset((void*)this, 0, sizeof(TransformDOF));
    }
    inline Vec4Type&     QuatAbs()                     { return abs_Quat; }
    inline Vec4Type&     Quat()                        { return quat; }
  };
  /// Pool of transformations.
  /// The transformations gather as well regular Bones, but also IKHandles, Physic Bodies and Physic constraints
  /// this done so that we have the whole tree structure in one array no matter the kind of transformations
  /// Now, most of the time transformation update for basic animation might only require to update
  /// regular transforms, while rigid bodies, constraints and IK handles can be skipped.
  /// taking the min of of offsetIKHandles, offsetRigidBodies and offsetConstraints would help.
  /// NOTE: this assumes that the builder ALWAYS added these 3 types AT THE END
  struct TransformPool
  {
	int       nBones            : 32;
	int       offsetIKHandles   : 32; ///< tell where is the first transformation that is an effector. tableIKHandleData must be accessed with (TransformID - offsetIKHandles)

    //int       offsetRigidBodies : 32; ///< tell where is the first rigid body
	//int       offsetConstraints : 32; ///< tell where is the first constraint

    // structure of arrays, pointing to tables that are the components of transforms
    // that way data are ready to be sent to the GPU if needed
    // some could be NULL if not needed anywhere (tableMayaTransformData, for example)
    Ptr64<MatrixType>   tableMatrixAbs;
    Ptr64<MatrixType>   tableMatrixInvBindpose;
    Ptr64<MatrixType>   tableMatrixAbsInvBindposeMatrix;   ///< Mabs * inverse(BindposeMatrix)
    Ptr64<BoneDataType> tableBoneData;                     ///< for Bone
    Ptr64<unsigned int> tableChildrenLists;                ///< [N, child0, child1, ..., N, child0, child1, ... ]
    //Ptr64<unsigned int> tableIKHandlesLists;               ///< [N, ikh0, ikh1, ..., N, ikh0, ikh1, ... ]
    Ptr64<MayaTransformData> tableMayaTransformData;            ///< Maya-style data for transformations
    Ptr64<TransformDOF> tableTransformDOF;
    /// \brief data for IKHandle transforms. NOTE: these transforms must always be at the end of the tables
    /// read this table with (TransformID - offsetIKHandles) index
    /// why put Handle transforms at the end of other transforms ? Because in most of the cases, these handles are NOT necessary
    /// for the model to be animated: transform handles are used when editing positions, essentially
    /// then, not every transformation needs HandleData, obviously. IKHandleData will add data for the handles only, starting offsetIKHandles
    Ptr64<IKHandleData> tableIKHandleData;
    /// [weights and transform ID] lists. [(N,0), (transfID, weight), (transfID, weight), ..., (N,0),...]
    /// Why done this way ? Because it will be convenient to send this whole table at once to the GPU for fast processing
    Ptr64<EffectorTransformAndWeight> tableEffectorWeights;
    // NOTE: we could add 2 more tables for bodies and constraints. But this is only interesting for GPU processing... later ?

    // Now, the table of transforms is not meant to be passed to the GPU
    // it is just a convenient way to access to the transform nodes containing a bit more, such as transform name etc.
	Ptr64<Bone> pBones[1];    						///< transforms referenced by pointers
  };

  struct TransformPool2
  {
	int       n : 32;
	Ptr64<Bone> p[1];    						///< transforms referenced by pointers
  };
  ///
  /// \brief Bone (NODE_BONE) : base of transformation system
  ///
  /// P2 = translate * Qrotation * P
  ///
  /// 
  struct Bone : public Node
  {
      unsigned int  ID: 32; // the id is the offset in the tables where to find data for the bone
      int             : 32;
      /// 2 User data or one user pointer
	  union {
	    void * userPtr;
        int userData[2];
      };
      PTR64(BoneDataType* pBoneData);
      Ptr64<TransformPool> parentPool; // the access to where its data are stored
      PTR64(FloatArrayPool *pFloatArrays);   ///< array of float coming from curves or anything else derived from FloatArray
      PTR64(Bone *pParent);        ///< pointer to the parent
      PTR64(TransformPool2 *pChildren);
      PTR64(TransformDOF   *pDOF);           ///< ptr to a DOF component if this transform should be limited in its movement...
      PTR64(IKHandlePool   *pIKHandles);     ///< Pool of IK Handles to which the tranform can be influenced by

      PTR64(MatrixType      *pMatrixAbs);
      PTR64(MatrixType      *pMatrixAbsInvBindposeMatrix);
      PTR64(MatrixType      *pMatrixInvBindpose);
// TODO : connection with animation curves, if existing
      //CurveData  *cvPos[3];
      // scale[3] etc...
      // ? Shall I set a Pool ?
      //PTR64(CurvePool *pCurves);
      Bone() {init();}
      void init()
      {
        memset((void*)this, 0, sizeof(Bone));
        nodeType = NODE_BONE;
	    version = RAWMESHVERSION;
      }
      // simple 'accessors'
#if 1
      // This version uses the fact that we still keep an easy CPU pointer reference
      // even though we could find back the pointers through IDs:
      // this helps to debug and shall be faster on CPU
      inline BoneDataType& BoneData()                    { return *pBoneData; }
      inline unsigned int& ValidComps()                  { return pBoneData->validComps; }
      inline bool          getDirty()                       { return pBoneData->bDirty ? true:false; }
      inline void          setDirty(bool b)                 { pBoneData->bDirty = b?1:0; }
      inline MatrixType&   Matrix()                      { return pBoneData->matrix; }
      inline MatrixType&   MatrixAbs()                   { return *pMatrixAbs; }
      inline MatrixType&   MatrixAbsInvBindposeMatrix()  { return *pMatrixAbsInvBindposeMatrix; }
      inline MatrixType&   MatrixInvBindpose()           { return *pMatrixInvBindpose; }
      inline Vec3Type&     PosAbs()                      { return (Vec3Type&)(*pMatrixAbs->pos()); }
      inline Vec3Type&     Pos()                         { return (Vec3Type&)(*pBoneData->matrix.pos()); }
      inline Vec3Type&     PosBoneTail()                 { return pBoneData->posBoneTail; }
      inline Vec4Type&     QuatAbs()                     { return pBoneData->quatAbs; }
      inline Vec4Type&     Quat()                        { return pBoneData->quat; }
      inline Bone*         getParent()                      { return pParent; }
      inline int           getNumChildren()                 { return pChildren ? pChildren->n : 0; }
      inline Bone*         getChild(int n)                  { DBGASSERT(n<pChildren->n); return pChildren->p[n]; }
      inline TransformDOF* getTransformDOF()                { return pDOF; }
#else
      // these accessors do use directly the IDs and fetch pointers according to the logic of ID references
      // might be more expensive and more complicated for debugging purpose
      inline BoneDataType& BoneData()                    { return parentPool->tableBoneData.p[ID]; }
      inline unsigned int& ValidComps()                  { return BoneData().validComps; }
      inline bool          getDirty()                       { return BoneData().bDirty; }
      inline void          setDirty(bool b)                 { BoneData().bDirty = b; }
      inline MatrixType&   Matrix()                      { return BoneData().matrix; }
      inline MatrixType&   MatrixAbs()                   { return parentPool->tableMatrixAbs.p[ID]; }
      inline MatrixType&   MatrixAbsInvBindposeMatrix()  { return parentPool->tableMatrixAbsInvBindposeMatrix[ID]; }
      inline MatrixType&   MatrixInvBindpose()           { return parentPool->tableMatrixInvBindpose[ID]; }
      inline Vec3Type&     PosAbs()                      { return (Vec3Type&)(*parentPool->tableMatrixAbs[ID].pos()); }
      inline Vec3Type&     Pos()                         { return (Vec3Type&)(*parentPool->tableBoneData[ID].matrix.pos()); }
      inline Vec3Type&     PosBoneTail()                 { return parentPool->tableBoneData[ID].posBoneTail; }
      inline Vec4Type&     QuatAbs()                     { return parentPool->tableBoneData[ID].quatAbs; }
      inline Vec4Type&     Quat()                        { return parentPool->tableBoneData[ID].quat; }

      inline Bone*         getParent()                      { int pID = BoneData().parentID; return pID == 0xFFFF ? NULL : (parentPool->pBones + pID)->p; }
      inline int           getNumChildren()                 { return parentPool->tableChildrenLists[BoneData().childrenListID]; }
      inline Bone*         getChild(int n)                  { return (parentPool->pBones + (parentPool->tableChildrenLists[BoneData().childrenListID + n +1]))->p; }
      inline TransformDOF* getTransformDOF()                { int ID = BoneData().DOFID; return ID == 0xFFFF ? NULL : parentPool->tableTransformDOF.p + ID; }
#endif
      inline bk3d::Transform*       asTransf()       { DBGASSERT(nodeType==NODE_TRANSFORM) return (bk3d::Transform*)(this); }
      inline bk3d::TransformSimple* asTransfSimple() { DBGASSERT((nodeType==NODE_TRANSFORM)||(nodeType==NODE_TRANSFORMSIMPLE)) return (bk3d::TransformSimple*)(this); }
      inline bk3d::IKHandle*        asIKHandle()     { 
          DBGASSERT((nodeType==NODE_IKHANDLE)||(nodeType==NODE_IKHANDLEROLLINFLUENCE)||(nodeType==NODE_IKHANDLEROTATEINFLUENCE)) return (bk3d::IKHandle*)(this); }
  };
  ///
  /// \brief TransformSimple (NODE_TRANSFORMSIMPLE) : a simplified transformation system
  ///
  /// P2 = translate * Qrotation * scale * P
  ///
  /// 
  struct TransformSimple : public Bone
  {
      Vec3Type  scale;
      Vec3Type  scaleAbs;
      TransformSimple() {init();}
      void init()
      {
        memset((void*)this, 0, sizeof(TransformSimple));
        nodeType = NODE_TRANSFORMSIMPLE;
	    version = RAWMESHVERSION;
      }
      inline Vec3Type&  Scale() { return scale; }
      inline Vec3Type&  ScaleAbs() { return scaleAbs; }
#ifdef _DEBUG
      inline bk3d::Transform* asTransf() { DBGASSERT(nodeType==NODE_TRANSFORM) return (bk3d::Transform*)(this); }
#else
#endif
  };
  ///
  /// \brief Transform (NODE_TRANSFORM) : a Maya-centric transformation system
  ///
  /// Joint transformation order (OpenGL order) :
  ///
  /// P2 = Mtranslate * MjointOrient * Mrotation * Mrotorientation * Mscale * P
  ///
  /// basic Transformation order :
  ///
  /// P2 = MrotPivotTransl * MrotPivot * Mrotation * MrotOrient * MrotPivotInv * MscalePivotTransl 
  ///      * MscalePivot * Mscale * MscalePivotInv * P
  /// 
  struct Transform : public TransformSimple
  {
      //PTR64(MayaTransformData *pMayaTransformData); // TODO
      Transform() {init();}
      void init()
      {
        memset((void*)this, 0, sizeof(Transform));
        nodeType = NODE_TRANSFORM;
	      version = RAWMESHVERSION;
        //validComps = TRANSFCOMP_pos|TRANSFCOMP_scale|TRANSFCOMP_rotation|TRANSFCOMP_rotationOrder|TRANSFCOMP_scalePivot|TRANSFCOMP_scalePivotTranslate|TRANSFCOMP_rotationPivot|TRANSFCOMP_rotationPivotTranslate|TRANSFCOMP_rotationOrientation|TRANSFCOMP_jointOrientation|TRANSFCOMP_bindPose
      }
      inline MayaTransformData& TransformData()     { return parentPool->tableMayaTransformData.p[ID]; }
      inline Vec3Type&  Rotation()                  { return TransformData().rotation; }
      inline char*      RotationOrder()             { return TransformData().rotationOrder; }
      inline Vec3Type&  ScalePivot()                { return TransformData().scalePivot; }
      inline Vec3Type&  ScalePivotTranslate()       { return TransformData().scalePivotTranslate; }
      inline Vec3Type&  RotationPivot()             { return TransformData().rotationPivot; }
      inline Vec3Type&  RotationPivotTranslate()    { return TransformData().rotationPivotTranslate; }
      inline Vec4Type&  RotationOrientation()       { return TransformData().rotationOrientation; }
      inline Vec4Type&  JointOrientation()          { return TransformData().jointOrientation; }
  };

  /// \brief pool of transformations
  ///
  /// Here we get 3 choice :
  /// -# normal mesh will have a 1 Transformation reference. Or even none
  /// -# Skinned meshes will get N transforms for Bones. Skinning indices in Vertex attribs will refer to transforms in the array here.
  /// -# Simple instancing : N transforms for N instances of this same Mesh
  struct TransformRefs// : public Node
  {
	int				 n	: 32;        
	int					: 32;
	Ptr64<Bone> p[1]; 		///< first we get offsets here. Load will resolve as correct ptrs
  };

///
/// pool of Inverse Kinematic Handles
///
struct IKHandlePool
{
	int       n;
	int		  : 32;
	Ptr64<IKHandle> p[1];
};
///
/// \brief Handle for IK
///
/// This handle can be attached to a transform parent: handleTransform
/// 
/// The IK process : try to make this handle reach a destPos
/// \remark Maya defines IK with 2 special parts : Effector and Handle
/// Both can have their own transforms with all the related properties : Pivots, rotations, pos etc.
/// The Handle class is a way to simplify things:
/// - the transformation inherits from 'Bone'
/// - the End-effector that is supposed to reach the handle is the posBoneTail of the first pEffectorTransforms
/// There are 3 ways of using Handles:
/// - common way (NODE_IKHANDLE): the Handle position is the target to reach
/// - Roll (NODE_IKHANDLEROLLINFLUENCE): the handle is used to spread the rotation of handleTransform to pEffectorTransforms around bone tails
///   for example is can be usefule for wrist or arm layers for skinning
/// - rotation (NODE_IKHANDLEROTATEINFLUENCE): for the case where we want some transforms to copy values from the Handle's transform (handleTransform)
///   It can be useful for eyes control etc
/// I agree... the last 2 ones are NOT IK. But it was simpler to put them all in one
///
struct IKHandle : public Bone
{
    PTR64(IKHandleData      *pIKHandleData);
    PTR64(FloatPool         *pEffectorWeights); ///< weights for each effector transform
    PTR64(TransformPool2    *pEffectorTransforms); ///< attached transforms for IK
    // Removed: now IKHandle is the transform
    //PTR64(Transform         *handleTransform); ///< parent transform for the handle, if this one is part of the tree
    /// FloatArrays allow to connect to pos or target (animation...)
    PTR64(FloatArrayPool    *pFloatArrays);
	IKHandle() {init();}
	void init()
    {
      memset((void*)this, 0, sizeof(IKHandle));
      strcpy(name, "IKHANDLE");
      nodeType = NODE_IKHANDLE;
	  version = RAWMESHVERSION;
    }
#if 1
    inline IKHandleData&    IKHandleData() { return *pIKHandleData; }
    inline int&             Priority()     { return pIKHandleData->priority; }
    inline float&           Weight()       { return pIKHandleData->weight; }
    inline int&             MaxIter()       { return pIKHandleData->maxIter; }
    inline float&           EffectorWeight(int n)    { return pEffectorWeights->f[n]; }
    inline int              getNumEffectors() { return pEffectorTransforms ? pEffectorTransforms->n : 0; }
    inline Bone*            getEffectorTransform(int n) { return pEffectorTransforms->p[n]; }
#else
    inline IKHandleData&    IKHandleData() { return parentPool->tableIKHandleData.p[ID - parentPool->offsetIKHandles]; }
    inline int&             Priority()     { return IKHandleData().priority; }
    inline float&           Weight()       { return IKHandleData().weight; }
    inline int&             MaxIter()      { return IKHandleData().maxIter; }
    inline float&           EffectorWeight(int n)    { return parentPool->tableEffectorWeights[IKHandleData().effectorWeightAndTransformListID+1+n].weight; }
    inline int              getNumEffectors() { return IKHandleData().numEffectors; }
    inline Bone*            getEffectorTransform(int n) { return parentPool->pBones[parentPool->tableEffectorWeights[IKHandleData().effectorWeightAndTransformListID+1+n].transformID]; }
#endif
};

///
/// \brief single Material attribute for arbitrary attributes.
///
/// A material can contain additional attributes on top of the known attributes.
/// Example : Maya allows you to dedine custome properties in a material...
///
struct MaterialAttr
{
	PTR64(const char*	name);
	union {
		float*	pFloat;
		int*	pInt;
		unsigned int* pUInt;
		void *	p;
		struct { int : 32; int : 32; };
	};
	DXGI_FORMAT	type	: 32; ///< DXGI_FORMAT is rather good to have a nice list of possible things
	int					: 32;
};

// this structure is for the table of materials: a big table we can send to the GPU
// NOTE: later I should add some bindless texture information
struct MaterialData
{
	Vec3Type    diffuse;         ///< RGB diffuse color
	float	    specexp;            ///< exponent of the specular lighting

	Vec3Type    ambient;         ///< RGB ambient color
	float	    reflectivity;       ///< intensity of the reflection

	Vec3Type    transparency;    ///< RGB transparency
    float	translucency;       ///< translucency : 0, no transparency at all; 1 : object 100% transparent (transparency could be used)

	Vec3Type    specular;        ///< specular RGB color
	int		: 32;

    // TODO: Add Bindless Texture Handles for the GPU !
	//TextureAddr	diffuseTexture;
	//TextureAddr	specExpTexture;
	//TextureAddr	ambientTexture;
	//TextureAddr	reflectivityTexture;
	//TextureAddr	transparencyTexture;
	//TextureAddr	translucencyTexture;
	//TextureAddr	specularTexture;

    MaterialData() {init();}
	void init()
    {
      memset((void*)this, 0, sizeof(MaterialData));
    }
};
///
/// \brief Pool of materials
///
struct MaterialPool //: public Node
{
	int			nMaterials;
	int			: 32;
    Ptr64<MaterialData> tableMaterialData; // fetch data here with ID of material. This table can easily be sent to the GPU
	Ptr64<Material>	pMaterials[1];
};
///
/// \brief Material (NODE_MATERIAL) containing usual properties
///
struct Material : public Node
{
    unsigned int ID; // use it for tableMaterialData[0][ID]
    int          : 32;
    Ptr64<MaterialPool> parentPool; // the access to where its data are stored
    PTR64(MaterialData* pMaterialData);
	/// \name Shader information : in Maya, this would come from Custom parameters "shader" and "technique"
	/// @{
	PTR64(char*	shaderName);    ///< shader name if exists (most of DCC apps don't have this but we can customize it in Maya as a custom property)
	PTR64(char*	techniqueName); ///< technique name if the shader is an effect.
	/// @}
	/// \brief Texture information
	struct Texture
	{
		PTR64(char*	name);       ///< name of the texture
		PTR64(char*	filename);   ///< filename of the texture. Or whatever helps to find it in whathever storage
        union {
            struct {
		        unsigned LONG userHandle; ///< when the application found the texture, one may want to keep track of resource references with this
		        unsigned LONG userData; ///< when the application found the texture, one may want to keep track of resource references with this
            };
            char* userPtr; /// < or the same area as a pointer (32 or 64 bits)
        };
	};
    /// \name textures for various property fields. Optional... would override the RGB/float values if defined
    /// @{
	Texture	diffuseTexture;
	Texture	specExpTexture;
	Texture	ambientTexture;
	Texture	reflectivityTexture;
	Texture	transparencyTexture;
	Texture	translucencyTexture;
	Texture	specularTexture;
    /// @}
	/// can be used after load for storing handles... etc. (effect & technique, for example)
	unsigned LONG userData[2];
	/// Extra parameters : user-defined parameters. For example uniform values for the shader shaderName
	PTR64(MaterialAttrPool*	pAttrPool);
	/// Basic methods
    Material() {init();}
	void init()
    {
      memset((void*)this, 0, sizeof(Material));
	  nodeType = NODE_MATERIAL;
	  version = RAWMESHVERSION;
    }
    inline MaterialData&    MaterialData() { return *pMaterialData; }
    //inline MaterialData&    MaterialData() { return parentPool->tableMaterialData.p[ID]; }
    inline Vec3Type&        Diffuse() { return MaterialData().diffuse; }
    inline Vec3Type&        Ambient() { return MaterialData().ambient; }
    inline float&           Reflectivity() { return MaterialData().reflectivity; }
    inline Vec3Type&        Transparency() { return MaterialData().transparency; }
    inline float&           Translucency() { return MaterialData().translucency; }
    inline Vec3Type&        Specular() { return MaterialData().specular; }
    inline float&           SpecularExp() { return MaterialData().specexp; }
};

///
/// pool of Rigid bodies
///
struct RigidBodyPool
{
	int       n;
	int		  : 32;
	Ptr64<RigidBody> p[1];
};
///
/// pool of Constraints
///
struct ConstraintPool
{
	int       n;
	int		  : 32;
	Ptr64<Constraint> p[1];
};
///
/// \brief Rigid body for physic simulation
/// derives from Bone: rotation+translation and related matrix slots are then available
///
struct RigidBody : public Bone
{
    //char    name[20]; // in Node
    //unsigned short  bone_index; // in Bone: parent
    unsigned char   collision_group;
    unsigned char   shape_type; //0: sphere; 1: box; 2:capsule
    unsigned char   mode; // 0: no mass:static; 1: dynamic
    int             :8; // to align to 32 bits
    int             collision_group_mask; // 32 bits again: 64 with previous 32

    Vec3Type        shape_size; ///< width: for all; heigh: for sphere/capsules; depth: box shapes
    float           mass;
    //Vec3Type        shape_position; // in Bone
    //Vec3Type        shape_rotation;

    float           linear_dampening;
    float           angular_dampening;
    float           restitution;
    float           friction;

    /// Basic methods
    RigidBody() {init();}
	void init()
    {
      memset((void*)this, 0, sizeof(RigidBody));
	  nodeType = NODE_RIGIDBODY;
	  version = RAWMESHVERSION;
    }
};
///
/// \brief constraint for physic simulation
/// note that we don't setup data outside in a structure like Bone: most of these parameters will be
/// sent to a physics engine and aren't of any use directly by the GPU. Maybe later
///
struct Constraint : public Bone
{
    //char    name[20]; // in Node
    PTR64(Bone* pRigidBody1);        ///< first body
    PTR64(Bone* pRigidBody2);        ///< second body
    //Vec3Type    position; // in Bone
    //Vec3Type    rotation; // in Bone

    Vec3Type    translation_limit_min;
    Vec3Type    translation_limit_max;

    Vec3Type    rotation_limit_min;
    Vec3Type    rotation_limit_max;

    Vec3Type    spring_constant_translation;
    Vec3Type    spring_constant_rotation;

    /// Basic methods
    Constraint() {init();}
	void init()
    {
      memset((void*)this, 0, sizeof(Constraint));
	  nodeType = NODE_CONSTRAINT;
	  version = RAWMESHVERSION;
    }
};


//------------------------------------------------------------------------------------------
/// Helper to find some components.
/// The name MUST be in the form of name + '_' + component-name
//------------------------------------------------------------------------------------------
INLINE float* findComponentf(FileHeader *pH, const char *compname, unsigned char **pDirty)
{
    float *pComp = NULL;
    if(!compname)
        return NULL;
    char *name, *comp = NULL;
    int l = (int)strlen(compname)+1;
    name = new char[l];
#if     _MSC_VER > 1310
    strcpy_s(name, l, compname);
#else
    strcpy(name, compname);
#endif
    for(int i=l-1; i>=0; i--) if(name[i] == '_') 
    { 
        name[i] = '\0';
        comp = name + i + 1;
        break;
    }
    if(!comp) {
        delete [] name;
        return NULL;
    }
    //search in transforms
    for(int i=0; i<pH->pTransforms->nBones; i++)
    {
        Bone *pt = pH->pTransforms->pBones[i];
        if(strcmp(pt->name, name))
            continue;
        if(pDirty) *pDirty = &(pt->BoneData().bDirty);
        if(!strcmp(comp, "translate")) {        
            pComp = pt->Pos();
        } else if(!strcmp(comp, "scale")) {     
            pComp = ((Transform*)pt)->Scale();
        } else if(!strcmp(comp, "rotation")) {  
            pComp = ((Transform*)pt)->TransformData().rotation;
        } else if((strcmp(comp, "quat"))) {  
            pComp = pt->BoneData().quat;
        } //some more to add...
        break;
    }
    //search in Mesh ? (TODO later)
    //...
    delete [] name;
    return pComp;
}

//------------------------------------------------------------------------------------------
/// Helper to find some components.
/// This one requires a name and a component value from TRANSFCOMP_xxx (see above)
//------------------------------------------------------------------------------------------
INLINE float* findComponentf(FileHeader *pH, const char *name, unsigned int component, unsigned char **pDirty, Bone** ppBone=NULL)
{
    float *pComp = NULL;
    //search in transforms
    for(int i=0; i<pH->pTransforms->nBones; i++)
    {
        Bone *pt = pH->pTransforms->pBones[i];
        if(strcmp(pt->name, name))
            continue;
        if(ppBone) *ppBone = pt;
        if(pDirty) *pDirty = &(pt->BoneData().bDirty);
        switch(component)
        {
          case TRANSFCOMP_pos:
            pComp = pt->Pos();
            break;
          case TRANSFCOMP_scale:
            pComp = ((Transform*)pt)->Scale();
            break;
          case TRANSFCOMP_rotation:
            pComp = ((Transform*)pt)->Rotation();
            break;
          case TRANSFCOMP_Quat:
            pComp = pt->Quat();
            break;
          case TRANSFCOMP_bindpose_matrix:
            pComp = pt->MatrixInvBindpose();
            break;
          case TRANSFCOMP_rotationOrder:
          case TRANSFCOMP_scalePivot:
          case TRANSFCOMP_scalePivotTranslate:
          case TRANSFCOMP_rotationPivot:
          case TRANSFCOMP_rotationPivotTranslate:
          case TRANSFCOMP_rotationOrientation:
          case TRANSFCOMP_jointOrientation:
            //assert(!"TODO");
            break;
        }
        break;
    }
    return pComp;
}

} //namespace bk3d

#endif //#ifndef __BK3DEX__


