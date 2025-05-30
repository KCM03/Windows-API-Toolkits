#include "SerialPort.h"



void SerialPort::OptionalPrint(std::string Message) {

    if (PrintStatus == TRUE) {
        std::cout << Message << std::endl;
    }

    else
    {
        return;
    }

}

SerialPort::SerialPort(int ComNum, DWORD BAUDRATE, int ByteSize,BOOL PrintStatus) {

    this->PrintStatus = PrintStatus;

    std::string PortName = "\\\\.\\COM" + std::to_string(ComNum);
    LPCSTR FileName = PortName.c_str();

    this->hComm = CreateFileA(FileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);


    if (this->hComm == INVALID_HANDLE_VALUE) {

        this->OptionalPrint("Error Opening Serial Port!");
        return;
    }

    else {

        this->OptionalPrint("Serial Port Opened Successfully!");
        PurgeComm(this->hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);



        this->dcbSerialParams.DCBlength = sizeof(this->dcbSerialParams);



        this->status = GetCommState(this->hComm, &this->dcbSerialParams);
        if (this->status == FALSE) { this->OptionalPrint("Couldn't Get Com State!"); }

        this->dcbSerialParams.BaudRate = BAUDRATE; //eg. CBR_115200;
        this->dcbSerialParams.ByteSize = ByteSize; ////eg. 8;
        this->dcbSerialParams.StopBits = ONESTOPBIT;
        this->dcbSerialParams.Parity = NOPARITY;


        this->status = SetCommState(this->hComm, &this->dcbSerialParams);
        if (this->status == FALSE) { OptionalPrint("Couldn't Set Com State!"); }

        this->Timeouts.ReadIntervalTimeout = 50;
        this->Timeouts.ReadTotalTimeoutConstant = 50;
        this->Timeouts.ReadTotalTimeoutMultiplier = 10;
        this->Timeouts.WriteTotalTimeoutConstant = 50;
        this->Timeouts.WriteTotalTimeoutMultiplier = 10;

        if (SetCommTimeouts(this->hComm, &this->Timeouts) == FALSE) {

            this->OptionalPrint("Failed to Set Timeout Conditions!");
        }

        return;

    }

    return;

}

SerialPort::SerialPort()
{

}

SerialPort::~SerialPort() {

    bool print = this->PrintStatus;

    CloseHandle(this->hComm);

    if (print == TRUE) {

        std::cout << "Serial Port Closed!"<<std::endl;
    }
    return;

}

std::vector<char> SerialPort::getSerialBuffer() {
    return this->SerialBuffer;
}
BOOL SerialPort::SerialRead() {

    while (this->BUSY) {}

    this->BUSY = TRUE;

    DWORD NoBytesRead;
    char ReadData{ 0 };
    PurgeComm(this->hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);

    this->status = SetCommMask(this->hComm, EV_RXCHAR);
    if (this->status == FALSE) { this->OptionalPrint("Failed to Set Comm Mask(RXCHAR)!");

        return this->status;
    }


    this->SerialBuffer.clear();

    this->status = WaitCommEvent(this->hComm, &this->dwEventMask, NULL);
    if (this->status == FALSE) {
        this->OptionalPrint("RX-Wait Timeout");
        return this->status;

    }

    do {
        this->status = ReadFile(this->hComm, &ReadData, sizeof(ReadData),&NoBytesRead, NULL);
        if (this->status == FALSE) {
            this->OptionalPrint("Failed to Read Byte!");
        }
        this->SerialBuffer.push_back(ReadData);
    } while (NoBytesRead > 0);

    this->BUSY = FALSE;
    return this->status;
}

BOOL SerialPort::SerialRead_NB()
{
    DWORD NoBytesRead;
    char ReadData{ 0 };
    PurgeComm(this->hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);

    this->status = SetCommMask(this->hComm, EV_RXCHAR);
    if (this->status == FALSE) {
        this->OptionalPrint("Failed to Set Comm Mask(RXCHAR)!");

        return this->status;
    }


    this->SerialBuffer.clear();



    do {
        this->status = ReadFile(this->hComm, &ReadData, sizeof(ReadData), &NoBytesRead, NULL);
        if (this->status == FALSE) {
            this->OptionalPrint("Failed to Read Byte!");
        }
        if (ReadData != NULL) {
            this->SerialBuffer.push_back(ReadData);
        }
    } while (NoBytesRead > 0);

    return this->status;
}


BOOL SerialPort::SerialWrite() {

    while(this->BUSY){


    }

    this->BUSY = TRUE;
    DWORD BytesWritten{ 0 };
    PurgeComm(this->hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
    this->status = WriteFile(this->hComm, &this->SerialBuffer, sizeof(SerialBuffer), &BytesWritten, NULL);

    if (this->status == FALSE) {
        this->OptionalPrint("Failed to Write Message!");
    }
    this->BUSY = false;
    return this->status;
}

BOOL SerialPort::SerialWrite_External(LPCVOID input) {

    PurgeComm(this->hComm,PURGE_TXCLEAR);

    int TotalBytes{ 0 };
    DWORD BytesWritten{ 0 };
    std::string str = (char*)input;

    do {
        char byte = str[TotalBytes];
        this->status = WriteFile(this->hComm, &byte, sizeof(byte), &BytesWritten, NULL);

        if (this->status == FALSE) {
            this->OptionalPrint("Failed to Send Byte!");
        }

        else {

        }
        TotalBytes++;
    } while (TotalBytes < str.length());

    if (this->status == FALSE) {
        this->OptionalPrint("Failed to Write Message!");
    }
    else{
        this->OptionalPrint("Succesfully Wrote:" + std::string((char*)input) +"("+ std::to_string(TotalBytes)+" Bytes)");
        this->OptionalPrint(std::to_string(TotalBytes));
    }
    return this->status;
}

void SerialPort::setStatus(BOOL Status) {

    this->status = Status;

}
BOOL SerialPort::getStatus() {

    return this->status;
}
