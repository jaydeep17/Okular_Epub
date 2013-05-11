#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include "epubdocument.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setUpGUI();

    Ui::MainWindow *ui;
    EpubDocument *epubDoc;

    QToolButton *prev, *next;
    QLineEdit *edit;
    QLabel *label;
    QScrollArea *scroll;
    int pageIndex;

private slots:
    void setPicture();
    void nextPage();
    void prevPage();
    void editReturnPress();
};

#endif // MAINWINDOW_H
