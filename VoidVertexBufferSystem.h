#pragma once

#include "VoidVertexBufferSystemImpl.h"
#include "VoidVertexBufferModel.h"
#include <ndtech\BaseApp.h>

namespace ndtech {
  namespace components {

    static std::atomic<size_t>                             VoidVertexBufferSystem_nextModelId;

    struct Vertex {
      unsigned int instanceId;
      unsigned int index;
      unsigned int eye;
      unsigned int materialId;
    };

    struct CPUOnlyInstanceData {
      glm::vec3 position;
      glm::vec3 scale;
      glm::quat orientation;
      size_t modelId;
    };

    struct PerInstanceData {
      glm::mat4 world;
    };

    struct Material {
      glm::vec4 color;
    };

    struct Light {
      glm::vec3 color;
      float intensity;
    };

    struct DirectionalLight : public Light {
      glm::vec3 direction;
    };

    struct PointLight : public Light {
      glm::vec3 location;
    };

    struct ConeLight : public Light {
      glm::vec3 location;
      glm::vec3 direction;
      float innerAngle;
      float outerAngle;
    };

    struct ModelVertex {
      glm::vec3 position;
      float pad;
    };


    struct VoidVertexBufferSystem : public VoidVertexBufferSystemImpl<VoidVertexBufferSystem, VoidVertexBufferModel<size_t, size_t, glm::mat4>> {
      using ThisType = VoidVertexBufferSystem;
      using Component = VoidVertexBufferModel<size_t, size_t, glm::mat4>;

      using ModelIndexType = unsigned int;

      using PerVertexDataType = Vertex;
      using ModelDataType = ModelVertex;
      using CPUOnlyInstanceDataType = CPUOnlyInstanceData;
      using PerInstanceDataType = PerInstanceData;
      using MaterialType = Material;
      using LightType = Light;
      using DirectionalLightType = DirectionalLight;
      using PointLightType = PointLight;
      using ConeLightType = ConeLight;

      size_t numberOfVertices;

      BaseApp* m_app;

      std::vector<Vertex>                                    m_perVertexData;
      std::vector<PerInstanceData>                                  m_perInstanceData;
      std::vector<CPUOnlyInstanceData>                              m_cpuOnlyInstanceData;
      std::vector<Material>                                         m_materials;

      std::atomic<unsigned short>                                   m_nextInstanceId;

      std::vector<ModelVertex>                                      m_modelVertices;
      std::vector<ModelIndexType>                                   m_modelIndices;

      std::map<size_t, std::tuple<size_t, size_t, size_t, size_t>>  m_models;
      // vertexStart, vertices.size(), m_indices.size(), indices.size()

      // *** m_models data model ***
      // each entry in m_models is a pair
      // the first item in the pair is the model id
      // the second item in the pair is a set of data that describes how this model's data is stored.
      // model data is actually stored in the m_vertices and m_indices vectors
      // the first element is the offset of this models verticies from the beginning of the m_vertices vector
      // the second element is the number of vertices
      // the thrid element is the offset of this models indices from the beginning of the m_indices vector
      // the fourth element is the number of indices

      std::vector<DirectionalLight>                                 m_directionalLights;
      std::vector<PointLight>                                       m_pointLights;
      std::vector<ConeLight>                                        m_coneLights;




      //VoidVertexBufferSystem() {

      //}

      //VoidVertexBufferSystem(const VoidVertexBufferSystem& other) :
      //  VoidVertexBufferSystemImpl<VoidVertexBufferSystem, VoidVertexBufferModel<size_t, size_t, glm::mat4>>(other),
      //  m_perVertexData(other.m_perVertexData),
      //  m_perInstanceData(other.m_perInstanceData),
      //  m_cpuOnlyInstanceData(other.m_cpuOnlyInstanceData),
      //  m_materials(other.m_materials),
      //  m_vertices(other.m_vertices),
      //  m_indices(other.m_indices),
      //  m_models(other.m_models)
      //{

      //}

      void UpdateComponent(Component* component, BaseApp* app = nullptr) {
        // Update the transformation matrix for each instance
        int index = 0;
        for (CPUOnlyInstanceData cpuOnlyInstanceData : m_cpuOnlyInstanceData) {
          m_perInstanceData[index++].world = glm::translate(glm::mat4(), cpuOnlyInstanceData.position) * glm::mat4_cast(cpuOnlyInstanceData.orientation) * glm::scale(cpuOnlyInstanceData.scale);
        }

      };

      template <typename AppType>
      size_t CreateModel(std::vector<ModelVertex> vertices, std::vector<unsigned short> indices, AppType* app) {

        size_t matchingModelId = -1;

        for (std::pair<size_t, std::tuple<size_t, size_t, size_t, size_t>> testModelPair : m_models) {

          bool matches = true;
          size_t verticesIndex = 0;
          for (size_t verticesMemberIndex = std::get<0>(testModelPair.second); verticesMemberIndex < std::get<0>(testModelPair.second) + std::get<1>(testModelPair.second); verticesMemberIndex++) {
            if (vertices.at(verticesIndex).position != m_modelVertices.at(verticesMemberIndex).position) {
              matches = false;
              break;
            }

            verticesIndex++;
          }

          if (matches) {

            size_t indicesIndex = 0;
            for (size_t indicesMemberIndex = std::get<2>(testModelPair.second); indicesMemberIndex < std::get<2>(testModelPair.second) + std::get<3>(testModelPair.second); indicesMemberIndex++) {
              if (indices.at(indicesIndex) != m_modelIndices.at(indicesMemberIndex)) {
                matches = false;
                break;
              }

              indicesIndex++;
            }

            if (matches) {
              matchingModelId = testModelPair.first;
              break;
            }
            else {
              matches = true;
            }
          }
          else {
            matches = true;
          }
        }

        if (matchingModelId == -1) {
          // add a new model
          matchingModelId = VoidVertexBufferSystem_nextModelId.fetch_add(1);
          size_t vertexStart = m_modelVertices.size();
          m_models.insert(std::make_pair(matchingModelId, std::make_tuple(vertexStart, vertices.size(), m_modelIndices.size(), indices.size())));
          m_modelVertices.insert(m_modelVertices.end(), vertices.begin(), vertices.end());

          m_modelIndices.insert(m_modelIndices.end(), indices.begin(), indices.end());

          VoidVertexBufferSystemImpl<VoidVertexBufferSystem, VoidVertexBufferModel<size_t, size_t, glm::mat4>>::CreateModel(app, vertices, indices);
        }

        return matchingModelId;

      };

      template <typename AppType>
      unsigned int CreateMaterial(AppType* app, glm::vec4 color) {
        m_materials.push_back(Material{ color });

        VoidVertexBufferSystemImpl<VoidVertexBufferSystem, VoidVertexBufferModel<size_t, size_t, glm::mat4>>::CreateMaterial(app, Material{ color });

        return m_materials.size() - 1;
      };

      template <typename AppType>
      Component CreateComponent(size_t modelId, unsigned int materialId, glm::vec3 position, glm::vec3 scale, glm::quat orientation, AppType* app) {

        unsigned int instanceId = m_nextInstanceId.fetch_add(1);

        std::tuple<size_t, size_t, size_t, size_t> model = m_models.at(modelId);

        unsigned int vertexStart = std::get<0>(model);
        unsigned int vertexCount = std::get<1>(model);
        unsigned int indexStart = std::get<2>(model);
        unsigned int indexCount = std::get<3>(model);

        unsigned int instanceOffset = m_perVertexData.size();


        for (size_t indexPos = 0; indexPos < indexCount; indexPos++) {

          Vertex pvd{
            instanceId,
            static_cast<unsigned int>(m_modelIndices.at(indexPos) + vertexStart),
            0,  // left eye
            materialId };

          m_perVertexData.push_back(pvd);
        }


        // Have to enter these into the instance data twice to handle both eyes
        for (size_t indexPos = 0; indexPos < indexCount; indexPos++) {

          Vertex pvd{
            instanceId,
            static_cast<unsigned int>(m_modelIndices.at(indexPos) + vertexStart),
            1, // right eye
            materialId };

          m_perVertexData.push_back(pvd);
        }

        m_cpuOnlyInstanceData.push_back(
          CPUOnlyInstanceData{
            position,
            scale,
            orientation
          }
        );

        Component component(modelId);
        component.m_position = position;
        component.m_scale = scale;
        component.m_orientation = orientation;

        component.m_world = glm::translate(glm::mat4(), position) * glm::mat4_cast(orientation) * glm::scale(scale);

        component.m_instanceId = m_perInstanceData.size();

        PerInstanceData pid{ component.m_world };
        m_perInstanceData.push_back(pid);

        VoidVertexBufferSystemImpl<VoidVertexBufferSystem, VoidVertexBufferModel<size_t, size_t, glm::mat4>>::CreateComponent(app, pid, Vertex{instanceId, 0, 0, materialId}, instanceOffset, vertexCount * 2);

        return component;
      }

      size_t GetModelVertexCount(size_t modelId) {
        return std::get<1>(m_models.at(modelId));
      }

      size_t GetModelIndexCount(size_t modelId) {
        return std::get<3>(m_models.at(modelId));
      }

      Component GetComponent(size_t index) {
        if (index > m_perInstanceData.size()) {
          throw "index is greater than m_perInstanceData.size()";
        }

        CPUOnlyInstanceData cpuOnlyInstanceData = m_cpuOnlyInstanceData[index];

        Component component(cpuOnlyInstanceData.modelId);

        component.m_position = cpuOnlyInstanceData.position;
        component.m_scale = cpuOnlyInstanceData.scale;
        component.m_orientation = cpuOnlyInstanceData.orientation;

        return component;
      }

      void Destroy(std::vector<Component>* components) {
        LOG(INFO) << "ndtech::components::ModelSystem::Destroy typeid(ThisType).name() = " << typeid(ThisType).name();
      };


      template<typename AppType>
      void CreateDirectionalLight(DirectionalLight directionalLight, AppType* app) {
        m_directionalLights.push_back(directionalLight);

        VoidVertexBufferSystemImpl<VoidVertexBufferSystem, VoidVertexBufferModel<size_t, size_t, glm::mat4>>::CreateDirectionalLight(app);
      }

      template<typename AppType>
      void CreatePointLight(PointLight pointLight, AppType* app) {
        m_pointLights.push_back(pointLight);

        VoidVertexBufferSystemImpl<VoidVertexBufferSystem, VoidVertexBufferModel<size_t, size_t, glm::mat4>>::CreatePointLight(app);
      }

      template<typename AppType>
      void CreateConeLight(ConeLight coneLight, AppType* app) {
        m_coneLights.push_back(coneLight);

        VoidVertexBufferSystemImpl<VoidVertexBufferSystem, VoidVertexBufferModel<size_t, size_t, glm::mat4>>::CreateConeLight(app);
      }

      template<typename AppType>
      void SetMaterialByVertexId(AppType* app, unsigned int vertexId, unsigned int materialId) {
        m_perVertexData[vertexId].materialId = materialId;

        //VoidVertexBufferSystemImpl<VoidVertexBufferSystem, VoidVertexBufferModel<size_t, size_t, glm::mat4>>::UpdateVertex(app, vertexId);
      }

    };

  }
}