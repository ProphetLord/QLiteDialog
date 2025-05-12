#include "app.h"

#include "tip/tipsmanager.h"

#include <Windows.h>

App::App(QWidget *parent)
    : QWidget(parent) {
    ui.setupUi(this);

    InnerDialog::setMainWidget(this);

    weakDlg = new WeakHintDialog;
    weakDlg->setParentWidget(this);
    auto font = qApp->font();
    font.setPixelSize(30);
    weakDlg->setWeakHintWidgetFont(font);

    weakDlg->setWeakHintDisplayType(WeakHintDialog::ParentCenter, WeakHintDialog::Arrange);

    TipsManager::setTipsLabelStyleSheet("background:#2B2D30;border: 1px solid gray;border-radius:4px;color:red;font-size:16px;");
    TipsManager::setTipsLabelMargins(QMargins(4, 4, 4, 4));
    TipsManager::addTipsWidget(this, "这是一个测试提示弹窗的界面");
}

void App::on_btn_Screen_Center_clicked() {
    weakDlg->addWeakHintLabel(QString("Center"), WeakHintDialog::ParentCenter);
}
void App::on_btn_Screen_Left_clicked() {
    weakDlg->addWeakHintLabel(QString("Left"), WeakHintDialog::ParentLeft);
}
void App::on_btn_Screen_Right_clicked() {
    weakDlg->addWeakHintLabel(QString("Right"), WeakHintDialog::ParentRight);
}
void App::on_btn_Screen_Top_clicked() {
    weakDlg->addWeakHintLabel(QString("Top"), WeakHintDialog::ParentTop);
}
void App::on_btn_Screen_Bottom_clicked() {
    weakDlg->addWeakHintLabel(QString("Bottom"), WeakHintDialog::ParentBottom);
}