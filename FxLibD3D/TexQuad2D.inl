#ifndef LOGMSG
#define LOGMSG fprintf
#define LOG_ERR stderr
#define LOG_MSG stdout
#define LOG_WARN stdout
#endif
namespace TexQuad2D
{  
    struct VtxAttribs
    {
        D3DXVECTOR4 p; // position
        D3DXVECTOR2 t;
    };
    D3D1X_INPUT_ELEMENT_DESC    g_VtxAttribsDesc[] =
    {
      { "position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D1X(INPUT_PER_VERTEX_DATA), 0 },
      { "texcoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*4, D3D1X(INPUT_PER_VERTEX_DATA), 0 },
    };
    ID3D1XInputLayout*          g_InputLayout = NULL;
    ID3D1XBuffer *              g_VtxBufferStatic = NULL;
    //--------------------------------------------------------------------------------------
    // Init Quad
    //--------------------------------------------------------------------------------------
    HRESULT Init(ID3D1XDevice* pd3dDevice, void* pSignature, int signatureSize)
    {
        HRESULT hr=S_OK;

		static TexQuad2D::VtxAttribs  quad[] = {
			D3DXVECTOR4(-1.0,1.0,0.0, 1),
			D3DXVECTOR2(0,0),
			D3DXVECTOR4(-1.0,-1.0,0.0, 1),
			D3DXVECTOR2(0,1),
			D3DXVECTOR4(1.0,1.0, 0.0, 1),
			D3DXVECTOR2(1,0),
			D3DXVECTOR4(1.0,-1.0, 0.0, 1),
			D3DXVECTOR2(1,1)
		};

#if 0
        D3D10_SUBRESOURCE_DATA data;
        data.pSysMem = quad;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;

        D3D10_BUFFER_DESC vtxBufferDesc =
        {
            sizeof(TexQuad2D::VtxAttribs)*4,
            D3D10_USAGE_DEFAULT,
            D3D10_BIND_VERTEX_BUFFER,
            0,
            0
        };
        if(TexQuad2D::g_VtxBufferStatic)
            TexQuad2D::g_VtxBufferStatic->Release();
        hr = pd3dDevice->CreateBuffer( &vtxBufferDesc, &data, &TexQuad2D::g_VtxBufferStatic);
        if(FAILED(hr))
        {
            LOGMSG(LOG_ERR, "Error while creating the Vtx Buffer for Quad");
        }

        if(TexQuad2D::g_InputLayout)
            TexQuad2D::g_InputLayout->Release();
        if(FAILED(hr=pd3dDevice->CreateInputLayout(TexQuad2D::g_VtxAttribsDesc, 2, pSignature, signatureSize, &TexQuad2D::g_InputLayout ) ) )
        {
          LOGMSG(LOG_WARN, "Vtx Declaration failed");
        }
#else
		D3D1X_SUBRESOURCE_DATA data;
		data.pSysMem = quad;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		D3D1X_BUFFER_DESC vtxBufferDesc =
		{
			sizeof(TexQuad2D::VtxAttribs)*4,
			D3D1X(USAGE_DEFAULT),
			D3D1X(BIND_VERTEX_BUFFER),
			0,
			0
		};

		ID3D1XDevice * pd3dDeviceDerived = reinterpret_cast<ID3D1XDevice *>(nvFX::getDevice());

		if(TexQuad2D::g_VtxBufferStatic)
			TexQuad2D::g_VtxBufferStatic->Release();
		hr = pd3dDeviceDerived->CreateBuffer( &vtxBufferDesc, &data, &TexQuad2D::g_VtxBufferStatic);
		if(FAILED(hr))
		{
			LOGMSG(LOG_ERR, "Error while creating the Vtx Buffer for Quad");
		}

		if(TexQuad2D::g_InputLayout)
			TexQuad2D::g_InputLayout->Release();
		if(FAILED(hr=pd3dDeviceDerived->CreateInputLayout(TexQuad2D::g_VtxAttribsDesc, 2, pSignature, signatureSize, &TexQuad2D::g_InputLayout ) ) )
		{
			LOGMSG(LOG_WARN, "Vtx Declaration failed");
		}
#endif

        return hr;
    }
    void Destroy()
    {
        if(g_VtxBufferStatic)
            g_VtxBufferStatic->Release();
    }
    //--------------------------------------------------------------------------------------
    // Draw Fullscreen Quad
    //--------------------------------------------------------------------------------------
    HRESULT DrawFullScreenQuad(ID3D10Device* pd3dDevice)
    {
        HRESULT hr = S_OK;
        UINT strides = sizeof(VtxAttribs);
        UINT offsets = 0;

#ifdef USE_D3D11
		ID3D11DeviceContext *pd3d1X;
		((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
		ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif

		pd3d1X->IASetInputLayout(TexQuad2D::g_InputLayout);
		pd3d1X->IASetVertexBuffers(0,1,&TexQuad2D::g_VtxBufferStatic, &strides, &offsets);
		pd3d1X->IASetPrimitiveTopology(D3D1X(PRIMITIVE_TOPOLOGY_TRIANGLESTRIP));

		pd3d1X->Draw(4,0);

        return hr;
    }
    //--------------------------------------------------------------------------------------
    // Draw a Quad on the screen
    // - set the viewport for screen resolution
    // - render simple 2D : Identity mvp
    //--------------------------------------------------------------------------------------
    HRESULT Draw(ID3D1XDevice* pd3dDevice, float size, float offsetx, float offsety)
    {
      HRESULT hr = S_OK;
      UINT strides = sizeof(VtxAttribs);
      UINT offsets = 0;

#ifdef USE_D3D11
	  ID3D11DeviceContext *pd3d1X;
	  ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
	  ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif

	  pd3d1X->IASetInputLayout(TexQuad2D::g_InputLayout);
	  pd3d1X->IASetVertexBuffers(0,1,&TexQuad2D::g_VtxBufferStatic, &strides, &offsets);
	  pd3d1X->IASetPrimitiveTopology(D3D1X(PRIMITIVE_TOPOLOGY_TRIANGLESTRIP));

	  pd3d1X->Draw(4,0);

      return hr;
    }

} // namespace TexOverlay


