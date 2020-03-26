//
// Created by clemens on 19.03.19.
//

#include "TriggerController.h"

using namespace selfomat::camera::gphoto;
using namespace selfomat::tools;

const std::string TriggerController::TAG = "TriggerController";

TriggerController::TriggerController(GPContext *gp, Camera *camera, CameraWidget *rootWidget)
        : BaseController(gp, camera, rootWidget) {
    // Find the widgets we need in order to focus and trigger the camera
    LOG_D(TAG, "Looking for trigger widget");

    if (findWidget("eosremoterelease", &triggerWidget)) {
        mode = TRIGGER_MODE_EOSREMOTERELEASE;
        return;
    }

    mode = TRIGGER_MODE_DEFAULT;
}

bool TriggerController::trigger(bool force_legacy_trigger) {
    LOG_D(TAG, "Triggering");

    if (mode == TRIGGER_MODE_DEFAULT || force_legacy_trigger) {
        bool success = GP_OK == gp_camera_trigger_capture(camera, gp);
        if (!success) {
            LOG_E(TAG, "Error triggering capture");
        }
        return success;
    } else if (mode == TRIGGER_MODE_EOSREMOTERELEASE) {
        bool success = true;

        success &= (GP_OK == gp_widget_set_value(triggerWidget, "Immediate"));
        success &= (GP_OK == gp_camera_set_config(camera, rootWidget, gp));

        if (!success) {
            LOG_E(TAG, "Error triggering capture");
        }
        return success;
    } else {
        LOG_E(TAG, "Invalid Trigger Mode");
        return false;
    }
}

bool TriggerController::releaseTrigger() {


    switch (mode) {
        case TRIGGER_MODE_DEFAULT:
            // No need
            return true;
        case TRIGGER_MODE_EOSREMOTERELEASE: {
            LOG_D(TAG, "Releasing Trigger");
            bool success = true;

            success &= (GP_OK == gp_widget_set_value(triggerWidget, "Release Full"));
            success &= (GP_OK == gp_camera_set_config(camera, rootWidget, gp));
            success &= (GP_OK == gp_widget_set_value(triggerWidget, "None"));
            success &= (GP_OK == gp_camera_set_config(camera, rootWidget, gp));

            if (!success) {
                LOG_E(TAG, "Error releasing trigger");
            }
            return success;
        }
        default:
            LOG_E(TAG, "Invalid Trigger Mode");
            return false;
    }
}

bool TriggerController::supportsCamera() {
    // We hope so
    return true;
}

bool TriggerController::pullSettings() {
    return true;
}
