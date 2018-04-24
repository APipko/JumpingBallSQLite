#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QCoreApplication>

DbManager::DbManager(const QString &file)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", "ball_state_db");
    QString path = QCoreApplication::applicationDirPath() + "/" + file;
//    qDebug() << path;
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
//        qDebug() << "Database: connection ok";
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
    QString dbName = m_db.connectionName();
    m_db = QSqlDatabase();
    m_db.close();
    QSqlDatabase::removeDatabase(dbName);
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

bool DbManager::addBallState(const BallState& state)
{
    bool success = false;

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO ball_state (x, y, forwardDirection) VALUES (:x, :y, :forwardDirection)");
    query.bindValue(":x", state.x);
    query.bindValue(":y", state.y);
    query.bindValue(":forwardDirection", state.forwardDirection);

    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "add ball state to db failed: " << query.lastError();
    }
    return success;
}

bool DbManager::getLastState(BallState &state){

    bool success = false;

    QSqlQuery query("SELECT * FROM ball_state ORDER BY ids DESC LIMIT 1", m_db);

    if(query.exec())
    {
        success = true;
        int idX = query.record().indexOf("x");
        int idY = query.record().indexOf("y");
        int idD = query.record().indexOf("forwardDirection");
        query.first();
        state.x = query.value(idX).toInt();
        state.y = query.value(idY).toInt();
        state.forwardDirection = query.value(idD).toBool();
    }
    else
    {
        qDebug() << "get ball state from db failed: " << query.lastError();
    }
    return success;

}
