#include "mainwindow.h"

#include <QtGui>
#include <QtWebKit>
#include <QtWebKitWidgets>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
//    connect(highlightButton, SIGNAL(onClick()), this, SLOT(on_highlightButton_clicked()));
//    connect(addSpotButton, SIGNAL(onClick()), this, SLOT(on_addSpotButton_clicked()));
//    connect(treeWidget->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(on_selectTreeViewItem(const QItemSelection &, const QItemSelection &)));
}

void MainWindow::setUrl(const QUrl &url)
{    
    webView->setUrl(url);
}
    
void MainWindow::on_webView_loadFinished()
{
    treeWidget->clear();

    QWebFrame *frame = webView->page()->mainFrame();
    QWebElement document = frame->documentElement();

    applyCSS(document);

    examineChildElements(document, treeWidget->invisibleRootItem());
}

void MainWindow::on_elementLineEdit_returnPressed()
{
    QWebFrame *frame = webView->page()->mainFrame();

    QWebElement document = frame->documentElement();
    QWebElementCollection elementsAll = document.findAll("*");
    foreach (QWebElement element, elementsAll) {
        element.removeAttribute("style");
    }

    QWebElementCollection elements = document.findAll(elementLineEdit->text());
    foreach (QWebElement element, elements) {
        element.setAttribute("style", "background-color: #f0f090");
    }
}

void MainWindow::on_highlightButton_clicked()
{
    on_elementLineEdit_returnPressed();
}

void MainWindow::on_selectTreeViewItem(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected != deselected && selected.size()) {
        QString str = treeWidget->selectedItems()[0]->data(1, 0).toString();
        QWebFrame *frame = webView->page()->mainFrame();

        QWebElement document = frame->documentElement();
        QWebElementCollection elementsAll = document.findAll("*");
        foreach (QWebElement element, elementsAll) {
            element.removeAttribute("style");
        }
        QWebElementCollection elements = document.findAll(str);
        foreach (QWebElement element, elements) {
            element.setAttribute("style", "background-color: #f090f0");
        }
    }
}

void MainWindow::applyCSS(QWebElement &document)
{
    QWebElement head = document.findFirst("body");
    head.appendInside("<style>body {font-size: 10;}</style>");
}

void MainWindow::examineChildElements(const QWebElement &parentElement,
                                  QTreeWidgetItem *parentItem)
{
    QWebElement element = parentElement.firstChild();
    while (!element.isNull()) {
        QString tagName = element.tagName();
        QTreeWidgetItem *item = new QTreeWidgetItem();

        if (tagName == "SPOT") {
            QString className = element.classes().count()? "." + element.classes()[0]: "";
            QString idName = element.attribute("id", "");
            if (!idName.isEmpty()) {
                idName = "#" + idName;
            }
            item->setText(0, tagName + className + idName);
            QTreeWidgetItem *itemAttr = new QTreeWidgetItem();
            itemAttr->setText(0, element.toInnerXml());
            itemAttr->setData(1, 0, idName);
            item->addChild(itemAttr);
            item->setData(1, 0, idName);
        }
        else {
            item->setText(0, tagName);
            examineChildElements(element, item);
        }

        parentItem->addChild(item);
        element = element.nextSibling();
    }
}
