#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPainter>
#include<QKeyEvent>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <QtGui>

#define random(x) (rand()% (x + 1))

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    
    
    gamemode = 1;
    isPause = true;
    canPlay = true;
    
    speed_simple = 200;
    speed_normal = 100;
    speed_hard = 50;
    speed_veryhard = 15;
    
    curr_speed = speed_normal;   //默认速度为normal
    
    START_X = 60;
    START_Y = 100;
    /*初始化方向*/
    DIR_RIGHT = 1;
    DIR_LEFT = 2;
    DIR_DOWN = 3;
    DIR_UP =4;
    curr_dir = DIR_DOWN;
    snake_dir = DIR_DOWN;
    
    head = new snake;
    /*食物*/
    food = new snake;
    food->y = 5;
    food->x = 5;
    food->next = NULL;
    
    /*初始化全局变量*/
    LINE_WIDTH = 15;   //设置线宽
    LINE_NUMBER = 20;   //设置线条数——游戏面板大小
    
    /*默认游戏开始时就存在的蛇*/
    head->x = 0;
    head->y = 0;
    
    snake *temp = new snake;
    temp = head;
    
    /*自定义测试长度*/
    int initialSnake = 5;
    while(initialSnake > 0){
        snake *newsnake = new snake;
        temp->next = newsnake;
        newsnake->x = temp->x + 1;
        newsnake->y = temp->y;
        newsnake->next = NULL;
        
        temp = temp->next;
        initialSnake --;
    }
    
    
    startTimer(speed_simple);  //定时器
    startTimer(speed_normal);
    startTimer(speed_hard);
    startTimer(speed_veryhard);
    
    
    ////////////////////////
    ui->setupUi(this);
    resize(400, 420);
    
    actgrp = new QActionGroup(this);
    actgrp->addAction(ui->simple);
    actgrp->addAction(ui->normal);
    actgrp->addAction(ui->hard);
    actgrp->addAction(ui->veryhard);
    actgrp->setExclusive(true);
    ui->normal->setChecked(true);
    //label_state = new QLabel("hello");
    
    modegroup = new QActionGroup(this);
    modegroup->addAction(ui->auto_1);
    modegroup->addAction(ui->auto_2);
    modegroup->setExclusive(true);
    ui->auto_1->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//////////////////////////////////////
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    
    
    /*画背景网格线*/
    for (int i = 0; i < LINE_NUMBER ; i++)
    {
        p.drawLine(START_X,                            START_Y + i * LINE_WIDTH,
                   START_X + (LINE_NUMBER-1) * LINE_WIDTH, START_Y + i * LINE_WIDTH);
        
        p.drawLine(START_X + i * LINE_WIDTH,  START_Y,
                   START_X + i * LINE_WIDTH, START_Y + LINE_WIDTH * (LINE_NUMBER-1));
    }
    
    
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    p.setBrush(brush);
    
    snake * temp = new snake;
    temp = head;
    
    int i = 20;
    
    /*遍历链表，画蛇*/
    while(temp != NULL){
        
        brush.setColor(QColor(0,i,0));
        p.setBrush(brush);
        p.drawRect(temp->x * LINE_WIDTH + START_X, temp->y * LINE_WIDTH + START_Y, LINE_WIDTH, LINE_WIDTH);
        if(i <= 250 ){
            i += 5;
        }
        
        
        temp = temp->next;
    }
    
    /*画食物*/
    brush.setColor(QColor(170,4,0));
    p.setBrush(brush);
    p.drawRect(food->x * LINE_WIDTH + START_X, food->y * LINE_WIDTH + START_Y, LINE_WIDTH, LINE_WIDTH);
    
}

/*接收键盘事件，移动蛇头 + 移动蛇尾，重绘*/
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int keyValue = event->key();
    if(keyValue == Qt::Key_Left && snake_dir != DIR_RIGHT){
        curr_dir = DIR_LEFT;
    }
    
    if(keyValue == Qt::Key_Right && snake_dir != DIR_LEFT){
        curr_dir = DIR_RIGHT;
    }
    
    if(keyValue == Qt::Key_Up && snake_dir != DIR_DOWN){
        curr_dir = DIR_UP;
    }
    
    if(keyValue == Qt::Key_Down && snake_dir != DIR_UP){
        curr_dir = DIR_DOWN;
        
    }
    
    if(keyValue == Qt::Key_0){
        if(isPause){
            isPause = false;
        }
        else{
            isPause = true;
        }
    }
    
    if(keyValue == Qt::Key_1){
        canPlay = true;
    }

    if(keyValue == Qt::Key_2){
        if(gamemode == 2){
            gamemode = 1;
        }
        else{
            gamemode = 2;
        }
    }

        if(keyValue == Qt::Key_A){
            curr_speed = speed_simple;
        }
        
        if(keyValue == Qt::Key_S){
            curr_speed = speed_normal;
        }
        
        if(keyValue == Qt::Key_D){
            curr_speed = speed_hard;
        }
        if(keyValue == Qt::Key_F){
            curr_speed = speed_veryhard;
        }
        
        // cout<<head->x<<" "<<head->y<<endl;
        update();
    }
    
    /*蛇移动函数，以speed的时间间隔调用*/
    void MainWindow::moveSnake(){
        if(!isPause && canPlay){
            ui->label_state->setText("GAME OVER");
            if(isHitWall() != -1){
                canPlay = false;
                
            }
            else if(isBodyAhead() != -1){
                if(isBodyAhead() == DIR_DOWN){
                    cout<<"isBodyAhead() == DIR_DOWN"<<endl;
                }
                if(isBodyAhead() == DIR_LEFT){
                    cout<<"isBodyAhead() == DIR_LEFT"<<endl;
                }
                if(isBodyAhead() == DIR_RIGHT){
                    cout<<"isBodyAhead() == DIR_RIGHT"<<endl;
                }
                if(isBodyAhead() == DIR_UP){
                    cout<<"isBodyAhead() == DIR_UP"<<endl;
                }
                
                canPlay = false;
            }
            else{
                ui->label_state->setText("GAME START");
                snake_dir = curr_dir;
                if(snake_dir == DIR_DOWN){
                    if(foodAhead() == DIR_DOWN){   //情景1：进食
                        cout<<foodAhead()<<"DIR_DOWN"<<endl;
                        eat();
                    }
                    else{                         //情景2：正常移动
                        head->y += 1;
                        //if(head->next != NULL)
                        moveTail(DIR_DOWN);
                        //  snake_dir = DIR_DOWN;
                    }
                }
                
                if(snake_dir == DIR_LEFT){
                    if(foodAhead() == DIR_LEFT){
                        cout<<foodAhead()<<"DIR_LEFT"<<endl;
                        eat();
                    }
                    else{
                        head->x -= 1;     //要写成函数走一步，
                        //if(head->next != NULL)
                        moveTail(DIR_LEFT);
                        //  snake_dir = DIR_LEFT;
                    }
                }
                
                if(snake_dir == DIR_RIGHT){
                    if(foodAhead() == DIR_RIGHT){
                        cout<<foodAhead()<<"DIR_RIGHT"<<endl;
                        eat();
                    }
                    else{
                        head->x += 1;
                        //if(head->next != NULL)
                        moveTail(DIR_RIGHT);
                        //   snake_dir = DIR_RIGHT;
                        
                    }
                }
                
                if(snake_dir == DIR_UP){
                    if(foodAhead() == DIR_UP){
                        eat();
                        cout<<foodAhead()<<"DIR_UP"<<endl;
                    }
                    else{
                        head->y -= 1;
                        //if(head->next != NULL)
                        moveTail(DIR_UP);
                        //  snake_dir = DIR_UP;
                    }
                }
            }
            
        }
        
        long curr_score = score();
        QString str = QString::number(curr_score,10);
        ui->label->setText("Score:"+str);
        
        
        
    }
    
    /*移动蛇的尾部*/
    void MainWindow::moveTail(int direction){
        
        /*first用于保存蛇头移动前的坐标*/
        int first_x;
        int first_y;
        
        if(direction == DIR_LEFT){   //右
            first_x = head->x + 1;
            first_y = head->y;
        }
        
        if(direction == DIR_RIGHT){   //左
            first_x = head->x - 1;
            first_y = head->y;
        }
        
        if(direction == DIR_UP){   //上
            first_x = head->x;
            first_y = head->y + 1;
        }
        
        if(direction == DIR_DOWN){   //
            first_x = head->x;
            first_y = head->y - 1;
        }
        
        int second_x;
        int second_y;
        
        snake *temp = new snake; //temp相当于一个游标，指向蛇尾从头到尾的每一节
        temp = head->next;    //temp指向 蛇尾 的第一节
        
        while(temp != NULL){
            /*保存该蛇节的坐标至second*/
            second_x = temp->x;
            second_y = temp->y;
            /*将该a蛇节坐标修改为上一节的坐标first*/
            temp->x = first_x;
            temp->y = first_y;
            /*first是下一趟循环中，蛇节的上一节的坐标*/
            first_x = second_x;
            first_y = second_y;
            
            temp = temp->next;
        }
    }
    
    
    
    /*判断是否游戏结束*/
    bool MainWindow::isGameover(){
        return false;
    }
    
    /*判断撞墙后墙的位置，返回-1表示还没撞墙*/
    int MainWindow::isHitWall(){
        if(head->y == 0 && curr_dir == DIR_UP){
            return DIR_UP;
        }
        else if(head->y == LINE_NUMBER - 2 && curr_dir == DIR_DOWN){
            return DIR_DOWN;
        }
        else if(head->x == 0 && curr_dir == DIR_LEFT){
            return DIR_LEFT;
        }
        else if(head->x == LINE_NUMBER - 2 && curr_dir == DIR_RIGHT){
            return DIR_RIGHT;
        }
        else{
            return -1;
        }
    }
    
    /*判断食物是否在前方，返回方向值，否则返回-1*/
    int MainWindow::foodAhead(){
        if(curr_dir == DIR_LEFT && food->x == head->x - 1 && food->y == head->y){
            return DIR_LEFT;
            
        }
        else if(curr_dir == DIR_RIGHT && food->x == head->x + 1 && food->y == head->y){
            return DIR_RIGHT;
            
        }
        
        else if(curr_dir == DIR_UP && food->y == head->y - 1 && food->x == head->x){
            return DIR_UP;
            
        }
        
        else if(curr_dir == DIR_DOWN && food->y == head->y + 1 && food->x == head->x){
            return DIR_DOWN;
            
        }
        else return -1;
    }
    
    /*进食后变长*/
    void MainWindow::eat(){
        cout<<"eat"<<endl;
        snake *temp = new snake;
        temp->x = food->x;
        temp->y = food->y;
        temp->next = head;
        head = temp;
        
        //cout<<"准备resetfood"<<endl;
        resetFood();
        cout<<"food-----"<<food->x<<" "<<food->y<<endl;
    }
    
    
    
    
    /*显示蛇的长度（分数）*/
    int MainWindow::score(){
        snake *temp = new snake;
        temp = head;
        int i = 0;
        while(temp != NULL){
            i++;
            temp = temp->next;
        }
        return i;
    }
    
    /*重置食物坐标*/
    void MainWindow::resetFood(){
        
        int x = 0;
        int y = 0;
        bool isOverlap = false;
        
        
        snake *temp = new snake;
        temp = head;
        int n = score();
        int **p = new int* [n];
        for(int i = 0; i < n ; i++){
            p[i] = new int [2];
        }
        
        /*填充数组*/
        int i = 0;
        while(temp != NULL){
            //cout<<i<<temp->x<<" "<<temp->y<<endl;
            p[i][0] = temp->x;
            p[i][1] = temp->y;
            // cout<<i<<p[i][0]<<" "<<p[i][1]<<endl;
            i++;
            temp = temp->next;
        }
        
        /*只要重叠*/
        do{
            isOverlap = false;
            
            //srand((int)time(0));
            x = random(LINE_NUMBER - 2);
            //srand((int)time(0));
            y = random(LINE_NUMBER - 2);
            
            for(int i = 0; i < n ; i++){
                if(p[i][0] == x && p[i][1] == y){
                    isOverlap = true;
                    break;
                }
            }
        }while(isOverlap == true);
        
        food->x = x;
        food->y = y;
    }
    
    
    /*若身体在前方，返回身体在前方的位置，若不在前方，可以继续游戏，返回-1*/
    int MainWindow::isBodyAhead(){
        snake *temp = new snake;
        temp = head;
        int n = score();
        int **p = new int* [n];
        for(int i = 0; i < n ; i++){
            p[i] = new int [2];
        }
        
        /*填充数组*/
        int i = 0;
        while(temp != NULL){
            p[i][0] = temp->x;
            p[i][1] = temp->y;
            i++;
            temp = temp->next;
        }
        
        /*遍历数组*/
        for(int i = 0; i < n; i++){
            bool isRight = head->y == p[i][1] && head->x + 1 == p[i][0] && curr_dir == DIR_RIGHT;
            bool isLeft = head->y == p[i][1] && head->x - 1 == p[i][0] && curr_dir == DIR_LEFT;
            bool isUp = head->x == p[i][0] && head->y - 1 == p[i][1] && curr_dir== DIR_UP;
            bool isDown = head->x == p[i][0] && head->y + 1 == p[i][1] && curr_dir== DIR_DOWN;
            if(isRight){
                return DIR_RIGHT;
            }
            else if(isLeft){
                return DIR_LEFT;
            }
            else if(isUp){
                return DIR_UP;
            }
            else if(isDown){
                return DIR_DOWN;
            }
        }
        return -1;
    }
    
    
    void MainWindow::on_BeginorStart_triggered()
    {
        if(isPause){
            isPause = false;
        }
        else{
            isPause = true;
        }
    }
    
    
    
    void MainWindow::on_simple_triggered()
    {
        curr_speed = speed_simple;
    }
    
    void MainWindow::on_normal_triggered()
    {
        curr_speed = speed_normal;
    }
    
    void MainWindow::on_hard_triggered()
    {
        curr_speed = speed_hard;
    }
    
    void MainWindow::on_veryhard_triggered()
    {
        curr_speed = speed_veryhard;
    }
    
    
    /*定时器事件*/
    void MainWindow::timerEvent(QTimerEvent *t)//定时器时间
    {
        if(t->timerId() == 1 && curr_speed == speed_simple){
            moveSnake();
            update();
            if(gamemode == 2){
                dirAutoChange();
            }
        }
        if(t->timerId() == 2 && curr_speed == speed_normal){
            moveSnake();
            update();
            if(gamemode == 2){
                dirAutoChange();
            }
        }
        if(t->timerId() == 3 && curr_speed == speed_hard){
            moveSnake();
            update();
            if(gamemode == 2){
                dirAutoChange();
            }
        }
        if(t->timerId() == 4 && curr_speed == speed_veryhard){
            moveSnake();
            update();
            if(gamemode == 2){
                dirAutoChange();
            }
        }
        //cout<<"score = "<<score()<<endl;
    }
    
    void MainWindow::dirAutoChange(){
        //cout<<"dirAutoChange"<<endl;
        
        if(head->x < food->x && snake_dir != DIR_LEFT  ){     //蛇在左边
            curr_dir = DIR_RIGHT;
            if(isBodyAhead() == DIR_RIGHT){
                curr_dir = DIR_DOWN;
                if(isBodyAhead() == DIR_DOWN){
                    curr_dir = DIR_UP;
                }
            }
        }
        else if(head->x > food->x && snake_dir != DIR_RIGHT){    //蛇在右边
            curr_dir = DIR_LEFT;
        }
        else if(head->y < food->y && snake_dir != DIR_UP){    //蛇在上面
            curr_dir = DIR_DOWN;
        }
        else if(head->y > food->y && snake_dir != DIR_DOWN){   //蛇在下面
            curr_dir = DIR_UP;
        }
        else if(head->x == food->x && (snake_dir == DIR_UP || snake_dir == DIR_DOWN)){
            if(isHitWall() != DIR_LEFT){
                curr_dir = DIR_LEFT;
            }
            else{
                curr_dir = DIR_RIGHT;
            }
        }
        
    }
    
    
    void MainWindow::on_auto_1_triggered()
    {
        gamemode = 1;
    }
    
    void MainWindow::on_auto_2_triggered()
    {
        gamemode = 2;
    }
    

void MainWindow::newgame(){
    cout<<"newgame"<<endl;
    head->next = NULL;
        head->x = 0;
    head->y = 0;

    snake *temp = new snake;
    temp = head;

    /*自定义测试长度*/
    int initialSnake = 5;
    while(initialSnake > 0){
        snake *newsnake = new snake;
        temp->next = newsnake;
        newsnake->x = temp->x + 1;
        newsnake->y = temp->y;
        newsnake->next = NULL;

        temp = temp->next;
        initialSnake --;
    }
}

void MainWindow::on_newgame_triggered()
{
    newgame();
}
