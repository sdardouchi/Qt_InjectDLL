#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Window.h"

#include "CApp.h"
#include "macros.h"

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget *parent = Q_NULLPTR);
    ~Window();

private slots:
    void btnInjectClicked();
    void btnBrowseClicked();
    void lineEditsEdited();

private:
    Ui::WindowClass ui;
    CApp app;
    QList<QLineEdit*> _editList;

};
