#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include "ballstate.h"

class DbManager
{
public:
    DbManager(const QString& file);
    ~DbManager();

    bool isOpen() const;
    bool addBallState(const BallState& state);
    bool getLastState(BallState& state);
private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
