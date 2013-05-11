#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <epub.h>
#include <QTimer>
#include <QResizeEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), pageIndex(0)
{
    ui->setupUi(this);
    setUpGUI();
    epubDoc = new EpubDocument("## Path to ur epub goes here ##");
    //epubDoc = new EpubDocument("/media/Local D/epubs/welcome.epub");
    epubDoc->renderPage();

    connect(epubDoc,SIGNAL(loadFinished(bool)),SLOT(setPicture()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpGUI()
{
    scroll = new QScrollArea(this);
    label = new QLabel(scroll);
    scroll->setWidgetResizable(true);
    scroll->setWidget(label);
    this->setCentralWidget(scroll);

    prev = new QToolButton(ui->statusBar);
    prev->setArrowType(Qt::LeftArrow);
    prev->setAutoRaise(true);
    connect(prev,SIGNAL(clicked()),SLOT(prevPage()));
    ui->statusBar->addPermanentWidget(prev);

    edit = new QLineEdit(ui->statusBar);
    edit->setFixedWidth(40);
    edit->setMaxLength(4);
    edit->setText(QString::number(pageIndex));
    ui->statusBar->addPermanentWidget(edit);

    next = new QToolButton(ui->statusBar);
    next->setArrowType(Qt::RightArrow);
    next->setAutoRaise(true);
    connect(next,SIGNAL(clicked()),SLOT(nextPage()));
    ui->statusBar->addPermanentWidget(next);
}

void MainWindow::setPicture()
{
    label->setPixmap(epubDoc->renderPixmap(pageIndex));
}

void MainWindow::nextPage()
{
    pageIndex++;
    edit->setText(QString::number(pageIndex));
    setPicture();
    scroll->ensureVisible(0,0);
}

void MainWindow::prevPage()
{
    if(pageIndex){
        pageIndex--;
        edit->setText(QString::number(pageIndex));
        setPicture();
        scroll->ensureVisible(0,0);
    }
}
