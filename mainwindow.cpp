#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <epub.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    epubDoc = new EpubDocument("## Path to ur epub goes here ##");
    ui->webView->setPage(epubDoc->convert());
}

MainWindow::~MainWindow()
{
    delete ui;
}
