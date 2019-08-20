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
#ifndef DIALOGENTROPY_H
#define DIALOGENTROPY_H

#include "qxdialogx.h"
#include "threadentropy.h"

#include "qwt_plot.h"
#include "qwt_plot_marker.h"
#include "qwt_plot_curve.h"
#include "qwt_legend.h"
#include "qwt_series_data.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_panner.h"
#include "qwt_plot_magnifier.h"
#include "qwt_text.h"
#include "qwt_math.h"
#include "qwt_plot_renderer.h"
#include "qwt_plot_histogram.h"
#include "qwt_plot_zoneitem.h"
#include "qwt_plot_renderer.h"
#include <QImageWriter>

namespace Ui
{
class DialogEntropy;
}

class DialogEntropy : public QXDialogX
{
    Q_OBJECT

public:
    explicit DialogEntropy(QWidget *parent=nullptr);
    ~DialogEntropy();
    void setData(__DIE_OPTIONS *pOptions,Binary *_binary,unsigned int nOffset,unsigned int nSize);

private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonReload_clicked();
    bool reload();
    void setProgressBar(int nMax, int nValue);
    void setEntropy(double dEntropy);
    void reloadGraph();
    void on_listWidget_itemSelectionChanged();
    void on_tabWidget_currentChanged(int index);
    void on_pushButtonSaveDiagram_clicked();
    void tabCurve();
    void tabHistogram();
    void tabBytes();
    void _on_finished();
    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButtonExtra_clicked();

signals:
    void reset();

private:
    Ui::DialogEntropy *ui;

    Binary *_binary;
    unsigned int nOffset;
    unsigned int nSize;
    unsigned int _nOffset;
    unsigned int _nSize;
    QwtPlot *plot;
    QwtPlot *plot2;
    QwtPlotHistogram *histogram;
    QwtPlotCurve *curve;
    ThreadEntropy *scan;

    double xCurve[101];
    double yCurve[101];
    double xHistogram[256];
    double yHistogram[256];

    QList<QwtPlotZoneItem *> listZones;
    QList<QString> listRegions;

    QShortcut *shortcut1;
    QShortcut *shortcut2;
    QShortcut *shortcut3;

    bool bIsScan;
};

#endif // DIALOGENTROPY_H
