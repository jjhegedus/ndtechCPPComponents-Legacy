#pragma once
#include "pch.h"

namespace ndtech {
  namespace components {

    // The DerivedVoidVertexBufferSystemType template argument represents the generic VertexBufferSystem which this implementation is implementing
    template <typename DerivedVoidVertexBufferSystemType, typename Component>
    struct VoidVertexBufferSystemImpl {

      unsigned int                                      m_shaderProgram;
      GLuint                                            m_vao;

      glm::mat4x4                                       m_transformationMatrix = glm::mat4x4();

      const GLuint                                      m_vertexStride = sizeof(glm::vec3);
      const GLuint                                      m_vertexCapacity = 1000;
      GLuint                                            m_nextVertexOffset = 0;
      GLuint                                            m_verticesBuffer;
      GLuint                                            m_verticesBufferIndex = 0;


      const GLuint                                      m_materialStride = sizeof(typename DerivedVoidVertexBufferSystemType::MaterialType);
      const GLuint                                      m_materialCapacity = 1000;
      GLuint                                            m_nextMaterialOffset = 0;
      GLuint                                            m_materialsBuffer = 0;
      GLuint                                            m_materialsBufferIndex = 1;

      const GLuint                                      m_perVertexDataStride = sizeof(typename DerivedVoidVertexBufferSystemType::PerVertexDataType);
      const GLuint                                      m_perVertexDataCapacity = 1000;
      GLuint                                            m_nextPerVertexDataOffset = 0;
      GLuint                                            m_perVertexDataBuffer = 0;
      GLuint                                            m_perVertexDataBufferIndex = 2;

      const GLuint                                      m_instanceStride = sizeof(typename DerivedVoidVertexBufferSystemType::PerInstanceDataType);
      const GLuint                                      m_instanceCapacity = 1000;
      GLuint                                            m_nextInstanceOffset = 0;
      GLuint                                            m_instancesBuffer = 0;
      GLuint                                            m_instancesBufferIndex = 3;

      template <typename AppType>
      void Initialize(AppType* app) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplMagicLeap::Initialize : Begin";

        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        m_shaderProgram = app->m_renderingSystem.GetShaderProgramId(L"void-vb-vert.glsl", L"void-vb-frag.glsl");

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_verticesBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_verticesBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_vertexStride * m_vertexCapacity, NULL, GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_verticesBufferIndex, m_verticesBuffer);

        glGenBuffers(1, &m_materialsBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_materialsBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_materialStride * m_materialCapacity, NULL, GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_materialsBufferIndex, m_materialsBuffer);

        glGenBuffers(1, &m_perVertexDataBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_perVertexDataBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_perVertexDataStride * m_perVertexDataCapacity, NULL, GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_perVertexDataBufferIndex, m_perVertexDataBuffer);

        glGenBuffers(1, &m_instancesBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instancesBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_instanceStride * m_instanceCapacity, NULL, GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_instancesBufferIndex, m_instancesBuffer);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
      }

      template <typename AppType, typename ModelData>
      void CreateModel(AppType* app, std::vector<ModelData> vertices, std::vector<unsigned short> indices) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplMagicLeap::CreateModel : Begin";

        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        size_t vertexStride = sizeof(ModelData);


        // Do some work with the m_verticesBuffer as the GL_SHADER_STORAGE_BUFFER
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_verticesBuffer);

        //glm::vec3* modelVertices = (glm::vec3*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, m_nextVertexOffset, m_vertexStride * vertices.size(), GL_MAP_WRITE_BIT);
        ModelData* modelVertices = (ModelData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, m_nextVertexOffset, vertexStride * vertices.size(), GL_MAP_WRITE_BIT || GL_MAP_UNSYNCHRONIZED_BIT);
        
        memcpy(modelVertices, vertices.data(), vertices.size() * vertexStride);
        
        // Make sure the m_verticesBuffer is uploaded
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        m_nextVertexOffset += vertexStride * vertices.size();
      }

      template <typename AppType, typename MaterialType>
      void CreateMaterial(AppType* app, MaterialType material) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplMagicLeap::CreateMaterial : Begin";

        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);


        // Do some work with the m_materialsBuffer as the GL_SHADER_STORAGE_BUFFER
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_materialsBuffer);

        MaterialType *newMaterial = (MaterialType*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, m_nextMaterialOffset, m_materialStride, GL_MAP_WRITE_BIT || GL_MAP_UNSYNCHRONIZED_BIT);

        *newMaterial = material;

        // Make sure the m_materialesBuffer is uploaded
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        m_nextMaterialOffset += m_materialStride;
      }

      template <typename AppType, typename PerInstanceType, typename PerVertexDataType>
      void CreateComponent(AppType* app, PerInstanceType& instance, PerVertexDataType pvd, unsigned int instanceOffset, unsigned int vertexCount) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplMagicLeap::CreateComponent : Begin";

        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);


        // Do some work with the m_perVertexDataBuffer as the GL_SHADER_STORAGE_BUFFER
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_perVertexDataBuffer);

        PerVertexDataType *perVertexData = (PerVertexDataType*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, m_nextPerVertexDataOffset, m_perVertexDataStride, GL_MAP_WRITE_BIT || GL_MAP_UNSYNCHRONIZED_BIT);

        std::memcpy(perVertexData, derivedComponentSystem.m_perVertexData.data(), m_perVertexDataStride * vertexCount);

        // Make sure the m_perVertexDataBuffer is uploaded
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        m_nextPerVertexDataOffset += m_perVertexDataStride * vertexCount;


        // Do some work with the m_instancesBuffer as the GL_SHADER_STORAGE_BUFFER
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instancesBuffer);

        PerInstanceType *newInstance = (PerInstanceType*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, m_nextInstanceOffset, m_instanceStride, GL_MAP_WRITE_BIT || GL_MAP_UNSYNCHRONIZED_BIT);

        *newInstance = instance;

        // Make sure the m_instancesBuffer is uploaded
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        m_nextInstanceOffset += m_instanceStride;
      }

      template<typename AppType>
      void CreateDirectionalLight(AppType* app) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplMagicLeap::CreateDirectionalLight : Begin";


        //        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);
        //
        //        const UINT directionalLightStride = sizeof(DirectionalLight);
        //
        //        ndtech::DirectXHelper::CreateBuffer(
        //          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
        //          m_directionalLightsBuffer,
        //          directionalLightStride * derivedComponentSystem.m_directionalLights.size(),
        //          D3D11_USAGE_DEFAULT,
        //          D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
        //          0,
        //          D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
        //          directionalLightStride);
        //
        //#ifdef _DEBUG
        //        {
        //          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_directionalLightsBuffer");
        //          (m_directionalLightsBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        //        }
        //#endif
        //
        //        ndtech::DirectXHelper::CreateShaderResourceView(
        //          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
        //          m_directionalLightsBuffer,
        //          m_directionalLightsSRV,
        //          DXGI_FORMAT_UNKNOWN,
        //          derivedComponentSystem.m_directionalLights.size()
        //        );
        //
        //#ifdef _DEBUG
        //        {
        //          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_directionalLightsSRV");
        //          (m_directionalLightsSRV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        //        }
        //#endif
        //
        //        ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
        //          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
        //          m_directionalLightsBuffer,
        //          m_directionalLightsUAV,
        //          DXGI_FORMAT_UNKNOWN,
        //          derivedComponentSystem.m_directionalLights.size());
        //
        //#ifdef _DEBUG
        //        {
        //          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_directionalLightsUAV");
        //          (m_directionalLightsUAV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        //        }
        //#endif
      }

      template<typename AppType>
      void CreatePointLight(AppType* app) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplMagicLeap::CreatePointLight : Begin";


        //        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);
        //
        //        const UINT pointLightStride = sizeof(PointLight);
        //
        //        ndtech::DirectXHelper::CreateBuffer(
        //          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
        //          m_pointLightsBuffer,
        //          pointLightStride * derivedComponentSystem.m_pointLights.size(),
        //          D3D11_USAGE_DEFAULT,
        //          D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
        //          0,
        //          D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
        //          pointLightStride);
        //
        //#ifdef _DEBUG
        //        {
        //          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_pointLightsBuffer");
        //          (m_pointLightsBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        //        }
        //#endif
        //
        //        ndtech::DirectXHelper::CreateShaderResourceView(
        //          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
        //          m_pointLightsBuffer,
        //          m_pointLightsSRV,
        //          DXGI_FORMAT_UNKNOWN,
        //          derivedComponentSystem.m_pointLights.size()
        //        );
        //
        //#ifdef _DEBUG
        //        {
        //          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_pointLightsSRV");
        //          (m_pointLightsSRV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        //        }
        //#endif
        //
        //        ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
        //          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
        //          m_pointLightsBuffer,
        //          m_pointLightsUAV,
        //          DXGI_FORMAT_UNKNOWN,
        //          derivedComponentSystem.m_pointLights.size());
        //
        //#ifdef _DEBUG
        //        {
        //          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_pointLightsUAV");
        //          (m_pointLightsUAV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        //        }
        //#endif
      }

      template<typename AppType>
      void CreateConeLight(AppType* app) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplMagicLeap::CreateConeLight : Begin";


        //        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);
        //
        //        const UINT coneLightStride = sizeof(ConeLight);
        //
        //        ndtech::DirectXHelper::CreateBuffer(
        //          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
        //          m_coneLightsBuffer,
        //          coneLightStride * derivedComponentSystem.m_coneLights.size(),
        //          D3D11_USAGE_DEFAULT,
        //          D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
        //          0,
        //          D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
        //          coneLightStride);
        //
        //#ifdef _DEBUG
        //        {
        //          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_coneLightsBuffer");
        //          (m_coneLightsBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        //        }
        //#endif
        //
        //        ndtech::DirectXHelper::CreateShaderResourceView(
        //          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
        //          m_coneLightsBuffer,
        //          m_coneLightsSRV,
        //          DXGI_FORMAT_UNKNOWN,
        //          derivedComponentSystem.m_coneLights.size()
        //        );
        //
        //#ifdef _DEBUG
        //        {
        //          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_coneLightsSRV");
        //          (m_coneLightsSRV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        //        }
        //#endif
        //
        //        ndtech::DirectXHelper::CreateUnorderedAcceesViewBuffer(
        //          app->m_renderingSystem.m_deviceResources->GetD3DDevice(),
        //          m_coneLightsBuffer,
        //          m_coneLightsUAV,
        //          DXGI_FORMAT_UNKNOWN,
        //          derivedComponentSystem.m_coneLights.size());
        //
        //#ifdef _DEBUG
        //        {
        //          std::string d3d11debugname("ndtech::components::VoidVertexBufferSystemImplHoloLens::m_coneLightsUAV");
        //          (m_coneLightsUAV)->SetPrivateData(WKPDID_D3DDebugObjectName, d3d11debugname.size(), d3d11debugname.c_str());
        //        }
        //#endif
      }

      template<typename AppType>
      Component InitializeComponent(Component component, AppType* app) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplMagicLeap::InitializeComponent - Begin";

        return component;
      };

      // Uncomment the following to run code before rendering
      //template <typename AppType>
      //void PreRenderComponentSystem(AppType* app) {
      //  LOG(INFO) << "ndtech::components::IndexedInstancedModelSystemImplMagicLeap::PreRenderComponentSystem";
      //};

      template <typename RenderingSystemType, typename AppType>
      void RenderComponents(RenderingSystemType* renderingSystem, AppType* app, int cameraIndex) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplMagicLeap::RenderComponents - Begin";
        glUseProgram(m_shaderProgram);

        auto currentCamera = app->m_renderingSystem.m_virtual_camera_array.virtual_cameras[cameraIndex];
        glm::mat4 viewProjectionMatrix = glm::make_mat4(currentCamera.projection.matrix_colmajor) * glm::inverse(ndtech::utilities::MLTransformToGLMMat4(currentCamera.transform));

        // Load the viewProjectionMatrix
        int viewProjectionMatrixLocation = glGetUniformLocation(m_shaderProgram, "ViewProjectionMatrix");

        if (viewProjectionMatrixLocation >= 0) {
          if (viewProjectionMatrixLocation == GL_INVALID_VALUE) {
            LOGF(INFO, "ndtech::VoidVertexBufferSystemImplMagicLeap::RenderComponents - Invalid Location for ViewProjectionMatrix");
          }

        }
        else
        {
          LOGF(INFO, "ndtech::VoidVertexBufferSystemImplMagicLeap::RenderComponents - Unable to retrieve location of ViewProjectionMatrix");
        }

        glUniformMatrix4fv(viewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));


        // Load the model world matrix
        int worldMatrixLocation = glGetUniformLocation(m_shaderProgram, "ModelWorldMatrix");

        if (worldMatrixLocation >= 0) {
          if (worldMatrixLocation == GL_INVALID_VALUE) {
            LOGF(INFO, "ndtech::VoidVertexBufferSystemImplMagicLeap::RenderComponents - Invalid Location for ModelWorldMatrix");
          }

        }
        else
        {
          LOGF(INFO, "ndtech::MagicLeapRenderingSystem::RenderComponents - Unable to retrieve location of ModelWorldMatrix");
        }

        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_transformationMatrix));

        // Bind the vertex array
        glBindVertexArray(m_vao);

        // Bind the vertexBuffer
        //glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        // Bind the indexBuffer
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component.m_ibo);

        //// Draw the triangles !
        //glDrawElements(
        //  GL_TRIANGLES,      // mode
        //  component.m_indices.size(),    // count
        //  GL_UNSIGNED_SHORT,   // type
        //  (void*)0           // element array buffer offset
        //);


        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        using ModelDataType = typename DerivedVoidVertexBufferSystemType::ModelDataType;
        using PerVertexDataType = typename DerivedVoidVertexBufferSystemType::PerVertexDataType;


        // Do some work with the m_instancesBuffer as the GL_SHADER_STORAGE_BUFFER
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instancesBuffer);

        void *instancesPointer = (void*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_instanceStride * derivedComponentSystem.m_perInstanceData.size(), GL_MAP_WRITE_BIT);

        memcpy(instancesPointer, derivedComponentSystem.m_perInstanceData.data(), derivedComponentSystem.m_perInstanceData.size() * m_instanceStride);

        // Make sure the m_instancesBuffer is uploaded
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);




        // Do some work with the m_perVertexDataBuffer as the GL_SHADER_STORAGE_BUFFER
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_perVertexDataBuffer);

        PerVertexDataType *perVertexData = (PerVertexDataType*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_perVertexDataStride * derivedComponentSystem.m_perVertexData.size(), GL_MAP_WRITE_BIT);

        std::memcpy(perVertexData, derivedComponentSystem.m_perVertexData.data(), m_perVertexDataStride * derivedComponentSystem.m_perVertexData.size());

        // Make sure the m_perVertexDataBuffer is uploaded
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);



        //// Do some work with the m_verticesBuffer as the GL_SHADER_STORAGE_BUFFER
        //glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_verticesBuffer);

        //using PerVertexDataType = DerivedVoidVertexBufferSystemType::PerVertexDataType;

        //PerVertexDataType *verticesPointer = (PerVertexDataType*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_perVertexDataStride * derivedComponentSystem.m_perVertexData.size(), GL_MAP_WRITE_BIT);

        //memcpy(verticesPointer, derivedComponentSystem.m_perVertexData.data(), derivedComponentSystem.m_perVertexData.size() * m_perVertexDataStride);

        //// Make sure the m_verticesBuffer is uploaded
        //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


        // Do some work with the m_verticesBuffer as the GL_SHADER_STORAGE_BUFFER
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_verticesBuffer);

        unsigned int vertexStride = sizeof(ModelDataType);
        ModelDataType* modelVertices = (ModelDataType*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, vertexStride * derivedComponentSystem.m_modelVertices.size(), GL_MAP_WRITE_BIT);

        memcpy(modelVertices, derivedComponentSystem.m_modelVertices.data(), derivedComponentSystem.m_modelVertices.size() * vertexStride);

        // Make sure the m_verticesBuffer is uploaded
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);



        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_verticesBufferIndex, m_verticesBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_materialsBufferIndex, m_materialsBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_perVertexDataBufferIndex, m_perVertexDataBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_instancesBufferIndex, m_instancesBuffer);


        //glDrawArrays(GL_TRIANGLES, 0, derivedComponentSystem.m_indices.size() * 2);
        //glDrawArrays(GL_TRIANGLES, 0, derivedComponentSystem.m_modelVertices.size());
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, derivedComponentSystem.m_perVertexData.size());
      };


      template <typename AppType>
      void UpdateVertex(AppType* app, unsigned int vertexId) {
        LOG(INFO) << "ndtech::components::VoidVertexBufferSystemImplMagicLeap::UpdateInstance : Begin";

        DerivedVoidVertexBufferSystemType& derivedComponentSystem = static_cast<DerivedVoidVertexBufferSystemType&>(*this);

        using PerVertexDataType = typename DerivedVoidVertexBufferSystemType::PerVertexDataType;

        // Do some work with the m_perVertexDataBuffer as the GL_SHADER_STORAGE_BUFFER
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_perVertexDataBuffer);

        PerVertexDataType *perVertexData = (PerVertexDataType*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, m_perVertexDataStride * vertexId, m_perVertexDataStride, GL_MAP_WRITE_BIT);

        std::memcpy(perVertexData, derivedComponentSystem.m_perVertexData.data() + (m_perVertexDataStride * vertexId), m_perVertexDataStride);

        // Make sure the m_perVertexDataBuffer is uploaded
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        auto vertexData = derivedComponentSystem.m_perVertexData[vertexId];

        // Do some work with the m_instancesBuffer as the GL_SHADER_STORAGE_BUFFER
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instancesBuffer);

        void *instancePointer = (void*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, m_instanceStride * vertexData.instanceId, m_instanceStride, GL_MAP_WRITE_BIT);

        std::memcpy(instancePointer, derivedComponentSystem.m_perInstanceData.data() + (m_instanceStride * vertexData.instanceId), m_instanceStride);

        // Make sure the m_instancesBuffer is uploaded
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
      }

    };

  }
}