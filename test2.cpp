#include <stdio.h>
#include <ctype.h>

int main(){
    FILE *file;
    file = fopen("test.text","r");
    if (file == NULL){
        perror("Error");
        return 1;
    }
    return 0;
}