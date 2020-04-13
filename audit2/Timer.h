#pragma once
#include <iomanip> 
#include <iostream> 
#include <stdlib.h> 
#include <Windows.h>
#include <io.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;

class Timer
{
private:
	int seconds = 0;
	
public:
	Timer();
	~Timer();
	void timer();
	void displayClock();
};