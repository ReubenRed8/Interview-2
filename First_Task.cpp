#include <iostream>
#include <thread>
#include <mutex>
using namespace std;


mutex mtx;
bool even = true;

void printeven(int x, int y) {
    while (x<=y) {
        while(true){
            lock_guard<mutex> lock(mtx);
            if (even){
                cout << x << endl;
                x=x+2;
                even = false;
                break;
            }
        }
    }    
}


void printodd(int x, int y){
    while (x<=y) {
        while(true){
            lock_guard<mutex> lock(mtx);
            if (!even){
                cout << x << endl;
                x += 2;
                even = true;
                break;
            }
        }
    } 
}

int main(){
//   cout << "Hi" << endl;
  std::thread t1(printeven,0,10);
  std::thread t2(printodd,1,9);
  t1.join();
  t2.join();
  return 0;
}