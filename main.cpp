#include <QtWidgets/QtWidgets>
#include "widgets.h"

int main(int argc , char* argv[])
{
    QApplication a(argc , argv);
    MyQWidget dlg;
    dlg.show();
    
    return a.exec(); 
}
