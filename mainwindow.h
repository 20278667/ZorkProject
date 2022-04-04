#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QObject>
#include <QTimer>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void output(std::string);
    void output(QString);
    QString latestInput;
    bool input;
    bool finished;
    bool canPlay;

private slots:
    void on_pushButton_clicked();
    void on_dialogue_textChanged();
    void updatePlay();

private:
    Ui::MainWindow *ui;
    QPushButton *push_button2;

};
#endif // MAINWINDOW_H
