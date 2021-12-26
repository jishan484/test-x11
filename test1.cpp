#include<stdio.h>
#include<iostream>
#include<thread>
#include<functional>
void check(int i){ printf("%d\n",i);}
// template <typename... T>
// void DoSmth2(T &&...param)
// {
//     check(param...);
// }
// template <typename P, typename... T>
// void DoSmth(P predicate, T &&...param)
// {
//     predicate(param...);
// }
// int main()
// {
//     DoSmth(check, 7);
//     DoSmth2(7);
// }



// int main(){

//     func = (cb_t)nothing;
//     func(1);
//     printf("ook");
// }

#include <stdio.h>
#include"Frame.h"

void nothing(Frame &frame)
{
    printf("%d\n",frame.height);
    // return 'r';
}

typedef void* (*cb_t)(void *...);
cb_t func;
void *params;
template <typename P, typename... T>
void DoSmth(P predicate, T &&...param)
{
    func = (cb_t)nothing;
    params = param...;
}


int main(void)
{
    Frame frame(400,200);
    
    // int i;

    // func = (cb_t)nothing;
    DoSmth(nothing,frame);
    func(params);
    printf("%d\n",func);
    frame.show();
    return 0;
}