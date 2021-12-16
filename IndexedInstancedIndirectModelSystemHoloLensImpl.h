#pragma once

#include <vector>

#include <ndtech/VertexTypes.h>
#include <ndtech/DirectXHelper.h>
#include "IndexedInstancedModelInstance.h"

namespace ndtech {
  namespace components {

    template <typename Component, typename VertexType>
    struct IndexedInstancedIndirectModelSystemImpl {

      winrt::com_ptr<ID3D11InputLayout>               m_inputLayout;
      std::wstring                                    m_vertexShaderFileName;
      std::wstring                                    m_pixelShaderFileName;
      std::wstring                                    m_geometryShaderFileName;

      size_t                                          m_componentCount = 0;
      size_t                                          m_instanceCount = 0;

      DirectXHelper::IndexedInstancedIndirectCallArgs m_indirectCallArgs;
      winrt::com_ptr<ID3D11Buffer>			              m_indirectCallArgsBuffer;
      winrt::com_ptr<ID3D11UnorderedAccessView>       m_indirectCallArgsUnorderedAccessView;
      winrt::com_ptr<ID3D11Buffer>			              m_instanceRenderingBuffer;
      winrt::com_ptr<ID3D11UnorderedAccessView>       m_instanceRenderingUnorderedAccessView;

      UINT                                            m_stride = sizeof(VertexType);

      std::vector<VertexType>                         m_vertices;
      std::vector<unsigned short>                     m_indices;

      template <typename AppType>
      void Initialize(AppType* app) {
        LOG(INFO) << "ndtech::components::IndexedInstancedModelSystemImplHoloLens::Initialize : Begin";

        bool m_usingVprtShaders = app->m_renderingSystem.m_deviceResources->GetDeviceSupportsVprt();

        // Set up the command buffer
        DirectXHelper::CreateCommandBuffer(app->m_renderingSystem.m_deviceResources->GetD3DDevice(), m_indirectCallArgsBuffer);

        // Create an unordered access view over the command buffer to allow the command to be updated from the GPU
        DirectXHelper::CreateUnorderedAcceesViewBuffer(app->m_renderingSystem.m_deviceResources->GetD3DDevice(), m_indirectCallArgsBuffer, m_indirectCallArgsUnorderedAccessView, DXGI_FORMAT_R32_UINT, 5);

        // Create the vertex shader for this ComponentSystem
        m_vertexShaderFileName = m_usingVprtShaders ? L"ms-appx:///vprt-indirect-instanced-vertex-shader.cso" : L"ms-appx:///indirect-instanced-vertex-shader.cso";
        app->m_renderingSystem.CreateShader<winrt::com_ptr<ID3D11VertexShader>>(m_vertexShaderFileName);

#ifdef _DEBUG
        std::string vertexShaderName("ndtech::components::IndexedInstancedImplHoloLens::vertexShader");
        (app->m_renderingSystem.m_vertexShadersStore.GetItem(m_vertexShaderFileName))->SetPrivateData(WKPDID_D3DDebugObjectName, vertexShaderName.size(), vertexShaderName.c_str());
#endif

        // Create the input layout based on the VertexType and Vertex Shader
        m_inputLayout = app->m_renderingSystem.SetLayout<ndtech::vertexTypes::VertexPositionColor>(m_inputLayout, m_vertexShaderFileName);

        // Create the Pixel Shader
        m_pixelShaderFileName = L"ms-appx:///pixel-shader.cso";
        app->m_renderingSystem.CreateShader<winrt::com_ptr<ID3D11PixelShader>>(m_pixelShaderFileName);

#ifdef _DEBUG
        std::string pixelShaderName("ndtech::components::IndexedInstancedImplHoloLens::pixelShader");
        (app->m_renderingSystem.m_pixelShadersStore.GetItem(m_pixelShaderFileName))->SetPrivateData(WKPDID_D3DDebugObjectName, pixelShaderName.size(), pixelShaderName.c_str());
#endif

        // If the hardware doesn't support vprt, create a geometry shader to add the instance id
        if (!m_usingVprtShaders)
        {
          m_geometryShaderFileName = L"ms-appx:///geometry-shader.cso";
          app->m_renderingSystem.CreateShader<winrt::com_ptr<ID3D11GeometryShader>>(m_geometryShaderFileName);

#ifdef _DEBUG
          {
            std::string debugName("ndtech::components::IndexedInstancedImplHoloLens::geometryShader");
            (app->m_renderingSystem.m_geometryShadersStore.GetItem(m_geometryShaderFileName))->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.size(), debugName.c_str());
          }
#endif
        }

      }

      template<typename AppType>
      Component InitializeComponent(Component& component, AppType* app) {

        LOG(INFO) << "ndtech::components::IndexedInstancedModelSystemImplHoloLens::RenderComponents - Begin";

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
        std::string vbufferName("ndtech::components::IndexedInstancedImplHoloLens::m_vertexBuffer");
        (component.m_vertexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, vbufferName.size(), vbufferName.c_str());
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
        std::string indexBufferName("ndtech::components::IndexedInstancedImplHoloLens::indexBuffer");
        (component.m_indexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, indexBufferName.size(), indexBufferName.c_str());
#endif

        // Add this components vertices to the vertices for the system
        m_vertices.insert(m_vertices.end(), component.m_vertices.begin(), component.m_vertices.end());

        // Add this component indices to the indices for the system
        m_indices.insert(m_indices.end(), component.m_indices.begin(), component.m_indices.end());

        // Initialize the transformation matrix for each instance
        for (typename Component::InstanceType& instance : component.m_instances) {
          instance.m_transformationMatrix = glm::translate(glm::mat4(), instance.m_position) * glm::mat4_cast(instance.m_orientation) * glm::scale(instance.m_scale);
        }

        return component;
      }

      // Uncomment the following to run code before rendering
      //template <typename AppType>
      //void PreRenderComponentSystem(AppType* app) {
      //  LOG(INFO) << "ndtech::components::IndexedInstancedModelSystemImplHoloLens::PreRenderComponentSystem";
      //};

      template <typename RenderingSystemType, typename AppType>
      void RenderComponents(RenderingSystemType* renderingSystem, AppType* app) {

        LOG(INFO) << "ndtech::components::IndexedInstancedModelSystemImplHoloLens::RenderComponents - Begin";


        const auto context = renderingSystem->m_deviceResources->GetD3DDeviceContext();

        // Each vertex is one instance of the VertexPositionColor struct.
        const UINT stride = sizeof(InstanceRenderingData);

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

        std::vector<InstanceRenderingData> instanceRenderingData;
        std::vector< DirectX::XMFLOAT4X4> instanceRenderingDataTest;

        // Loop through the components and get the total count of components and component instances
        std::vector<Component> componentVector = std::get<std::vector<Component>>(app->m_componentVectors);
        size_t componentCount = 0;
        size_t instanceCount = 0;
        for (AppType::EntityIndexType componentIndex = 0; componentIndex < app->m_freeComponentIndices[TypeUtilities::IndexOf<Component, AppType::Components>()]; componentIndex++) {
          componentCount++;
          Component& component = componentVector.at(componentIndex);

          // Add the renderingData from the component to the renderingData for the draw
          instanceRenderingData.insert(instanceRenderingData.end(), component.m_instanceRenderingData.begin(), component.m_instanceRenderingData.end());

          // Update the instanceCount
          instanceCount += component.m_instances.size();

          // Loop through the instances and load the test data
          for (typename Component::InstanceType& instance : component.m_instances) {
            const DirectX::FXMMATRIX xmMatrix = DirectX::XMMATRIX(&instance.m_transformationMatrix[0][0]);
            DirectX::XMFLOAT4X4 xmFloat4x4;
            DirectX::XMStoreFloat4x4(&xmFloat4x4, DirectX::XMMatrixTranspose(xmMatrix));
            instanceRenderingDataTest.push_back(xmFloat4x4);
          }
        }


        // If the count of components has changed, update the locally stored count
        if (componentCount != m_componentCount) {
          m_componentCount = componentCount;
        }

        // If the count of instances has changed, update the locally stored count
        // and update the append buffer to be the size of all instances
        if (instanceCount != m_instanceCount) {
          m_instanceCount = instanceCount;

          ndtech::DirectXHelper::CreateBuffer(
            app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
            m_instanceRenderingBuffer,
            stride * m_instanceCount,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
            0,
            D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
            stride);

#ifdef _DEBUG
          std::string d3d11debugname("ndtech::components::IndexedInstancedIndirectModelSystemImpl::InstanceRenderingBuffer");
          (m_instanceRenderingBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
#endif

          ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
            app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
            m_instanceRenderingBuffer,
            m_instanceRenderingUnorderedAccessView,
            DXGI_FORMAT_UNKNOWN,
            m_instanceCount);
          
          ID3D11UnorderedAccessView *const uavs[1] = { m_instanceRenderingUnorderedAccessView.get() };

          //context->OMSetRenderTargetsAndUnorderedAccessViews(
          //  D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL,
          //  nullptr,
          //  nullptr,
          //  2,
          //  1,
          //  uavs,
          //  &m_instanceCount
          //);

          //context->VSSetShaderResources(
          //  0, // start slot
          //  1, // number of views
          //  &uav // pointer to the shader resource view
          //);

          //// Update the m_indirectCallArgsBuffer
          //context->UpdateSubresource(
          //  m_instanceRenderingBuffer.get(),
          //  0,
          //  nullptr,
          //  instanceRenderingData.data(),
          //  0,
          //  0
          //);



          // Update the m_indirectCallArgsBuffer
          context->UpdateSubresource(
            m_instanceRenderingBuffer.get(),
            0,
            nullptr,
            instanceRenderingDataTest.data(),
            0,
            0
          );

        }


        UINT vertexOffset = 0;
        UINT indexOffset = 0;
        UINT instanceOffset = 0;

        for (AppType::EntityIndexType componentIndex = 0; componentIndex < app->m_freeComponentIndices[TypeUtilities::IndexOf<Component, AppType::Components>()]; componentIndex++) {
          Component& component = componentVector.at(componentIndex);

          RenderComponent(component, app, vertexOffset, indexOffset, instanceOffset);

          //vertexOffset += component.m_vertices.size();
          //indexOffset += component.m_indices.size();
          instanceOffset += component.m_instances.size();
        }

      };

      template <typename AppType>
      void RenderComponent(Component& component, AppType* app, UINT vertexOffset, UINT indexOffset, UINT instanceOffset) {

        LOG(INFO) << "ndtech::components::IndexedInstancedModelSystemImplHoloLens::RenderComponent - Begin";

        const auto context = app->m_renderingSystem.m_deviceResources->GetD3DDeviceContext();

        // Set the Vertex Buffer
        ID3D11Buffer * vbuffer = component.m_vertexBuffer.get();
        context->IASetVertexBuffers(
          0,
          1,
          &vbuffer,
          &m_stride,
          &vertexOffset
        );

        // Set the Index Buffer
        context->IASetIndexBuffer(
          component.m_indexBuffer.get(),
          DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
          indexOffset
        );

        // Update the indirect call args
        m_indirectCallArgs.IndexCountPerInstance = component.m_indices.size();
        m_indirectCallArgs.InstanceCount = component.m_instances.size();
        m_indirectCallArgs.StartIndexLocation = 0;
        m_indirectCallArgs.BaseVertexLocation = 0;
        m_indirectCallArgs.StartInstanceLocation = instanceOffset;


        // Update the m_indirectCallArgsBuffer
        context->UpdateSubresource(
          m_indirectCallArgsBuffer.get(),
          0,
          nullptr,
          &m_indirectCallArgs,
          0,
          0
        );

        // Render each instance
        for (typename Component::InstanceType& instance : component.m_instances) {

          // Updating m_modelConstantBufferData.model based on the transformation matrix here instead of in ModelSystem
          // to keep ModelSystem from having any DirectX / HoloLens specific functionality
          const DirectX::FXMMATRIX xmMatrix = DirectX::XMMATRIX(&instance.m_transformationMatrix[0][0]);
          DirectX::XMStoreFloat4x4(&component.m_modelConstantBufferData.model, DirectX::XMMatrixTranspose(xmMatrix));
          
          component.m_modelConstantBufferData.instanceId = (float)instanceOffset;
          instanceOffset++;

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

          // Draw the objects using the m_indirectCallArgsBuffer to get arguments
          UINT         AlignedByteOffsetForArgs = 0;
          context->DrawIndexedInstancedIndirect(m_indirectCallArgsBuffer.get(), AlignedByteOffsetForArgs);

        }

      };

      void PostRender(BaseApp* app) {
        LOG(INFO) << "ndtech::components::IndexedInstancedModelSystemImplHoloLens::PostRender";
      };



    };

  }
}