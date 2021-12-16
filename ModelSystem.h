#pragma once

#include <ndtech/BaseApp.h>

#include "Model.h"

#include "ModelSystemImpl.h"

namespace ndtech {
  namespace components {

    template <typename ThisVertexType>
    struct ModelSystem : public ModelSystemImpl<Model<ThisVertexType>, ThisVertexType> {
      using VertexType = ThisVertexType;
      using Component = Model<VertexType>;
      using ThisType = ModelSystem<Component>;

      BaseApp* m_app;

      template <typename AppType>
      void Initialize(AppType* app) {
        ModelSystemImpl<Model<ThisVertexType>, ThisVertexType>::Initialize(app);
      }

      template<typename AppType>
      Component InitializeComponent(Component component, AppType* app) {
        return ModelSystemImpl<Model<ThisVertexType>, ThisVertexType>::InitializeComponent(component, app);
      };

      void UpdateComponent(Component* component, BaseApp* app = nullptr) {

        double elapsedSeconds = app->m_timer.GetElapsedSeconds();

        // Update the orientation
        glm::quat orientationChange =
          glm::angleAxis(
            static_cast<float>(component->m_angularVelocityRadiansPerSecond * app->m_timer.GetElapsedSeconds()),
            component->m_angularVelocityAxis);

        component->m_orientation = orientationChange * component->m_orientation;

        // Update the transformation matrix       
        component->m_transformationMatrix = glm::translate(glm::mat4(), component->m_position) * glm::mat4_cast(component->m_orientation) * glm::scale(component->m_scale);
      };

      void Destroy(std::vector<Component>* components) {
        LOG(INFO) << "ndtech::components::ModelSystem::Destroy typeid(ThisType).name() = " << typeid(ThisType).name();
      };

    };

  }
}