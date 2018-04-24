#ifndef RENDER_H
#define RENDER_H

#include "ballstate.h"

#include <QObject>
#include <QImage>

class Render : public QObject {
    Q_OBJECT

public:
    Render(int width, int height, int radius);
    ~Render();

private:
    int frameWidth_;
    int frameHeight_;
    int ballRadius_;

public slots:
    void updateFrame(BallState state);
    void stop();

signals:
    void frameUpdated(QImage frame);
    void finished();
};

#endif // RENDER_H
