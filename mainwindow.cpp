#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(testing_url(QUrl)));
    MainWindow::connect(ui->Start_OAuth_PB, SIGNAL(clicked()), this, SLOT(start_oauth_f()));
    MainWindow::connect(ui->GET_PB, SIGNAL(clicked()), this, SLOT(request_to_api()));
//    MainWindow::connect(ui->Token_req_PB, SIGNAL(clicked()), this, SLOT(token_request()));





}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::start_oauth_f()
{
    state_num = 555; // добавить сюда рандом?
    QString url = "https://www.reddit.com/api/v1/authorize?client_id=KQg-lG5GcUEnhQ&response_type=code&state=" +
            QString(state_num) +"&redirect_uri=http://www.reddit.com&duration=temporary&scope=read";
    ui->webView->load(QUrl(url));
    ui->oaut_done->setValue(25);
}

void MainWindow::finishedSlot(QNetworkReply* reply)
{
// Не произошло-ли ошибки?
if (reply->error() == QNetworkReply::NoError)
{
// Читаем ответ от сервера
QByteArray bytes = reply->readAll();

QString string(bytes);
if (a_token.isEmpty())
{
QJsonDocument j_doc = QJsonDocument::fromJson(bytes);
a_token = j_doc.object()["access_token"].toString();
}
if (not a_token.isEmpty())
{
//qDebug() << j_doc.object()["access_token"];
//qDebug() << a_token;
    ui->oaut_done->setValue(100);
    ui->label_tt->setText(a_token);
}
// Выводим ответ на экран
qDebug() << string;
QMessageBox::information(0, "Результат запроса ", string);
}
// Произошла какая-то ошибка
else
{
// обрабатываем ошибку
qDebug() << reply->errorString();
QMessageBox::information(0, "Результат запроса ", reply->errorString());
}

// добавим переход туда с вебвью

delete reply;
}


void MainWindow::testing_url(QUrl url)
{if (auth_code.isEmpty())
  auth_code = QUrlQuery(url).queryItemValue("code");
qDebug() << "изменение url";
qDebug() << url;
 if ((not auth_code.isEmpty() ) and ( a_token.isEmpty()))
 {
         ui->oaut_done->setValue(55);
         token_request();
 }
}

void MainWindow::token_request()
{
    QNetworkAccessManager *managerr = new QNetworkAccessManager(this);
    connect(managerr,SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

    QUrl url("https://www.reddit.com/api/v1/access_token");
    QNetworkRequest req(url);
    QByteArray sss = "grant_type=authorization_code&code=";
    sss += QUrl::toPercentEncoding(auth_code.toUtf8());
    sss += "&redirect_uri=http://www.reddit.com" ;

    QString concatenated = "KQg-lG5GcUEnhQ:";
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    req.setRawHeader("Authorization", headerData.toLocal8Bit());
    qDebug() << sss;

    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    managerr->post(req, sss );
}
void MainWindow::request_to_api()
{

    QNetworkAccessManager *managerr = new QNetworkAccessManager(this);
    connect(managerr,SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
    QUrl url("https://oauth.reddit.com/api/v1/scopes");
    QNetworkRequest req(url);
    req.setRawHeader("Authorization: bearer ",a_token.toUtf8());
    managerr->get(req);
}
