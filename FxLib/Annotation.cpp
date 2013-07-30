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
#include <windows.h> // for OutputDebugString()
#endif
#include <stdarg.h> // Container::eprintf()

#include <fstream>
#include <assert.h>
#include "Fx.h"
#pragma warning(disable:4996)
using namespace nvFX;

#ifdef ANDROID
#   pragma message("Building for Android")
#endif

/***************************************************************************/
/** Annotation Annotation Annotation Annotation Annotation Annotation     **/
/***************************************************************************/
static Annotation g_annotSingleton[4];

IAnnotationEx* IAnnotationEx::getAnnotationSingleton(int n)
{
    if(n>=4)
        return false;
    return g_annotSingleton + n;
}

bool         Annotation::copy(IAnnotation* pIAnnotations)
{
    if(pIAnnotations == NULL)
        return false;
    Annotation* pAnnot = static_cast<Annotation*>(pIAnnotations);
    StringMap::iterator iS = pAnnot->m_strings.begin();
    while(iS != pAnnot->m_strings.end())
    {
        m_strings[iS->first] = iS->second;
        ++iS;
    }
    FloatMap::iterator iF = pAnnot->m_floats.begin();
    while(iF != pAnnot->m_floats.end())
    {
        m_floats[iF->first] = iF->second;
        ++iF;
    }
    IntMap::iterator iI = pAnnot->m_ints.begin();
    while(iI != pAnnot->m_ints.end())
    {
        m_ints[iI->first] = iI->second;
        ++iI;
    }
    return true;
}
bool         Annotation::clear()
{
    m_strings.clear();
    m_floats.clear();
    m_ints.clear();
    return true;
}

bool         Annotation::addAnnotation(const char *annotName, const char * str)
{
    m_strings[std::string(annotName)] = std::string(str);
    return true;
}
bool         Annotation::addAnnotation(const char *annotName, float f)
{
    m_floats[std::string(annotName)] = f;
    return true;
}
bool         Annotation::addAnnotation(const char *annotName, int i)
{
    m_ints[std::string(annotName)] = i;
    return true;
}

bool         Annotation::delAnnotation(const char *annotName)
{
    bool found = false;
    std::map<std::string, std::string>::iterator it = m_strings.find(std::string(annotName));
    if(it != m_strings.end())
    {
        m_strings.erase(it);
        found = true;
    }
    std::map<std::string, float>::iterator it2 = m_floats.find(std::string(annotName));
    if(it2 != m_floats.end())
    {
        m_floats.erase(it2);
        found = true;
    }
    std::map<std::string, int>::iterator it3 = m_ints.find(std::string(annotName));
    if(it3 != m_ints.end())
    {
        m_ints.erase(it3);
        found = true;
    }
    return found;
}

const char * Annotation::getAnnotationString(const char *annotName)
{
    std::map<std::string, std::string>::iterator it = m_strings.find(std::string(annotName));
    if(it == m_strings.end())
        return NULL;
    return it->second.c_str();
}
float Annotation::getAnnotationFloat(const char *annotName)
{
    std::map<std::string, float>::iterator it = m_floats.find(std::string(annotName));
    if(it != m_floats.end())
        return it->second;
    std::map<std::string, int>::iterator it2 = m_ints.find(std::string(annotName));
    if(it2 != m_ints.end())
        return (float)it2->second;
    return 0.0f;
}
int Annotation::getAnnotationInt(const char *annotName)
{
    std::map<std::string, int>::iterator it = m_ints.find(std::string(annotName));
    if(it != m_ints.end())
        return it->second;
    std::map<std::string, float>::iterator it2 = m_floats.find(std::string(annotName));
    if(it2 != m_floats.end())
        return (int)it2->second;
    return 0;
}

