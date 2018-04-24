#include "worker.h"
#include <QThread>
#include <iostream>

const int STEP = 2;

Worker:: Worker (int width, int height) : maxWidth_(width), maxHeight_(height)
{
}

Worker::~ Worker ()
{
}

void Worker::calculateNewState(BallState state){
    // magic
    if (state.forwardDirection){
        state.x += STEP;
    } else {
        state.x -= STEP;
    }
    if (state.x <= 0){
        state.x = - state.x;
        state.forwardDirection = true;
    } else {
        if (state.x >= maxWidth_){
            state.x = 2 * maxWidth_ - state.x;
            state.forwardDirection = false;
        }
    }
    QThread::msleep(10);
    emit stateReady(state);
}

void Worker::stop(){
    emit finished();
}
