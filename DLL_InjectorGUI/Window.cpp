#include "Window.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <Windows.h>

Window::Window(QWidget *parent)
    : QMainWindow(parent),
    app()
{
    ui.setupUi(this);
    connect(ui.btnInject, &QPushButton::clicked, this, &Window::btnInjectClicked);
    connect(ui.btnBrowse, &QPushButton::clicked, this, &Window::btnBrowseClicked);

    connect(ui.leProc, &QLineEdit::textEdited, this, &Window::lineEditsEdited);
    connect(ui.leDLL, &QLineEdit::textEdited, this, &Window::lineEditsEdited);
    _editList.append(ui.leProc);
    _editList.append(ui.leDLL);

    ui.btnInject->setEnabled(false);
}

Window::~Window() {
}

void Window::btnBrowseClicked() {
    QString qstrDllPath = QFileDialog::getOpenFileName(this, "Select DLL to inject", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    qstrDllPath.replace("/", "\\");

    ui.leDLL->setText(qstrDllPath);
    app.SetDllPath(qstrDllPath.toStdString());
}

void Window::btnInjectClicked() {
    CreateThread(NULL, 0, app.InjectToProc, &app, 0, NULL);
}

void Window::lineEditsEdited() {
    int iNbModified = 0;
    for (int k = 0; k < _editList.length(); k++) {
        if (!_editList[k]->text().isEmpty())
            iNbModified++;
    }
    if (iNbModified == _editList.length())
        ui.btnInject->setEnabled(true);
    else
        ui.btnInject->setEnabled(false);

    app.SetDllPath(ui.leDLL->text().toStdString());
    app.SetProcName(ui.leProc->text().toStdString());

}
