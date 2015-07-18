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
#ifdef WIN32
#include <windows.h> // for OutputDebugString
#endif
#include <fstream>
#include <assert.h>
#include "Fx.h"
#include "FxGL.h"
#pragma warning(disable:4996)
using namespace nvFX;

#ifdef USE_OLDPROGRAM
// Only using the regular uniform assignment
#define glPROGRAMUNIFORM(bSp, s, p, l, w) glUniform##s(l,w)
#define glPROGRAMUNIFORMV(bSp, s, p, l, c, w) glUniform##s##v(l,c,w)
#define glPROGRAMUNIFORMMATRIXV(bSp, s, p, l, m, b, w) glUniformMatrix##s##v(l, m, b, w);
#else
// We are changing the function call depending on separate shader mode On or Off
#define glPROGRAMUNIFORM(bSp, s, p, l, w) { if(bSp) glProgramUniform##s(p,l,w); else glUniform##s(l,w); }
#define glPROGRAMUNIFORMV(bSp, s, p, l, c, w) { if(bSp) glProgramUniform##s##v(p,l,c,w); else glUniform##s##v(l,c,w); }
#define glPROGRAMUNIFORMMATRIXV(bSp, s, p, l, m, b, w) { if(bSp) glProgramUniformMatrix##s##v(p, l, m, b, w); else glUniformMatrix##s##v(l, m, b, w); }
#endif


/*************************************************************************
 **
 ** UniformGLSL UniformGLSL UniformGLSL UniformGLSL UniformGLSL UniformGLSL
 ** UniformGLSL UniformGLSL UniformGLSL UniformGLSL UniformGLSL UniformGLSL
 **
 *************************************************************************/ 
UniformGLSL::UniformGLSL(const char* name, const char* groupname, const char* semantic) :
#ifdef USECUDA
    UniformCUDA(name, groupname, semantic)
#else
    Uniform(name, groupname, semantic)
#endif
{
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
UniformGLSL::~UniformGLSL()
{
    //Uniform::~Uniform(); we don't have to call this...
    if(m_data) 
    {
#ifndef OGLES2
        if(m_type == TSubroutineUniform)
            for(int i=0; i<16; i++)
                if(m_data->subroutineNames[i]) delete [] m_data->subroutineNames[i];
#endif
    }
}
/*************************************************************************
 **
 **  method invoked by update()
 **
 *************************************************************************/ 
void UniformGLSL::updateGLSL(ShadowedData *pData, STarget &t)
{
    NXPROFILEFUNCCOL2(m_name.c_str(), COLOR_YELLOW2, t.uniformLocation);
    if(t.pass)
    {
        // Here we take into account either the single-program case, or the separable shader case
        GLSLProgram *glslProgram;
        bool bSp;
        GLSLProgramPipeline* glslProgramPipeline = static_cast<GLSLProgramPipeline*>(t.pass->getProgramPipeline(t.passLayerId));
        if(glslProgramPipeline)
        {
            glslProgram = static_cast<GLSLProgram*>(glslProgramPipeline->getShaderProgram(t.shaderProgramNumber));
            bSp = true;
        } else {
            glslProgram = static_cast<GLSLProgram*>(t.pass->getProgram(t.passLayerId));
            bSp = false;
        }
        int prog = glslProgram->getProgram();
        if(!t.valid)
        {
            t.uniformLocation = glGetUniformLocation(prog, m_name.c_str());
            t.valid = t.uniformLocation < 0 ? false : true;
        }
        if(t.valid && pData) switch(m_type)
        {
            case Uniform::TBool:
                glPROGRAMUNIFORM(bSp, 1i, prog, t.uniformLocation, pData->bvals[0] ? 1 : 0);
                CHECKGLERRORS("Uniform::TBool");
                break;
            case Uniform::TFloat:
                glPROGRAMUNIFORMV(bSp, 1f, prog, t.uniformLocation, m_arraySz, pData->fvals);
                CHECKGLERRORS("Uniform::TFloat");
                break;
            case Uniform::TInt:
                glPROGRAMUNIFORMV(bSp, 1i, prog, t.uniformLocation, m_arraySz, pData->ivals);
                CHECKGLERRORS("Uniform::TInt");
                break;
            case Uniform::TInt2:
                glPROGRAMUNIFORMV(bSp, 2i, prog, t.uniformLocation, m_arraySz, pData->ivals);
                CHECKGLERRORS("Uniform::TInt2");
                break;
            case Uniform::TInt3:
                glPROGRAMUNIFORMV(bSp, 3i, prog, t.uniformLocation, m_arraySz, pData->ivals);
                CHECKGLERRORS("Uniform::TInt3");
                break;
            case Uniform::TInt4:
                glPROGRAMUNIFORMV(bSp, 4i, prog, t.uniformLocation, m_arraySz, pData->ivals);
                CHECKGLERRORS("Uniform::TInt4");
                break;
            //case Uniform::TUInt:
            //case Uniform::TBool:
            case Uniform::TVec2:
                glPROGRAMUNIFORMV(bSp, 2f, prog, t.uniformLocation, m_arraySz, pData->fvals);
                CHECKGLERRORS("Uniform::TVec2");
                break;
            case Uniform::TVec3:
                glPROGRAMUNIFORMV(bSp, 3f, prog, t.uniformLocation, m_arraySz, pData->fvals);
                CHECKGLERRORS("Uniform::TVec3");
                break;
            case Uniform::TVec4:
                glPROGRAMUNIFORMV(bSp, 4f, prog, t.uniformLocation, m_arraySz, pData->fvals);
                CHECKGLERRORS("Uniform::TVec4");
                break;
            case Uniform::TMat4:
                glPROGRAMUNIFORMMATRIXV(bSp, 4f, prog, t.uniformLocation, m_arraySz, GL_FALSE, pData->fvals);
                CHECKGLERRORS("Uniform::TMat4");
                break;
#ifndef OGLES2
#ifndef USE_OLDPROGRAM
#pragma MESSAGE("WARNING WARNING WARNING WARNING : subroutines and separable shader NOT tested. Need to clarify how to setup the values")
#endif
            case Uniform::TSubroutineUniform:
                {
                    // Here we update the table of connections of Sub-routine uniforms <> implementations
                    // m_idx is used as an offset
                    if(!glslProgram->bindSubRoutineToUniform(t.uniformLocation, t.shadertype, pData->subroutineNames, 16))
                    {
                        nvFX::printf("Warning: can't associate subroutine %s(...) to %s. Did you put subroutine(...) before it ?\n", m_data->subroutineNames[0], m_name.c_str());
                    }
                }
                CHECKGLERRORS("Uniform::TSubroutineUniform");
                break;
#endif
            case Uniform::TTexture:
            case IUniform::TTexture1D:
            case IUniform::TTexture2D:
            case IUniform::TTexture2DRect:
            case IUniform::TTexture3D:
            case IUniform::TTextureCube:
                {
                    if(pData->tex.textureUnit<0)
                        t.pass->validateUniformTextureUnit(this, true);
					//LOGI("UpdateGLSL: sampler %s in unit %d\n", m_name.c_str(), pData->tex.textureUnit);
                    glPROGRAMUNIFORM(bSp, 1i, prog, t.uniformLocation, pData->tex.textureUnit);
                    if(pData->tex.pRes)
                    {
                        if(pData->tex.pRes->getGLTextureID())
                        {
                            if(pData->tex.textureUnit >= 0)
                                glActiveTexture(GL_TEXTURE0 + pData->tex.textureUnit);
                            GLenum target = getGLTarget(pData->tex.pRes->getType());
                            GLuint tex = pData->tex.pRes->getGLTextureID();
                            if(pData->tex.access == ACCESS_UNDEFINED)
                                glBindTexture(target, tex);
#ifndef OGLES2
                            else {
                                GLenum access = getGLAccess(pData->tex.access);
                                GLenum format = ResourceFormatAsGL(pData->tex.pRes->getFormat());
                                glBindImageTexture(pData->tex.textureUnit, tex, 0/*pData->tex.level*/,
                                    pData->tex.bLayered ? GL_TRUE:GL_FALSE, pData->tex.layer, access, format);
                            }
#else
                            else CHECK_TRUE(false);
#endif
                            CHECKGLERRORS("glBindTexture");
                            //GLenum err = glGetError();
                            // If a sampler state is assigned, set-it up
                            // TODO: optimize it to avoid redundancies
                            if(pData->tex.pSState)
                                pData->tex.pSState->updateGLTexture(getGLTarget(pData->tex.pRes->getType()), pData->tex.pRes->getGLTextureID() );
                            // TODO: in this case, maybe we can just call the sampler-state setting at validation time ONLY !
                            else if(pData->tex.pRes->getSamplerState())
                                pData->tex.pRes->getSamplerState()->updateGLTexture(getGLTarget(pData->tex.pRes->getType()), pData->tex.pRes->getGLTextureID() );
                            if(pData->tex.textureUnit > 0)
                                glActiveTexture(GL_TEXTURE0);
                        } else {
                            LOGD("Warning: %s references a texture resource (%s) with OpenGL texture Id to 0\n", m_name.c_str(), pData->tex.pRes->getName());
                        }
                    } else {
                        if(pData->tex.textureUnit >= 0)
                            glActiveTexture(GL_TEXTURE0 + pData->tex.textureUnit);
                        if((pData->tex.textureID >= 0)&&(pData->tex.textureType != RESOURCE_UNKNOWN))
                        {
                            if(pData->tex.access == ACCESS_UNDEFINED)
                                glBindTexture(pData->tex.textureType, pData->tex.textureID);
    #ifndef OGLES2
                            else {
                                CHECK_TRUE_MSG(false,"TODO");
                                //GLenum access = getGLAccess(pData->tex.access);
                                //GLenum format = ???
                                //glBindImageTexture(pData->tex.textureUnit, pData->tex.textureID, 0/*pData->tex.level*/,
                                //    pData->tex.layered ? GL_TRUE:GL_FALSE, pData->tex.layer, access, format);
                            }
    #else
                            else CHECK_TRUE(false);
    #endif
                            // If a sampler state is assigned, set-it up
                            // TODO: optimize it to avoid redundancies
                            if(pData->tex.pSState)
                                pData->tex.pSState->updateGLTexture(pData->tex.textureType, pData->tex.textureID);
                        }
                    }
                    glActiveTexture(GL_TEXTURE0);
                    CHECKGLERRORS("Uniform::TTexture");
                }
                break;
            case Uniform::TUBO:
#ifndef OGLES2
                glBindBufferBase(GL_UNIFORM_BUFFER, t.uniformLocation, pData->ivals[0]);
                CHECKGLERRORS("Uniform::TUBO");
#else
                LOGE("Cannot use UBO in OpenGL ES2 !!");
#endif
                break;
            case Uniform::TUniform:
            default:
                //CHECK_TRUE(false);
                break;
        }
        t.dirty = false;
    }
}

/*************************************************************************
 **
 **  update a uniform for GLSL
 **
 *************************************************************************/ 
Uniform*    UniformGLSL::update2(ShadowedData* pData, Pass *pass, bool bCreateIfNeeded)
{
    if (NULL == pass)
      return this;
    int id;
    for(int i=0; (id=pass->getLayerId(i)) >= 0; i++)
        update(pData, pass, id, bCreateIfNeeded);
    return this;
}
/*************************************************************************
 **
 **  update a uniform for GLSL
 ** TODO: push this code to Uniform Class, instead...
 **
 *************************************************************************/ 
Uniform*    UniformGLSL::update(ShadowedData* pData, Pass *pass, int layerID, bool bCreateIfNeeded)
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_YELLOW, 10);
    if (NULL == pass)
      return this;
    if(layerID < 0)
        return update2(pData, pass, bCreateIfNeeded);
#ifdef USECUDA
    UniformCUDA::update(pData, pass, layerID, bCreateIfNeeded);
#endif
    bool bCreate = pass ? bCreateIfNeeded : false;
	if((m_activeTarget >= 0) 
		&& (m_targets[m_activeTarget].pass == pass)
		&& (m_targets[m_activeTarget].passLayerId == layerID))
	{
        STarget &t = m_targets[m_activeTarget];
        if(t.valid)
		{
			// Force the target to be updated if the update data came from something else than m_data
			if(m_data != pData)
				t.dirty = true;
			switch(t.ttype)
			{
			case TGLSL:
				updateGLSL(pData, t);
				break;
			case TCSTBUFFER:
				LOGI(">Warning : uniform update for Constan buffert target should not be done. use setXXX() instead and then call ICstBuffer::update()\n");
				break;
			}
			// Force AGAIN the target to be updated if the update data came from something else than m_data
			// this allows the original m_data to be updated later if needed
			if(m_data && (m_data != pData))
				t.dirty = true;
		}
		return this;
	}
	int tsz = (int)m_targets.size();
	for(int i=0; i<tsz; i++)
	{
		STarget &t = m_targets[i];
		if(!t.valid)
			continue;
		// Force the target to be updated if the update data came from something else than m_data
		if(m_data != pData)
			t.dirty = true;
		switch(t.ttype)
		{
		case TGLSL:
			if(pass)
			{ // In this case we only update matching pass and return
				if((t.pass == pass)&&(t.passLayerId == layerID))
				{
					updateGLSL(pData, t);
					m_activeTarget = i;
					bCreate = false;
					break;
				}
			} else {
				updateGLSL(pData, t); // if updating all, we need to bind programs in any case
				LOGI(">Warning : uniform update for many targets at the same time... normal ?\n");
			}
			break;
		case TCSTBUFFER:
			LOGI(">Warning : uniform update for Constan buffer target should not be done. use setXXX() instead and then call ICstBuffer::update()\n");
			break;
		}
		// Force AGAIN the target to be updated if the update data came from something else than m_data
		// this allows the original m_data to be updated later if needed
		if(m_data && (m_data != pData))
			t.dirty = true;
	}
    //
    // a pass ptr was specified and nothing found, we shall try to find the uniform in the pass program
    //
    if(bCreate)
    {
//#ifdef USECUDA
//        createCUDA(pData, pass, layerID);
//#endif
        STarget t;
        int p = 0;
        memset(&t, 0, sizeof(STarget));
        bool bSp;
        // Here we take into account either the single-program case, or the separable shader case
        GLSLProgram *glslProgram;
        GLSLProgramPipeline* glslProgramPipeline = static_cast<GLSLProgramPipeline*>(pass->getProgramPipeline(layerID));
        if(glslProgramPipeline)
        {
            glslProgram = static_cast<GLSLProgram*>(glslProgramPipeline->getShaderProgram(p++));
            bSp = true;
        } else {
            glslProgram = static_cast<GLSLProgram*>(pass->getProgram(layerID));
            if(glslProgram && glslProgram->getProgram() == 0)
                glslProgram = NULL;
            bSp = false;
        }

        while(glslProgram)
        {
#ifndef OGLES2
            if((m_type == TSubroutineUniform) && glGetSubroutineUniformLocation)
            {
                // C:\p4\sw\docs\gpu\drivers\OpenGL\specs\GL_ARB_shader_subroutine.txt
                // case of sub-routine : try to create one target for each shader type, if needed
                static const GLenum allShaderTyles[] = {GL_VERTEX_SHADER_ARB, GL_FRAGMENT_SHADER_ARB, GL_GEOMETRY_SHADER_ARB, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, 0}; //TODO: add more
                const GLenum *pShdType = allShaderTyles;
                while(*pShdType)
                {
                    t.uniformLocation = glGetSubroutineUniformLocation(glslProgram->getProgram(), *pShdType, m_name.c_str());
                    t.shadertype = *pShdType;
                    if(t.uniformLocation >= 0)
                    {
                        t.valid = t.uniformLocation < 0 ? false : true;
                        t.pass = pass;
                        t.ttype = TGLSL;
                        t.passLayerId = layerID;
                        t.shaderProgramNumber = p-1;
                        // NOTE: maybe we should use a std::map<pass,target> for targets...
                        int i = 0;
                        int tsz = (int)m_targets.size();
                        for(i=0; i<tsz; i++)
                            if((m_targets[i].pass == pass) && (m_targets[i].ttype == t.ttype) && (m_targets[i].passLayerId == t.passLayerId))
                            {
                                m_targets[i] = t;
                                break;
                            }
                        if(i == tsz)
                        {
                            m_targets.push_back(t);
                            // let's add this uniform and it's target number directly into the pass, to make it faster at pass execution
                            pass->addUniform(this, i);
                        }
                        updateGLSL(pData, t);
						m_activeTarget = i;
                    }
                    ++pShdType;
                }
            } 
            // Now test if we are dealing with a uniform Block for UBO
            else if(glGetUniformBlockIndex
                && ((t.uniformLocation = glGetUniformBlockIndex(glslProgram->getProgram(), m_name.c_str()))>=0))
            {
                t.shadertype = 0;
                // we must bind this block index to our own number... yeah... let's take the same
                glUniformBlockBinding(glslProgram->getProgram(), t.uniformLocation, t.uniformLocation);
                t.valid = t.uniformLocation < 0 ? false : true;
                t.pass = pass;
                t.ttype = TGLSL;
                t.passLayerId = layerID;
                t.shaderProgramNumber = p-1;
                // NOTE: maybe we should use a std::map<pass,target> for targets...
                int i = 0;
                int tsz = (int)m_targets.size();
                for(i=0; i<tsz; i++)
                    if((m_targets[i].pass == pass) 
                    && (m_targets[i].ttype == t.ttype) 
                    && (m_targets[i].passLayerId == t.passLayerId)
                    && (m_targets[i].shaderProgramNumber == t.shaderProgramNumber))
                    {
                        m_targets[i] = t;
                        break;
                    }
                if(i == tsz)
                {
                    m_targets.push_back(t);
                    // let's add this uniform and it's target number directly into the pass, to make it faster at pass execution
                    pass->addUniform(this, i);
                }
                updateGLSL(pData, t);
            } else
#endif
            {
                // regular case of uniforms :
                t.uniformLocation = glGetUniformLocation(glslProgram->getProgram(), m_name.c_str());
                t.shadertype = 0;
                if(t.uniformLocation >= 0)
                {
                    t.valid = t.uniformLocation < 0 ? false : true;
                    t.pass = pass;
                    t.ttype = TGLSL;
                    t.passLayerId = layerID;
                    t.shaderProgramNumber = p-1;
                    // NOTE: maybe we should use a std::map<pass,target> for targets...
                    int i = 0;
                    int tsz = (int)m_targets.size();
                    for(i=0; i<tsz; i++)
                        if((m_targets[i].pass == pass) 
                        && (m_targets[i].ttype == t.ttype) 
                        && (m_targets[i].passLayerId == t.passLayerId)
                        && (m_targets[i].shaderProgramNumber == t.shaderProgramNumber))
                        {
                            m_targets[i] = t;
                            break;
                        }
                    if(i == tsz)
                    {
                        m_targets.push_back(t);
                        // let's add this uniform and it's target number directly into the pass, to make it faster at pass execution
                        pass->addUniform(this, i);
                    }
                    updateGLSL(pData, t);
                } else {
                    t.valid = false;
                }
            }
            //
            // Move to the next program is available
            //
            if(glslProgramPipeline)
                glslProgram = static_cast<GLSLProgram*>(glslProgramPipeline->getShaderProgram(p++));
            else
                glslProgram = NULL;
        }//while(glslProgram)
#ifdef USE_CG
        else // or update uniform reference for Cg
            return updateCg(pData, pass);
#endif
    }//if(pass && bCreateIfNeeded)
    return this;
}
/*************************************************************************
 **
 **  
 **
 *************************************************************************/ 
Uniform*    UniformGLSL::updateForTarget(ShadowedData* pData, int target)
{
	STarget& t = m_targets[target];
	m_activeTarget = target;
    switch(t.ttype)
    {
#ifdef USECUDA
    case TCUDA:
        UniformCUDA::updateForTarget(pData, target);
        break;
#endif
    case TGLSL:
        updateGLSL(pData, t);
        break;
    case TCSTBUFFER:
#ifndef OGLES2
        if(t.dirty)
        {
            //assert(t.pBuffer);
#ifdef USEBUFFERSUBDATA
            //NXPROFILEFUNCCOL2("upd.BufferSubData", COLOR_RED2, 11);
            glBindBuffer(GL_UNIFORM_BUFFER, t.pBuffer->getGLBuffer());
            // we expect ONE target
            int &offset = m_targets[0].offsetBytes;
            int szBytes = 0;
            int pk = 0;
            int &sz = m_targets[0].size;
            CstBufferGL *pB = static_cast<CstBufferGL*>(t.pBuffer);
            static char pData[4*4*sizeof(float)*16]; // TODO: allocate what is needed
            if(sz == 0)
            {
                sz = pB->bufferSizeAndData(NULL, this, szBytes, pk);
                // correction of the offset (when at setup, we didn't know how to align better)
                if((offset%16)&&((offset%16) + sz > 16))
                    offset += 16 - (offset%16);
            }
            assert(sz <= 4*4*sizeof(float)*16);
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sz, pData);
            setDirty(false, NULL);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
#elif defined(USEMAPRANGE)
            //NXPROFILEFUNCCOL2("upd.BufferSubData", COLOR_RED2, 11);
            glBindBuffer(GL_UNIFORM_BUFFER, t.pBuffer->getGLBuffer());
            // we expect ONE target
            int &offset = m_targets[0].offsetBytes;
            int szBytes = 0;
            int pk = 0;
            int &sz = m_targets[0].size;
            CstBufferGL *pB = static_cast<CstBufferGL*>(t.pBuffer);
            if(sz == 0)
            {
                sz = pB->bufferSizeAndData(NULL, this, szBytes, pk);
                // correction of the offset (when at setup, we didn't know how to align better)
                if((offset%16)&&((offset%16) + sz > 16))
                    offset += 16 - (offset%16);
            }
            char *pData = (char*)glMapBufferRange(GL_UNIFORM_BUFFER, offset, sz, GL_MAP_UNSYNCHRONIZED_BIT|GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_RANGE_BIT);
            szBytes = pk = 0;
            pB->bufferSizeAndData(pData, this, szBytes, pk);
            glUnmapBuffer(GL_UNIFORM_BUFFER);
            setDirty(false, NULL);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
#else
            // special case of target being an intermediate constant buffer need shadowed data anyways
            if(!m_data)
                m_data = (ShadowedData*)malloc(pData->size); //no... change
            memcpy(m_data, pData, sizeof(ShadowedData));
            t.pBuffer->setDirty(true, static_cast<nvFX::Pass*>(t.pass));
            // this case is not ideal. So let's tell it
            static bool msgOnceCstBuffer = true;
            if(msgOnceCstBuffer)
            {
                DebugBreak();
                LOGI(">Warning : update on a uniform that belongs to a Constant buffer is not so efficient (many map/unmap could occur)\n");
                msgOnceCstBuffer = false;
            }
            t.pBuffer->updateFromUniforms(true);
#endif
            CHECKGLERRORS("UniformGLSL::updateForTarget");
        }
#else
        LOGE("OGL ES2 doesn't have constant buffers. Should never happen\n");
        CHECK_TRUE(false);
        return this;
#endif
        break;
    }
    return this;
}
