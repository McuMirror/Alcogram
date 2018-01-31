#pragma once

enum DeviceName {
    CAMERA, BILL_ACCEPTOR, POS, ALCOTESTER, PRINTER, NON_DEVICE
};

enum RequestName {
    // common requests for all devices
    START_DEVICE, FINISH_DEVICE, RESTART_DEVICE, CHECK_STATUS, CHECK_CONNECTION,

    // camera requests
    GET_IMAGE, TAKE_IMAGE, STOP_GET_IMAGE,

    // alcotester requests
    WARMING_UP_ALCOTESTER, COOLING_DOWN_ALCOTESTER, ACTIVATE_ALCOTESTER,

    // POS requests
    ACTIVATE_POS, DEACTIVATE_POS, TAKE_MONEY,

    // printer requests
    WARMING_UP_PRINTER, COOLING_DOWN_PRINTER, PRINT_IMAGE
};

const int NO_ERROR = 0;
const int ERROR = 1;
const int DEVICE_CONNECTED = 2;
const int REQUEST_TIMEOUT = 3;
const int POS_NOT_ENOUGH_MONEY = 4;
const int POS_WRONG_PIN_CODE = 5;
const int DEVICE_NOT_CONNECTED = 6;

// contains information about a result of a request
class Status
{
public:
    Status(int errorCoder, DeviceName deviceName, RequestName requestName);

    int getErrorCode() const;
    DeviceName getDeviceName() const;
    RequestName getRequestName() const;

private:
    int _errorCode;
    DeviceName _deviceName;
    RequestName _requestName;
};
