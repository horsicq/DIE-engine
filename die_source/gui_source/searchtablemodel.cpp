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
#include "searchtablemodel.h"

SearchTableModel::SearchTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int SearchTableModel::rowCount(const QModelIndex &parent) const
{
    return listNames.count();
}

int SearchTableModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant SearchTableModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole)
    {
        if(index.column()==0)
        {
            return QVariant(listNames.at(index.row()));
        }
        else if(index.column()==1)
        {
            return QVariant(listOffsets.at(index.row()));
        }
        else if(index.column()==2)
        {
            return QVariant(listSizes.at(index.row()));
        }
    }
    else
    {
        return QVariant();
    }

}

QVariant SearchTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch(section)
            {
                case 0:
                    return QString("Value");

                case 1:
                    return QString("Offset");

                case 2:
                    return QString("Size");
            }
        }
    }

    return QVariant();
}

void SearchTableModel::addRow(QString sName, unsigned long long nOffset, unsigned long long nSize)
{
    listNames.append(sName);
    listOffsets.append(QString("%1").arg(nOffset,8,16,QChar('0')));
    listSizes.append(QString("%1").arg(nSize,8,16,QChar('0')));
}

void SearchTableModel::clear()
{
    listNames.clear();
    listOffsets.clear();
    listSizes.clear();
}

unsigned long long SearchTableModel::getOffset(int nIndex)
{
    return listOffsets.at(nIndex).toULongLong(0,16);
}

unsigned long long SearchTableModel::getSize(int nIndex)
{
    return listSizes.at(nIndex).toULongLong(0,16);
}

QString SearchTableModel::getName(int nIndex)
{
    return listNames.at(nIndex);
}
