#pragma once

#include "configManager.h"
#include "stateMachine.h"
#include "devices/machinery.h"
#include "interfaces/faceDetectionInterface.h"
#include "sessionData.h"
#include "devicesChecker.h"

enum PageName {
    NONCRITICAL_ERROR_PAGE = 0
    , CRITICAL_ERROR_PAGE = 1
    , SPLASH_SCREEN_NONCRITICAL_ERROR_PAGE = 2
    , SPLASH_SCREEN_ETERNAL_SLEEP_PAGE = 3
    , SPLASH_SCREEN_PAGE = 4
    , START_PAGE = 5
    , TAKE_PHOTO_PAGE = 6
    , PAY_PAGE = 7
    , ALCOTEST_PAGE = 8
    , PHOTO_PRINT_PAGE = 9
};

class MainWindowInterface {
public:
    // sets pageName as active
    // @param pageName - name of the page to set active
    virtual void setPage(PageName pageName) = 0;

    // returns ConfigManager
    virtual ConfigManager* getConfigManager() const = 0;

    // returns Machinary
    virtual Machinery* getMachinery() const = 0;

    // returns FaceDetectorInterface
    virtual FaceDetectionInterface* getFaceDetector() const = 0;

    // move to state
    virtual void goToState(StateName targetState) = 0;

    // returns current state machine state
    virtual StateName getCurrentStateName() const = 0;

    // returns SessionData
    virtual SessionData& getSessionData() = 0;

    // returns DeviceChecker
    virtual DevicesChecker& getDevicesChecker() = 0;

    // switch interface language
    virtual void switchLanguage() = 0;
};
