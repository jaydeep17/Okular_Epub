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
    QString epubPath = "/media/Local D/epubs/wasteland-20120118.epub";

    epubDoc = new EpubDocument(epubPath);
    connect(epubDoc,SIGNAL(loadFinished(bool)),SLOT(setPicture()));

    epubDoc->renderPage();
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
    connect(edit,SIGNAL(returnPressed()),SLOT(editReturnPress()));
    ui->statusBar->addPermanentWidget(edit);

    next = new QToolButton(ui->statusBar);
    next->setArrowType(Qt::RightArrow);
    next->setAutoRaise(true);
    connect(next,SIGNAL(clicked()),SLOT(nextPage()));
    ui->statusBar->addPermanentWidget(next);
}

void MainWindow::setPicture()
{
    QPixmap map = epubDoc->renderPixmap(pageIndex);
    QPainter paint(&map);
    paint.setPen(Qt::green);
    QStringList texts = epubDoc->pageText();
    qDebug() << "pg = " + QString::number(pageIndex) + " " + texts.join(";;");
    QList<QRect> list = epubDoc->textBounds();
    foreach (QRect rec, list){
        paint.drawRect(rec);
    }
    paint.end();
    label->setPixmap(map);
}

void MainWindow::nextPage()
{
    if(pageIndex >= epubDoc->pages()){
        return;
    }
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

void MainWindow::editReturnPress()
{
    bool ok;
    int indx = edit->text().toInt(&ok);
    if(ok && indx < epubDoc->pages()){
        pageIndex = indx;
        setPicture();
    } else {
        edit->setText(QString::number(pageIndex));
    }
}
