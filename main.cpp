// SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#include <final/final.h>

#include "MainDialog.h"

int main(int argc, char *argv[])
{
    finalcut::FApplication app(argc, argv);

    MainDialog mainDialog(&app);
    mainDialog.show();
    finalcut::FWidget::setMainWidget(&mainDialog);

    return app.exec();
}
