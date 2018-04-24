#include "render.h"

#include <QPainter>


Render:: Render (int width, int height, int radius) : frameWidth_(width), frameHeight_(height), ballRadius_(radius)
{
}

Render::~ Render ()
{
}

void Render::updateFrame(BallState state){
    QImage frame(frameWidth_, frameHeight_, QImage::Format_RGB32);
    QPainter painter(&frame);
    painter.fillRect(0, 0, frameWidth_, frameHeight_, Qt::blue);
    painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    QPoint center(state.x + ballRadius_, state.y + ballRadius_);
    painter.drawEllipse(center, ballRadius_, ballRadius_);

    emit frameUpdated(frame);
}

void Render::stop(){
    emit finished();
}

