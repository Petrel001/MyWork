#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <cstdlib>
using namespace std;
typedef struct{
    bool is_admin = false;
    int id;
    char name[20];
    char password[20];
}users;
int option(char*op){
    if(strcmp(op,"Login")){
        return 1;
    }else if(strcmp(op,"Exit")){
        return 2;
    }else if(strcmp(op,"Quit")){
        return 3;
    }else{
        return 0;
    }
}
int main(){
    system("chcp 65001");
    users user;
    bool is_login = false;
    char op[10];
    while(strcmp(op,"Quit")!=0){
        printf("输入:");
        scanf("%s",op);
        if(strcmp(op,"Login")==0){
            if(is_login){
                printf("已登录\n");
            }else{
                printf("请输入用户名\n");
                printf("输入:");
                scanf("%s",user.name);
                printf("登录成功\n");
                is_login = true;
            }
        }else if(strcmp(op,"Exit")==0){
            if(is_login){
                printf("已退出登录\n");
                is_login = false;
            }else{
                printf("还没有登录\n");
            }
        }else if(strcmp(op,"Quit")!=0){
            printf("非法输入\n");
        }
    }
    printf("程序退出\n");
    return 0;
}