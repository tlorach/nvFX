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
#   define  LOGI(...)  { printf(__VA_ARGS__); printf("\n"); }
#   define  LOGE(...)  { printf(">>Error : "); printf(__VA_ARGS__); printf("\n"); }
#ifdef WIN32
#include <windows.h> // for OutputDebugString()
#endif
#include <stdarg.h> // Container::eprintf()

#include <fstream>
#include <assert.h>
#include "FX.h"
#include "FXD3D.h"
#pragma warning(disable:4996)
using namespace nvFX;

/*************************************************************************
 **
 ** Resource Resource Resource Resource Resource Resource Resource
 **
 *************************************************************************/ 
bool ResourceD3D::setD3DResourceView(void *p)
{
    if(m_ptr) // we can't assign an external resource view if it was create from ResourceD3D
        return false;
    else m_pTextureView = (ID3D10ShaderResourceView*)p;
    return true;
}
void* ResourceD3D::getD3DResourceView()
{
    return m_pTextureView;
}

ResourceD3D::ResourceD3D(ResourceRepository* pCont, const char *name) : Resource(pCont, name)
{
    m_pTexture2D        = NULL;
    m_pTextureView      = NULL;
    m_pTextureRTView    = NULL;
    m_d3dFmt            = DXGI_FORMAT_UNKNOWN;
}
ResourceD3D::~ResourceD3D()
{
}
/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool ResourceD3D::createRenderResource()
{
    ID3D10Device* pd3dDevice = (ID3D10Device*)m_pOwner->getDevice();
    if(m_creationData.appDepSz)
    {
        const int* vp = m_pOwner->getViewport();
        m_creationData.appDepSz = true;
        m_creationData.sz[0] = vp[2];
        m_creationData.sz[1] = vp[3];
    }
    if(m_creationData.szFact[0] == 0.0f)
        m_creationData.szFact[0] = 1.0f;
    if(m_creationData.szFact[1] == 0.0f)
        m_creationData.szFact[1] = 1.0f;

    // Shall we store the real size of the resource in the class ?
    int width = (int)((float)m_creationData.sz[0] * m_creationData.szFact[0]);
    int height = (int)((float)m_creationData.sz[1] * m_creationData.szFact[1]);

    UINT bindFlags;
    switch(m_creationData.fmt)
    {
#pragma MESSAGE("TODO: all the possible formats in D3D")
    default:
    //case INTENSITY:
    //case FMT_INTENSITY8:        m_d3dFmt = ; break;
    //case FMT_INTENSITY16F:      m_d3dFmt = ; break;
    case FMT_INTENSITY32F:
        m_d3dFmt = DXGI_FORMAT_R32_FLOAT;
        bindFlags = D3D10_BIND_SHADER_RESOURCE|D3D10_BIND_RENDER_TARGET;
        break;

    //case FMT_LUMINANCE_ALPHA:
    //case FMT_LA:
    //case FMT_LA8:
    //case FMT_LUMINANCE_ALPHA8:  m_d3dFmt = ;  break;
    //case FMT_RG16F_NV:          m_d3dFmt = ;      break;
    //case FMT_LA16F:
    //case FMT_LUMINANCE_ALPHA16F:m_d3dFmt = ; break;
    //case FMT_LA32F:
    //case FMT_LUMINANCE_ALPHA32F:m_d3dFmt = ; break;
    //case FMT_RG32F_NV:          m_d3dFmt = ;      break;

    //case FMT_RGB:
    //case FMT_RGB8:              m_d3dFmt = ;               break;
    //case FMT_RGB16F:            m_d3dFmt = ;         break;
    //case FMT_RGB32F:            m_d3dFmt = ;         break;

    //case FMT_RGBA:
    case FMT_RGBA8:             
        m_d3dFmt = DXGI_FORMAT_R8G8B8A8_UNORM;
        bindFlags = D3D10_BIND_SHADER_RESOURCE|D3D10_BIND_RENDER_TARGET;
        break;
    //case FMT_RGBA8UI:           m_d3dFmt = ;        break;
    //case FMT_RGBA16F:           m_d3dFmt = ;        break;
    //case FMT_RGBA32F:           m_d3dFmt = ;        break;

    case FMT_DEPTH24STENCIL8:   
        m_d3dFmt = DXGI_FORMAT_D24_UNORM_S8_UINT;
        bindFlags = /*D3D10_BIND_SHADER_RESOURCE|*/D3D10_BIND_DEPTH_STENCIL;
        break;
    //case FMT_DEPTH_COMPONENT32F:m_d3dFmt = 
    case FMT_DEPTH32F_STENCIL8: 
        m_d3dFmt = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        bindFlags = /*D3D10_BIND_SHADER_RESOURCE|*/D3D10_BIND_DEPTH_STENCIL;
        break;
    }
    switch(m_type)
    {
    case RESTEX_1D:
        assert(1); // TODO
        break;
    //
    // Render buffer
    //
    case RESRBUF_2D:
        // This is the same as a texture in D3D... no renderbuffer
    case RESTEX_2D:
        // we have a relation with a texture supposed to be target for a 'resolve'
        // if we aren't in MSAA, let's use it instead and let's avoid allocating the renderbuffer
        // this will save use some useless intermediate step and memory
        if((m_creationData.resolveTarget))
        {
            if((m_creationData.resolveTarget->getWidth() != width)
             ||(m_creationData.resolveTarget->getHeight() != height))
            {
                LOGI("Warning>> we can't use ResolveTarget = %s for %s : sizes don't match\n", m_creationData.resolveTarget->getName(), m_name.c_str());
                m_creationData.resolveTarget = NULL;
            }
            //to remove else m_creationData.resolveTarget->addRef();
        }

        if((m_creationData.resolveTarget)&&(m_creationData.msaa[0] < 2))
        {
            LOGI("using RT %s instead of %s (no alloc on rb id %d)\n", m_creationData.resolveTarget->getName(), m_name.c_str(), m_OGLId);
            m_pTexture2D = static_cast<ResourceD3D*>(m_creationData.resolveTarget)->m_pTexture2D;
            m_pTextureView = static_cast<ResourceD3D*>(m_creationData.resolveTarget)->m_pTextureView;
            m_pTexture2D->AddRef();
            m_pTextureView->AddRef();
            m_type = RESTEX_2D;
        }
        else
        {
            D3D10_TEXTURE2D_DESC    rtTexDesc;
            rtTexDesc.Width         = width;
            rtTexDesc.Height        = height;
            rtTexDesc.MipLevels     = 1;
            rtTexDesc.ArraySize     = 1;
            rtTexDesc.Format        = m_d3dFmt;
            rtTexDesc.SampleDesc.Count = m_creationData.msaa[0] > 0 ? m_creationData.msaa[0] : 1;
            rtTexDesc.SampleDesc.Quality = m_creationData.msaa[1];
            rtTexDesc.Usage         = D3D10_USAGE_DEFAULT;
            rtTexDesc.BindFlags     = bindFlags;
            rtTexDesc.CPUAccessFlags = 0;
            rtTexDesc.MiscFlags     = 0;
            if( FAILED( pd3dDevice->CreateTexture2D(&rtTexDesc, NULL, &m_pTexture2D) ) )
            {
                LOGE("CreateTexture2D() failed.");
                return false;
            }
            //
            // Create the resource View now since we know this resource is meant to be used as a texture in shaders for most of the cases
            //
            if(bindFlags & D3D10_BIND_SHADER_RESOURCE)
            {
                D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
                viewDesc.Format = m_d3dFmt;
                viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
                viewDesc.Texture2D.MostDetailedMip = 0;
                viewDesc.Texture2D.MipLevels = 1;
                //SAFE_RELEASE(g_pRTViewDepth);
                if( FAILED( pd3dDevice->CreateShaderResourceView(m_pTexture2D, &viewDesc, &m_pTextureView) ) )
                {
                    LOGE("CreateRenderTargetView() failed.");
                    return false;
                }
            }
        }
        break;
    /*case RESTEX_3D:
        assert(1); // TODO
        break;
      case RESTEX_CUBE:
        assert(1); // TODO
        break;
      */

    default:
        assert(1);
    }
    return true;
}

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool ResourceD3D::invalidate()
{
    if(m_ptr > 0)
    {
        switch(m_type)
        {
        case RESRBUF_2D:
        case RESTEX_2D:
#pragma MESSAGE(__FILE__"(412) : TODO TODO TODO TODO release D3D interface")
            LOGI("Event>> Texture %s \n", m_name.c_str());
        break;
        //case RESTEX_1D:
        //case RESTEX_3D:
        //case RESTEX_CUBE:
        }
    }
    return true;
}

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool        ResourceD3D::validate()
{
    if(validationNeeded())
        return createRenderResource();
    return true;
}
bool        ResourceD3D::validationNeeded()
{
    //if(m_OGLId)
    {
        const int* vp = m_pOwner->getViewport();
        const int* msaa = m_pOwner->getMsaa();
        if(m_creationData.appDepSz && 
            ((m_creationData.sz[0] != vp[2]) || 
            (m_creationData.sz[1] != vp[3])))
            return true;
        if(m_creationData.appDepMSAA &&
            ((m_creationData.msaa[0] != msaa[0]) ||
            (m_creationData.msaa[1] != msaa[1])))
            return true;
        return false;
    }
    return true;
}

/*************************************************************************
 **
 ** FrameBufferObject FrameBufferObject FrameBufferObject FrameBufferObject
 **
 *************************************************************************/ 

bool FrameBufferObject::validate()
{
    ID3D10Device* pd3dDevice = (ID3D10Device*)m_pOwner->getDevice();
    bool validated = (m_colors.size() > 0) || (m_dst) ? true : false;
    // run through resources and see if we created the RTViews or if sizes changed
    for(int i=0; i<(int)m_colors.size(); i++)
    {
        if(m_colors[i] <= 0)
            break;
        ResourceD3D* pres = static_cast<ResourceD3D*>(m_colors[i]);
        if(pres->m_pTextureRTView == NULL)
        {
            validated = false;
            break;
        }
        if((m_colors[i]->getWidth() != m_w)
         ||(m_colors[i]->getHeight() != m_h))
        {
            validated = false;
            break;
        }
        // TODO: FBO to re-validate if MSAA of resources changed
        //if((m_colors[i]->m_creationData.msaa() != m_w)...
        //{
        //    validated = false;
        //    break;
        //}
    }
    // No need to check DST : color buffers are enough
    //if(m_dst &&((m_colors[i]->getWidth() != m_w)
    // ||(m_colors[i]->getHeight() != m_h)))
    //{
    //    validated = false;
    //    break;
    //}
    if(validated)
        return true;
    bool fbo_failed = false;
    //LOGI("Event>> Creation of FBO %d : %s\n", p.id, p.name.c_str());
    IResourceRepository* pRep = getResourceRepositorySingleton();

    //LOGI("Event>> Creation of FBO (OGL id %d) : %s\n", m_fboID, m_name.c_str());
    //
    // Walk through the referenced textures/renderbuffers
    //
    for(int i=0; i<(int)m_colors.size(); i++)
    {
        if(m_colors[i])
        {
            m_colors[i]->validate();
            switch(m_colors[i]->m_type)
            {
            //case RESTEX_1D:
            //case RESTEX_3D:
            //case RESTEX_CUBE:
            case RESRBUF_2D:
            case RESTEX_2D:
            {    
                ResourceD3D *pResource;
                if(m_colors[i]->m_creationData.resolveTarget) // case where we shortcut the rb to the texutre directly
                {
                    pResource = static_cast<ResourceD3D*>(m_colors[i]->m_creationData.resolveTarget);
                    LOGD("RE-Attaching %s to FBO %s at Color %d (via %s)\n", m_colors[i]->m_creationData.resolveTarget->getName(), m_name.c_str(), i, m_colors[i]->getName());
                } else {
                    pResource = pResource = static_cast<ResourceD3D*>(m_colors[i]);
                }
                if(pResource->m_pTextureRTView)
                    pResource->m_pTextureRTView->Release();
                if( FAILED( pd3dDevice->CreateRenderTargetView(pResource->m_pTexture2D, NULL, &pResource->m_pTextureRTView) ) )
                {
                    LOGE("CreateRenderTargetView() failed.");
                    return false;
                }
            }
            break;
            default:
                assert(1);
            }
            //
            // take the size of the buffer for FBO : blit operation would require it
            //
            m_w = m_colors[i]->getWidth();
            m_h = m_colors[i]->getHeight();
        }
        else 
        {
            LOGE("Error>> Could not find Texture/renderbuffer %d for FBO");
            return false;
        }
    }
    //
    // Depth buffer
    //
    if(m_dst) // found it
    {
        m_dst->validate();
        switch(m_dst->m_type) // we are dealing with a texture
        {
        //case RESTEX_1D:
        //case RESTEX_3D:
        //case RESTEX_CUBE:
        case RESRBUF_2D:
        case RESTEX_2D:
        {
            ResourceD3D *pResource;
            if(m_dst->m_creationData.resolveTarget)
            {
                pResource = static_cast<ResourceD3D*>(m_dst->m_creationData.resolveTarget);
                LOGD("RE-Attaching %s to FBO %s at dst (via %s)\n", pResource->getName(), m_name.c_str(), m_dst->getName());
            } else {
                pResource = static_cast<ResourceD3D*>(m_dst);
            }
            switch(m_dst->m_creationData.fmt)
            {
            case FMT_DEPTH24STENCIL8:
            case FMT_DEPTH32F_STENCIL8: //0x8CAD...GL_DEPTH32F_STENCIL8_ARB
                if( FAILED( pd3dDevice->CreateDepthStencilView(pResource->m_pTexture2D, NULL, &pResource->m_pTextureDSTView) ) )
                {
                    LOGE("CreateDepthStencilView() failed.");
                    return false;
                }
                break;
            default:
                if( FAILED( pd3dDevice->CreateRenderTargetView(pResource->m_pTexture2D, NULL, &pResource->m_pTextureRTView) ) )
                {
                    LOGE("CreateRenderTargetView() failed.");
                    return false;
                }
                break;
            }
        }
        break;
        default:
            assert(1);
            break;
        }
        //if(fbo_failed)
        //{
        //    LOGE("Error>> Could not find Depth Texture/renderbuffer %d for FBO");
        //    glDeleteFramebuffersEXT(1, &m_fboID);
        //    m_fboID = 0;
        //    return NULL;
        //}
        //
        // take the size of the buffer for FBO : blit operation would require it
        // Note: was already done in colors[]. But what if we have a depth-only FBO ?
        //
        m_w = m_dst->getWidth();
        m_h = m_dst->getHeight();
    } 
    else 
    {
        LOGE("Error>> Could not find Texture/renderbuffer %d for FBO");
        return false;
    }
    return true;
}
/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool FrameBufferObject::setCurrent(bool bAdjustViewport)
{
    ID3D10Device* pd3dDevice = (ID3D10Device*)m_pOwner->getDevice();
    ID3D10RenderTargetView *pRTs[4];
    for(int i=0; i<(int)m_colors.size(); i++)
    {
        pRTs[i] = static_cast<ResourceD3D*>(m_colors[i])->m_pTextureRTView;
    }
    ID3D10DepthStencilView* pDSTView = static_cast<ResourceD3D*>(m_dst)->m_pTextureDSTView;
    pd3dDevice->OMSetRenderTargets((int)m_colors.size(), pRTs, pDSTView);
    if(bAdjustViewport)
    {
        D3D10_VIEWPORT vp;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        vp.Width = m_w;
        vp.Height = m_h;
#pragma MESSAGE(__FILE__ "(572) : FrameBufferObject::setCurrent() : take care of the depth viewport info, too")
        vp.MinDepth = 0.0f; // TODO
        vp.MaxDepth = 1.0f; // TODO
        pd3dDevice->RSSetViewports(1, &vp);
    }
    return true;
}
/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool FrameBufferObject::blitTo(IFrameBufferObject* pIDst)
{
    ID3D10Device* pd3dDevice = (ID3D10Device*)m_pOwner->getDevice();
    FrameBufferObject* pDst = static_cast<FrameBufferObject*>(pIDst);
    int n = pDst->m_colors.size();
    if((int)m_colors.size() < n)
        n = (int)m_colors.size();
    for(int i=0; i<n; i++)
    {
        ResourceD3D* pDstD3D = static_cast<ResourceD3D*>(pDst->m_colors[i]);
        ResourceD3D* pSrcD3D = static_cast<ResourceD3D*>(m_colors[i]);
        pd3dDevice->CopyResource(pDstD3D->m_pTexture2D, pSrcD3D->m_pTexture2D);
    }
    if(m_dst && pDst->m_dst)
    {
        ResourceD3D* pDstD3D = static_cast<ResourceD3D*>(pDst->m_dst);
        ResourceD3D* pSrcD3D = static_cast<ResourceD3D*>(m_dst);
        pd3dDevice->CopyResource(pDstD3D->m_pTexture2D, pSrcD3D->m_pTexture2D);
    }
    return true;
}
bool      FrameBufferObjectsRepository::blit(IFrameBufferObject* pIDst, IFrameBufferObject* pISrc)
{
    ID3D10Device* pd3dDevice = (ID3D10Device*)getDevice();
    FrameBufferObject* pDst = pIDst ? static_cast<FrameBufferObject*>(pIDst) : NULL;
    FrameBufferObject* pSrc = pISrc ? static_cast<FrameBufferObject*>(pISrc) : NULL;
    if(pDst != pSrc)
    {
        // For now, 
        // let's avoid any case involving the backbuffer
        // let's just test on color[0] : if the target, as textures, are the same
        // then it means we are in the case of resolveTextureTarget hack. So no need to blit anything
        if((pDst != 0)&&(pSrc != 0))
        {
            ResourceD3D* pDstD3D = static_cast<ResourceD3D*>(pDst->m_colors[0]);
            ResourceD3D* pSrcD3D = static_cast<ResourceD3D*>(pSrc->m_colors[0]);
            if(pDst == pSrc)
            {
                //LOGI("Debug message: No Blit FBO required...\n");
                return true;
            }
        }
        int n = pDst ? (int)pDst->m_colors.size() : 1;
        if(pSrc && ((int)pSrc->m_colors.size() < n))
            n = (int)pSrc->m_colors.size();
        ID3D10Resource *pBackBuffer;
        ID3D10Resource *pBackBufferDST;
        static_cast<ID3D10RenderTargetView*>(m_backbuffer)->GetResource(&pBackBuffer);
        static_cast<ID3D10DepthStencilView*>(m_backbufferDST)->GetResource(&pBackBufferDST);
        for(int i=0; i<n; i++)
        {
            ResourceD3D* pDstD3D = NULL;
            if(pDst)
                pDstD3D = static_cast<ResourceD3D*>(pDst->m_colors[i]);
            ResourceD3D* pSrcD3D = NULL;
            if(pSrc)
                pSrcD3D = static_cast<ResourceD3D*>(pSrc->m_colors[i]);
            // TODO: ResolveSubresource
            if(pSrcD3D)
                pd3dDevice->ResolveSubresource(pBackBuffer, 0, pSrcD3D->m_pTexture2D, 0, pSrcD3D->m_d3dFmt); // CopyResource(pBackBuffer, pSrcD3D->m_pTexture2D);
            else
                pd3dDevice->ResolveSubresource(pBackBuffer, 0, pSrcD3D->m_pTexture2D, 0, pSrcD3D->m_d3dFmt); // CopyResource(pDstD3D->m_pTexture2D, pBackBuffer);
        }
        //{
        //    ResourceD3D* pDstD3D = NULL;
        //    if(pDst)
        //        pDstD3D = static_cast<ResourceD3D*>(pDst->m_dst);
        //    ResourceD3D* pSrcD3D = NULL;
        //    if(pSrc)
        //        pSrcD3D = static_cast<ResourceD3D*>(pSrc->m_dst);
        //    // TODO: ResolveSubresource
        //    if(pSrcD3D)
        //        pd3dDevice->CopyResource(pBackBuffer, pSrcD3D->m_pTexture2D);
        //    else
        //        pd3dDevice->CopyResource(pDstD3D->m_pTexture2D, pBackBuffer);
        //}
    }
    else
    {
        LOGE("Error>> glBlitFramebufferEXT cannot blit to itself\n");
    }
    return true;
}
/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool  FrameBufferObject::invalidate()
{
    return true;
}

/*************************************************************************
 **
 ** FrameBufferObject FrameBufferObject FrameBufferObject FrameBufferObject
 **
 *************************************************************************/ 
bool FrameBufferObjectsRepository::setCurrent(IFrameBufferObject* pFBO, bool bAdjustViewport)
{
    if(pFBO == NULL)
    {
        ID3D10Device* pd3dDevice = (ID3D10Device*)m_pDevice;
        ID3D10RenderTargetView* pRTV  = static_cast<ID3D10RenderTargetView*>(m_backbuffer);
        ID3D10DepthStencilView* pDSTV = static_cast<ID3D10DepthStencilView*>(m_backbufferDST);
        pd3dDevice->OMSetRenderTargets(1, &pRTV, pDSTV);
        if(bAdjustViewport)
        {
            D3D10_VIEWPORT vp;
            vp.TopLeftX = m_vp[0];
            vp.TopLeftY = m_vp[1];
            vp.Width = m_vp[2];
            vp.Height = m_vp[3];
    #pragma MESSAGE(__FILE__ "(572) : FrameBufferObject::setCurrent() : take care of the depth viewport info, too")
            vp.MinDepth = 0.0f; // TODO
            vp.MaxDepth = 1.0f; // TODO
            pd3dDevice->RSSetViewports(1, &vp);
        }
        return true;
    }
    bool bRes = static_cast<FrameBufferObject*>(pFBO)->setCurrent(bAdjustViewport);
    return bRes;
    }



