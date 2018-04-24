#ifndef BALLSTATE_H
#define BALLSTATE_H

#include <QMetaType>

struct BallState{
    int x = 0;
    int y = 0;
    bool forwardDirection = true;
};

Q_DECLARE_METATYPE(BallState)

#endif // BALLSTATE_H
