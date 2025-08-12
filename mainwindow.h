#pragma once

#include <QMainWindow>
#include <QMessageBox>

#include "database.h"
#include "dbdata.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void ScreenDataFromDB(QSqlTableModel *, QSqlQueryModel *, int typeRequest);
    void ReceiveStatusConnectionToDB(bool status);

private slots:
    void on_act_addData_triggered();
    void on_act_connect_triggered();
    void on_pb_request_clicked();
    void on_pb_clear_clicked();

signals:
    void sig_RequestToDb(QString request);

private:
    QVector<QString> dataForConnect;

    Ui::MainWindow *m_ui;
    DbData *m_dataDb;
    DataBase *m_dataBase;
    QMessageBox *m_msg;
    QSqlTableModel *m_tableModel{nullptr};
    QSqlQueryModel *m_queryModel{nullptr};
};
