/* Copyright (c) 2020-2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "dialogoptions.h"

#include "ui_dialogoptions.h"

DialogOptions::DialogOptions(QWidget *pParent, XOptions *pOptions, XOptions::GROUPID groupId) : XShortcutsDialog(pParent, false), ui(new Ui::DialogOptions)
{
    ui->setupUi(this);

    g_pDIEOptionsWidget = new DIEOptionsWidget(this);
    g_pSearchSignaturesOptionsWidget = new SearchSignaturesOptionsWidget(this);
    g_pXHexViewOptionsWidget = new XHexViewOptionsWidget(this);
    g_pXDisasmViewOptionsWidget = new XDisasmViewOptionsWidget(this);
    g_pXOnlineToolsOptionsWidget = new XOnlineToolsOptionsWidget(this);
    g_pXInfoDBOptionsWidget = new XInfoDBOptionsWidget(this);

    ui->widgetOptions->setOptions(pOptions, X_APPLICATIONDISPLAYNAME);

    ui->widgetOptions->addPage(g_pDIEOptionsWidget, tr("Scan"));
    g_pDIEOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->addPage(g_pSearchSignaturesOptionsWidget, tr("Signatures"));
    g_pSearchSignaturesOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->addPage(g_pXHexViewOptionsWidget, tr("Hex"));
    g_pXHexViewOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->addPage(g_pXDisasmViewOptionsWidget, tr("Disasm"));
    g_pXDisasmViewOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->addPage(g_pXOnlineToolsOptionsWidget, tr("Online tools"));
    g_pXOnlineToolsOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->addPage(g_pXInfoDBOptionsWidget, tr("Info"));
    g_pXInfoDBOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->setCurrentPage(groupId);
}

DialogOptions::~DialogOptions()
{
    delete ui;
}

void DialogOptions::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetOptions->setGlobal(pShortcuts, pXOptions);
    XShortcutsDialog::setGlobal(pShortcuts, pXOptions);
}

void DialogOptions::adjustView()
{
    ui->widgetOptions->adjustView();
}

void DialogOptions::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
