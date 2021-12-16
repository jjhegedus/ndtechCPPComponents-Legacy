#pragma once

#include "pch.h"

#include <vector>

#include "IndexedInstancedModelImpl.h"
#include "IndexedInstancedModelInstance.h"

namespace ndtech {
  namespace components {

    template <typename ThisVertexType>
    struct IndexedInstancedModel : public IndexedInstancedModelImpl<ThisVertexType> {
      using VertexType = ThisVertexType;
      using InstanceType = IndexedInstancedModelInstance;

      std::vector<VertexType>                                 m_vertices;
      std::vector<unsigned short>                             m_indices;
      std::vector<InstanceType>                               m_instances;

      std::vector<glm::vec3>                                  m_instanceScales;
      std::vector<glm::vec3>                                  m_instancePositions;
      std::vector<glm::quat>                                  m_instanceOrientations;
      std::vector<glm::vec3>                                  m_instanceVelocities;
      std::vector<glm::quat>                                  m_instanceAngularVelocities;
      std::vector<glm::mat4>                                  m_instanceTransformationMatrices;

      std::vector<InstanceRenderingData>                      m_instanceRenderingData;

      InstanceType& AddInstance(InstanceType inputInstance) {
        m_instanceScales.push_back(inputInstance.m_scale);
        m_instanceScales.push_back(inputInstance.m_scale);
        m_instancePositions.push_back(inputInstance.m_position);
        m_instanceOrientations.push_back(inputInstance.m_orientation);
        m_instanceVelocities.push_back(inputInstance.m_velocity);
        m_instanceAngularVelocities.push_back(inputInstance.m_angularVelocity);
        m_instanceTransformationMatrices.push_back(inputInstance.m_transformationMatrix);

        m_instanceRenderingData.push_back(InstanceRenderingData{ inputInstance.m_transformationMatrix });

        return *m_instances.insert(m_instances.end(), inputInstance);
      }

    };

  }
}