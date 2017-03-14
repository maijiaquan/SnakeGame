#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtGui>
#include <QActionGroup>

namespace Ui {
class MainWindow;
class Qlabel;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //////////////////////////////////////

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);



    void moveTail(int a);  //移动蛇尾
    void newgame();

    int foodAhead();  //判断食物是否在前方
    int isHitWall();
    int isBodyAhead();
    bool isGameover();

    void moveSnake();
    void eat();
    int score();
    void resetFood();
    void dirAutoChange();


protected:
    void timerEvent(QTimerEvent *event);
    //QMenu::mouseMoveEvent(QMouseEvent * e);
private slots:


   // void on_label_windowIconChanged(const QIcon &icon);

    void on_BeginorStart_triggered();

    void on_simple_triggered();

    void on_hard_triggered();

    void on_veryhard_triggered();

    void on_normal_triggered();

    void on_auto_1_triggered();

    void on_auto_2_triggered();



    void on_newgame_triggered();

private:
    Ui::MainWindow *ui;

    QActionGroup *actgrp;   //QActionGroup

    QActionGroup *modegroup;   //QActionGroup

    //QLabel  *label_state;
    struct snake{
        int x;
        int y;
        snake *next;
    };
    snake *head;
    snake *food;

    int gamemode;
    int LINE_WIDTH;
    int LINE_NUMBER;

    int START_X;
    int START_Y;



    bool isPause;
    bool canPlay;

    int TOP_LEFT_X;
    int TOP_LEFT_Y;

    /*速度选择*/
    int curr_speed;
    int speed_simple;
    int speed_normal;
    int speed_hard;
    int speed_veryhard;

    /*四个方向*/
    int DIR_UP;
    int DIR_DOWN;
    int DIR_LEFT;
    int DIR_RIGHT;
    int curr_dir;

    int snake_dir;


};

#endif // MAINWINDOW_H
