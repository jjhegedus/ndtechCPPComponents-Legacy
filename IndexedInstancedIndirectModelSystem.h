#pragma once

#include <ndtech/BaseApp.h>
#include "IndexedInstancedModel.h"
#include "IndexedInstancedIndirectModelSystemImpl.h"
#include "IndexedInstancedModelInstance.h"



namespace ndtech {
  namespace components {

    template <typename ThisVertexType>
    struct IndexedInstancedIndirectModelSystem : public IndexedInstancedIndirectModelSystemImpl<IndexedInstancedModel<ThisVertexType>, ThisVertexType> {
      using VertexType = ThisVertexType;
      using Component = IndexedInstancedModel<VertexType>;
      using ThisType = IndexedInstancedIndirectModelSystem<Component>;

      BaseApp* m_app;

      template <typename AppType>
      void Initialize(AppType* app) {
        IndexedInstancedIndirectModelSystemImpl<Component, ThisVertexType>::Initialize(app);
      }

      template<typename AppType>
      Component InitializeComponent(Component component, AppType* app) {
        return IndexedInstancedIndirectModelSystemImpl<Component, ThisVertexType>::InitializeComponent(component, app);
      };

      void UpdateComponent(Component* component, BaseApp* app = nullptr) {
        // Update the transformation matrix for each instance
        int i = 0;
        for (typename Component::InstanceType& instance : component->m_instances) {
          instance.m_transformationMatrix = glm::translate(glm::mat4(), instance.m_position) * glm::mat4_cast(instance.m_orientation) * glm::scale(instance.m_scale);
          component->m_instanceRenderingData[i] = InstanceRenderingData{ instance.m_transformationMatrix };
          i++;
        }
      };

      void Destroy(std::vector<Component>* components) {
        LOG(INFO) << "ndtech::components::ModelSystem::Destroy typeid(ThisType).name() = " << typeid(ThisType).name();
      };


    };
  }
}