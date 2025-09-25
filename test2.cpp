#include <stdio.h>
#include <ctype.h>
#include <iostream>
using namespace std;
int main(){
    // FILE *file;
    // file = fopen("test.txt","r");
    // if (file == NULL){
    //     perror("Error");
    //     return 1;
    // }
    // scanf("%c",&a);
    // b = a;
    // cout<<b<<endl;
    // // printf("%c",a[1][1]);
    char *a[101];
    for (int i = 0;i<2;i++){
        cin>>a[i];
    }
    // cin>>a;
    // scanf("%s",&a);
    for (int i = 0;i<2;i++){
        printf("%s\n",a[i]);
    }
    return 0;
}