// Copyright (c) 2012-2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "dialogselectplugin.h"
#include "ui_dialogselectplugin.h"

DialogSelectPlugin::DialogSelectPlugin(QWidget *parent) :
    QXDialogX(parent),
    ui(new Ui::DialogSelectPlugin)
{
    ui->setupUi(this);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

void DialogSelectPlugin::setData(__DIE_OPTIONS *pOptions)
{
    setOptions(pOptions);

    QDir dir(Utils::getScriptsPath(pOptions));
    QFileInfoList dircontent=dir.entryInfoList(QStringList()<<"*.sg");

    for(int i=0; i<dircontent.count(); i++)
    {
        ui->listWidgetPlugins->addItem(dircontent.at(i).baseName());
    }
}

DialogSelectPlugin::~DialogSelectPlugin()
{
    delete ui;
}

bool DialogSelectPlugin::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->listWidgetPlugins->setToolTip(tr("List of plugins"));
    }

    return true;
}

void DialogSelectPlugin::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogSelectPlugin::on_pushButtonOK_clicked()
{
    if(ui->listWidgetPlugins->count())
    {
        pOptions->sPluginName=ui->listWidgetPlugins->currentItem()->text();
    }

    on_pushButtonCancel_clicked();
}

void DialogSelectPlugin::on_listWidgetPlugins_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    if(ui->listWidgetPlugins->count())
    {
        if(ui->listWidgetPlugins->currentItem()->text()!="")
        {
            on_pushButtonOK_clicked();
        }
    }
}
