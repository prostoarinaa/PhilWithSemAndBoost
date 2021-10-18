#include <iostream>
#include <string>
#include <vector>
//#include <mutex>
#include <semaphore>
#include <boost/optional.hpp>
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/timer/timer.hpp>
//#include <boost/pthread/mutex.hpp>

//#include <thread>
#include <windows.h>
//#include <process.h>

using namespace std;

//vector<int> Prioritet = {5,5,5,5,5,5};

#define N 5
int numOfPhilosopher = 0;
int status[N];


//int status[N];
boost::mutex mtx, mtx1, mtx2;
HANDLE m_hSemaphore;

////vector<counting_semaphore<5>> vecOfSem[5];

class PHILOSOPHER {

public:
    int numOfPhil = 0;
    PHILOSOPHER(const int& numOfPhilo) {
        // process();
        numOfPhil = numOfPhilo;
    };
    void process() {
        while (1) {

            Thinking();
            mtx2.lock();
            TakeFork();
            Eating();
            PutFork();
            // Prioritet[numOfPhil]--;
            boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));
            mtx2.unlock();
        }
    };

    void Eating() {
        mtx1.lock();
        cout << "Philosopher N " << numOfPhil << " " << "is eating" << endl;
        mtx1.unlock();
    };
    void Thinking() {
        mtx1.lock();
        cout << "Philosopher N " << numOfPhil << " " << "is thinking" << endl;
        mtx1.unlock();
    };
    void CheckForkIsFree(int numOfPhil) {
        if (status[numOfPhil] == 0 && status[(numOfPhil - 1) % N] != 1 && status[(numOfPhil + 1) % N] != 1) {
            status[numOfPhil] = 1;

            // mtx2.lock();
            ReleaseSemaphore(m_hSemaphore, 1, NULL);
        }

    };
    void TakeFork() {
        mtx.lock();
        status[numOfPhil] = 0;
        CheckForkIsFree(numOfPhil);
        mtx.unlock();
        // mtx2.unlock()   ;
    };
    void PutFork() {
        mtx.lock();
        status[numOfPhil] = 0;
        CheckForkIsFree((numOfPhil - 1) % N);
        CheckForkIsFree((numOfPhil + 1) % N);
        mtx.unlock();
        ReleaseSemaphore(m_hSemaphore, -1, NULL);
    };
};

int main() {
    boost::thread::id id;

    vector <PHILOSOPHER> vecOfPhilosophers = {};
    for (int i = 0; i < 5; i++) {
        numOfPhilosopher = i;
        vecOfPhilosophers.push_back(PHILOSOPHER(numOfPhilosopher));
    }
    for (int i = 0; i < 5; i++) {
        // numOfPhilosopher = i;
        cout << vecOfPhilosophers[i].numOfPhil << endl;
        // vecOfPhilosophers[i].process();
    }
    //thread thread_array[5];
    boost::thread thread_array0(boost::bind(&PHILOSOPHER::process, &vecOfPhilosophers[0]));
    boost::thread thread_array1(boost::bind(&PHILOSOPHER::process, &vecOfPhilosophers[1]));
    boost::thread thread_array2(boost::bind(&PHILOSOPHER::process, &vecOfPhilosophers[2]));
    boost::thread thread_array3(boost::bind(&PHILOSOPHER::process, &vecOfPhilosophers[3]));
    boost::thread thread_array4(boost::bind(&PHILOSOPHER::process, &vecOfPhilosophers[4]));

        thread_array0.join();
        thread_array1.join();
        thread_array2.join();
        thread_array3.join();
        thread_array4.join();
        thread_array0.join();
   

    return 0;
}