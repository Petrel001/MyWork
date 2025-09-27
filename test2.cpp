#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <iostream>
#define SIZE 10001
using namespace std;
int main(){
    FILE *file;
    file = fopen("./test.txt","r");
    if (file == NULL){
        perror("Error");
        return 1;
    }
    char a[2][SIZE][101] = {0};
    char name[101] = "0d000721";
    int num = 0;
    while(fscanf(file," %[^:]:%s",a[0][num],a[1][num])!=EOF){
        num++;
    }
    while(strcmp(name,"Quit")!=0){
        scanf("%s",name);
        bool flag = false;
        for(int i = 0;i<num;i++){
            if(!strcmp(a[0][i],name)){
                flag = true;
                printf("%s\n",a[1][i]);
                break;
            }
        }
        if(!flag&&strcmp(name,"Quit")!=0){
            printf("Error\n");
        }
    }
    return 0;
}