#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MCModCrafter.h"

class MCModCrafter : public QMainWindow
{
    Q_OBJECT

public:
    MCModCrafter(QWidget *parent = Q_NULLPTR);

private:
    Ui::MCModCrafterClass ui;
};
