#include "session.h"
#include <iostream>
#include <fstream>
#include "dbmanager.h"

const int FRAME_WIDTH = 350;
const int FRAME_HEIGHT = 200;
const int BALL_RADIUS = 30;

Session::Session(QObject *parent) : worker_(FRAME_WIDTH - 2 * BALL_RADIUS, FRAME_HEIGHT - 2 * BALL_RADIUS),
    render_(FRAME_WIDTH, FRAME_HEIGHT, BALL_RADIUS) {
    worker_.moveToThread(&workerThread_);
    render_.moveToThread(&renderThread_);

    connect(&worker_, SIGNAL(finished()), &workerThread_, SLOT(quit()), Qt::DirectConnection);
    connect(this, SIGNAL(stopAll()), &worker_, SLOT(stop()), Qt::DirectConnection);
    connect(this, SIGNAL(getNewState(BallState)), &worker_, SLOT(calculateNewState(BallState)));
    connect(&worker_, SIGNAL(stateReady(BallState)), this, SLOT(updateState(BallState)));

    connect(&render_, SIGNAL(finished()), &renderThread_, SLOT(quit()), Qt::DirectConnection);
    connect(this, SIGNAL(stopAll()), &render_, SLOT(stop()), Qt::DirectConnection);
    connect(this, SIGNAL(drawFrame(BallState)), &render_, SLOT(updateFrame(BallState)));
    connect(&render_, SIGNAL(frameUpdated(QImage)), this, SLOT(submitFrame(QImage)));

}

void Session::loadState(){
    DbManager dbManager(stateFile_);
    if (dbManager.isOpen()){
        if (dbManager.getLastState(ballState_)){
            return;
        }
    }
    std::fstream in;
    in.open(stateFile_.toStdString(), std::ios::in);
    if(in.is_open()){
        in >> ballState_.x >> ballState_.y >> ballState_.forwardDirection;
        return;
    }
    ballState_.x = BALL_RADIUS + FRAME_WIDTH / 2;
    ballState_.y = FRAME_HEIGHT / 2;
    ballState_.forwardDirection = true;
}

void Session::saveState(){
    DbManager dbManager(stateFile_);
    if (dbManager.isOpen()){
        if (dbManager.addBallState(ballState_)){
            return;
        }
    }
    std::fstream out;
    out.open(stateFile_.toStdString(), std::ios::out);
    if(out.is_open()){
        out << ballState_.x << '\n' << ballState_.y << '\n' << ballState_.forwardDirection << '\n';
    }

}

void Session::start(){
    if (!isRunning_){
        isRunning_ = true;
        loadState();
        workerThread_.start();
        renderThread_.start();
        queryNewState();
    }
}

void Session::stop(){
    if (isRunning_){
        isRunning_ = false;
        saveState();
        emit stopAll();
        workerThread_.wait();
        renderThread_.wait();
    }
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

Session::~Session()
{
    stop();
}

void Session::submitFrame(QImage frame){
    emit showFrame(frame);
}
