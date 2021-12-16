#include "pch.h"

#include "hand_tracking.h"
#include <g3log/g3log.hpp>

#if NDTECH_ML
#endif


void ndtech::components::hand_tracking::HandTrackerSystem::UpdateComponent(HandTracker* handTracker, ndtech::BaseApp* app) {
#if NDTECH_ML  

    MLResult result = MLHandTrackingGetStaticData(handTracker->handTrackingHandle, &handTracker->handTrackingStaticData);
    if (result == MLResult_PrivilegeDenied) {
      ML_LOG(Info, "%s: result of MLHandTrackingGetStaticData is %s : PrivilegeDenied", log_context, MLGetResultString(result));
      return;
    }
    else if (result != MLResult_Ok) {
      ML_LOG(Info, "%s: result of MLHandTrackingGetStaticData is %s", log_context, MLGetResultString(result));
      return;
    }

    result = MLHandTrackingGetData(handTracker->handTrackingHandle, &handTracker->handTrackingData);
    if (result == MLResult_PrivilegeDenied) {
      ML_LOG(Info, "%s: result of MLHandTrackingGetData is %s : PrivilegeDenied", log_context, MLGetResultString(result));
      return;
    }
    else if (result != MLResult_Ok) {
      ML_LOG(Info, "%s: result of MLHandTrackingGetData is %s", log_context, MLGetResultString(result));
      return;
    }


    //ML_LOG(
    //  Info,
    //  "%s: this->handTrackingData.left_hand_state.hand_center_normalized before update = {%f, %f, %f}",
    //  log_context,
    //  handTracker->handTrackingData.left_hand_state.hand_center_normalized.x,
    //  handTracker->handTrackingData.left_hand_state.hand_center_normalized.y,
    //  handTracker->handTrackingData.left_hand_state.hand_center_normalized.z);

    //ML_LOG(Info, "%s: left_hand_state.hand_confidence before update = %f", log_context, handTracker->handTrackingData.left_hand_state.hand_confidence);

    if (handTracker->handTrackingData.left_hand_state.keypose == MLHandTrackingKeyPose::MLHandTrackingKeyPose_Ok) {

      ML_LOG(Info, "%s: left_hand_state.keypoints_mask includes OK: %s", log_context, handTracker->handTrackingData.left_hand_state.keypoints_mask[MLHandTrackingKeyPose::MLHandTrackingKeyPose_Ok] ? "true" : "false");
    }

    if (handTracker->handTrackingData.left_hand_state.keypose == MLHandTrackingKeyPose::MLHandTrackingKeyPose_Fist) {

      ML_LOG(Info, "%s: left_hand_state.keypoints_mask includes Fist: %s", log_context, handTracker->handTrackingData.left_hand_state.keypoints_mask[MLHandTrackingKeyPose::MLHandTrackingKeyPose_Fist] ? "true" : "false");
      app->m_applicationContext.m_state = false;
    }

    if (handTracker->handTrackingData.left_hand_state.keypose == MLHandTrackingKeyPose::MLHandTrackingKeyPose_C) {

      ML_LOG(Info, "%s: left_hand_state.keypoints_mask includes C: %s", log_context, handTracker->handTrackingData.left_hand_state.keypoints_mask[MLHandTrackingKeyPose::MLHandTrackingKeyPose_C] ? "true" : "false");

      //this->application_context.dummy = false;
    }

    if (handTracker->handTrackingData.left_hand_state.keypose == MLHandTrackingKeyPose::MLHandTrackingKeyPose_Finger) {

      ML_LOG(Info, "%s: left_hand_state.keypoints_mask includes Finger", log_context);
      handTracker->onMLHandTrackingKeyPose_Finger(handTracker, app);
    }

    if (handTracker->handTrackingData.left_hand_state.keypose == MLHandTrackingKeyPose::MLHandTrackingKeyPose_L) {

      ML_LOG(Info, "%s: left_hand_state.keypoints_mask includes L", log_context);
    }

    if (handTracker->handTrackingData.left_hand_state.keypose == MLHandTrackingKeyPose::MLHandTrackingKeyPose_NoPose) {

      ML_LOG(Info, "%s: left_hand_state.keypoints_mask includes NoPose", log_context);
    }

    if (handTracker->handTrackingData.left_hand_state.keypose == MLHandTrackingKeyPose::MLHandTrackingKeyPose_OpenHandBack) {

      ML_LOG(Info, "%s: left_hand_state.keypoints_mask includes OpenHandBack", log_context);
      handTracker->onMLHandTrackingKeyPose_OpenHandBack(handTracker, app);
    }

    if (handTracker->handTrackingData.left_hand_state.keypose == MLHandTrackingKeyPose::MLHandTrackingKeyPose_Pinch) {

      ML_LOG(Info, "%s: left_hand_state.keypoints_mask includes Pinch", log_context);
    }

    if (handTracker->handTrackingData.left_hand_state.keypose == MLHandTrackingKeyPose::MLHandTrackingKeyPose_Thumb) {

      ML_LOG(Info, "%s: left_hand_state.keypoints_mask includes Thumb", log_context);

      app->m_applicationContext.m_state = false;
    }
#endif

#if NDTECH_HOLO
#endif 

}

void ndtech::components::hand_tracking::HandTrackerSystem::Destroy(std::vector<HandTracker>* handTrackers){

#if NDTECH_ML  

  for (auto handTracker = handTrackers->begin(); handTracker != handTrackers->end(); handTracker++) {
    if (handTracker->handTrackingHandle) {
      MLResult result = MLHandTrackingDestroy(handTracker->handTrackingHandle);
      if (result != MLResult_Ok) {
        LOG(INFO) << "Error destroying hand tracker";
      }
    }
  }
#endif

#if NDTECH_HOLO
#endif 
}