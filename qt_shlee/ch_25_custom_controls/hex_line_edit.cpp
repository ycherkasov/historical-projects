#include <QRegExp>
#include <QRegExpValidator>
#include "hex_line_edit.h"



HexLineEdit::HexLineEdit(QWidget* parent /*= nullptr*/) :
QLineEdit(parent)
{
    QRegExp check_valid("[0-9A-Fa-f]+");
    setValidator(new QRegExpValidator(check_valid, this));
}
