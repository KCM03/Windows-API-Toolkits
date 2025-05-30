#pragma once
#include <mutex>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <chrono>

#define VK_LMOUSE 0x00
#define VK_RMOUSE 0x01
#define VK_MIDDLE_MOUSE 0x02

class KeyController {

public:

	
    KeyController();
	KeyController(int OnTimeMs, int OffTimeMs);
	virtual void run();
	virtual void abort();
	virtual ~KeyController();
	
	std::vector<INPUT> getKeys();
	void setOnTime(int milliseconds);
	void setOffTime(int milliseconds);
	int getOnTime();
	int getOffTime();
	virtual void AddKey(int key);
	virtual void AddInput(INPUT Input);


private:

	
	BOOL state{ TRUE };
	std::vector<INPUT> Keys{};

protected:


	std::chrono::milliseconds OnTime;
	std::chrono::milliseconds OffTime;
	bool active{ false };
	std::mutex Mutex;
	std::thread* Thread;
};

class KeyClicker :public KeyController {

public:
   

	KeyClicker();
	KeyClicker(int OnTimeMs,int OffTimeMs)
		:KeyController(OnTimeMs,OffTimeMs){};
	void run() override;
	void abort() override;
	void AddKey(int key) override;
	~KeyClicker();
	bool Click();
	static int getCount();
	std::vector<INPUT> KeyUps{};
	
private:
	
	static int count;
}; 

class MouseClicker :public KeyController
{
public:

	MouseClicker();
	MouseClicker(int OnTimeMs, int OffTimeMs)
		:KeyController(OnTimeMs, OffTimeMs) {};
	void run() override;
	void abort() override;
	void AddKey(int key) override;
	~MouseClicker();
	bool Click();
	static int getCount();
	std::vector<INPUT> KeyUps{};

private:
	static int count;

};

