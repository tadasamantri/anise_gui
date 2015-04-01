#include "parseerrorbox.h"
#include "ui_parseerrorbox.h"

ParseErrorBox::ParseErrorBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParseErrorBox)
{
    ui->setupUi(this);
    setWindowTitle("Errors while parsing file");
}

ParseErrorBox::~ParseErrorBox()
{
    delete ui;
}

void ParseErrorBox::setHeader(const QString &text){
    ui->header->setText(text);
}

void ParseErrorBox::setErrorText(const QString &text){
    ui->errorText->setText(text);
}
