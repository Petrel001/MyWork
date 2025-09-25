#include <iostream>
#include <stdio.h>
#include <cstring>
using namespace std;
/*
bool if_the_same(char a[],char id[]){
    for(int i = 0;i<strlen(a);i++){
        if(a[i]!=id[i]){
            return 0;
        }
    }
    return 1;
}
*/
int main(){
    char name[1001];
    char a[5];
    while(strcmp(name,"Quit")!=0){
        scanf("%s",&name);
        if(strcmp(name,"Dian")==0){
            printf("2002\n");
        }else if(strcmp(name,"Quit")!=0){
            printf("Error\n");
        }
//        printf("%s\n",name);
//        printf("%d\n",a == "Dian");
    }
    return 0;
}