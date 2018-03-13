
#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>

#include <iostream>

#include "pomodoro.h"

int main(int argc, char** argv)
{
    std::size_t divisor = 1;
    if (argc == 2) {
        try {
            divisor = std::stoll(argv[1]);
        } catch(const std::exception& e) {
            std::cout << e.what() << std::endl;
            return 1;
        }
    }
    QApplication app{argc, argv};

    CppLondonUni::Pomodoro w{static_cast<uint64_t>(divisor)};
    w.setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                      Qt::AlignCenter, w.size(), app.desktop()->availableGeometry()));
    w.show();

    return app.exec();
}
