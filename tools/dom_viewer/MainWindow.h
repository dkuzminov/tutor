#pragma once

#include <QMainWindow>
#include <QUrl>
#include <QWebElement>

#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
QT_END_NAMESPACE

class MainWindow : public QMainWindow, private Ui::Window
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void setUrl(const QUrl &url);

public slots:
    void on_webView_loadFinished();
    void on_elementLineEdit_returnPressed();
    void on_highlightButton_clicked();
    void on_selectTreeViewItem(const QItemSelection &, const QItemSelection &);

private:
    void examineChildElements(const QWebElement &parentElement,
                              QTreeWidgetItem *parentItem);
    void applyCSS(QWebElement &document);
};
