#include<stdio.h>

#define WIDTH 30
#define HEIGHT 30
#define QUEUESIZE WIDTH*HEIGHT*2

//algorithm to paint
typedef void algor(int a[WIDTH][HEIGHT]);
//init
typedef void init(int a[WIDTH][HEIGHT]);

//global var
int status[WIDTH][HEIGHT];
int a[WIDTH][HEIGHT];
int queue[QUEUESIZE];
int idx;//queue idx
int startx;
int starty;
int T;//target color
int NT;

//stats count
int steps =0;
int total = 0;
int duplicate = 0;
int border = 0;

void display(int a[WIDTH][HEIGHT]){
    int i,j;
    for(i=0;i<WIDTH;i++){
        for(j=0;j<HEIGHT;j++){
            printf("%d ",a[i][j]);
        }
        printf("\n");
    }
}

void stat(){
    printf("stats steps:%d total:%d duplicate:%d border:%d\n",steps,total,duplicate,border);
}

void rect(int a[WIDTH][HEIGHT]){
    printf("init use rect:\n");
    int i,j,ws,we,hs,he;
    ws = WIDTH/4;
    we = 3*WIDTH/4;

    hs = HEIGHT/4;
    he = 3*HEIGHT/4;

    for(i=ws;i<we;i++){
        for(j=hs;j<he;j++){
            a[i][j] = 1;
        }
    }
}

void gong(int a[WIDTH][HEIGHT]){
    printf("init use gong:\n");
    int i,j,ws,wm,we,hs,hm,he;
    ws = WIDTH/4;
    wm = WIDTH/2;
    we = 3*WIDTH/4;

    hs = HEIGHT/4;
    hm = HEIGHT/2;
    he = 3*HEIGHT/4;

    for(i=ws;i<ws+2;i++){
        for(j=hs;j<he;j++){
            a[i][j] = 1;
        }
    }

    for(i=ws+2;i<we-2;i++){
        for(j=hm;j<hm+2;j++){
            a[i][j] = 1;
        }
    }
    for(i=we-2;i<we;i++){
        for(j=hs;j<he;j++){
            a[i][j] = 1;
        }
    }
}

//paint current x,y
//add current to the stack
void paint(int currx,int curry){
    total += 1;
    if(a[currx][curry] == T){
        //same color
        queue[idx] = currx;
        queue[idx+1] = curry;
        idx+=2;
        //paint to new color
        a[currx][curry] = NT;
    }else if(a[currx][curry] == NT){
        //visit a painted point
        duplicate += 1;
    }else{
        border += 1;
    }
}

void check(int currx,int curry){
    if(!status[currx][curry]){
        paint(currx,curry);
    }
}

//algor
void paint8(int a[WIDTH][HEIGHT]){
    printf("paint use 8:\n");
    queue[idx] = startx;
    queue[idx+1] = starty;
    idx += 2;

    T = a[startx][starty];

    int headx,heady;
    while(idx >0){
        steps += 1;

        //fetch 2 elem from queue
        headx = queue[idx-2];
        heady = queue[idx-1];
        idx -= 2;

        //paint self
        if(a[startx][starty] == T){
            a[startx][starty] == NT;
        }

        //up
        if(headx > 0) {
            check(headx-1,heady);
        }

        //left up
        if(headx > 0 && heady >0) {
            check(headx-1,heady-1);
        }

        //left
        if(heady > 0) {
            check(headx,heady-1);
        }

        //left down
        if(heady > 0 && headx <WIDTH -1) {
            check(headx+1,heady-1);
        }

        //down
        if(headx < WIDTH -1) check(headx+1,heady);
        //down right
        if(headx < WIDTH -1 && heady < HEIGHT -1) check(headx+1,heady+1);
        //right
        if(heady < HEIGHT-1) check(headx,heady+1);
        //right up
        if(headx >0 && heady < HEIGHT-1) check(headx-1,heady+1);
        status[startx][starty] = 1;
    }
}

//四邻域
void paint4(int a[WIDTH][HEIGHT]){
    printf("paint use 4:\n");
    queue[idx] = startx;
    queue[idx+1] = starty;
    idx += 2;

    T = a[startx][starty];
    a[startx][starty] = NT;

    int headx,heady;
    while(idx >0){
        //fetch 2 elem from queue
        headx = queue[idx-2];
        heady = queue[idx-1];
        idx -= 2;
        steps += 1;

        //up
        if(headx > 0) {
            check(headx-1,heady);
        }

        //down
        if(headx < WIDTH -1){
            check(headx+1,heady);
        }

        //left
        if(heady > 0) {
            if(!status[headx][heady-1]){
                check(headx,heady-1);
            }
        }

        //right
        if(heady < HEIGHT-1) {
            if(!status[headx][heady+1]){
                check(headx,heady+1);
            }
        }

        status[headx][heady] = 1;
    }
}

//扫描线
void paintline(int a[WIDTH][HEIGHT]){
    printf("paint use spanline:\n");
    queue[idx] = startx;
    queue[idx+1] = starty;
    idx += 2;

    T = a[startx][starty];
    a[startx][starty] = NT;

    int headx,heady;
    int ly,ry;
    int i;
    while(idx >0){
        //fetch 2 elem from queue
        headx = queue[idx-2];
        heady = queue[idx-1];
        idx -= 2;
        steps += 1;

        //printf("current node:%d %d\n",headx,heady);
        //find far left and far right
        ly = heady -1;
        ry = heady +1;
        while(ly > 0 && a[headx][ly] == T){
            ly --;
        }
        ly += 1;

        while(ry < WIDTH -1 && a[headx][ry] == T){
            ry ++;
        }
        ry -= 1;

        //paint line
        for(i=ly;i<=ry;i++){
            a[headx][i] = NT;
            status[headx][i] = 1;
            //status[headx][heady] = 1;
        }

        for(i=ly;i<=ry;i++){
            if(headx >0) check(headx-1,i);
            if(headx <WIDTH-1) check(headx+1,i);
        }
    }
}

void reset(){
    memset(status,0,sizeof(status));
    memset(a,0,sizeof(a));

    startx= WIDTH/2;
    starty = HEIGHT/2;
    idx = 0;
    NT =2;

    //clear stat
    steps = 0;
    total = 0;
    duplicate = 0;
    border = 0;
}

void benchmark(algor* p,init* i,int debug){
    //0 for debug;1 for normal
    reset(); 
    i(a);

    if(debug){
        printf("after init:\n");
        display(a);
    }

    p(a);

    stat();

    if(debug){
        printf("after paint:\n");
        display(a);
    }
}

void main(){
    benchmark(paint4,rect,0);
    benchmark(paint8,rect,0);
    benchmark(paintline,rect,0);

    benchmark(paint4,gong,0);
    benchmark(paint8,gong,0);
    benchmark(paintline,gong,0);
}
