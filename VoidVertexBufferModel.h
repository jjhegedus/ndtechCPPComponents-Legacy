#pragma once

#include <ndtech/MultiItemStore.h>

namespace ndtech {
  namespace components {

    using VoidVertexBufferModelItemTypesTypeList =
      ndtech::TypeUtilities::Typelist<
      size_t, // Instance Id
      glm::vec3, // Scale
      glm::vec3, // Position
      glm::quat, // Orientation
      glm::mat4 // world
      >;

    using VoidVertexBufferModelItemTypes = typename VoidVertexBufferModelItemTypesTypeList::type;

    template <typename... ItemType>
    struct VoidVertexBufferModel : MultiItemStore<size_t, ItemType...> {
      size_t                                      m_instanceId = -1;
      size_t                                      m_modelId = -1;

      glm::vec3                                   m_position = { 0.0f, 0.0f, 0.0f };
      glm::vec3                                   m_scale = { 0.0f, 0.0f, 0.0f };
      glm::quat                                   m_orientation = glm::quat();

      glm::mat4                                   m_world = glm::mat4();

      VoidVertexBufferModel() {

      }

      VoidVertexBufferModel(size_t modelId) : m_modelId(modelId) {

      }

      //InstanceType& AddInstance(ItemType...) {
      void AddInstance(ItemType... values) {
        this->AddItem(values...);
      }


    };

  }
}