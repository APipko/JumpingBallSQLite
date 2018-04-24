#ifndef SESSION_H
#define SESSION_H

#include "ballstate.h"
#include "worker.h"
#include "render.h"

#include <QObject>
#include <QThread>
#include <QString>

class Session : public QObject {
    Q_OBJECT

public:
    Session(QObject *parent = 0);
    ~Session();

public slots:
    void start();
    void stop();

    void queryNewState();
    void updateState(BallState state);
    void submitFrame(QImage frame);

private:
    BallState ballState_;
    bool isRunning_ = false;
    Worker worker_;
    Render render_;
    QThread workerThread_;
    QThread renderThread_;
    //QString stateFile_ = "jumping_ball_state.txt";
    QString stateFile_ = "ball_state.db";

    void loadState();
    void saveState();

signals:
    void stopAll();
    void getNewState(BallState state);
    void drawFrame(BallState state);
    void showFrame(QImage frame);
};

#endif // SESSION_H
