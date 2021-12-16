#pragma once
#include <ndtech/VertexTypes.h>

namespace ndtech {
  namespace components {

    template <typename Component, typename VertexType>
    struct ModelSystemImpl {

      winrt::com_ptr<ID3D11InputLayout>         m_inputLayout;
      std::wstring                              m_vertexShaderFileName;
      std::wstring                              m_pixelShaderFileName;
      std::wstring                              m_geometryShaderFileName;

      template <typename AppType>
      void Initialize(AppType* app) {
        LOG(INFO) << "ndtech::components::ModelSystemMagiLeapImpl::Initialize : typeid(decltype(this)).name() = " << typeid(decltype(this)).name();

        // Each vertex is one instance of the VertexPositionColor struct.
        const UINT stride = sizeof(VertexType);
        const UINT offset = 0;

        bool m_usingVprtShaders = app->m_renderingSystem.m_deviceResources->GetDeviceSupportsVprt();

        m_vertexShaderFileName = m_usingVprtShaders ? L"ms-appx:///vprt-vertex-shader.cso" : L"ms-appx:///vertex-shader.cso";
        app->m_renderingSystem.CreateShader<winrt::com_ptr<ID3D11VertexShader>>(m_vertexShaderFileName);

        m_inputLayout = app->m_renderingSystem.SetLayout<ndtech::vertexTypes::VertexPositionColor>(m_inputLayout, m_vertexShaderFileName);

        m_pixelShaderFileName = L"ms-appx:///pixel-shader.cso";
        app->m_renderingSystem.CreateShader<winrt::com_ptr<ID3D11PixelShader>>(m_pixelShaderFileName);

        if (!m_usingVprtShaders)
        {
          m_geometryShaderFileName = L"ms-appx:///geometry-shader.cso";
          app->m_renderingSystem.CreateShader<winrt::com_ptr<ID3D11GeometryShader>>(m_geometryShaderFileName);
        }

        //winrt::com_ptr<ID3D11VertexShader> shader = app->m_renderingSystem.GetShader<winrt::com_ptr<ID3D11VertexShader>>(vertexShaderFileName);
      }

      template<typename AppType>
      Component InitializeComponent(Component& component, AppType* app) {
        const CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
        winrt::check_hresult(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice()->CreateBuffer(
            &constantBufferDesc,
            nullptr,
            component.m_modelConstantBuffer.put()
          ));

        const auto context = app->m_renderingSystem.m_deviceResources->GetD3DDeviceContext();

        ID3D11Buffer * mcbuffer = component.m_modelConstantBuffer.get();
        // Apply the model constant buffer to the vertex shader.
        context->VSSetConstantBuffers(
          0,
          1,
          &mcbuffer
        );

        D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
        vertexBufferData.pSysMem = component.m_vertices.data();
        vertexBufferData.SysMemPitch = 0;
        vertexBufferData.SysMemSlicePitch = 0;
        const CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexType) * static_cast<UINT>(component.m_vertices.size()), D3D11_BIND_VERTEX_BUFFER);
        winrt::check_hresult(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice()->CreateBuffer(
            &vertexBufferDesc,
            &vertexBufferData,
            component.m_vertexBuffer.put()
          ));

#ifdef _DEBUG
        {
          std::string vbufferName("ndtech::components::ModelSystemImplHoloLens::m_vertexBuffer");
          (component.m_vertexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, vbufferName.size(), vbufferName.c_str());
        }
#endif

        D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
        indexBufferData.pSysMem = component.m_indices.data();
        indexBufferData.SysMemPitch = 0;
        indexBufferData.SysMemSlicePitch = 0;
        CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * static_cast<UINT>(component.m_indices.size()), D3D11_BIND_INDEX_BUFFER);
        winrt::check_hresult(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice()->CreateBuffer(
            &indexBufferDesc,
            &indexBufferData,
            component.m_indexBuffer.put()
          ));

#ifdef _DEBUG
        std::string indexBufferName("ndtech::components::ModelSystemImplHoloLens::m_indexBuffer");
        (component.m_indexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, indexBufferName.size(), indexBufferName.c_str());
#endif


        return component;
      }

      template <typename AppType>
      void PreRenderComponentSystem(AppType* app) {
        LOG(INFO) << "ModelSystemHoloLensImpl::PreRenderComponentSystem";
      };

      template <typename RenderingSystemType, typename AppType>
      void RenderComponents(RenderingSystemType* renderingSystem, AppType* app) {

        LOG(INFO) << "ndtech::components::ModelSystemHoloLensImpl::RenderComponents - Begin";


        const auto context = renderingSystem->m_deviceResources->GetD3DDeviceContext();

        // Each vertex is one instance of the VertexPositionColor struct.
        const UINT stride = sizeof(VertexType);
        const UINT offset = 0;

        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->IASetInputLayout(m_inputLayout.get());

        // Attach the vertex shader.
        context->VSSetShader(
          renderingSystem->m_vertexShadersStore.GetItem(m_vertexShaderFileName).get(),
          nullptr,
          0
        );

        if (!renderingSystem->m_deviceResources->GetDeviceSupportsVprt())
        {

          // On devices that do not support the D3D11_FEATURE_D3D11_OPTIONS3::
          // VPAndRTArrayIndexFromAnyShaderFeedingRasterizer optional feature,
          // a pass-through geometry shader is used to set the render target 
          // array index.

          context->GSSetShader(
            renderingSystem->m_geometryShadersStore.GetItem(m_geometryShaderFileName).get(),
            nullptr,
            0
          );
        }

        // Attach the pixel shader.
        context->PSSetShader(
          renderingSystem->m_pixelShadersStore.GetItem(m_pixelShaderFileName).get(),
          nullptr,
          0
        );


        std::vector<Component> componentVector = std::get<std::vector<Component>>(app->m_componentVectors);

        for (AppType::EntityIndexType componentIndex = 0; componentIndex < app->m_freeComponentIndices[TypeUtilities::IndexOf<Component, AppType::Components>()]; componentIndex++) {
          Component& component = componentVector.at(componentIndex);
          RenderComponent(component, app, 0);
        }

      };

      template <typename AppType>
      void RenderComponent(Component& component, AppType* app, int loc) {

        LOG(INFO) <<  "ndtech::components::ModelSystemHoloLensImpl::RenderComponent - Begin";

        const auto context = app->m_renderingSystem.m_deviceResources->GetD3DDeviceContext();

        // Updating m_modelConstantBufferData.model based on the transformation matrix here instead of in ModelSystem
        // to keep ModelSystem from having any DirectX / HoloLens specific functionality
        const DirectX::FXMMATRIX xmMatrix = DirectX::XMMATRIX(&component.m_transformationMatrix[0][0]);
        DirectX::XMStoreFloat4x4(&component.m_modelConstantBufferData.model, DirectX::XMMatrixTranspose(xmMatrix));

        // Update the model transform buffer for the hologram.
        context->UpdateSubresource(
          component.m_modelConstantBuffer.get(),
          0,
          nullptr,
          &component.m_modelConstantBufferData,
          0,
          0
        );

        // Apply the model constant buffer to the vertex shader.
        ID3D11Buffer * mcbuffer = component.m_modelConstantBuffer.get();
        context->VSSetConstantBuffers(
          0,
          1,
          &mcbuffer
        );

        // Set the Vertex Buffer
        ID3D11Buffer * vbuffer = component.m_vertexBuffer.get();
        context->IASetVertexBuffers(
          0,
          1,
          &vbuffer,
          &component.m_stride,
          &component.m_offset
        );

        // Set the Index Buffer
        context->IASetIndexBuffer(
          component.m_indexBuffer.get(),
          DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
          0
        );

        // Draw the objects.
        context->DrawIndexedInstanced(
          component.m_indices.size(),   // Index count per instance.
          2,              // Instance count.
          0,              // Start index location.
          0,              // Base vertex location.
          0               // Start instance location.
        );

      };

      void PostRender(BaseApp* app) {
        LOG(INFO) << "here";
      };



    };

  }
}