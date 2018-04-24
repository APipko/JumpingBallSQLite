#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

const int FRAME_X = 20;
const int FRAME_Y = 60;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(onStartButtonClicked()));
    connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(onStopButtonClicked()));
    connect(&session_, SIGNAL(showFrame(QImage)), this, SLOT(repaintFrame(QImage)));
    connect(this, SIGNAL(repainted()), &session_, SLOT(queryNewState()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(FRAME_X, FRAME_Y), image_);
    emit repainted();
}

void MainWindow::onStartButtonClicked(){
    session_.start();
}

void MainWindow::onStopButtonClicked(){
    session_.stop();
}

void MainWindow::repaintFrame(QImage frame){
    image_ = frame;
    repaint();
}
