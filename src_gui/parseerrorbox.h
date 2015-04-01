#ifndef PARSEERRORBOX_H
#define PARSEERRORBOX_H

#include <QDialog>

namespace Ui {
class ParseErrorBox;
}

class ParseErrorBox : public QDialog
{
    Q_OBJECT

public:
    explicit ParseErrorBox(QWidget *parent = 0);
    ~ParseErrorBox();

    void setHeader(const QString &text);
    void setErrorText(const QString &text);
private:
    Ui::ParseErrorBox *ui;
};

#endif // PARSEERRORBOX_H
