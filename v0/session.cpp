#include "session.h"
#include <iostream>
#include <fstream>

const int FRAME_WIDTH = 350;
const int FRAME_HEIGHT = 200;
const int BALL_RADIUS = 30;

void Session::start(){
    if (!isRunning_){
        //std::cout << "start\n";
        isRunning_ = true;

        std::fstream in;
        in.open(state_file_, std::ios::in);
        if(!in.is_open()){
            ballState_.x = BALL_RADIUS + FRAME_WIDTH / 2;
            ballState_.y = FRAME_HEIGHT / 2;
            ballState_.forwardDirection = true;
        }
        in >> ballState_.x >> ballState_.y >> ballState_.forwardDirection;
        addWorker();
        addRender();
        queryNewState();
    }
}

void Session::stop(){
    if (isRunning_){
        //std::cout << "stop\n";
        isRunning_ = false;

        std::fstream out;
        out.open(state_file_, std::ios::out);
        if(!out.is_open()){
            out.clear();
            out.open(state_file_, std::ios::out);
            out.close();
            out.open(state_file_);
        }
        out << ballState_.x << '\n' << ballState_.y << '\n' << ballState_.forwardDirection << '\n';

        stopAll();
    }
}

void Session::addWorker()
{
    Worker* worker = new Worker(FRAME_WIDTH - 2 * BALL_RADIUS, FRAME_HEIGHT - 2 * BALL_RADIUS);
    QThread* thread = new QThread();
    worker->moveToThread(thread);

    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(this, SIGNAL(stopAll()), worker, SLOT(stop()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(this, SIGNAL(getNewState(BallState)), worker, SLOT(calculateNewState(BallState)));
    connect(worker, SIGNAL(stateReady(BallState)), this, SLOT(updateState(BallState)));
    thread->start();
    return ;
}

void Session::addRender()
{
    Render* render = new Render(FRAME_WIDTH, FRAME_HEIGHT, BALL_RADIUS);
    QThread* thread = new QThread();
    render->moveToThread(thread);

    connect(render, SIGNAL(finished()), thread, SLOT(quit()));
    connect(this, SIGNAL(stopAll()), render, SLOT(stop()));
    connect(render, SIGNAL(finished()), render, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(this, SIGNAL(drawFrame(BallState)), render, SLOT(updateFrame(BallState)));
    connect(render, SIGNAL(frameUpdated(QImage)), this, SLOT(submitFrame(QImage)));
    thread->start();
    return ;
}

void Session::queryNewState(){
    if (isRunning_){
        emit getNewState(ballState_);
    }
}


void Session::updateState(BallState state){
    ballState_ = state;
    emit drawFrame(ballState_);
}


void Session::stopThreads()
{
    emit  stopAll();
}

Session::~Session()
{
    stopThreads();
}

void Session::submitFrame(QImage frame){
    emit showFrame(frame);
}
