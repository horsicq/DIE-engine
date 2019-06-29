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
#include "qtexteditxeditor.h"

QTextEditXeditor::QTextEditXeditor(QWidget *parent) :
    QPlainTextEdit(parent)
{
    c=0;

    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    highlighter = new Highlighter(this->document());
}
void QTextEditXeditor::setCompleter(QCompleter *completer)
{
    if(c)
    {
        QObject::disconnect(c, 0, this, 0);
    }

    c = completer;

    if(!c)
    {
        return;
    }

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));

}

void QTextEditXeditor::setData(QList<QString> *pList, QString sType)
{
    this->sType=sType;
    this->pList=pList;
}

QCompleter *QTextEditXeditor::completer() const
{
    return c;
}

void QTextEditXeditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();


    while(block.isValid() && top <= event->rect().bottom())
    {
        if(block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int QTextEditXeditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());

    while(max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void QTextEditXeditor::enableHighlight(bool bState)
{
    highlighter->setDocument(bState?this->document():0);
}

void QTextEditXeditor::insertCompletion(const QString& completion)
{
    if(c->widget() != this)
    {
        return;
    }

    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    //     qDebug("insert");
    //     qDebug(tc.selectedText().toAscii().data());
    //     while(tc.selectedText()==" ")
    //     {
    //         tc.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,2);
    //     }
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

void QTextEditXeditor::updateLineNumberAreaWidth(int newBlockCount)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void QTextEditXeditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if(!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void QTextEditXeditor::updateLineNumberArea(const QRect & rect, int dy)
{
    if(dy)
    {
        lineNumberArea->scroll(0, dy);
    }
    else
    {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if(rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

QString QTextEditXeditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

bool QTextEditXeditor::checkType()
{
    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::PreviousWord);
    //    tc.movePosition(QTextCursor::Left);
    tc.select(QTextCursor::WordUnderCursor);
    QString s2=tc.selectedText();
    tc.movePosition(QTextCursor::PreviousWord,QTextCursor::MoveAnchor,2);
    tc.select(QTextCursor::WordUnderCursor);
    QString s1=tc.selectedText();
    tc.movePosition(QTextCursor::PreviousWord,QTextCursor::MoveAnchor,2);
    tc.select(QTextCursor::WordUnderCursor);
    QString s0=tc.selectedText();

    //    qDebug((s0+s1).toAscii().data());
    //    qDebug((s1+s2).toAscii().data());

    return (s1+s2==sType+".")||(s0+s1==sType+".");
}

void QTextEditXeditor::showTooltip()
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    QString sText=tc.selectedText();
    QString sTooltip;

    for(int i=0; i<pList->count(); i++)
    {
        if(pList->at(i).section(";",0,0)==sText)
        {
            sTooltip=pList->at(i).section(";",1,1);
            break;
        }
    }

    if(sTooltip!="")
    {
        QToolTip::showText(mapToGlobal(cursorRect().center()),sTooltip);
    }

}

void QTextEditXeditor::focusInEvent(QFocusEvent *e)
{
    if(c)
    {
        c->setWidget(this);
    }

    QPlainTextEdit::focusInEvent(e);
}

void QTextEditXeditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//bool QTextEditXeditor::event(QEvent *event)
//{
//    if (event->type() == QEvent::ToolTip)
//    {
//        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
//        QTextCursor cursor = cursorForPosition(helpEvent->pos());
//        cursor.select(QTextCursor::WordUnderCursor);
//        if (!cursor.selectedText().isEmpty())
//            QToolTip::showText(helpEvent->globalPos(), cursor.selectedText());
//        else
//            QToolTip::hideText();
//        return true;
//    }
//    return QTextEditX::event(event);
//}

void QTextEditXeditor::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Tab)
    {
        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier,"    ");
        QPlainTextEdit::keyPressEvent(event);
        delete event;

        return;
    }


    if(c && c->popup()->isVisible())
    {
        // The following keys are forwarded by the completer to the widget
        switch(e->key())
        {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                e->ignore();
                return; // let the completer do default behavior

            default:
                break;
        }
    }

    if((e->modifiers() & Qt::ALT))
    {
        showTooltip();

    }

#ifdef Q_OS_MAC
    bool isShortcut = ((e->modifiers() & Qt::META) && e->key() == Qt::Key_Space); // Meta+Space
#else
    bool isShortcut = ((e->modifiers() & Qt::CTRL) && e->key() == Qt::Key_Space); // CTRL+Space
#endif

    if(!c || !isShortcut)  // do not process the shortcut when we have a completer
    {
        QPlainTextEdit::keyPressEvent(e);
    }

    if(!isShortcut)
    {
        c->popup()->hide();
        return;
    }

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);

    if(!c || (ctrlOrShift && e->text().isEmpty()))
    {
        return;
    }

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix =textUnderCursor();

    if(!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                       || eow.contains(e->text().right(1))))
    {
        c->popup()->hide();
        return;
    }

    if(!checkType())
    {
        c->popup()->hide();
        return;
    }

    if(completionPrefix != c->completionPrefix())
    {
        c->setCompletionPrefix(completionPrefix);
        //         c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }

    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}
