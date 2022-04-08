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
    union latest;
    latest* latestColors;
    void output(std::string, std::string color = "#000000");
    void output(QString, QString color = "#000000");
    void outputAppend(QString, QString color = "#000000");
    void colorIn(QString color);
    void move(QString direction);
    void showMap();
    void showInfo();
    void look();
    void quitGame();
    void moveCursorToEnd();
    QString latestInput;
    bool input;
    bool finished;
    bool canPlay;

private slots:
    void on_pushButton_clicked();
    void on_dialogue_textChanged();
    void updatePlay();
    void on_west_clicked();
    void on_north_clicked();
    void on_east_clicked();
    void on_south_clicked();
    void on_map_clicked();
    void on_info_clicked();
    void on_quit_clicked();

    void on_look_clicked();

private:
    Ui::MainWindow *ui;
    QPushButton *push_button2;

};
#endif // MAINWINDOW_H
