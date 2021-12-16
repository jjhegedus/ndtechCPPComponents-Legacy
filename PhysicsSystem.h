#pragma once

#include "PhysicsComponent.h"
#include <ndtech/BaseApp.h>
#include <ndtech/TypeUtilities.h>
#include "ModelSystem.h"

namespace ndtech {
  namespace components {

    template <typename VertexType>
    struct PhysicsSystem {
      using Component = PhysicsComponent;
      using Dependencies = ndtech::TypeUtilities::Typelist<ModelSystem<VertexType>>;

      void UpdateComponent(PhysicsComponent component, BaseApp* app) {
      

      
      };

    };

  }
}