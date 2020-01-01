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
#include "qtexteditxhex.h"

QTextEditXHEX::QTextEditXHEX(QWidget *parent):
    QTextEditX(parent)
{

}

void QTextEditXHEX::keyPressEvent(QKeyEvent *e)
{
    QString alar="0123456789ABCDEFabsdef.?";


    if((e->modifiers() & Qt::CTRL) && e->key() == Qt::Key_V)
    {
        QClipboard *clipboard = QApplication::clipboard();
        setText(clipboard->text());
    }

    switch(e->key())
    {

        case Qt::Key_Copy:
        case Qt::Key_Paste:

            qDebug("taste");

            break;

        case Qt::Key_Space:
        case Qt::Key_Backspace:
            QTextEditX::keyPressEvent(e);
            return;

        default:
            break;
    }

    if(alar.contains(e->text()))
    {
        QTextEditX::keyPressEvent(e);
    }
}

