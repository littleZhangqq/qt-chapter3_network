#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QtNetwork/QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onClickFromGetRequest();
    void onClickFromPostRequest();
    void onClickFromDownloadRequest();
    void handleGetReply(QNetworkReply *reply);
    void handlePostReply(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QTextEdit *getcontent;
    QTextEdit *downcontent;
    QTextEdit *postcontent;
};
#endif // MAINWINDOW_H
