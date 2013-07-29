#pragma once
//----------------------------------------------------------------------------------
// File:   nvrawmesh.h
// Author: Tristan Lorach
// Email:  lorachnroll@gmail.com
// 
// Copyright (c) 2013 Tristan Lorach. All rights reserved.
//
// TO  THE MAXIMUM  EXTENT PERMITTED  BY APPLICABLE  LAW, THIS SOFTWARE  IS PROVIDED
// *AS IS*  AND T.LORACH AND  ITS SUPPLIERS DISCLAIM  ALL WARRANTIES,  EITHER  EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED  TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL  T.LORACH OR ITS SUPPLIERS
// BE  LIABLE  FOR  ANY  SPECIAL,  INCIDENTAL,  INDIRECT,  OR  CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION,  DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE  USE OF OR INABILITY  TO USE THIS SOFTWARE, EVEN IF T.LORACH HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
//
/**
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
		strncpy(name, "FloatArray", 31);
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
struct Transform;
struct MaterialAttr;
struct TransformDOF;
struct IKHandlePool;
struct IKHandle;

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

/// pool of curves
struct MayaCurvePool //: public Node
{
	int				n; ///< amount of curves
	int				: 32;
	Ptr64<MayaCurveVector> p[1]; ///< array of n curves
};

/// Pool of transformations
struct TransformPool
{
	int       n : 32;
	int			: 32;
	Ptr64<Transform> p[1];    						///< transforms referenced by pointers
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
  #define TRANSFCOMP_rotationQuat           (1<<3)
  #define TRANSFCOMP_rotationOrder          (1<<4)
  #define TRANSFCOMP_scalePivot             (1<<5)
  #define TRANSFCOMP_scalePivotTranslate    (1<<6)
  #define TRANSFCOMP_rotationPivot          (1<<7)
  #define TRANSFCOMP_rotationPivotTranslate (1<<8)
  #define TRANSFCOMP_rotationOrientation    (1<<9)
  #define TRANSFCOMP_jointOrientation       (1<<10)
  #define TRANSFCOMP_bindPose               (1<<11)
  #define TRANSFCOMP_isBone                 (1<<12)
  #define TRANSFCOMP_matrix                 (1<<13) // tells the matrix is the origin of the transformation and is not calculated from other params (pos, scale)
  #define TRANSFCOMP_abs_matrix             (1<<14) // tells the matrix is the origin of the transformation and is not calculated from other params (pos, scale)
  #define TRANSFCOMP_bindpose_matrix        (1<<15)
  #define TRANSFCOMP_abs_rotationQuat       (1<<16) // 0x133
  #define TRANSFCOMP_abs_rotationQuat_ready (1<<16) // 0x133 : let's keep the same bit for now : abs Quat is never defined as the origin
  #define TRANSFCOMP_matrix_ready           (1<<17) // 0x133 : tells the matrix is ready but wasn't natively defined in the object
  #define TRANSFCOMP_abs_matrix_ready       (1<<18) // 0x133 : tells the abs_matrix is ready but wasn't natively defined in the object
  #define TRANSFCOMP_rotationQuat_ready     (1<<19) // 0x133 : tells the quaternion is ready but wasn't natively defined in the object
  #define TRANSFCOMP_invalidMatrix          (1<<31)

  #define TRANSFCOMP_MAYATRANSF (TRANSFCOMP_scale|TRANSFCOMP_pos|TRANSFCOMP_rotation|TRANSFCOMP_scalePivot|TRANSFCOMP_scalePivotTranslate\
                                |TRANSFCOMP_rotationPivot|TRANSFCOMP_rotationPivotTranslate|TRANSFCOMP_rotationOrientation)
  #define TRANSFCOMP_MAYAJOINT (TRANSFCOMP_scale|TRANSFCOMP_rotationOrientation|TRANSFCOMP_rotation|TRANSFCOMP_jointOrientation|TRANSFCOMP_pos\
                               |TRANSFCOMP_isBone/*|TRANSFCOMP_bindpose_matrix|TRANSFCOMP_bindPose*/)
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
  struct Transform : public Node
  {
      PTR64(FloatArrayPool *pFloatArrays);   ///< array of float coming from curves or anything else derived from FloatArray
      PTR64(Transform      *pParent);        ///< pointer to the parent
      PTR64(TransformPool  *pChildren);
      PTR64(TransformDOF   *pDOF);           ///< ptr to a DOF component if this transform should be limited in its movement...
      PTR64(IKHandlePool   *pIKHandles);     ///< Pool of IK Handles to which the tranform can be influenced by
      union {
        float     matrix4x4[4][4];           ///< resulting matrix ( TRANSFCOMP_matrix to 1 if a default one is available )
        float     matrix[16];                ///< resulting matrix ( TRANSFCOMP_matrix to 1 if a default one is available )
      };
      union { 
        float     abs_matrix4x4[4][4];       ///< resulting matrix ( TRANSFCOMP_abs_matrix to 1 if a default one is available )
        float     abs_matrix[16];            ///< resulting matrix ( TRANSFCOMP_abs_matrix to 1 if a default one is available )
      };
      union {   ///< initially contains the Inverse default world->transf so the vertex can be put in local-to-bone space.
        float     bindpose_matrix4x4[4][4];  ///< bindpos matrix ( TRANSFCOMP_bindpose_matrix to 1 if a default one is available )
        float     bindpose_matrix[16];     	 ///< bindpos matrix ( TRANSFCOMP_bindpose_matrix to 1 if a default one is available )
      };
      // from 0x133 : absolute transformation in Quaternion notation
      struct QTransf {
        float Q[4];
        float pos[3];
        float scale[3];
      };
      QTransf abs_QuatTransf; // TRANSFCOMP_abs_rotationQuat
      /// 2 User data or one user pointer
	  union {
	    void * userPtr;
        int userData[2];
      };
      //
      // typical Maya things
      //
      float     pos[3];					///< TRANSFCOMP_pos
      float     scale[3];				///< TRANSFCOMP_scale
      float     rotationQuat[4];       	///< Euler Rotation in degres (TRANSFCOMP_rotation)
      float     rotation[3];           	///< Euler Rotation in degres (TRANSFCOMP_rotation)
      char      rotationOrder[3];      	///< 3 chars for "xyz" or any other (TRANSFCOMP_rotationOrder)
      // Do we really need all of them ?
      float     scalePivot[3];			///< TRANSFCOMP_scalePivot
      float     scalePivotTranslate[3];	///< TRANSFCOMP_scalePivotTranslate
      float     rotationPivot[3];		///< TRANSFCOMP_rotationPivot
      float     rotationPivotTranslate[3];///< TRANSFCOMP_rotationPivotTranslate

      float     rotationOrientation[4]; ///< TRANSFCOMP_rotationOrientation, Quaternion
      float     jointOrientation[4];    ///< TRANSFCOMP_jointOrientation, Quaternion

      /// when bDirty is true, you should recompute the matrices (relative, absolute, skin). The consequence is that
      /// some children of the dirty transformations should either re-compute their absolute matrices
      /// when bDirty is false, all the matrices are ready to use.
      bool          bDirty;
	  /// TRANSFCOMP_ flags. These bits are important to know which field is valid.
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
      unsigned int  validComps;
      //
      // 
      //
// TODO : connection with animation curves, if existing
      //CurveData  *cvPos[3];
      // scale[3] etc...
      // ? Shall I set a Pool ?
      //PTR64(CurvePool *pCurves);
      //
      //

      Transform() {init();}
      void init()
      {
        memset((void*)this, 0, sizeof(Transform));
        nodeType = NODE_TRANSFORM;
	      version = RAWMESHVERSION;
        //validComps = TRANSFCOMP_pos|TRANSFCOMP_scale|TRANSFCOMP_rotation|TRANSFCOMP_rotationOrder|TRANSFCOMP_scalePivot|TRANSFCOMP_scalePivotTranslate|TRANSFCOMP_rotationPivot|TRANSFCOMP_rotationPivotTranslate|TRANSFCOMP_rotationOrientation|TRANSFCOMP_jointOrientation|TRANSFCOMP_bindPose
      }
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
	Ptr64<Transform> p[1]; 		///< first we get offsets here. Load will resolve as correct ptrs
  };

//--------------------------------
/**
\brief Degree Of Freedom object : allows to restrict the freedom of a transformation

A DOF Component inherits from bk3d::Transform, with additional information, so that we can orient the DOF.
Note that we could have made the DOF a separate component but it would have made things more complex.
The fact that it inherits from bk3d::Transform allows DOF to be part of the transform tree / list as a Transform very simply.

Its pointer will be referenced in a transformation: bk3d::Transform::pDof it needs to be applied.

The following comes from the DOF Locator plugin
- "DOFAlpha", "dofa" :  angle limit. Used to dot product...
- "SingleDOF", "sdof": boolean  to tell the DOF is just one axis along Z
- "theColor", "tc"
- "drawLast", "dL"
- "OxAxisLimit", "oxlim" : boolean
- "aOxLimitStart", "oxstart" : float
- "aOxLimitRange", "oxrange" : float
**/
struct TransformDOF : public Transform
{
	float	DOFAlpha;///< "dofa" :  angle limit. Used to dot product...
	/*bool*/char	SingleDOF;///< "sdof": boolean  to tell the DOF is just one axis along Z
	/*bool*/char	OxAxisLimit;///< "oxlim" : if we want a DOF along the Ox axis (Ox is the axis of the bone
    int : 16;

	float	OxLimitStart;///< "oxstart" : start for the limitation
	float	OxLimitRange;///< "oxrange" : range for the limitation
	//float	theColor[3];///< "tc" : Color to display the DOF...
	//bool	drawLast;///< "dL" : if we want the DOF to be rendered after the 3D objects...
    TransformDOF() {init();}
    void init()
    {
        memset((void*)this, 0, sizeof(TransformDOF));
        nodeType = NODE_DOF;
          version = RAWMESHVERSION;
    }
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
/// This handle is attached to a transform.
/// The IK process : try to make this handle reach a destPos
/// \remark Maya defines IK with 2 special Transform Objects : Effector and Handle
/// Both are Transform with all the related properties : Pivots, rotations, pos etc.
/// Here I decided to not export those details and to even merge the Effector into the Handle.
/// Why ? Because the IK work doesn't need more than the position of the end-effector
/// (relative to the end Transform in the IK chain) and the position of the handle (relative to targetTransform)...
/// This simplification won't be so valid for the day we use rotation/scaling as part of the IK solver...
/// If The Handle is part of the DAG tree, targetTransform will have the parent transform.
///
struct IKHandle : public Node
{
    int priority;
    float weight;

    float effectorPos[3]; ///< position relative to the first transform : handle position
    float handlePos[3]; ///< target position relative to the optional transform
    /// pool of transforms for pos : has all from pos's parent tranform to Transform's parents...
    /// used to resolve the IK : if the transform isn't here, it won't be used for IK
    /// the first in this list is the tranform to which we attached the handle (pos) : the end-effector's transform
    /// the others are the active parents of this first transform : leaf to root order.
    PTR64(TransformPool     *pEffectorTransforms); ///< attached transforms for IK
    PTR64(Transform         *handleTransform); ///< parent transform for the handle, if this one is part of the tree
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

///
/// \brief Material (NODE_MATERIAL) containing usual properties
///
struct Material : public Node
{
    /// \name basic material properties that almost all have
    /// @{
	float	diffuse[3];         ///< RGB diffuse color
	float	specexp;            ///< exponent of the specular lighting

	float	ambient[3];         ///< RGB ambient color
	float	reflectivity;       ///< intensity of the reflection

	float	transparency[3];    ///< RGB transparency
    float	translucency;       ///< translucency : 0, no transparency at all; 1 : object 100% transparent (transparency could be used)

	float	specular[3];        ///< specular RGB color
	int		: 32;
    /// @}
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
		unsigned long userHandle; ///< when the application found the texture, one may want to keep track of resource references with this
		unsigned long userData; ///< when the application found the texture, one may want to keep track of resource references with this
	};
    /// \name textures for various property fields. Optional... would override the RGB/float values if defined
    /// @{
	Texture	diffuseTexture;
	Texture	specExpTexture;
	Texture	ambientTexture;
	Texture	reflectivityTexture;
	Texture	transparencyTexture;
	Texture	transluencyTexture;
	Texture	specularTexture;
    /// @}
	/// can be used after load for storing handles... etc. (effect & technique, for example)
	unsigned long userData[2];
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
};
///
/// \brief Pool of materials
///
struct MaterialPool //: public Node
{
	int			n;
	int			: 32;
	Ptr64<Material>	p[1];
};

//------------------------------------------------------------------------------------------
/// Helper to find some components.
/// \todo : give more details
//------------------------------------------------------------------------------------------
INLINE float* findComponentf(FileHeader *pH, const char *compname, bool **pDirty)
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
    for(int i=0; i<pH->pTransforms->n; i++)
    {
        Transform *pt = pH->pTransforms->p[i];
        if(strcmp(pt->name, name))
            continue;
        if(pDirty) *pDirty = &(pt->bDirty);
        if(!strcmp(comp, "translate")) {        
            pComp = pt->pos;
        } else if(!strcmp(comp, "scale")) {     
            pComp = pt->scale;
        } else if(!strcmp(comp, "rotation")) {  
            pComp = pt->rotation;
        } //some more to add...
        break;
    }
    //search in Mesh ? (TODO later)
    //...
    delete [] name;
    return pComp;
}

} //namespace bk3d

#endif //#ifndef __BK3DEX__


