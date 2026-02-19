#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <signal.h>
#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include <locale.h>

volatile sig_atomic_t signal_status = 0;
int second = 25*60;
int second_nonused;

void pomo_countdown(int time);
void pomo_stat();
void pomo_main(int time);
void scrmv();
void animation();
void screenwarning(int x, int y);
void prt_scr(FILE *file,int x, int y, int starty, int length);
int main(int argc,int* argv ){
pomo_main(second);
}
void sighandler(int s) {
    signal_status = s;
}
void prt_scr(FILE *file,int x, int y, int starty, int length){
    //get the file and print it to the screen
    int i = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)){
        if(i >= starty && i < starty + length){
            line[strcspn(line, "\n")] = '\0';
            mvaddstr(y++, x, line); 
        }
        i++;
    }
    return;
}
void pomo_main(int time){
    // print the main screen open a while and check the screens size
    FILE *f = fopen("main_screen.txt","r");
    if(f == NULL){
        printf("you have missing files on your project check the file named main_screen.txt " );
        return;
    }
    
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    curs_set(0);
    nodelay(stdscr, TRUE);
    signal(SIGINT, sighandler);
    signal(SIGQUIT, sighandler);
    
    int rows ,cols;
    

    use_default_colors(); // arka plan için şart
    while(1){
        int ch = getch();
        getmaxyx(stdscr, rows, cols);
        int x = cols/2 - 29;
        int y = rows/2 - 5;
        if (ch == 'q' || ch == 'Q'){ 
            // write a quiting function
            break;
        } else if (ch == 's'){
            pomo_stat();
        } else if (ch == ' '){
            pomo_countdown(time);
        }
        if(rows < 11 || cols < 60){
            clear();
            screenwarning(x+24, y+3);    
        } else {
            clear();
            rewind(f); 
            prt_scr(f, x,y,0, 9);
        }
        refresh();
        napms(100);
    }
    curs_set(1);
    fclose(f);
    endwin();

}
void pomo_stat(){

}
void pomo_countdown(int time){

}
void screenwarning(int x, int y){
    mvaddstr(y++,x,"terminal");
    mvaddstr(y++,x,"  too");
    mvaddstr(y++,x," small");
}
void prtscr(int *file, int x, int y, int starty, int length)
{
}
