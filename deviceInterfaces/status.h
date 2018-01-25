#pragma once


class Status
{
public:
    Status(int errorCoder)
        : _errorCode(errorCoder)
    {
    }

    int getErrorCode() const
    {
        return _errorCode;
    }
private:
    int _errorCode;
};
