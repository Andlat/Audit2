#include"Timer.h"

using namespace std;

Timer::Timer()
{

}

Timer::~Timer()
{

}
 

void Timer :: timer()
{ 
    while (true) {
        displayClock();
        this_thread::sleep_for(chrono::seconds(1));

     
        seconds++;
        if (seconds == 11) {

            cout << "temps ecoule";
            break;
        }
    }

}


void Timer :: displayClock()
{
    system("CLS");

    cout << setfill('0') << setw(2) << seconds << " sec |" << endl;
}
