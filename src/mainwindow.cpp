#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    // Set up the GUI

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    // Delete the GUI

    delete ui;
}
