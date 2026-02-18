#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>



int second = 25*60;
int second_nonused;

void pomo_countdown();
void pomo_stat();
void pomo_main(int time);
void scrmv();
void animation();
int screenMiddlex();
int screenMiddley();
void prtscr(int *file,int x, int y, int starty, int length);
int main(int argc,int* argv ){
pomo_main(second);
}


void prtscr(FILE *file,int x, int y, int starty, int length){
    //get the file and print it to the screen
    int i = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)){
        if(i >= starty && i < starty + length){
            line[strcspn(line, "\n")] = '\0';
            mvstr(y++, x, line); 
        }
        i++;
    }
    return;
}
void pomo_main(int time){
    // print the main screen open a while and check the screens size
    int x = screenMiddlex - 29;
    int y = screenMiddley - 5;
    FILE *f = fopen("main_screen.txt","r");
    if(f == NULL){
        printf("you havemissing files on your project check the file named main_screen.txt " );
        return 0;
    }
    initscr();
    // ad a function to check screens length
    while(1)
    prtscr(f, x,y,0, 9);
    refresh();
    

}
