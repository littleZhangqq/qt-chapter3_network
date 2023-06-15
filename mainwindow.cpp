#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QSizePolicy>
#include <QtNetwork/QNetworkAccessManager>
#include <QTextCodec>
#include <QStandardPaths>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *view = new QWidget(this);
    view->setStyleSheet("background-color:#33333333");
    setCentralWidget(view);

    QVBoxLayout *layout = new QVBoxLayout();
    view->setLayout(layout);

    QLabel *label = new QLabel("网络请求测试");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QPushButton *gettitle = new QPushButton("点我get测试");
    gettitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    getcontent = new QTextEdit("get结果");
    getcontent->setAlignment(Qt::AlignCenter);
    getcontent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *getlayout = new QVBoxLayout();
    getlayout->addWidget(gettitle);
    getlayout->addWidget(getcontent);

    QPushButton *downtitle = new QPushButton("点我down测试");
    downtitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    downcontent = new QTextEdit("down结果");
    downcontent->setAlignment(Qt::AlignCenter);
    QVBoxLayout *downlayout = new QVBoxLayout();
    downlayout->addWidget(downtitle);
    downlayout->addWidget(downcontent);

    QPushButton *posttitle = new QPushButton("点我post测试");
    posttitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    postcontent = new QTextEdit("post结果");
    postcontent->setAlignment(Qt::AlignCenter);
    QVBoxLayout *postlayout = new QVBoxLayout();
    postlayout->addWidget(posttitle);

    QLabel *param1 = new QLabel("参数1");
    QLineEdit *edit1 = new QLineEdit();
    edit1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout *p1Layout = new QHBoxLayout();
    p1Layout->addWidget(param1);
    p1Layout->addWidget(edit1);

    QLabel *param2 = new QLabel("参数1");
    QLineEdit *edit2 = new QLineEdit();
    edit2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout *p2Layout = new QHBoxLayout();
    p2Layout->addWidget(param2);
    p2Layout->addWidget(edit2);

    QLabel *param3 = new QLabel("参数3");
    QLineEdit *edit3 = new QLineEdit();
    edit3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout *p3Layout = new QHBoxLayout();
    p3Layout->addWidget(param3);
    p3Layout->addWidget(edit3);

    postlayout->addLayout(p1Layout);
    postlayout->addLayout(p2Layout);
    postlayout->addLayout(p3Layout);
    postlayout->addWidget(postcontent);

    QHBoxLayout *btmlayout = new QHBoxLayout();
    btmlayout->addLayout(getlayout);
    btmlayout->addLayout(downlayout);
    btmlayout->addLayout(postlayout);
    btmlayout->setStretch(0,1);
    btmlayout->setStretch(1,1);
    btmlayout->setStretch(2,2);
    layout->addLayout(btmlayout);

    getlayout->setStretch(0,1);
    getlayout->setStretch(1,2);

    downlayout->setStretch(0,1);
    downlayout->setStretch(1,2);

    postlayout->setStretch(0,1);
    postlayout->setStretch(1,1);
    postlayout->setStretch(2,1);
    postlayout->setStretch(3,1);
    postlayout->setStretch(4,4);

    layout->setStretch(0,1);
    layout->setStretch(1,5);

    connect(gettitle,SIGNAL(clicked(bool)),this,SLOT(onClickFromGetRequest()));
    connect(downtitle,SIGNAL(clicked(bool)),this,SLOT(onClickFromDownloadRequest()));
    connect(posttitle,SIGNAL(clicked(bool)),this,SLOT(onClickFromPostRequest()));
}

void MainWindow::onClickFromGetRequest(){
    QNetworkAccessManager *mananger = new QNetworkAccessManager();
    connect(mananger,SIGNAL(finished(QNetworkReply*)),this,SLOT(handleGetReply(QNetworkReply*)));
    mananger->get(QNetworkRequest(QUrl("http://jsonplaceholder.typicode.com/posts")));
}

void MainWindow::onClickFromPostRequest(){
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest req;
    req.setUrl(QUrl("http://jsonplaceholder.typicode.com/posts"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QByteArray param;
    param.append("userId=1&title=222&body=3333");
    manager->post(req,param);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(handlePostReply(QNetworkReply*)));
    //或者另一个新方法
    //QNetworkReply *reply = manager->post(req,param);
    //QObject::connect(reply,&QNetworkReply::finished,[=](){
    //
    //});
}

void MainWindow::onClickFromDownloadRequest(){
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // 创建下载请求
    QNetworkRequest request;
    request.setUrl(QUrl("https://via.placeholder.com/150/771796"));

    // 发送下载请求
    QNetworkReply *reply = manager->get(request);
    // 处理下载进度
    QObject::connect(reply, &QNetworkReply::downloadProgress, [](qint64 bytesReceived, qint64 bytesTotal) {
        // 处理下载进度更新
        int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
        qDebug() << "Download progress: " << progress << "%";
    });

    // 处理下载完成
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 下载完成，保存文件
            QString saveFilePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/image.jpg";
            QFile file(saveFilePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(reply->readAll());
                file.close();
                qDebug() << "Download complete";
            } else {
                qDebug() << "Failed to save file";
            }
        } else {
            // 处理下载错误
            qDebug() << "Download error: " << reply->errorString();
        }

        // 清理资源
        reply->deleteLater();
        manager->deleteLater();
    });
    //或者使用connect方法
    //connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(handleGetReply(QNetworkReply*)));

}

void MainWindow::handleGetReply(QNetworkReply *reply){
    QTextCodec *code = QTextCodec::codecForName("utf8");
    QString resp = code->toUnicode(reply->readAll());
    getcontent->setText(resp);
    reply->deleteLater();
}

void MainWindow::handlePostReply(QNetworkReply *reply){
    QTextCodec *code = QTextCodec::codecForName("utf8");
    QString resp = code->toUnicode(reply->readAll());
    postcontent->setText(resp);
    reply->deleteLater();
}

MainWindow::~MainWindow()
{
    delete ui;
}

