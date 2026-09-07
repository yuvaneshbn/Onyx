#ifndef DARK_THEME_H
#define DARK_THEME_H

#include <QApplication>
#include <QString>

namespace Onyx {

class DarkTheme {
public:
    static void applyTheme(QApplication& app);
    static QString getStyleSheet();
};

} // namespace Onyx

#endif // DARK_THEME_H
