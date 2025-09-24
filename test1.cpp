#include <iostream>
#include <stdio.h>
using namespace std;
int main(){
    string name;
    while(name != "Quit"){
        cin>>name;
        if(name == "Dian"){
            printf("2002\n");
        }else{
            printf("Error\n");
        }
        cout<<name<<endl;
    }
    return 0;
}