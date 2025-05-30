#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
class SerialPort {


public:

    SerialPort();
    virtual ~SerialPort();
    SerialPort(int ComNum,DWORD BAUDRATE,int ByteSize,BOOL PrintStatus);

    std::vector<char> getSerialBuffer();
    BOOL SerialRead();
    BOOL SerialRead_NB();
    BOOL SerialWrite();
    BOOL SerialWrite_External(LPCVOID input);
    void setStatus(BOOL Status);
    BOOL getStatus();
private:


    void OptionalPrint(std::string message);
    HANDLE hComm;
    std::vector<char> SerialBuffer;
    DCB dcbSerialParams{ 0 };
    COMMTIMEOUTS Timeouts;
    BOOL status{TRUE};
    BOOL PrintStatus{ 0 }; //enable this if you want error messages!
    DWORD dwEventMask{ 0 };
    BOOL BUSY{ FALSE };


};
