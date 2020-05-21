#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int ret = a.exec();
    if (argc == 2)
    {
        if (std::string(argv[1]) == "--shut")
        {
            Net::Client soc("fractal_cluster.soc");
            while (!soc.Connect());
            soc.Write(Net::MessageID::Shutdown);
            soc.Close();
        }
    }
    return ret;
}