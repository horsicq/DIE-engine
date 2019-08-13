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
#include "qlineeditx.h"

QLineEditX::QLineEditX(QWidget *parent): QLineEdit(parent)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContextMenuRequested(QPoint)));

    bIsHex=true;
    bIsDec=false;
    bIsString=true;

    actCopyAsString=new QAction("", this);
    actCopyAsHex=new QAction("", this);
    actCopyAsHex0x=new QAction("", this);
    actCopyAsDec=new QAction("", this);

    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));
    connect(actCopyAsHex0x,SIGNAL(triggered()),this,SLOT(copyAsHex0x()));
    connect(actCopyAsHex,SIGNAL(triggered()),this,SLOT(copyAsHex()));
    connect(actCopyAsDec,SIGNAL(triggered()),this,SLOT(copyAsDec()));
}
void QLineEditX::customContextMenuRequested(const QPoint &pos)
{
    bool bTemp;
    unsigned long long nValue=0;

    QMenu menu;

    if(bIsString)
    {
        menu.addAction(actCopyAsString);
        actCopyAsString->setText(tr("Copy as \"%1\"").arg(this->text()));
    }

    if(bIsHex)
    {
        nValue=this->text().toULongLong(&bTemp,16);
        actCopyAsHex0x->setText(tr("Copy as \"0x%1\"").arg(nValue,0,16));
        actCopyAsDec->setText(tr("Copy as \"%1\"").arg(nValue));
        menu.addAction(actCopyAsHex0x);
        menu.addAction(actCopyAsDec);
    }
    else if(bIsDec)
    {
        nValue=this->text().toULongLong();
        actCopyAsHex->setText(tr("Copy as \"%1\"").arg(nValue,0,16));
        actCopyAsHex0x->setText(tr("Copy as \"0x%1\"").arg(nValue,0,16));
        menu.addAction(actCopyAsHex);
        menu.addAction(actCopyAsHex0x);
    }

    menu.exec(this->mapToGlobal(pos));
}
void QLineEditX::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(this->text()));
}

void QLineEditX::copyAsHex0x()
{
    QClipboard *clipboard = QApplication::clipboard();

    if(bIsHex)
    {
        bool bTemp;
        unsigned long long nValue=0;
        nValue=this->text().toULongLong(&bTemp,16);
        clipboard->setText(QString("0x%1").arg(nValue,0,16));
    }
    else if(bIsDec)
    {
        clipboard->setText(QString("0x%1").arg(this->text().toULongLong(),0,16));
    }
}

void QLineEditX::copyAsHex()
{
    QClipboard *clipboard = QApplication::clipboard();

    if(bIsHex)
    {
        bool bTemp;
        unsigned long long nValue=0;
        nValue=this->text().toULongLong(&bTemp,16);
        clipboard->setText(QString("%1").arg(nValue,0,16));
    }
    else if(bIsDec)
    {
        clipboard->setText(QString("%1").arg(this->text().toULongLong(),0,16));
    }
}

void QLineEditX::copyAsDec()
{
    bool bTemp;
    QClipboard *clipboard = QApplication::clipboard();

    if(bIsHex)
    {
        clipboard->setText(QString("%1").arg(this->text().toULongLong(&bTemp,16)));
    }
}

void QLineEditX::setHex(bool bValue)
{
    bIsHex=bValue;
}

void QLineEditX::setDec(bool bValue)
{
    bIsDec=bValue;
}

void QLineEditX::setString(bool bValue)
{
    bIsString=bValue;
}
bool QLineEditX::event(QEvent *event)
{
    if(event->type() == QEvent::ToolTip)
    {
        if(toolTip()!="")
        {
            QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
            QPoint pos=helpEvent->globalPos();
            x=pos.rx();
            y=pos.ry();

            bIsShow=true;
            QTimer::singleShot(__TOOLTIPRELAY__, this, SLOT(_showTooltip()));

            return true;
        }
    }

    return QLineEdit::event(event);
}

void QLineEditX::leaveEvent(QEvent *myLeave)
{
    QLineEdit::leaveEvent(myLeave);
    bIsShow=false;
}

void QLineEditX::setCopyMode(bool bIsString, bool bIsDec, bool bIsHex)
{
    this->bIsString=bIsString;
    this->bIsDec=bIsDec;
    this->bIsHex=bIsHex;
}

void QLineEditX::_showTooltip()
{
    if(bIsShow)
    {
        QToolTip::showText(QPoint(x,y),toolTip());
    }
}
