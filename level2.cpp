#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_USERS 100
#define MAX_SEATS 100
#define MAX_RESERVATIONS 1000
#define DAYS 7
#define FLOORS 5
#define ROWS 4
#define COLS 4
#define FILENAME "library_data.txt"

typedef struct {
    int floor;
    int row;
    int col;
} Seat;

typedef struct {
    char username[50];
    Seat reservations[MAX_RESERVATIONS];
    int reservation_count;
} User;

typedef struct {
    int day;        // 0-6: Monday-Sunday
    int floor;      // 1-5
    int row;        // 1-4
    int col;        // 1-4
    char username[50];
    int active;     // 1 for active, 0 for cancelled
} Reservation;

// 全局变量
User users[MAX_USERS];
Reservation reservations[MAX_RESERVATIONS];
int user_count = 0;
int reservation_count = 0;
char current_user[50] = "";
int is_admin = 0;

// 函数声明
void initialize_system();
void save_data();
void load_data();
int find_user(const char* username);
void login();
void logout();
void quit_program();
void show_seats(int day, int floor);
void reserve_seat(int day, int floor, int row, int col);
void show_user_reservations();
void clear_data();
void admin_help();
void admin_reserve();
void admin_cancel();
void admin_add_seat();
void admin_remove_seat();
void cancel_day_reservations(int day);
void cancel_floor_reservations(int floor);
int is_seat_available(int day, int floor, int row, int col);
void show_all_reservations();
void show_user_reservations_admin();

const char* day_names[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

void initialize_system() {
    load_data();
}

void save_data() {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        printf("无法保存数据到文件\n");
        return;
    }
    
    // 保存用户数据
    fprintf(file, "USERS:%d\n", user_count);
    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s:%d\n", users[i].username, users[i].reservation_count);
    }
    
    // 保存预约数据
    fprintf(file, "RESERVATIONS:%d\n", reservation_count);
    for (int i = 0; i < reservation_count; i++) {
        fprintf(file, "%d:%d:%d:%d:%s:%d\n", 
                reservations[i].day, reservations[i].floor, 
                reservations[i].row, reservations[i].col,
                reservations[i].username, reservations[i].active);
    }
    
    fclose(file);
}

void load_data() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("创建新的数据文件\n");
        return;
    }
    
    char line[256];
    
    // 加载用户数据
    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "USERS:%d", &user_count);
        for (int i = 0; i < user_count; i++) {
            if (fgets(line, sizeof(line), file)) {
                int count;
                sscanf(line, "%[^:]:%d", users[i].username, &count);
                users[i].reservation_count = count;
            }
        }
    }
    
    // 加载预约数据
    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "RESERVATIONS:%d", &reservation_count);
        for (int i = 0; i < reservation_count; i++) {
            if (fgets(line, sizeof(line), file)) {
                int active;
                sscanf(line, "%d:%d:%d:%d:%[^:]:%d", 
                       &reservations[i].day, &reservations[i].floor,
                       &reservations[i].row, &reservations[i].col,
                       reservations[i].username, &active);
                reservations[i].active = active;
            }
        }
    }
    
    fclose(file);
}

int find_user(const char* username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

void login() {
    char username[50];
    printf("请输入用户名: ");
    scanf("%s", username);
    
    // 检查是否是管理员
    if (strcmp(username, "admin") == 0) {
        strcpy(current_user, "admin");
        is_admin = 1;
        printf("管理员登录成功\n");
        admin_help();
        return;
    }
    
    int user_index = find_user(username);
    if (user_index == -1) {
        // 新用户
        if (user_count < MAX_USERS) {
            strcpy(users[user_count].username, username);
            users[user_count].reservation_count = 0;
            user_count++;
            printf("新用户注册成功\n");
        } else {
            printf("用户数量已达上限\n");
            return;
        }
    }
    
    strcpy(current_user, username);
    is_admin = 0;
    printf("登录成功\n");
}

void logout() {
    if (strlen(current_user) == 0) {
        printf("当前没有用户登录\n");
        return;
    }
    
    printf("用户 %s 已退出登录\n", current_user);
    current_user[0] = '\0';
    is_admin = 0;
}

void quit_program() {
    save_data();
    printf("程序已退出\n");
    exit(0);
}

int is_seat_available(int day, int floor, int row, int col) {
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].day == day && 
            reservations[i].floor == floor &&
            reservations[i].row == row &&
            reservations[i].col == col &&
            reservations[i].active == 1) {
            return 0; // 座位已被预约
        }
    }
    return 1; // 座位可用
}

void show_seats(int day, int floor) {
    printf("\n%s 第%d层座位情况:\n", day_names[day], floor);
    
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (is_seat_available(day, floor, row + 1, col + 1)) {
                printf("0");
            } else {
                printf("2"); // 被预约的座位显示为2
            }
        }
        printf("\n");
    }
}

void reserve_seat(int day, int floor, int row, int col) {
    if (strlen(current_user) == 0) {
        printf("请先登录\n");
        return;
    }
    
    if (floor < 1 || floor > FLOORS || row < 1 || row > ROWS || col < 1 || col > COLS) {
        printf("无效的座位位置\n");
        return;
    }
    
    if (!is_seat_available(day, floor, row, col)) {
        printf("该座位已被预约\n");
        return;
    }
    
    if (reservation_count >= MAX_RESERVATIONS) {
        printf("预约数量已达上限\n");
        return;
    }
    
    // 创建新预约
    reservations[reservation_count].day = day;
    reservations[reservation_count].floor = floor;
    reservations[reservation_count].row = row;
    reservations[reservation_count].col = col;
    strcpy(reservations[reservation_count].username, current_user);
    reservations[reservation_count].active = 1;
    
    reservation_count++;
    printf("OK\n");
    
    // 更新用户预约计数
    int user_index = find_user(current_user);
    if (user_index != -1) {
        users[user_index].reservation_count++;
    }
}

void show_user_reservations() {
    if (strlen(current_user) == 0) {
        printf("请先登录\n");
        return;
    }
    
    printf("用户 %s 的预约:\n", current_user);
    int found = 0;
    
    for (int i = 0; i < reservation_count; i++) {
        if (strcmp(reservations[i].username, current_user) == 0 && reservations[i].active == 1) {
            printf("%s Floor %d Seat %d %d\n", 
                   day_names[reservations[i].day],
                   reservations[i].floor,
                   reservations[i].row,
                   reservations[i].col);
            found = 1;
        }
    }
    
    if (!found) {
        printf("暂无预约\n");
    }
}

void clear_data() {
    if (!is_admin) {
        printf("只有管理员可以执行此操作\n");
        return;
    }
    
    reservation_count = 0;
    user_count = 0;
    printf("所有数据已清空\n");
}

// 管理员功能
void admin_help() {
    printf("\n=== 管理员功能 ===\n");
    printf("Help - 显示帮助信息\n");
    printf("AllReservations - 显示所有预约\n");
    printf("UserReservations [username] - 查看指定用户的预约\n");
    printf("AdminReserve [day] [floor] [row] [col] [username] - 管理员代预约\n");
    printf("AdminCancel [day] [floor] [row] [col] - 取消指定预约\n");
    printf("CancelDay [day] - 取消某天的所有预约\n");
    printf("CancelFloor [floor] - 取消某层的所有预约\n");
    printf("AddSeat [floor] [row] [col] - 添加座位\n");
    printf("RemoveSeat [floor] [row] [col] - 移除座位\n");
    printf("Clear - 清空所有数据\n");
}

void show_all_reservations() {
    if (!is_admin) {
        printf("只有管理员可以执行此操作\n");
        return;
    }
    
    printf("\n所有预约信息:\n");
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].active == 1) {
            printf("%s Floor %d Seat %d %d - 用户: %s\n", 
                   day_names[reservations[i].day],
                   reservations[i].floor,
                   reservations[i].row,
                   reservations[i].col,
                   reservations[i].username);
        }
    }
}

void show_user_reservations_admin() {
    if (!is_admin) {
        printf("只有管理员可以执行此操作\n");
        return;
    }
    
    char username[50];
    printf("请输入要查询的用户名: ");
    scanf("%s", username);
    
    printf("用户 %s 的预约:\n", username);
    int found = 0;
    
    for (int i = 0; i < reservation_count; i++) {
        if (strcmp(reservations[i].username, username) == 0 && reservations[i].active == 1) {
            printf("%s Floor %d Seat %d %d\n", 
                   day_names[reservations[i].day],
                   reservations[i].floor,
                   reservations[i].row,
                   reservations[i].col);
            found = 1;
        }
    }
    
    if (!found) {
        printf("该用户暂无预约\n");
    }
}

void admin_reserve() {
    if (!is_admin) {
        printf("只有管理员可以执行此操作\n");
        return;
    }
    
    char day_str[20], username[50];
    int day, floor, row, col;
    
    printf("请输入预约信息 (日期 楼层 排 列 用户名): ");
    scanf("%s %d %d %d %s", day_str, &floor, &row, &col, username);
    
    // 转换日期字符串为数字
    day = -1;
    for (int i = 0; i < DAYS; i++) {
        if (strcasecmp(day_str, day_names[i]) == 0) {
            day = i;
            break;
        }
    }
    
    if (day == -1) {
        printf("无效的日期\n");
        return;
    }
    
    if (!is_seat_available(day, floor, row, col)) {
        printf("该座位已被预约\n");
        return;
    }
    
    if (reservation_count >= MAX_RESERVATIONS) {
        printf("预约数量已达上限\n");
        return;
    }
    
    // 创建新预约
    reservations[reservation_count].day = day;
    reservations[reservation_count].floor = floor;
    reservations[reservation_count].row = row;
    reservations[reservation_count].col = col;
    strcpy(reservations[reservation_count].username, username);
    reservations[reservation_count].active = 1;
    
    reservation_count++;
    printf("管理员代预约成功\n");
}

void admin_cancel() {
    if (!is_admin) {
        printf("只有管理员可以执行此操作\n");
        return;
    }
    
    char day_str[20];
    int day, floor, row, col;
    
    printf("请输入要取消的预约信息 (日期 楼层 排 列): ");
    scanf("%s %d %d %d", day_str, &floor, &row, &col);
    
    // 转换日期字符串为数字
    day = -1;
    for (int i = 0; i < DAYS; i++) {
        if (strcasecmp(day_str, day_names[i]) == 0) {
            day = i;
            break;
        }
    }
    
    if (day == -1) {
        printf("无效的日期\n");
        return;
    }
    
    int found = 0;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].day == day && 
            reservations[i].floor == floor &&
            reservations[i].row == row &&
            reservations[i].col == col &&
            reservations[i].active == 1) {
            reservations[i].active = 0;
            printf("预约取消成功\n");
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("未找到对应的预约\n");
    }
}

void cancel_day_reservations(int day) {
    if (!is_admin) {
        printf("只有管理员可以执行此操作\n");
        return;
    }
    
    int cancelled = 0;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].day == day && reservations[i].active == 1) {
            reservations[i].active = 0;
            cancelled++;
        }
    }
    
    printf("已取消%d个%s的预约\n", cancelled, day_names[day]);
}

void cancel_floor_reservations(int floor) {
    if (!is_admin) {
        printf("只有管理员可以执行此操作\n");
        return;
    }
    
    int cancelled = 0;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].floor == floor && reservations[i].active == 1) {
            reservations[i].active = 0;
            cancelled++;
        }
    }
    
    printf("已取消%d个第%d层的预约\n", cancelled, floor);
}

int main() {
    initialize_system();
    system("chcp 65001");
    printf("=== 图书馆座位预约系统 ===\n");
    
    while (1) {
        printf("\n请输入指令: ");
        char command[50];
        scanf("%s", command);
        
        // 转换为小写以便比较
        for (int i = 0; command[i]; i++) {
            command[i] = tolower(command[i]);
        }
        
        if (strcmp(command, "login") == 0) {
            login();
        } else if (strcmp(command, "exit") == 0) {
            logout();
        } else if (strcmp(command, "quit") == 0) {
            quit_program();
        } else if (strncmp(command, "monday", 6) == 0 || 
                   strncmp(command, "tuesday", 7) == 0 ||
                   strncmp(command, "wednesday", 9) == 0 ||
                   strncmp(command, "thursday", 8) == 0 ||
                   strncmp(command, "friday", 6) == 0 ||
                   strncmp(command, "saturday", 8) == 0 ||
                   strncmp(command, "sunday", 6) == 0) {
            
            // 提取日期
            int day = -1;
            for (int i = 0; i < DAYS; i++) {
                if (strcasecmp(command, day_names[i]) == 0) {
                    day = i;
                    break;
                }
            }
            
            char floor_str[10];
            int floor;
            scanf("%s %d", floor_str, &floor);
            
            if (strcasecmp(floor_str, "floor") == 0 && floor >= 1 && floor <= FLOORS) {
                show_seats(day, floor);
            } else {
                printf("无效的楼层参数\n");
            }
            
        } else if (strcmp(command, "reserve") == 0) {
            char day_str[20];
            int day, floor, row, col;
            scanf("%s %s %d %s %d %d", day_str, day_str, &floor, day_str, &row, &col);
            
            // 转换日期
            day = -1;
            for (int i = 0; i < DAYS; i++) {
                if (strcasecmp(day_str, day_names[i]) == 0) {
                    day = i;
                    break;
                }
            }
            
            if (day != -1) {
                reserve_seat(day, floor, row, col);
            } else {
                printf("无效的日期\n");
            }
            
        } else if (strcmp(command, "reservation") == 0) {
            show_user_reservations();
        } else if (strcmp(command, "clear") == 0) {
            clear_data();
        } else if (strcmp(command, "help") == 0 && is_admin) {
            admin_help();
        } else if (strcmp(command, "allreservations") == 0 && is_admin) {
            show_all_reservations();
        } else if (strcmp(command, "userreservations") == 0 && is_admin) {
            show_user_reservations_admin();
        } else if (strcmp(command, "adminreserve") == 0 && is_admin) {
            admin_reserve();
        } else if (strcmp(command, "admincancel") == 0 && is_admin) {
            admin_cancel();
        } else if (strcmp(command, "cancelday") == 0 && is_admin) {
            char day_str[20];
            scanf("%s", day_str);
            int day = -1;
            for (int i = 0; i < DAYS; i++) {
                if (strcasecmp(day_str, day_names[i]) == 0) {
                    day = i;
                    break;
                }
            }
            if (day != -1) {
                cancel_day_reservations(day);
            } else {
                printf("无效的日期\n");
            }
        } else if (strcmp(command, "cancelfloor") == 0 && is_admin) {
            int floor;
            scanf("%d", &floor);
            if (floor >= 1 && floor <= FLOORS) {
                cancel_floor_reservations(floor);
            } else {
                printf("无效的楼层\n");
            }
        } else {
            printf("未知指令\n");
            fflush(stdin);
        }
    }
    
    return 0;
}