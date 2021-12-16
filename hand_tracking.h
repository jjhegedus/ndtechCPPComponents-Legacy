#pragma once

#include <ndtech/pch.h>
#include <vector>
#include <functional>
#include <ndtech/App.h>


#if NDTECH_HOLO
#else
#include <ml_hand_tracking.h>
#endif // HOLO

namespace ndtech {
  namespace components {
    namespace hand_tracking{

      const char log_context[] = "ndtech.components.hand_tracking";

      struct HandTracker {

#if NDTECH_ML
        MLHandle handTrackingHandle;
        MLHandTrackingStaticData handTrackingStaticData;
        MLHandTrackingData handTrackingData;
        MLHandTrackingConfiguration handTrackingConfiguration;

        std::function<void(ndtech::components::hand_tracking::HandTracker* handTracker, ndtech::BaseApp* app)> onMLHandTrackingKeyPose_Finger;
        std::function<void(ndtech::components::hand_tracking::HandTracker* handTracker, ndtech::BaseApp* app)> onMLHandTrackingKeyPose_OpenHandBack;
#endif
      };

      struct HandTrackerSystem {
        using Component = HandTracker;


        template<typename AppType>
        HandTracker InitializeComponent(HandTracker handTracker, AppType* app) {

#if NDTECH_ML  
          MLResult result;
          result = MLHandTrackingCreate(&handTracker.handTrackingHandle);
          if (result == MLResult_PrivilegeDenied) {
            ML_LOG(Info, "%s: result of MLHandTrackingCreate is %s : PrivilegeDenied", log_context, MLGetResultString(result));
          }
          else if (result != MLResult_Ok) {
            const char* resString = MLGetResultString(result);
            ML_LOG(Info, "%s: result of MLHandTrackingCreate is %s", log_context, MLGetResultString(result));
          }

          result = MLHandTrackingSetConfiguration(handTracker.handTrackingHandle, &handTracker.handTrackingConfiguration);
          if (result == MLResult_PrivilegeDenied) {
            ML_LOG(Info, "%s: result of MLHandTrackingSetConfiguration is %s : PrivilegeDenied", log_context, MLGetResultString(result));
          }
          else if (result != MLResult_Ok) {
            ML_LOG(Info, "%s: result of MLHandTrackingSetConfiguration is %s", log_context, MLGetResultString(result));
          }
#endif

#if NDTECH_HOLO
#endif 

          return handTracker;
        }

        void UpdateComponent(HandTracker* handTracker, BaseApp* app = nullptr);
        void Destroy(std::vector<HandTracker>* handTrackers);
      };
    }
  }
}