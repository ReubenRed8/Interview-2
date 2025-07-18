#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;
using namespace std::chrono;

mutex mtx;

void printeven(int x, int y){
  while (x<=y){
    lock_guard<mutex> lock(mtx);
    cout << x << endl;
    x=x+2;
}
}

void printodd(int x, int y){
  while (x<=y){
    lock_guard<mutex> lock(mtx);
    cout << x << endl;
    x=x+2;
  }
}

int main(){
  std::thread t1(printeven,0,10);
  std::thread t2(printodd,1,9);
  t1.join();
  t2.join();
  return 0;
}
