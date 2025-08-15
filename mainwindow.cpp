#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableView>
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    m_ui->lb_statusConnect->setStyleSheet("color:red");
    m_ui->pb_request->setEnabled(false);


    m_dataDb = new DbData(this);
    m_dataBase = new DataBase(this);
    m_msg = new QMessageBox(this);

    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);


    m_dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);


    connect(m_dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData) {
        dataForConnect = receivData;
    });

    connect(m_dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);

    connect(m_dataBase,
            &DataBase::sig_SendStatusConnection,
            this,
            &MainWindow::ReceiveStatusConnectionToDB);
}

MainWindow::~MainWindow()
{
    if (m_tableModel) {
        delete m_tableModel;
        m_tableModel = nullptr;
    }
    if (m_queryModel) {
        delete m_queryModel;
        m_queryModel = nullptr;
    }

    delete m_ui;
}


void MainWindow::on_act_addData_triggered()
{
    m_dataDb->show();
}

void MainWindow::on_act_connect_triggered()
{


    if (m_ui->lb_statusConnect->text() == "Отключено") {
        m_ui->lb_statusConnect->setText("Подключение");
        m_ui->lb_statusConnect->setStyleSheet("color : black");

        auto conn = [&] { m_dataBase->ConnectToDataBase(dataForConnect); };
        QtConcurrent::run(conn);

    } else {
        m_dataBase->DisconnectFromDataBase(DB_NAME);
        m_ui->lb_statusConnect->setText("Отключено");
        m_ui->act_connect->setText("Подключиться");
        m_ui->lb_statusConnect->setStyleSheet("color:red");
        m_ui->pb_request->setEnabled(false);
    }
}


void MainWindow::on_pb_request_clicked()
{
    QString filter = m_ui->cb_category->currentText();

    if (filter == "Все")
        m_dataBase->SendDataToUI(requestAllFilms);
    else if (filter == "Комедия")
        m_dataBase->SendDataToUI(requestComedy);
    else if (filter == "Ужасы")
        m_dataBase->SendDataToUI(requestHorrors);
}

void MainWindow::ScreenDataFromDB(QSqlTableModel* tableModel, QSqlQueryModel* queryModel, int typeRequest)
{
    if(typeRequest == requestAllFilms && queryModel) {
        m_ui->tb_result->setModel(queryModel);
        m_ui->tb_result->resizeColumnsToContents();
        m_ui->tb_result->horizontalHeader()->setStretchLastSection(true);

    } else if((typeRequest == requestComedy || typeRequest == requestHorrors) && queryModel) {
        m_ui->tb_result->setModel(queryModel);
        m_ui->tb_result->resizeColumnsToContents();
        m_ui->tb_result->horizontalHeader()->setStretchLastSection(true);
    }
}

void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if (status) {
        m_ui->act_connect->setText("Отключиться");
        m_ui->lb_statusConnect->setText("Подключено к БД");
        m_ui->lb_statusConnect->setStyleSheet("color:green");
        m_ui->pb_request->setEnabled(true);
    } else {
        m_dataBase->DisconnectFromDataBase(DB_NAME);
        m_msg->setIcon(QMessageBox::Critical);
        m_msg->setText(m_dataBase->GetLastError().text());
        m_ui->lb_statusConnect->setText("Отключено");
        m_ui->lb_statusConnect->setStyleSheet("color:red");
        m_msg->exec();
    }
}

void MainWindow::on_pb_clear_clicked()
{
    m_ui->tb_result->setModel(nullptr);
}
