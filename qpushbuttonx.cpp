// Copyright (c) 2012-2020 hors<horsicq@gmail.com>
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
#include "qpushbuttonx.h"

QPushButtonX::QPushButtonX(QWidget *parent) : QPushButton(parent)
{
    //    qDebug("Constructor");
    x=0;
    y=0;
    bIsShow=false;
}

bool QPushButtonX::event(QEvent *event)
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

    return QPushButton::event(event);
}

void QPushButtonX::leaveEvent(QEvent *myLeave)
{
    QPushButton::leaveEvent(myLeave);
    bIsShow=false;
}

void QPushButtonX::setShortcut(const QKeySequence &key)
{
    sShortCut=key.toString();
    QPushButton::setShortcut(key);
}

void QPushButtonX::setEnabled(bool bState)
{
    QPushButton::setEnabled(bState);

    if(bState)
    {
        QPushButton::setShortcut(QKeySequence(sShortCut));
    }
}

void QPushButtonX::setText(const QString &text)
{
    QPushButton::setText(text);
    QPushButton::setShortcut(QKeySequence(sShortCut));
}

void QPushButtonX::setToolTip(const QString &text)
{
    QString sToolTip=text;
    QString sShortCut=shortcut().toString();

    if(sShortCut!="")
    {
        sToolTip+="(";
#ifdef Q_OS_MAC
        sShortCut=sShortCut.replace("Ctrl",QChar(0x2318));
#endif
        sToolTip+=sShortCut;
        sToolTip+=")";
    }

    QPushButton::setToolTip(sToolTip);
}

void QPushButtonX::_showTooltip()
{
    if(bIsShow)
    {
        QToolTip::showText(QPoint(x,y),toolTip());
    }
}
