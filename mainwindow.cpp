#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    input = false;
    finished = false;
    canPlay = false;

    auto timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePlay()) );
    timer->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updatePlay() {
    canPlay = true;
}

//Slots
void MainWindow::on_pushButton_clicked() {
    input = true;
    latestInput = ui->dialogue->toPlainText().trimmed();
    ui->dialogue->clear();
    output("> " + latestInput + "\n");
}

void MainWindow::on_dialogue_textChanged()
{
    if (ui->dialogue->toPlainText().endsWith("\n")) {
        on_pushButton_clicked();
    }
}

//inline display functions; the first function is a wrapper function for output(QString)
//contains default color value of black
inline void MainWindow::output(std::string out, std::string color) {
    output(QString::fromStdString(out), QString::fromStdString(color));
}

inline void MainWindow::output(QString out, QString color) {
    outputAppend(out, color);
    ui->display->insertPlainText("\n");
    ui->display->ensureCursorVisible();
}

inline void MainWindow::outputAppend(QString out, QString color) {
    colorStart(color);
    ui->display->insertPlainText(out);
    colorEnd();
}

inline void MainWindow::colorStart(QString color) {
    ui->display->insertHtml("<font color =\"" + color + "\">");
}

inline void MainWindow::colorEnd() {
    ui->display->insertHtml("<\font>");
}
