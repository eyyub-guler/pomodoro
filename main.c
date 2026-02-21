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
int second_nonused = 25*60;

void pomo_countdown(int time);
void pomo_stat();
void pomo_main(int time);
void pomo_choose(int time);
void scrmv(int ch,int color,int firstrow,int lastrow,int col);
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
    d4 = (time/60)/10;
    d3 = (time/60)%10;
    d2 = (time%60)/10;
    d1 = (time%60)%10;
    int whereisindex= -1,;
    FILE *f = fopen("main_screen.txt","r");
    FILE *work = fopen("work.txt","r");
    FILE *brek = fopen("break.txt","r"); //its a typo but i couldnt find a better woed for it
    FILE *number = fopen("clock_animation.txt","r");
    bool iswork = 1, exit = 0;
    int i = 0,j;
    clear()
    FILE *number = fopen("clock_animation.txt" , "r");
    if(f==NULL){
        mvaddstr(0,0,"file clock_animation.txt couldn open check your files");
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
            pomo_countdown(int time);
        break;
        case KEY_UP:
            if(whereisindex == -1 || whereisindex == 1){
                whereisindex = 1;
                int startingx = x + whereisindex*8;
                int startingy = y + 17;
                if(d4 >= 9){ break; d4 = 9;}
                d4++;
                second += 600;
                second_nonused += 600;
                while (fgets(line, sizeof(line), number)){
                    if(i >= (d4-1)* 10 + 1 && i < (d4 * 10) + 1){
                            for(j = 0;j < 6; j++){
                                char c = line[j];
                                scrmv(line[j],WHITE,startingy,startingy+7,startingx+j); //make colorpair and define white
                            }
                }
                i++;
                }
                i = 0;
            } else if (whereisindex == 2){
                int startingx = x + whereisindex*8;
                int startingy = y + 17;
                whereisindex = 1;
                if(d3 == 9)
                d3++;
                second += 60;
                second_nonused += 60;
                while (fgets(line, sizeof(line), number)){
                    if(i >= (d4-1)* 10 + 1 && i < (d4 * 10) + 1){
                            for(j = 0;j < 6; j++){
                                char c = line[j];
                                scrmv(line[j],WHITE,startingy,startingy+7,startingx+j); //make colorpair and define white
                            }
                }
                i++;
                }
                i = 0;
            } else if
            
            break;

        case KEY_DOWN:
            move_down();
        break;

            case KEY_LEFT:
        move_left();
        break;

            case KEY_RIGHT:
        move_right();
        break;
    }
    if(exit) break;
    refresh();
    // for printing word pomodoro
    rewind(f); 
    prt_scr(f, x+6,y,0, 3);
    // for printing the work break screen
    if(iswork){
    rewind(work);
    prt_scr(work, x+20,y+7,0, 3);
    } else {
        wind(brek);
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
    //for printing the time and its animation
    rewind(number); 
    if(isd1moved || isd2moved || isd3moved || isd4moved){
        if(isd1moved){
            if(d1move_way == 1){
                // make it move up
            } else {
                //make it move down
            }
            isd1moved = 0;
        }
        if(isd2moved){
            if(d2move_way == 1){
                // make it move up
            } else {
                //make it move down
            }
            isd2moved = 0;
        }
        if(isd3moved){
            if(d3move_way == 1){
                // make it move up
            } else {
                //make it move down
            }
            isd3moved = 0;
        }
        if(isd4moved){
            if(d4move_way == 1){
                // make it move up
            } else {
                //make it move down
            }
            isd4moved = 0;
        }

    } else {
        //print index 1234 and dont make an animation
        rewind(number); 
        prt_scr(f, x+8,y+16,d4*10, 7);
        prt_scr(f, x+16,y+16,d3*10, 7);
        prt_scr(f, x+32,y+16,d2*10, 7);
        prt_scr(f, x+40,y+16,d1*10, 7);
    }
        napms(100);
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