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
inline void MainWindow::output(std::string out) {
    output(QString::fromStdString(out));
}

inline void MainWindow::output(QString out) {
    ui->display->insertPlainText(out.trimmed() + "\n");
}
