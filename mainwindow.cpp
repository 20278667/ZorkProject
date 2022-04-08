#include "mainwindow.h"
#include "ui_mainwindow.h"

union MainWindow:: latest {
    latest():s("") { }
    std::string s;
    QString q;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    input = false;
    finished = false;
    canPlay = false;
    latestColors = new latest();

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
    output("> " + latestInput);
}

void MainWindow::on_dialogue_textChanged()
{
    if (ui->dialogue->toPlainText().endsWith("\n")) {
        on_pushButton_clicked();
    }
}

void MainWindow::on_west_clicked() { return move("west"); }
void MainWindow::on_north_clicked() { return move("north"); }
void MainWindow::on_east_clicked() { return move("east"); }
void MainWindow::on_south_clicked() { return move("south"); }
void MainWindow::on_map_clicked() { return showMap(); }
void MainWindow::on_info_clicked() { return showInfo(); }
void MainWindow::on_quit_clicked() { return quitGame(); }
void MainWindow::on_look_clicked() { return look(); }
//look button handler
void MainWindow::look() {
    input = true;
    latestInput = "look";
    output("> " + latestInput);
}
//quit button handler
void MainWindow::quitGame() {
    input = true;
    latestInput = "quit";
    output("> " + latestInput);
}

//info button handler
void MainWindow::showInfo() {
    input = true;
    latestInput = "info";
    output("> " + latestInput);
}

//map button handler
void MainWindow::showMap() {
    input= true;
    latestInput = "map";
    output("> " + latestInput);
}

//movement button handler
void MainWindow::move(QString dir) {
    input = true;
    latestInput = "go " + dir;
    output("> " + latestInput);
}

//inline display functions
//have default color value of black|#000000
inline void MainWindow::output(std::string out, std::string color) {
    latestColors->s = color;
    output(QString::fromStdString(out), QString::fromStdString(color));
}

inline void MainWindow::output(QString out, QString color) {
    latestColors->q = color;
    outputAppend(out, latestColors->q);
    ui->display->insertPlainText("\n");
    ui->display->ensureCursorVisible();
}

inline void MainWindow::outputAppend(QString out, QString color) {
    moveCursorToEnd();
    colorIn(color);
    ui->display->insertPlainText(out);
}

inline void MainWindow::colorIn(QString color) {
    ui->display->setTextColor(QColor(color));
}

inline void MainWindow::moveCursorToEnd() {
    QTextCursor newCursor = ui->display->textCursor();
    newCursor.movePosition(QTextCursor::End);
    ui->display->setTextCursor(newCursor);
}
