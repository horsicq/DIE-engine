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

    m_pDIEOptionsWidget = new DIEOptionsWidget(this);
    m_pSearchSignaturesOptionsWidget = new SearchSignaturesOptionsWidget(this);
    m_pXHexViewOptionsWidget = new XHexViewOptionsWidget(this);
    m_pXDisasmViewOptionsWidget = new XDisasmViewOptionsWidget(this);
    m_pXOnlineToolsOptionsWidget = new XOnlineToolsOptionsWidget(this);
    m_pXInfoDBOptionsWidget = new XInfoDBOptionsWidget(this);

    ui->widgetOptions->setOptions(pOptions, X_APPLICATIONDISPLAYNAME);

    ui->widgetOptions->addPage(m_pDIEOptionsWidget, tr("Scan"));
    m_pDIEOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->addPage(m_pSearchSignaturesOptionsWidget, tr("Signatures"));
    m_pSearchSignaturesOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->addPage(m_pXHexViewOptionsWidget, tr("Hex"));
    m_pXHexViewOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->addPage(m_pXDisasmViewOptionsWidget, tr("Disasm"));
    m_pXDisasmViewOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->addPage(m_pXOnlineToolsOptionsWidget, tr("Online tools"));
    m_pXOnlineToolsOptionsWidget->setOptions(pOptions);

    ui->widgetOptions->addPage(m_pXInfoDBOptionsWidget, tr("Info"));
    m_pXInfoDBOptionsWidget->setOptions(pOptions);

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
