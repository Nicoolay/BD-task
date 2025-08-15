#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QVector>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

#define NUM_DATA_FOR_CONNECT_TO_DB 5

enum fieldsForConnect { hostName = 0, dbName = 1, login = 2, pass = 3, port = 4 };

enum requestType { requestAllFilms = 1, requestComedy = 2, requestHorrors = 3 };

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject* parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void ConnectToDataBase(QVector<QString> dataForConnect);
    void DisconnectFromDataBase(QString nameDb = "");

    QSqlQueryModel* GetAllFilms();
    QSqlQueryModel* GetGenreFilms(const QString& genre);

    QSqlError GetLastError(void);
    void SendDataToUI(int typeRequest);

signals:
    void sig_SendDataFromDB(QSqlTableModel*, QSqlQueryModel*, int typeRequest);
    void sig_SendStatusConnection(bool status);

private:
    QSqlDatabase* m_dataBase{nullptr};
};
