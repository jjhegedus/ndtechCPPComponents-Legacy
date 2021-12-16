#pragma once

#include <ndtech/ShaderStructures.h>
#include <ndtech/DirectXHelper.h>

namespace ndtech {
  namespace components {

    // The DerivedVoidVertexBufferSystemType template argument represents the generic VertexBufferSystem which this implementation is implementing
    template <typename DerivedVoidVertexBufferSystemType, typename Component>
    struct VoidVertexBufferSystemImpl {

      winrt::com_ptr<ID3D11InputLayout>               m_inputLayout;
      std::wstring                                    m_vertexShaderFileName;
      std::wstring                                    m_pixelShaderFileName;
      std::wstring                                    m_geometryShaderFileName;

      winrt::com_ptr<ID3D11Buffer>			              m_verticesBuffer;
      winrt::com_ptr<ID3D11ShaderResourceView>			  m_verticesSRV;
      winrt::com_ptr<ID3D11UnorderedAccessView>			  m_verticesUAV;

      winrt::com_ptr<ID3D11Buffer>			              m_materialsBuffer;
      winrt::com_ptr<ID3D11ShaderResourceView>			  m_materialsSRV;
      winrt::com_ptr<ID3D11UnorderedAccessView>			  m_materialsUAV;

      winrt::com_ptr<ID3D11Buffer>			              m_perVertexDataBuffer;
      winrt::com_ptr<ID3D11ShaderResourceView>			  m_perVertexDataSRV;
      winrt::com_ptr<ID3D11UnorderedAccessView>			  m_perVertexDataUAV;

      winrt::com_ptr<ID3D11Buffer>			              m_perInstanceDataBuffer;
      winrt::com_ptr<ID3D11ShaderResourceView>			  m_perInstanceDataSRV;
      winrt::com_ptr<ID3D11UnorderedAccessView>			  m_perInstanceDataUAV;

      winrt::com_ptr<ID3D11Buffer>			              m_vertexBuffer;
      winrt::com_ptr<ID3D11Buffer>			              m_instanceIndicesBuffer;
      winrt::com_ptr<ID3D11Buffer>			              m_instanceMaterialsBuffer;




      winrt::com_ptr<ID3D11Buffer>			              m_directionalLightsBuffer;
      winrt::com_ptr<ID3D11ShaderResourceView>			  m_directionalLightsSRV;
      winrt::com_ptr<ID3D11UnorderedAccessView>			  m_directionalLightsUAV;

      winrt::com_ptr<ID3D11Buffer>			              m_pointLightsBuffer;
      winrt::com_ptr<ID3D11ShaderResourceView>			  m_pointLightsSRV;
      winrt::com_ptr<ID3D11UnorderedAccessView>			  m_pointLightsUAV;

      winrt::com_ptr<ID3D11Buffer>			              m_coneLightsBuffer;
      winrt::com_ptr<ID3D11ShaderResourceView>			  m_coneLightsSRV;
      winrt::com_ptr<ID3D11UnorderedAccessView>			  m_coneLightsUAV;





      //VoidVertexBufferSystemImpl<DerivedVoidVertexBufferSystemType, Component>(){
      //}

      //VoidVertexBufferSystemImpl<DerivedVoidVertexBufferSystemType, Component>(const VoidVertexBufferSystemImpl<DerivedVoidVertexBufferSystemType, Component>& other) :
      //  m_vertexShaderFileName(other.m_vertexShaderFileName),
      //  m_pixelShaderFileName(other.m_pixelShaderFileName),
      //  m_geometryShaderFileName(other.m_geometryShaderFileName),
      //  m_verticesBuffer(other.m_verticesBuffer),
      //  m_verticesSRV(other.m_verticesSRV),
      //  m_verticesUAV(other.m_verticesUAV),
      //  m_materialsBuffer(other.m_materialsBuffer),
      //  m_materialsSRV(other.m_materialsSRV),
      //  m_materialsUAV(other.m_materialsUAV),
      //  m_perVertexDataBuffer(other.m_perVertexDataBuffer),
      //  m_perVertexDataSRV(other.m_perVertexDataSRV),
      //  m_perVertexDataUAV(other.m_perVertexDataUAV),
      //  m_perInstanceDataBuffer(other.m_perInstanceDataBuffer),
      //  m_perInstanceDataSRV(other.m_perInstanceDataSRV),
      //  m_perInstanceDataUAV(other.m_perInstanceDataUAV),
      //  m_vertexBuffer(other.m_vertexBuffer)
      //{

      //}

      template <typename AppType>
      void Initialize(AppType* app) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplHoloLens::Initialize : Begin";

        bool m_usingVprtShaders = app->m_renderingSystem.m_deviceResources->GetDeviceSupportsVprt();

        // Create the vertex shader for this ComponentSystem
        m_vertexShaderFileName = m_usingVprtShaders ? L"ms-appx:///void-vb-vertex-shader-vprt.cso" : L"ms-appx:///void-vb-vertex-shader.cso";
        app->m_renderingSystem.CreateShader<winrt::com_ptr<ID3D11VertexShader>>(m_vertexShaderFileName);

#ifdef _DEBUG
        // Set the D3D debug name for the vertex shader
        std::string vertexShaderName("ndtech::components::VoidVertexBufferSystemImpl::vertexShader");
        (app->m_renderingSystem.m_vertexShadersStore.GetItem(m_vertexShaderFileName))->SetPrivateData(WKPDID_D3DDebugObjectName, vertexShaderName.size(), vertexShaderName.c_str());
#endif

        // Create the input layout based on the VertexType and Vertex Shader
        m_inputLayout = app->m_renderingSystem.SetLayout<ndtech::vertexTypes::VertexPositionColor>(m_inputLayout, m_vertexShaderFileName);

        // Create the Pixel Shader
        m_pixelShaderFileName = L"ms-appx:///pixel-shader.cso";
        app->m_renderingSystem.CreateShader<winrt::com_ptr<ID3D11PixelShader>>(m_pixelShaderFileName);

#ifdef _DEBUG
        std::string pixelShaderName("ndtech::components::VoidVertexBufferSystemImpl::pixelShader");
        (app->m_renderingSystem.m_pixelShadersStore.GetItem(m_pixelShaderFileName))->SetPrivateData(WKPDID_D3DDebugObjectName, pixelShaderName.size(), pixelShaderName.c_str());
#endif

        // If the hardware doesn't support vprt, create a geometry shader to add the instance id
        if (!m_usingVprtShaders)
        {
          m_geometryShaderFileName = L"ms-appx:///geometry-shader.cso";
          app->m_renderingSystem.CreateShader<winrt::com_ptr<ID3D11GeometryShader>>(m_geometryShaderFileName);

#ifdef _DEBUG
          {
            std::string debugName("ndtech::components::VoidVertexBufferSystemImpl::geometryShader");
            (app->m_renderingSystem.m_geometryShadersStore.GetItem(m_geometryShaderFileName))->SetPrivateData(WKPDID_D3DDebugObjectName, debugName.size(), debugName.c_str());
          }
#endif
        }

      }

      template <typename AppType, typename ModelData>
      void CreateModel(AppType* app, std::vector<ModelData> vertices, std::vector<unsigned short> indices) {

        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        const UINT vertexStride = sizeof(ModelData);

        ndtech::DirectXHelper::CreateBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_verticesBuffer,
          vertexStride * derivedComponentSystem.m_modelVertices.size(),
          D3D11_USAGE_DEFAULT,
          D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
          0,
          D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
          vertexStride);

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_verticesBuffer");
          (m_verticesBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateShaderResourceView(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_verticesBuffer,
          m_verticesSRV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_modelVertices.size()
        );

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_verticesSRV");
          (m_verticesSRV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_verticesBuffer,
          m_verticesUAV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_modelVertices.size());

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_verticesUAV");
          (m_verticesUAV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif
      }

      template <typename AppType, typename MaterialType>
      void CreateMaterial(AppType* app, MaterialType material) {

        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        const UINT materialStride = sizeof(glm::vec3);

        ndtech::DirectXHelper::CreateBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_materialsBuffer,
          materialStride * derivedComponentSystem.m_materials.size(),
          D3D11_USAGE_DEFAULT,
          D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
          0,
          D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
          materialStride);

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_materialsBuffer");
          (m_materialsBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateShaderResourceView(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_materialsBuffer,
          m_materialsSRV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_materials.size()
        );

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_materialsSRV");
          (m_materialsSRV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_materialsBuffer,
          m_materialsUAV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_materials.size());

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_materialsUAV");
          (m_materialsUAV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif
      }

      template <typename AppType, typename PerInstanceType, typename PerVertexDataType>
      void CreateComponent(AppType* app, PerInstanceType& instance, PerVertexDataType pvd, unsigned int instanceOffset, unsigned int vertexCount) {

        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        const UINT perInstanceStride = sizeof(PerInstanceData);

        ndtech::DirectXHelper::CreateBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_perInstanceDataBuffer,
          perInstanceStride * derivedComponentSystem.m_perInstanceData.size(),
          D3D11_USAGE_DEFAULT,
          D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
          0,
          D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
          perInstanceStride);

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_perInstanceDataBuffer");
          (m_perInstanceDataBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateShaderResourceView(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_perInstanceDataBuffer,
          m_perInstanceDataSRV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_perInstanceData.size()
        );

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_perInstanceDataSRV");
          (m_perInstanceDataSRV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_perInstanceDataBuffer,
          m_perInstanceDataUAV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_perInstanceData.size());

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_perInstanceDataUAV");
          (m_perInstanceDataUAV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif


        const UINT perVertexStride = sizeof(Vertex);

        ndtech::DirectXHelper::CreateBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_perVertexDataBuffer,
          perVertexStride * derivedComponentSystem.m_perVertexData.size(),
          D3D11_USAGE_DEFAULT,
          D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
          0,
          D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
          perVertexStride);

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_perVertexDataBuffer");
          (m_perVertexDataBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateShaderResourceView(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_perVertexDataBuffer,
          m_perVertexDataSRV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_perVertexData.size()
        );

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_perVertexDataSRV");
          (m_perVertexDataSRV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_perVertexDataBuffer,
          m_perVertexDataUAV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_perVertexData.size());

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_perVertexDataUAV");
          (m_perVertexDataUAV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif
      }

            template<typename AppType>
      void CreateDirectionalLight(AppType* app) {


        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        const UINT directionalLightStride = sizeof(DirectionalLight);

        ndtech::DirectXHelper::CreateBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_directionalLightsBuffer,
          directionalLightStride * derivedComponentSystem.m_directionalLights.size(),
          D3D11_USAGE_DEFAULT,
          D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
          0,
          D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
          directionalLightStride);

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_directionalLightsBuffer");
          (m_directionalLightsBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateShaderResourceView(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_directionalLightsBuffer,
          m_directionalLightsSRV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_directionalLights.size()
        );

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_directionalLightsSRV");
          (m_directionalLightsSRV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_directionalLightsBuffer,
          m_directionalLightsUAV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_directionalLights.size());

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_directionalLightsUAV");
          (m_directionalLightsUAV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif
      }

      template<typename AppType>
      void CreatePointLight(AppType* app) {


        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        const UINT pointLightStride = sizeof(PointLight);

        ndtech::DirectXHelper::CreateBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_pointLightsBuffer,
          pointLightStride * derivedComponentSystem.m_pointLights.size(),
          D3D11_USAGE_DEFAULT,
          D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
          0,
          D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
          pointLightStride);

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_pointLightsBuffer");
          (m_pointLightsBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateShaderResourceView(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_pointLightsBuffer,
          m_pointLightsSRV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_pointLights.size()
        );

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_pointLightsSRV");
          (m_pointLightsSRV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_pointLightsBuffer,
          m_pointLightsUAV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_pointLights.size());

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_pointLightsUAV");
          (m_pointLightsUAV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif
      }

      template<typename AppType>
      void CreateConeLight(AppType* app) {


        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        const UINT coneLightStride = sizeof(ConeLight);

        ndtech::DirectXHelper::CreateBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_coneLightsBuffer,
          coneLightStride * derivedComponentSystem.m_coneLights.size(),
          D3D11_USAGE_DEFAULT,
          D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
          0,
          D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
          coneLightStride);

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_coneLightsBuffer");
          (m_coneLightsBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateShaderResourceView(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_coneLightsBuffer,
          m_coneLightsSRV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_coneLights.size()
        );

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_coneLightsSRV");
          (m_coneLightsSRV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif

        ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
          m_coneLightsBuffer,
          m_coneLightsUAV,
          DXGI_FORMAT_UNKNOWN,
          derivedComponentSystem.m_coneLights.size());

#ifdef _DEBUG
        {
          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_coneLightsUAV");
          (m_coneLightsUAV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        }
#endif
      }


      template<typename AppType>
      Component InitializeComponent(Component component, AppType* app) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplHoloLens::InitializeComponent - Begin";

        return component;
      };

      // Uncomment the following to run code before rendering
      //template <typename AppType>
      //void PreRenderComponentSystem(AppType* app) {
      //  LOG(INFO) << "ndtech::components::IndexedInstancedModelSystemImplHoloLens::PreRenderComponentSystem";
      //};

      template <typename RenderingSystemType, typename AppType>
      void RenderComponents(RenderingSystemType* renderingSystem, AppType* app) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplHoloLens::RenderComponents - Begin";

        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        const auto context = renderingSystem->m_deviceResources->GetD3DDeviceContext();

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

        // Update the vertices buffer.
        context->UpdateSubresource(
          m_verticesBuffer.get(),
          0,
          nullptr,
          derivedComponentSystem.m_modelVertices.data(),
          0,
          0
        );

        // Set the vertices SRV on the VertexShader
        ID3D11ShaderResourceView* verticesSRV = m_verticesSRV.get();
        context->VSSetShaderResources(
          0, // start slot
          1, // number of views
          &verticesSRV // pointer to the shader resource view
        );

        // Update the materials buffer.
        context->UpdateSubresource(
          m_materialsBuffer.get(),
          0,
          nullptr,
          derivedComponentSystem.m_materials.data(),
          0,
          0
        );

        // Set the materials SRV on the VertexShader
        ID3D11ShaderResourceView* materialsSRV = m_materialsSRV.get();
        context->VSSetShaderResources(
          1, // start slot
          1, // number of views
          &materialsSRV // pointer to the shader resource view
        );

        // Update the perInstanceData buffer.
        context->UpdateSubresource(
          m_perInstanceDataBuffer.get(),
          0,
          nullptr,
          derivedComponentSystem.m_perInstanceData.data(),
          0,
          0
        );

        // Set the materials SRV on the VertexShader
        ID3D11ShaderResourceView* perInstanceDataSRV = m_perInstanceDataSRV.get();
        context->VSSetShaderResources(
          2, // start slot
          1, // number of views
          &perInstanceDataSRV // pointer to the shader resource view
        );

        // Update the perVertexData buffer.
        context->UpdateSubresource(
          m_perVertexDataBuffer.get(),
          0,
          nullptr,
          derivedComponentSystem.m_perVertexData.data(),
          0,
          0
        );

        // Set the perVertexData SRV on the VertexShader
        ID3D11ShaderResourceView* perVertexDataSRV = m_perVertexDataSRV.get();
        context->VSSetShaderResources(
          3, // start slot
          1, // number of views
          &perVertexDataSRV // pointer to the shader resource view
        );




        if (derivedComponentSystem.m_directionalLights.size()) {

          // Update the directional lights buffer.
          context->UpdateSubresource(
            m_directionalLightsBuffer.get(),
            0,
            nullptr,
            derivedComponentSystem.m_directionalLights.data(),
            0,
            0
          );

          // Set the directional lights SRV on the VertexShader
          ID3D11ShaderResourceView* directionalLightsSRV = m_directionalLightsSRV.get();
          context->VSSetShaderResources(
            4, // start slot
            1, // number of views
            &directionalLightsSRV // pointer to the shader resource view
          );

        }


        context->Draw(
          derivedComponentSystem.m_modelIndices.size() * 2, // Vertex Count = double the indexCount to account for both eyes
          0  // Start Vertex
        );

      };

    };

  }
}