#pragma once

namespace ndtech {
  namespace components {

    template <typename Component, typename VertexType>
    struct ModelSystemImpl {
    public:
      unsigned int m_shaderProgram;

      template <typename AppType>
      void Initialize(AppType* app) {
        m_shaderProgram = app->m_renderingSystem.GetShaderProgramId(L"vertex-shader.glsl", L"fragment-shader.glsl");
        LOG(INFO) << "ndtech::components::ModelSystemMagiLeapImpl::Initialize : typeid(decltype(this)).name() = " << typeid(decltype(this)).name();
      }

      template<typename AppType>
      Component InitializeComponent(Component component, AppType* app) {
        char* versionGL = (char *)(glGetString(GL_VERSION));
        char* vendor = (char *)glGetString(GL_VENDOR);
        glGenBuffers(1, &component.m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, component.m_vbo);
        glBufferData(GL_ARRAY_BUFFER, component.m_vertices.size() * sizeof(VertexType), component.m_vertices.data(), GL_STATIC_DRAW);

        glGenVertexArrays(1, &component.m_vao);
        glBindVertexArray(component.m_vao);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), NULL);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void*)(sizeof(glm::vec3)));

        glGenBuffers(1, &component.m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component.m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, component.m_indices.size() * sizeof(unsigned short), component.m_indices.data(), GL_STATIC_DRAW);

        return component;
      }

      template <typename RenderingSystemType, typename AppType>
      void RenderComponents(RenderingSystemType* renderingSystem, AppType* app, int cameraIndex) {
        //LOG(INFO) << "ndtech::components::ModelSystemMagicLeapImpl::RenderComponents - Begin";

        std::vector<Component> componentVector = std::get<std::vector<Component>>(app->m_componentVectors);

        for (typename AppType::EntityIndexType componentIndex = 0; componentIndex < app->m_freeComponentIndices[TypeUtilities::IndexOf<Component, typename AppType::Components>()]; componentIndex++) {
          Component& component = componentVector.at(componentIndex);
          RenderComponent(component, app, cameraIndex);
        }


      };

      template <typename AppType>
      void RenderComponent(Component component, AppType* app, int cameraIndex) {
        glUseProgram(m_shaderProgram);

        auto currentCamera = app->m_renderingSystem.m_virtual_camera_array.virtual_cameras[cameraIndex];
        glm::mat4 viewProjectionMatrix = glm::make_mat4(currentCamera.projection.matrix_colmajor) * glm::inverse(ndtech::utilities::MLTransformToGLMMat4(currentCamera.transform));

        // Load the viewProjectionMatrix
        int viewProjectionMatrixLocation = glGetUniformLocation(m_shaderProgram, "ViewProjectionMatrix");

        if (viewProjectionMatrixLocation >= 0) {
          if (viewProjectionMatrixLocation == GL_INVALID_VALUE) {
            LOGF(INFO, "ndtech::MagicLeapRenderingSystem::Render - Invalid Location for ViewProjectionMatrix");
          }

        }
        else
        {
          LOGF(INFO, "ndtech::MagicLeapRenderingSystem::Render - Unable to retrieve location of ViewProjectionMatrix");
        }

        glUniformMatrix4fv(viewProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));


        // Load the model world matrix
        int worldMatrixLocation = glGetUniformLocation(m_shaderProgram, "ModelWorldMatrix");

        if (worldMatrixLocation >= 0) {
          if (worldMatrixLocation == GL_INVALID_VALUE) {
            LOGF(INFO, "ndtech::MagicLeapRenderingSystem::Render - Invalid Location for ModelWorldMatrix");
          }

        }
        else
        {
          LOGF(INFO, "ndtech::MagicLeapRenderingSystem::Render - Unable to retrieve location of ModelWorldMatrix");
        }

        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, glm::value_ptr(component.m_transformationMatrix));

        // Bind the vertex array
        glBindVertexArray(component.m_vao);

        // Bind the vertexBuffer
        glBindBuffer(GL_ARRAY_BUFFER, component.m_vbo);

        // Bind the indexBuffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component.m_ibo);

        // Draw the triangles !
        glDrawElements(
          GL_TRIANGLES,      // mode
          component.m_indices.size(),    // count
          GL_UNSIGNED_SHORT,   // type
          (void*)0           // element array buffer offset
        );

      };

    };

  }
}