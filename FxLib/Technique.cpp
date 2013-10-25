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

/***************************************************************************/
/* Technique Technique Technique Technique Technique Technique Technique   */
/***************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
Technique::Technique(Container *pCont, const char *name)
{
    if(name)
        m_name = std::string(name);
    m_container = pCont;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
Technique::~Technique()
{
    PassVec::iterator iP = m_passes.begin();
    while(iP != m_passes.end())
    {
        if((iP->pOwner == this) && iP->pass)
            delete iP->pass;
        ++iP;
    }
    m_passes.clear();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPass*   Technique::addPass(const char *name)
{
    Pass *pPass = new Pass(m_container, this, name);
    m_passes.push_back(PassTable(pPass, this, true));
    return pPass;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool    Technique::destroy(IPass *p)
{
    PassVec::iterator iP = m_passes.begin();
    while(iP != m_passes.end())
    {
        if(iP->pass == p)
        {
            if(iP->pOwner == this)
            {
                delete p;
                p = NULL;
            }
            m_passes.erase(iP);
            return true;
        }
        ++iP;
    }
    return false;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool    Technique::invalidate()
{
    LOGD("Invalidating Technique %s\n", m_name.c_str());
    bool bRes = true;
    for(int i=0; i<(int)m_passes.size(); i++)
    {
        PassTable &pt = m_passes[i];
        if((pt.pOwner == this)&&(!pt.pass->invalidate()))
            bRes = false;
    }
    return bRes;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool    Technique::validate()
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE, 1);
    bool bRes = true;
    bool bBindAttr = false;
    for(int i=0; i<(int)m_passes.size(); i++)
    {
        PassTable &pt = m_passes[i];
        if(!pt.pass->isValidated())
        {
            bBindAttr = true;
            if(!pt.pass->validate())
                bRes = false;
            else
                pt.pass->bindAttributes();
        }
    }
    if(bBindAttr)
        bindAttributes();
    return bRes;
}
bool    Technique::validateResources()
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE, 1);
    bool bRes = true;
    for(int i=0; i<(int)m_passes.size(); i++)
    {
        PassTable &pt = m_passes[i];
        if(pt.pass->isValidated())
        {
            if(!pt.pass->validateResources())
                bRes = false;
        }
    }
    return bRes;
}
bool    Technique::invalidateResources()
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE, 1);
    bool bRes = true;
    for(int i=0; i<(int)m_passes.size(); i++)
    {
        PassTable &pt = m_passes[i];
        if(pt.pass->isValidated())
        {
            if(!pt.pass->invalidateResources())
                bRes = false;
        }
    }
    return bRes;
}

bool            Technique::isPassActive(IPass* p)
{ 
    for(int i=0; i<(int)m_passes.size();i++)
        if(m_passes[i].pass == p)return m_passes[i].bActive;
    return false;
}
bool            Technique::setPassActive(IPass* p, bool bYes)
{
    for(int i=0; i<(int)m_passes.size();i++)
        if(m_passes[i].pass == p){ m_passes[i].bActive = bYes; return true; }
    return false;
};
IPass* Technique::getPass(int i, PassInfo *pPassInfo)
{
    if(i<(int)m_passes.size())
    {
        PassTable &pt = m_passes[i];
        Pass* p = pt.pass;
        if(pPassInfo)
        {
            pPassInfo->bActive = pt.bActive;
            pPassInfo->loopCnt = p->m_loopCnt;
        }
        return p;
    }
    return NULL;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool            Technique::bindAttribute(const char * name, int unit)
{
    // TODO: check if valid ? if the unit was already used ?
    // later we want to also raise warnings if the names aren't in the shader...
    m_attributes[unit] = std::string(name);
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool            Technique::bindAttributes()
{
    if(m_attributes.size() == 0)
        return true;
    // TODO: check if valid ? if the unit was already used ?
    // later we want to also raise warnings if the names aren't in the shader...
    PassVec::iterator ip = m_passes.begin();
    while(ip != m_passes.end())
    {
        Pass* pPass = ip->pass;
        pPass->bindAttributes(m_attributes);
        ++ip;
    }
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void            Technique::clearAttributeBinding()
{
    m_attributes.clear();
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
const char * Technique::getAttributeNameForLocation(int loc)
{
    if(m_attributes.empty())
        return NULL;
    AttribMap::iterator it = m_attributes.find(loc);
    if(it != m_attributes.end() )
        return it->second.c_str();
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
int Technique::getNumBoundAttributes()
{
    return (int)m_attributes.size();
}

void Technique::setActiveProgramLayer(int id, bool createIfNA)
{
    PassVec::iterator ip = m_passes.begin();
    while(ip != m_passes.end())
    {
        Pass* pPass = ip->pass;
        pPass->setActiveProgramLayer(id, createIfNA);
        ++ip;
    }
}
void Technique::removeProgramLayer(int id)
{
    PassVec::iterator ip = m_passes.begin();
    while(ip != m_passes.end())
    {
        Pass* pPass = ip->pass;
        pPass->removeProgramLayer(id);
        ++ip;
    }
}
int  Technique::getActiveProgramLayer()
{
    int id = 0;
    PassVec::iterator ip = m_passes.begin();
    while(ip != m_passes.end())
    {
        Pass* pPass = ip->pass;
        int id2 = pPass->getActiveProgramLayer();
        if(id2 > id)
            id = id2;
        ++ip;
    }
    return id;
}

bool            Technique::addSubTechnique(ITechnique *pTech, bool bActive)
{
    if(pTech == this)
        return false;
    Technique *pT = static_cast<Technique*>(pTech);
    PassVec::iterator ip = pT->m_passes.begin();
    while(ip != pT->m_passes.end())
    {
        m_passes.push_back(PassTable(ip->pass, ip->pOwner, bActive));
        ++ip;
    }
    return true;
}
void            Technique::setSubTechniqueActive(ITechnique *pTech, bool bActive)
{
    PassVec::iterator ip = m_passes.begin();
    while(ip != m_passes.end())
    {
        if(ip->pOwner == pTech)
            ip->bActive = bActive;
        ++ip;
    }
}
bool            Technique::getSubTechniqueActive(ITechnique *pTech)
{
    PassVec::iterator ip = m_passes.begin();
    while(ip != m_passes.end())
    {
        if(ip->pOwner == pTech)
            return ip->bActive;
        ++ip;
    }
    return false;
}


