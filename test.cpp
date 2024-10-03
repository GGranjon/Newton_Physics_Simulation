#include <iostream>
#include <vector>
#include <string.h>
using namespace std;


struct test{
    int a;
    int b;
};

void fct_test(int& a){
    a = 3;
}

int main(int argc, char const *argv[])
{
    vector<int*> test;
    int a =1;
    int b = 2;
    int* ptr1 = &a;
    int* ptr2 = &b;
    test.push_back(ptr1);
    test.push_back(ptr2);
    for (int* elt : test){
        if (elt == test[test.size() - 1]){
            cout<<"salut!\n";
        }
        else{
            cout<<"prems!\n";
        }
    }
    cout<<2e5;
    return 0;
}

