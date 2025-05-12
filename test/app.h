#pragma once

#include <qwidget.h>

#include "ui_app.h"
#include "inner/innerdialog.h"
#include "normal/normaldialog.h"
#include "weakhint/weakhintdialog.h"
#include "tip/tipsdialog.h"

LITE_USE_NAMESPACE

class App : public QWidget {
    Q_OBJECT
public:
    explicit App(QWidget* parent = nullptr);

private slots:
    void on_btn_Screen_Bottom_clicked();
    void on_btn_Screen_Center_clicked();
    void on_btn_Screen_Left_clicked();
    void on_btn_Screen_Right_clicked();
    void on_btn_Screen_Top_clicked();

private:
    Ui::App ui{};

    WeakHintDialog* weakDlg = nullptr;
};
