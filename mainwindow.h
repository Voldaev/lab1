#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include<QJsonValue>
#include <QtWebKitWidgets>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void start_oauth_f();
    void finishedSlot(QNetworkReply*);
    void testing_url(QUrl);
    void token_request();
    void request_to_api();

private:
    Ui::MainWindow *ui;
    QString auth_code;
    int state_num;
    QString a_token;
};

#endif // MAINWINDOW_H
