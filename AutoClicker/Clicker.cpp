#include "Clicker.h"

int KeyClicker::count{ 0 };
int MouseClicker::count{ 0 };



KeyController::KeyController()
{
    OnTime = std::chrono::milliseconds(NULL);
    OffTime = std::chrono::milliseconds(NULL);
    std::cout << "Creating Key Controller!" << std::endl;
	Mutex.lock();
	//this->keys = key;
	Thread = new std::thread;
	Mutex.unlock();
}

KeyController::KeyController(int OnTimeMs, int OffTimeMs)
{
   
   OnTime = std::chrono::milliseconds(OnTimeMs);
   OffTime = std::chrono::milliseconds(OffTimeMs);
   std::cout << "Creating Key Controller!" << std::endl;
   Mutex.lock();
   //this->keys = key;
   Thread = new std::thread;
   Mutex.unlock();
}

void KeyController::run()
{
	//do nothing...
}

void KeyController::abort()
{
	//do nothing...
}

KeyController::~KeyController()
{
    std::cout << "Deleting Key Controller!" << std::endl;
	delete this->Thread;
}

std::vector<INPUT> KeyController::getKeys()
{
   
    return this->Keys;
}

void KeyController::setOnTime(int milliseconds)
{
    this->OnTime = std::chrono::milliseconds(milliseconds);
}

void KeyController::setOffTime(int milliseconds)
{
    this->OffTime = std::chrono::milliseconds(milliseconds);
}

int KeyController::getOnTime()
{
    return this->OnTime.count();
}

int KeyController::getOffTime()
{
    return this->OffTime.count();
}

void KeyController::AddKey(int key)
{
    INPUT NewInput{};
    NewInput.type = INPUT_KEYBOARD;
    NewInput.ki.wVk = key;
    Keys.push_back(NewInput);

}

void KeyController::AddInput(INPUT Input)
{
    this->Keys.push_back(Input);
}





KeyClicker::KeyClicker()
{
    
	count++;
}

void KeyClicker::run()
{
    Mutex.lock();
    active = TRUE;
    Mutex.unlock();
    *(this->Thread) = std::thread([&] {

        while (active) {

            this->Click();
            std::this_thread::sleep_for(this->OffTime);
        }
       
        std::cout << "Thread Stopped!\n";
        }
    
    );
       
    Thread->detach();
	//finish later...
}

void KeyClicker::abort()
{

    if (this->active) {
        Mutex.lock();
        this->active = FALSE;
        Mutex.unlock();
        std::cout << "Aborting...\n";
    }
    
}

void KeyClicker::AddKey(int key) 
{
    KeyController::AddKey(key);
    INPUT NewInput{};
    NewInput.type = INPUT_KEYBOARD;
    NewInput.ki.wVk = key;
    NewInput.ki.dwFlags = KEYEVENTF_KEYUP;
    this->KeyUps.push_back(NewInput);
} 

KeyClicker::~KeyClicker()
{
    std::cout << "Deleting KeyClicker!\n";
    this->abort();
    count--;
}

bool KeyClicker::Click()
{

    std::vector<INPUT> start =this->getKeys();
    std::vector<INPUT> stop = this->KeyUps;
    
    UINT sent = SendInput(start.size(), start.data(), sizeof(INPUT));

    if (sent != start.size()) {

        std::cout << "Failed to SendInput!\n";
        return false;
    }
    else {
        std::this_thread::sleep_for(this->OnTime); //last here!
        sent = SendInput(stop.size(), stop.data(), sizeof(INPUT));
        if (sent != stop.size()) {
            std::cout << "Failed to SendInput!\n";
            return false; 

       }
        else {
            std::cout << "Successful Input\n";
            return true;
        }
         
    } 


    
   
}

int KeyClicker::getCount()
{
    return KeyClicker::count;
}

MouseClicker::MouseClicker()
{
    count++;
}

void MouseClicker::run()
{
    Mutex.lock();
    active = TRUE;
    Mutex.unlock();
    *(this->Thread) = std::thread([&] {

        while (active) {

            this->Click();
            std::this_thread::sleep_for(this->OffTime);
        }

        std::cout << "Thread Stopped!\n";
        }

    );

    Thread->detach();
}

void MouseClicker::abort()
{
    if (this->active) {
        Mutex.lock();
        this->active = FALSE;
        Mutex.unlock();
        std::cout << "Aborting...\n";
    }
    
}

void MouseClicker::AddKey(int key)
{
    INPUT NewInput{}, Newinput_KEYUP{};

    Newinput_KEYUP.type = NewInput.type = INPUT_MOUSE;
    
    
    switch (key) {

    case VK_LMOUSE:
     
        NewInput.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
        Newinput_KEYUP.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
        break;
    

    case VK_RMOUSE:
    
        NewInput.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN);
        Newinput_KEYUP.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP);
        break;
 
    case VK_MIDDLE_MOUSE: 
        NewInput.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MIDDLEDOWN);
        Newinput_KEYUP.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MIDDLEUP);
        break;

       
    
    default:

        std::cerr << "Error: Invalid Mouse Input!\n";
        break;

    }

    this->AddInput(NewInput);
    this->KeyUps.push_back(Newinput_KEYUP);
}

MouseClicker::~MouseClicker()
{
    std::cout << "Deleting MouseClicker!\n";
    this->abort();
    count--;
}

bool MouseClicker::Click()
{
   const std::vector<INPUT>& start = getKeys();
   const std::vector<INPUT>& stop = KeyUps;


   UINT sent = SendInput(start.size(), (LPINPUT)start.data(), sizeof(INPUT));

   if (sent != start.size()) {

       std::cout << "Failed to SendInput!\n";
       return false;
   }
   else {
       std::this_thread::sleep_for(this->OnTime); //last here!
       sent = SendInput(stop.size(), (LPINPUT)stop.data(), sizeof(INPUT)); //check this...
       if (sent != stop.size()) {
           std::cout << "Failed to SendInput!\n";
           return false;

       }
       else {
           std::cout << "Successful Input\n";
           return true;
       }

   }


   
}

int MouseClicker::getCount()
{
    return MouseClicker::count;
}
