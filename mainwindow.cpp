#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    input = false;
    /*
    push_button2 = new QPushButton(this);
    push_button2->setText("Click");
    push_button2->setGeometry(QRect(QPoint(0, 0), QSize(100, 50)));
    connect(push_button2, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));*/
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    input = true;
    latestInput = ui->dialogue->toPlainText();
    ui->dialogue->clear();
    output("> " + latestInput + "\n");
}

//inline display functions
inline void MainWindow::output(std::string out) {
    output(QString::fromStdString(out));
}

inline void MainWindow::output(QString out) {
    ui->display->insertPlainText(out);
}
