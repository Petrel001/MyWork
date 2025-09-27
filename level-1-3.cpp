#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <cstdlib>
using namespace std;

typedef struct{
    int floor,is_reserved = 0;
    char name[21];
}seat;
typedef struct{
    char name[21];
    bool is_admin = false;
    seat Reservations[101];
    int count;
}users;
typedef struct{
    char name;
    int day,floor,row,col;
    int active;
}reservation;

const char* day_names[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
seat se[20][20][10][7];
users user[101];
int num;

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
void print_seat(int seat[4][4]){
    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            printf("%d ",seat[i][j]);
        }
        printf("\n");
    }
}
int main(){
    system("chcp 65001");

    // FILE *rfile = fopen("data.bat","rb");
    // FILE *wfile = fopen("data.dat","wb");
    FILE *file = fopen("data.bat","rb");
    if(file == NULL){
        perror("数据文件错误");
        return 1;
    }
    fread(&num,sizeof(int),1,file);
    fread(&user,sizeof(users),num,file);
    fread(&se,sizeof(se),20*20*10*7,file);
    users current_user;
    bool is_login = false;
    char op[10];

    while(strcmp(op,"Quit")!=0){
        printf("输入:");
        scanf("%s",op);
        if(strcmp(op,"Login")==0){
            if(is_login){
                printf("已登录\n");
                fflush(stdin);
            }else{
                printf("请输入用户名\n");
                printf("输入:");
                scanf("%s",current_user.name);
                printf("登录成功\n");
                is_login = true;
            }
        }else if(strcmp(op,"Exit")==0){
            if(is_login){
                printf("已退出登录\n");
                is_login = false;
            }else{
                printf("还没有登录\n");
                fflush(stdin);
            }
        }else if(strcmp(op, "Monday") == 0 || 
                strcmp(op, "Tuesday") == 0 ||
                strcmp(op, "Wednesday") == 0 ||
                strcmp(op, "Thursday") == 0 ||
                strcmp(op, "Friday") == 0 ||
                strcmp(op, "Saturday") == 0 ||
                strcmp(op, "Sunday") == 0) {
            if(!is_login){
                printf("还没有登录\n");
                fflush(stdin);
            }else{
                int day = -1;
                for (int i = 0; i < 7; i++) {
                    if (strcasecmp(op, day_names[i]) == 0) {
                        day = i;
                        break;
                    }
                }
                int flo;
                scanf(" %s %d",op,&flo);
                for(int i = 0;i<4;i++){
                    for(int j = 0;j<4;j++){
                        if(se[i][j][flo][day].is_reserved){
                            if(strcmp(se[i][j][flo][day].name,current_user.name)==0){
                                printf("2");
                            }else{
                                printf("1");
                            }
                        }else{
                            printf("0");
                        }
                    }
                    printf("\n");
                }
            }
        }else if(strcmp(op,"Reserve")==0){
            scanf(" %s",op);
            if(strcmp(op, "Monday") == 0 || 
                strcmp(op, "Tuesday") == 0 ||
                strcmp(op, "Wednesday") == 0 ||
                strcmp(op, "Thursday") == 0 ||
                strcmp(op, "Friday") == 0 ||
                strcmp(op, "Saturday") == 0 ||
                strcmp(op, "Sunday") == 0){
                int day = -1;
                for (int i = 0; i < 7; i++) {
                    if (strcasecmp(op, day_names[i]) == 0) {
                        day = i;
                        break;
                    }
                }
                char s[10];
                int flo,row,col;
                scanf(" %s %d %s %d %d",op,&flo,s,&row,&col);
                col -=1;
                row -= 1;
                if(se[row][col][flo][day].is_reserved==0){
                    printf("OK\n");
                    se[row][col][flo][day].is_reserved = 1;
                    strcpy(se[row][col][flo][day].name,current_user.name);
                }else{
                    printf("NO\n");
                }
            }else{
                printf("输入异常\n");
                fflush(stdin);
            }
        }else if(strcmp(op,"Quit")!=0){
            printf("输入异常\n");
            fflush(stdin);
        }
        
    }
    printf("程序退出\n");

    return 0;
}