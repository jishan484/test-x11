#include <iostream>
#include <tuple>
#include <functional>

using namespace std;

std::function<void()> callBack;



template<typename Func, typename... Args>
struct nest {

    void setup(Func func1, Args... args) {
        callBack = [func1, args...]()
        {
            (func1)(args...);
        };
    }

    unsigned process() {
        callBack();
        return 0;
    }
};

template<typename Func, typename... Args>
void handleFunc(Func func, Args&&... args) {
    nest<Func, Args...> myNest;
    myNest.setup(func, args...);
}


void check(int i){
    cout << "Value is : "<< i << endl; 
}


int main()
{
   handleFunc(check,8);
   callBack();
   return 0;
}
