#ifndef WORKER_H
#define WORKER_H

#include "ballstate.h"
#include <QObject>

class Worker : public QObject {
    Q_OBJECT

public:
    Worker(int width, int height);
    ~Worker();

private:
    int maxWidth_;
    int maxHeight_;

public slots:
    void calculateNewState(BallState state);
    void stop();

signals:
    void stateReady(BallState state);
    void finished();
};

#endif // WORKER_H
