#pragma once
#include <QLineEdit>

// HexLineEdit supports only Hex format due to Validator
class HexLineEdit : public QLineEdit
{
public:
    HexLineEdit(QWidget* parent = nullptr);
};

