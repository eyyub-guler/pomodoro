#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <signal.h>
#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include <locale.h>

#define WHITE 1

volatile sig_atomic_t signal_status = 0;
int second = 25*60;
int second_nonused = 25*60;

void pomo_countdown(int time);
void pomo_stat();
void pomo_main(int time);
void pomo_choose(int time);
void scrmv(int ch,int color,int firstrow,int lastrow,int col);
void numbermv(FILE *number,int digit,int startingy,int startingx, int moveway);
void scrstrmv(FILE *file ,int copyindex, int startingx,int x_length,int startiny,int y_length);
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
    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
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
            pomo_choose(time);
            break;
        }
        if(rows < 11 || cols < 60){
            clear();
            screenwarning(cols/2 -3, rows/2-4);   
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
void pomo_choose(int time){
    int d4,d3,d2,d1,isd4moved = 0,isd3moved  = 0,isd2moved  = 0,isd1moved  = 0;
    int d4move_way = 0,d3move_way = 0,d2move_way = 0,d1move_way = 0;//d is for digit move way 1 is for up -1 is for down
    int rows, cols;
    d4 = (time/60)/10;
    d3 = (time/60)%10;
    d2 = (time%60)/10;
    d1 = (time%60)%10;
    int whereisindex= -1;
    FILE *f = fopen("main_screen.txt","r");
    FILE *work = fopen("work.txt","r");
    FILE *brek = fopen("break.txt","r"); //its a typo but i couldnt find a better woed for it
    FILE *number = fopen("clock_animation.txt","r");
    FILE *numberS = fopen("clock_animation_6digit.txt","r");
    bool iswork = 1, exit = 0;
    int i = 0,j;
    clear();
    if(f==NULL || work == NULL || brek == NULL || number == NULL || numberS == NULL){
        mvaddstr(0,0,"file(s) couldn open check your files");
        return;
    }
    //print screen here first
    while(1){
        int ch = getch();
        getmaxyx(stdscr, rows, cols);
        int x = cols/2 - 27;
        int y = rows/2 - 16;
        i = 0;
        if(rows < 11 || cols < 60){
            clear();
            screenwarning(cols/2 -3, rows/2-4); 
            refresh();
            napms(100);
            continue;
        }
        switch(ch){
        case 'q':
        case 'Q':
            exit = 1 ;
        break;
        case ' ':
            pomo_countdown(time);
        break;
        case KEY_UP:
            
            break;

        case KEY_DOWN:
          //  move_down();
        break;

            case KEY_LEFT:
        //move_left();
        break;

            case KEY_RIGHT:
        //move_right();
        break;
    }
    if(exit) break;
    refresh();
    // for printing word pomodoro
    rewind(f); 
    prt_scr(f, x,y,0, 5);
    // for printing the work break screen
    if(iswork){
    rewind(work);
    prt_scr(work, x+20,y+7,0, 3);
    } else {
    rewind(brek);
    prt_scr(brek, x+19,y+7,0, 3);
    }
    //for printing the warnings on the bottom
    rewind(f); 
    prt_scr(f, x,y+30,9, 2);
    //for printing the two dots n the middle of numbers
    mvaddstr(y+17,x+25,"██");
    mvaddstr(y+18,x+25,"████");
    mvaddstr(y+20,x+25,"████");
    mvaddstr(y+21,x+25,"██");
    //for printing under the index
        switch(whereisindex){
            case 1:
            mvaddstr(y+24,x+9,"▀▀▀▀");
            break;
            case 2:
            mvaddstr(y+24,x+17,"▀▀▀▀");
            break;
            case 3:
            mvaddstr(y+24,x+33,"▀▀▀▀");
            break;
            case 4:
            mvaddstr(y+24,x+41,"▀▀▀▀");
            break;
        }
    //for printing the time and its animation
    rewind(number); 
    rewind(numberS);
        if(isd1moved){
            numbermv(number,d1,x+8,y+17,d1move_way);
            isd1moved = 0;
        } else {
            prt_scr(f, x+8,y+17,d1*10, 7);
        }
        if(isd2moved){
            numbermv(numberS,d2,x+16,y+17,d2move_way);
            isd2moved = 0;
        } else {
            prt_scr(f, x+16,y+17,d2*10, 7);
        }
        if(isd3moved){
            numbermv(number,d3,x+32,y+17,d3move_way);
            isd3moved = 0;
        } else {
            prt_scr(f, x+32,y+17,d3*10, 7);
        }
        if(isd4moved){
            numbermv(number,d4,x+40,y+17,d4move_way);
            isd4moved = 0;
        } else {
            prt_scr(f, x+40,y+17,d4*10, 7);
        }
        refresh();
        napms(100);
        clear();
}
}
void pomo_stat(){

}

void scrmv(int ch,int color,int firstrow,int lastrow,int col){
if (firstrow == lastrow){
    mvaddch(firstrow,col,ch | COLOR_PAIR(color));
    return;
}

if (firstrow < lastrow){
    chtype v = mvinch(firstrow, col);
    char c = v & A_CHARTEXT;
    short pair = PAIR_NUMBER(v);
    int a = c;
    short currpair = pair;
    mvaddch(firstrow, col, ch| COLOR_PAIR(color));

    for(int i = firstrow + 1; i <= lastrow; i++){
        
        v = mvinch(i, col);
        c = v & A_CHARTEXT;
        pair = PAIR_NUMBER(v);
        if(a == ' ' && c == ' '){
                currpair = pair;
                continue;
        }
        mvaddch(i, col, a| COLOR_PAIR(currpair));
        a = c;
        currpair = pair;
    }
} else {
    chtype v = mvinch(lastrow, col);
    char c = v & A_CHARTEXT;
    short pair = PAIR_NUMBER(v);
    int a = c;
    short currpair = pair;
    mvaddch(lastrow, col, ch| COLOR_PAIR(color));

    for(int i = lastrow - 1; i >= firstrow; i--){
        v = mvinch(i, col);
        c = v & A_CHARTEXT;
        pair = PAIR_NUMBER(v);
        if(a == ' ' && c == ' '){
                currpair = pair;
                continue;
        }
        mvaddch(i, col, a| COLOR_PAIR(currpair));
        a = c;
        currpair = pair;
}
}
}

void pomo_countdown(int time){

}
void screenwarning(int x, int y){
    mvaddstr(y++,x,"terminal");
    mvaddstr(y++,x,"  too");
    mvaddstr(y++,x," small");
}

void scrstrmv(FILE *file ,int copyindex, int startingx,int x_length,int startingy,int y_length){
    int i = 1,a = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)){
        if(i == copyindex){
            for(int j = 0; j < y_length; j++){
                mvaddch(startingy+j,startingx+a,line[j]);
            }
            a++;
        }
        i++;
    }

}
void numbermv(FILE *number,int digit,int startingy,int startingx, int moveway){
    char str[10][6];
    char line[256];
    int a = 9,i = 1, j;
    int startpoint = (10 - digit)*10 + 1;
    if(digit == 0) startpoint = 1;
    while (fgets(line, sizeof(line), number)){ 
                    if(i >= startpoint  && i < startpoint + 10){
                        for( j= 0; j < 6; j++){
                            str[a][j] = line[j];
                        }
                        a--;
                    }
                    i++;
    }
    if(moveway == 1){
        for(i = 0; i<10;i++){
            for (j = 0; j<6; j++){
                scrmv(str[i][j],WHITE,startingy,startingy+7,startingx+j);
            }
            //make colorpair and define white
            // put refresh namps 
        }
        } else {
                for(i = 9; i >=0 ;i++){
                for (j = 0; j<6; j++){
                scrmv(str[i][j],WHITE,startingy+7,startingy,startingx+j);
                }
                //make colorpair and define white
                // put refresh namps 
                }
        
            }
}