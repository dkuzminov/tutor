#include "MainWindow.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    QUrl url;
    MainWindow window;
    window.show();
    window.setUrl(QUrl("file:///C:/git/tutor/resources/El%20Coronel%20no%20tiene%20quien%20le%20escriba.html"));
    return app.exec();
}
