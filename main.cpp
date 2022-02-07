#include "principal.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator traducion;
    QString idiomaSistema = QLocale::system().nativeLanguageName().toLower();

        if (idiomaSistema == "english"){
            traducion.load(":/MiPaint_en.qm");
        }
        //Français
        if (idiomaSistema == "français"){
            traducion.load(":/MiPaint_fr.qm");
        }

        if (idiomaSistema != "español"){
            a.installTranslator(&traducion);
        }

    Principal w;
    w.show();
    return a.exec();
}
