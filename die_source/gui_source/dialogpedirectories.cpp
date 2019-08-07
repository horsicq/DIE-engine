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
#include "dialogpedirectories.h"
#include "ui_dialogpedirectories.h"

DialogPEDirectories::DialogPEDirectories(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEDirectories)
{
    ui->setupUi(this);

    ui->lineEditOffset->setHex(false);
    ui->lineEditOffset->setDec(true);
    ui->lineEditSize->setHex(false);
    ui->lineEditSize->setDec(true);

    QTableWidgetItem *newItem;

    ui->tableWidgetDirectories->setRowCount(16);
    ui->tableWidgetDirectories->setColumnCount(6);

    QStringList labels;

    labels.append("");
    labels.append("Name");
    labels.append("Address");
    labels.append("Size");
    labels.append("");
    labels.append("");

    ui->tableWidgetDirectories->setHorizontalHeaderLabels(labels);

    QString sLabels[]= { "EXPORT",
                         "IMPORT",
                         "RESOURCE",
                         "EXCEPTION",
                         "SECURITY",
                         "BASERELOC",
                         "DEBUG",
                         "COPYRIGHT",
                         "GLOBALPTR",
                         "TLS",
                         "LOAD_CONFIG",
                         "BOUND_IMPORT",
                         "IAT",
                         "DELAY_IMPORT",
                         "COM_DESCRIPTOR",
                         "Reserved"
                       };

    for(int i=0; i<16; i++)
    {
        newItem = new QTableWidgetItem;
        newItem->setText(QString("%1").arg(i));
        ui->tableWidgetDirectories->setItem(i,0,newItem);
        ui->tableWidgetDirectories->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        newItem = new QTableWidgetItem;
        newItem->setText(sLabels[i]);
        ui->tableWidgetDirectories->setItem(i,1,newItem);
        ui->tableWidgetDirectories->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        ui->tableWidgetDirectories->setRowHeight(i,20);
    }

    lineeditEXPORTAddress=new QLineEditX;
    lineeditIMPORTAddress=new QLineEditX;
    lineeditRESOURCEAddress=new QLineEditX;
    lineeditEXCEPTIONAddress=new QLineEditX;
    lineeditSECURITYAddress=new QLineEditX;
    lineeditBASERELOCAddress=new QLineEditX;
    lineeditDEBUGAddress=new QLineEditX;
    lineeditCOPYRIGHTAddress=new QLineEditX;
    lineeditGLOBALPTRAddress=new QLineEditX;
    lineeditTLSAddress=new QLineEditX;
    lineeditLOAD_CONFIGAddress=new QLineEditX;
    lineeditBOUND_IMPORTAddress=new QLineEditX;
    lineeditIATAddress=new QLineEditX;
    lineeditDELAY_IMPORTAddress=new QLineEditX;
    lineeditCOM_DESCRIPTORAddress=new QLineEditX;
    lineeditReservedAddress=new QLineEditX;

    lineeditEXPORTSize=new QLineEditX;
    lineeditIMPORTSize=new QLineEditX;
    lineeditRESOURCESize=new QLineEditX;
    lineeditEXCEPTIONSize=new QLineEditX;
    lineeditSECURITYSize=new QLineEditX;
    lineeditBASERELOCSize=new QLineEditX;
    lineeditDEBUGSize=new QLineEditX;
    lineeditCOPYRIGHTSize=new QLineEditX;
    lineeditGLOBALPTRSize=new QLineEditX;
    lineeditTLSSize=new QLineEditX;
    lineeditLOAD_CONFIGSize=new QLineEditX;
    lineeditBOUND_IMPORTSize=new QLineEditX;
    lineeditIATSize=new QLineEditX;
    lineeditDELAY_IMPORTSize=new QLineEditX;
    lineeditCOM_DESCRIPTORSize=new QLineEditX;
    lineeditReservedSize=new QLineEditX;

    lineeditEXPORTAddress->setAlignment(Qt::AlignCenter);
    lineeditIMPORTAddress->setAlignment(Qt::AlignCenter);
    lineeditRESOURCEAddress->setAlignment(Qt::AlignCenter);
    lineeditEXCEPTIONAddress->setAlignment(Qt::AlignCenter);
    lineeditSECURITYAddress->setAlignment(Qt::AlignCenter);
    lineeditBASERELOCAddress->setAlignment(Qt::AlignCenter);
    lineeditDEBUGAddress->setAlignment(Qt::AlignCenter);
    lineeditCOPYRIGHTAddress->setAlignment(Qt::AlignCenter);
    lineeditGLOBALPTRAddress->setAlignment(Qt::AlignCenter);
    lineeditTLSAddress->setAlignment(Qt::AlignCenter);
    lineeditLOAD_CONFIGAddress->setAlignment(Qt::AlignCenter);
    lineeditBOUND_IMPORTAddress->setAlignment(Qt::AlignCenter);
    lineeditIATAddress->setAlignment(Qt::AlignCenter);
    lineeditDELAY_IMPORTAddress->setAlignment(Qt::AlignCenter);
    lineeditCOM_DESCRIPTORAddress->setAlignment(Qt::AlignCenter);
    lineeditReservedAddress->setAlignment(Qt::AlignCenter);

    lineeditEXPORTSize->setAlignment(Qt::AlignCenter);
    lineeditIMPORTSize->setAlignment(Qt::AlignCenter);
    lineeditRESOURCESize->setAlignment(Qt::AlignCenter);
    lineeditEXCEPTIONSize->setAlignment(Qt::AlignCenter);
    lineeditSECURITYSize->setAlignment(Qt::AlignCenter);
    lineeditBASERELOCSize->setAlignment(Qt::AlignCenter);
    lineeditDEBUGSize->setAlignment(Qt::AlignCenter);
    lineeditCOPYRIGHTSize->setAlignment(Qt::AlignCenter);
    lineeditGLOBALPTRSize->setAlignment(Qt::AlignCenter);
    lineeditTLSSize->setAlignment(Qt::AlignCenter);
    lineeditLOAD_CONFIGSize->setAlignment(Qt::AlignCenter);
    lineeditBOUND_IMPORTSize->setAlignment(Qt::AlignCenter);
    lineeditIATSize->setAlignment(Qt::AlignCenter);
    lineeditDELAY_IMPORTSize->setAlignment(Qt::AlignCenter);
    lineeditCOM_DESCRIPTORSize->setAlignment(Qt::AlignCenter);
    lineeditReservedSize->setAlignment(Qt::AlignCenter);

    lineeditEXPORTAddress->setInputMask("HHHHHHHH");
    lineeditIMPORTAddress->setInputMask("HHHHHHHH");
    lineeditRESOURCEAddress->setInputMask("HHHHHHHH");
    lineeditEXCEPTIONAddress->setInputMask("HHHHHHHH");
    lineeditSECURITYAddress->setInputMask("HHHHHHHH");
    lineeditBASERELOCAddress->setInputMask("HHHHHHHH");
    lineeditDEBUGAddress->setInputMask("HHHHHHHH");
    lineeditCOPYRIGHTAddress->setInputMask("HHHHHHHH");
    lineeditGLOBALPTRAddress->setInputMask("HHHHHHHH");
    lineeditTLSAddress->setInputMask("HHHHHHHH");
    lineeditLOAD_CONFIGAddress->setInputMask("HHHHHHHH");
    lineeditBOUND_IMPORTAddress->setInputMask("HHHHHHHH");
    lineeditIATAddress->setInputMask("HHHHHHHH");
    lineeditDELAY_IMPORTAddress->setInputMask("HHHHHHHH");
    lineeditCOM_DESCRIPTORAddress->setInputMask("HHHHHHHH");
    lineeditReservedAddress->setInputMask("HHHHHHHH");

    lineeditEXPORTSize->setInputMask("HHHHHHHH");
    lineeditIMPORTSize->setInputMask("HHHHHHHH");
    lineeditRESOURCESize->setInputMask("HHHHHHHH");
    lineeditEXCEPTIONSize->setInputMask("HHHHHHHH");
    lineeditSECURITYSize->setInputMask("HHHHHHHH");
    lineeditBASERELOCSize->setInputMask("HHHHHHHH");
    lineeditDEBUGSize->setInputMask("HHHHHHHH");
    lineeditCOPYRIGHTSize->setInputMask("HHHHHHHH");
    lineeditGLOBALPTRSize->setInputMask("HHHHHHHH");
    lineeditTLSSize->setInputMask("HHHHHHHH");
    lineeditLOAD_CONFIGSize->setInputMask("HHHHHHHH");
    lineeditBOUND_IMPORTSize->setInputMask("HHHHHHHH");
    lineeditIATSize->setInputMask("HHHHHHHH");
    lineeditDELAY_IMPORTSize->setInputMask("HHHHHHHH");
    lineeditCOM_DESCRIPTORSize->setInputMask("HHHHHHHH");
    lineeditReservedSize->setInputMask("HHHHHHHH");

    connect(lineeditEXPORTAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditIMPORTAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditRESOURCEAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditEXCEPTIONAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditSECURITYAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditBASERELOCAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditDEBUGAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditCOPYRIGHTAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditGLOBALPTRAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditTLSAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditLOAD_CONFIGAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditBOUND_IMPORTAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditIATAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditDELAY_IMPORTAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditCOM_DESCRIPTORAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditReservedAddress,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(0,2),lineeditEXPORTAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(1,2),lineeditIMPORTAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(2,2),lineeditRESOURCEAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(3,2),lineeditEXCEPTIONAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(4,2),lineeditSECURITYAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(5,2),lineeditBASERELOCAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(6,2),lineeditDEBUGAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(7,2),lineeditCOPYRIGHTAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(8,2),lineeditGLOBALPTRAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(9,2),lineeditTLSAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(10,2),lineeditLOAD_CONFIGAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(11,2),lineeditBOUND_IMPORTAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(12,2),lineeditIATAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(13,2),lineeditDELAY_IMPORTAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(14,2),lineeditCOM_DESCRIPTORAddress);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(15,2),lineeditReservedAddress);

    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(0,3),lineeditEXPORTSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(1,3),lineeditIMPORTSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(2,3),lineeditRESOURCESize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(3,3),lineeditEXCEPTIONSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(4,3),lineeditSECURITYSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(5,3),lineeditBASERELOCSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(6,3),lineeditDEBUGSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(7,3),lineeditCOPYRIGHTSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(8,3),lineeditGLOBALPTRSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(9,3),lineeditTLSSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(10,3),lineeditLOAD_CONFIGSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(11,3),lineeditBOUND_IMPORTSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(12,3),lineeditIATSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(13,3),lineeditDELAY_IMPORTSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(14,3),lineeditCOM_DESCRIPTORSize);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(15,3),lineeditReservedSize);

    connect(lineeditEXPORTSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditIMPORTSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditRESOURCESize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditEXCEPTIONSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditSECURITYSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditBASERELOCSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditDEBUGSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditCOPYRIGHTSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditGLOBALPTRSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditTLSSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditLOAD_CONFIGSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditBOUND_IMPORTSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditIATSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditDELAY_IMPORTSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditCOM_DESCRIPTORSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineeditReservedSize,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    pushButtonEXPORTExtra=new QPushButtonX;
    pushButtonEXPORTExtra->setText("...");
    pushButtonEXPORTExtra->setFixedWidth(30);
    pushButtonIMPORTExtra=new QPushButtonX;
    pushButtonIMPORTExtra->setText("...");
    pushButtonIMPORTExtra->setFixedWidth(30);
    pushButtonRESOURCEExtra=new QPushButtonX;
    pushButtonRESOURCEExtra->setText("...");
    pushButtonRESOURCEExtra->setFixedWidth(30);
    pushButtonEXCEPTIONExtra=new QPushButtonX;
    pushButtonEXCEPTIONExtra->setText("...");
    pushButtonEXCEPTIONExtra->setFixedWidth(30);
    pushButtonSECURITYExtra=new QPushButtonX;
    pushButtonSECURITYExtra->setText("...");
    pushButtonSECURITYExtra->setFixedWidth(30);
    pushButtonBASERELOCExtra=new QPushButtonX;
    pushButtonBASERELOCExtra->setText("...");
    pushButtonBASERELOCExtra->setFixedWidth(30);
    pushButtonDEBUGExtra=new QPushButtonX;
    pushButtonDEBUGExtra->setText("...");
    pushButtonDEBUGExtra->setFixedWidth(30);
    pushButtonCOPYRIGHTExtra=new QPushButtonX;
    pushButtonCOPYRIGHTExtra->setText("...");
    pushButtonCOPYRIGHTExtra->setFixedWidth(30);
    pushButtonGLOBALPTRExtra=new QPushButtonX;
    pushButtonGLOBALPTRExtra->setText("...");
    pushButtonGLOBALPTRExtra->setFixedWidth(30);
    pushButtonTLSExtra=new QPushButtonX;
    pushButtonTLSExtra->setText("...");
    pushButtonTLSExtra->setFixedWidth(30);
    pushButtonLOAD_CONFIGExtra=new QPushButtonX;
    pushButtonLOAD_CONFIGExtra->setText("...");
    pushButtonLOAD_CONFIGExtra->setFixedWidth(30);
    pushButtonBOUND_IMPORTExtra=new QPushButtonX;
    pushButtonBOUND_IMPORTExtra->setText("...");
    pushButtonBOUND_IMPORTExtra->setFixedWidth(30);
    pushButtonIATExtra=new QPushButtonX;
    pushButtonIATExtra->setText("...");
    pushButtonIATExtra->setFixedWidth(30);
    pushButtonDELAY_IMPORTExtra=new QPushButtonX;
    pushButtonDELAY_IMPORTExtra->setText("...");
    pushButtonDELAY_IMPORTExtra->setFixedWidth(30);
    pushButtonCOM_DESCRIPTORExtra=new QPushButtonX;
    pushButtonCOM_DESCRIPTORExtra->setText("...");
    pushButtonCOM_DESCRIPTORExtra->setFixedWidth(30);
    pushButtonReservedExtra=new QPushButtonX;
    pushButtonReservedExtra->setText("...");
    pushButtonReservedExtra->setFixedWidth(30);

    pushButtonEXPORTHex=new QPushButtonX;
    pushButtonEXPORTHex->setText(tr("H"));
    pushButtonEXPORTHex->setFixedWidth(30);
    pushButtonIMPORTHex=new QPushButtonX;
    pushButtonIMPORTHex->setText(tr("H"));
    pushButtonIMPORTHex->setFixedWidth(30);
    pushButtonRESOURCEHex=new QPushButtonX;
    pushButtonRESOURCEHex->setText(tr("H"));
    pushButtonRESOURCEHex->setFixedWidth(30);
    pushButtonEXCEPTIONHex=new QPushButtonX;
    pushButtonEXCEPTIONHex->setText(tr("H"));
    pushButtonEXCEPTIONHex->setFixedWidth(30);
    pushButtonSECURITYHex=new QPushButtonX;
    pushButtonSECURITYHex->setText(tr("H"));
    pushButtonSECURITYHex->setFixedWidth(30);
    pushButtonBASERELOCHex=new QPushButtonX;
    pushButtonBASERELOCHex->setText(tr("H"));
    pushButtonBASERELOCHex->setFixedWidth(30);
    pushButtonDEBUGHex=new QPushButtonX;
    pushButtonDEBUGHex->setText(tr("H"));
    pushButtonDEBUGHex->setFixedWidth(30);
    pushButtonCOPYRIGHTHex=new QPushButtonX;
    pushButtonCOPYRIGHTHex->setText(tr("H"));
    pushButtonCOPYRIGHTHex->setFixedWidth(30);
    pushButtonGLOBALPTRHex=new QPushButtonX;
    pushButtonGLOBALPTRHex->setText(tr("H"));
    pushButtonGLOBALPTRHex->setFixedWidth(30);
    pushButtonTLSHex=new QPushButtonX;
    pushButtonTLSHex->setText(tr("H"));
    pushButtonTLSHex->setFixedWidth(30);
    pushButtonLOAD_CONFIGHex=new QPushButtonX;
    pushButtonLOAD_CONFIGHex->setText(tr("H"));
    pushButtonLOAD_CONFIGHex->setFixedWidth(30);
    pushButtonBOUND_IMPORTHex=new QPushButtonX;
    pushButtonBOUND_IMPORTHex->setText(tr("H"));
    pushButtonBOUND_IMPORTHex->setFixedWidth(30);
    pushButtonIATHex=new QPushButtonX;
    pushButtonIATHex->setText(tr("H"));
    pushButtonIATHex->setFixedWidth(30);
    pushButtonDELAY_IMPORTHex=new QPushButtonX;
    pushButtonDELAY_IMPORTHex->setText(tr("H"));
    pushButtonDELAY_IMPORTHex->setFixedWidth(30);
    pushButtonCOM_DESCRIPTORHex=new QPushButtonX;
    pushButtonCOM_DESCRIPTORHex->setText(tr("H"));
    pushButtonCOM_DESCRIPTORHex->setFixedWidth(30);
    pushButtonReservedHex=new QPushButtonX;
    pushButtonReservedHex->setText(tr("H"));
    pushButtonReservedHex->setFixedWidth(30);

    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(0,4),pushButtonEXPORTExtra);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(1,4),pushButtonIMPORTExtra);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(2,4),pushButtonRESOURCEExtra);
    //    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(3,4),pushButtonEXCEPTIONExtra);
    //    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(4,4),pushButtonSECURITYExtra);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(5,4),pushButtonBASERELOCExtra);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(6,4),pushButtonDEBUGExtra);
    //    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(7,4),pushButtonCOPYRIGHTExtra);
    //    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(8,4),pushButtonGLOBALPTRExtra);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(9,4),pushButtonTLSExtra);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(10,4),pushButtonLOAD_CONFIGExtra);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(11,4),pushButtonBOUND_IMPORTExtra);
    //    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(12,4),pushButtonIATExtra);
    //    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(13,4),pushButtonDELAY_IMPORTExtra);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(14,4),pushButtonCOM_DESCRIPTORExtra);
    //    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(15,4),pushButtonReservedExtra);

    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(0,5),pushButtonEXPORTHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(1,5),pushButtonIMPORTHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(2,5),pushButtonRESOURCEHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(3,5),pushButtonEXCEPTIONHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(4,5),pushButtonSECURITYHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(5,5),pushButtonBASERELOCHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(6,5),pushButtonDEBUGHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(7,5),pushButtonCOPYRIGHTHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(8,5),pushButtonGLOBALPTRHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(9,5),pushButtonTLSHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(10,5),pushButtonLOAD_CONFIGHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(11,5),pushButtonBOUND_IMPORTHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(12,5),pushButtonIATHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(13,5),pushButtonDELAY_IMPORTHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(14,5),pushButtonCOM_DESCRIPTORHex);
    ui->tableWidgetDirectories->setIndexWidget(ui->tableWidgetDirectories->model()->index(15,5),pushButtonReservedHex);

    connect(pushButtonEXPORTExtra,SIGNAL(clicked()),this,SLOT(pushButtonEXPORTExtra_clicked()));
    connect(pushButtonIMPORTExtra,SIGNAL(clicked()),this,SLOT(pushButtonIMPORTExtra_clicked()));
    connect(pushButtonRESOURCEExtra,SIGNAL(clicked()),this,SLOT(pushButtonRESOURCEExtra_clicked()));
    connect(pushButtonEXCEPTIONExtra,SIGNAL(clicked()),this,SLOT(pushButtonEXCEPTIONExtra_clicked()));
    connect(pushButtonSECURITYExtra,SIGNAL(clicked()),this,SLOT(pushButtonSECURITYExtra_clicked()));
    connect(pushButtonBASERELOCExtra,SIGNAL(clicked()),this,SLOT(pushButtonBASERELOCExtra_clicked()));
    connect(pushButtonDEBUGExtra,SIGNAL(clicked()),this,SLOT(pushButtonDEBUGExtra_clicked()));
    connect(pushButtonCOPYRIGHTExtra,SIGNAL(clicked()),this,SLOT(pushButtonCOPYRIGHTExtra_clicked()));
    connect(pushButtonGLOBALPTRExtra,SIGNAL(clicked()),this,SLOT(pushButtonGLOBALPTRExtra_clicked()));
    connect(pushButtonTLSExtra,SIGNAL(clicked()),this,SLOT(pushButtonTLSExtra_clicked()));
    connect(pushButtonLOAD_CONFIGExtra,SIGNAL(clicked()),this,SLOT(pushButtonLOAD_CONFIGExtra_clicked()));
    connect(pushButtonBOUND_IMPORTExtra,SIGNAL(clicked()),this,SLOT(pushButtonBOUND_IMPORTExtra_clicked()));
    connect(pushButtonIATExtra,SIGNAL(clicked()),this,SLOT(pushButtonIATExtra_clicked()));
    connect(pushButtonDELAY_IMPORTExtra,SIGNAL(clicked()),this,SLOT(pushButtonDELAY_IMPORTExtra_clicked()));
    connect(pushButtonCOM_DESCRIPTORExtra,SIGNAL(clicked()),this,SLOT(pushButtonCOM_DESCRIPTORExtra_clicked()));
    connect(pushButtonReservedExtra,SIGNAL(clicked()),this,SLOT(pushButtonReservedExtra_clicked()));

    connect(pushButtonEXPORTHex,SIGNAL(clicked()),this,SLOT(pushButtonEXPORTHex_clicked()));
    connect(pushButtonIMPORTHex,SIGNAL(clicked()),this,SLOT(pushButtonIMPORTHex_clicked()));
    connect(pushButtonRESOURCEHex,SIGNAL(clicked()),this,SLOT(pushButtonRESOURCEHex_clicked()));
    connect(pushButtonEXCEPTIONHex,SIGNAL(clicked()),this,SLOT(pushButtonEXCEPTIONHex_clicked()));
    connect(pushButtonSECURITYHex,SIGNAL(clicked()),this,SLOT(pushButtonSECURITYHex_clicked()));
    connect(pushButtonBASERELOCHex,SIGNAL(clicked()),this,SLOT(pushButtonBASERELOCHex_clicked()));
    connect(pushButtonDEBUGHex,SIGNAL(clicked()),this,SLOT(pushButtonDEBUGHex_clicked()));
    connect(pushButtonCOPYRIGHTHex,SIGNAL(clicked()),this,SLOT(pushButtonCOPYRIGHTHex_clicked()));
    connect(pushButtonGLOBALPTRHex,SIGNAL(clicked()),this,SLOT(pushButtonGLOBALPTRHex_clicked()));
    connect(pushButtonTLSHex,SIGNAL(clicked()),this,SLOT(pushButtonTLSHex_clicked()));
    connect(pushButtonLOAD_CONFIGHex,SIGNAL(clicked()),this,SLOT(pushButtonLOAD_CONFIGHex_clicked()));
    connect(pushButtonBOUND_IMPORTHex,SIGNAL(clicked()),this,SLOT(pushButtonBOUND_IMPORTHex_clicked()));
    connect(pushButtonIATHex,SIGNAL(clicked()),this,SLOT(pushButtonIATHex_clicked()));
    connect(pushButtonDELAY_IMPORTHex,SIGNAL(clicked()),this,SLOT(pushButtonDELAY_IMPORTHex_clicked()));
    connect(pushButtonCOM_DESCRIPTORHex,SIGNAL(clicked()),this,SLOT(pushButtonCOM_DESCRIPTORHex_clicked()));
    connect(pushButtonReservedHex,SIGNAL(clicked()),this,SLOT(pushButtonReservedHex_clicked()));

    ui->tableWidgetDirectories->resizeColumnToContents(0);
    ui->tableWidgetDirectories->resizeColumnToContents(1);
    ui->tableWidgetDirectories->setColumnWidth(4,30);
    ui->tableWidgetDirectories->setColumnWidth(5,30);

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
    ui->pushButtonHex->setShortcut(QKeySequence(__KeySequence_hex));
}

DialogPEDirectories::~DialogPEDirectories()
{
    delete ui;
}
bool DialogPEDirectories::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));

        ui->lineEditOffset->setToolTip(tr("File offset of %1").arg(tr("Directories")));
        ui->lineEditSize->setToolTip(tr("Size of %1 in bytes").arg(tr("Directories")));
        ui->pushButtonHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Directories")));

        pushButtonEXPORTHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Export")));
        pushButtonIMPORTHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Import")));
        pushButtonRESOURCEHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Resource")));
        pushButtonEXCEPTIONHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Exception")));
        pushButtonSECURITYHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Security")));
        pushButtonBASERELOCHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Basereloc")));
        pushButtonDEBUGHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Debug")));
        pushButtonCOPYRIGHTHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Copyright")));
        pushButtonGLOBALPTRHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Globalptr")));
        pushButtonTLSHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("TLS")));
        pushButtonLOAD_CONFIGHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Load config")));
        pushButtonBOUND_IMPORTHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Bound import")));
        pushButtonIATHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("IAT")));
        pushButtonDELAY_IMPORTHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Delay import")));
        pushButtonCOM_DESCRIPTORHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("COM descriptor")));
        pushButtonReservedHex->setToolTip(tr("%1 HEX/Disasm").arg(tr("Reserved")));

        pushButtonEXPORTExtra->setToolTip(tr("Export"));
        pushButtonIMPORTExtra->setToolTip(tr("Import"));
        pushButtonRESOURCEExtra->setToolTip(tr("Resource"));
        pushButtonEXCEPTIONExtra->setToolTip(tr("Exception"));
        pushButtonSECURITYExtra->setToolTip(tr("Security"));
        pushButtonBASERELOCExtra->setToolTip(tr("Basereloc"));
        pushButtonDEBUGExtra->setToolTip(tr("Debug"));
        pushButtonCOPYRIGHTExtra->setToolTip(tr("Copyright"));
        pushButtonGLOBALPTRExtra->setToolTip(tr("Globalptr"));
        pushButtonTLSExtra->setToolTip(tr("TLS"));
        pushButtonLOAD_CONFIGExtra->setToolTip(tr("Load config"));
        pushButtonBOUND_IMPORTExtra->setToolTip(tr("Bound import"));
        pushButtonIATExtra->setToolTip(tr("IAT"));
        pushButtonDELAY_IMPORTExtra->setToolTip(tr("Delay import"));
        pushButtonCOM_DESCRIPTORExtra->setToolTip(tr("COM descriptor"));
        pushButtonReservedExtra->setToolTip(tr("Reserved"));
    }

    IMAGE_DATA_DIRECTORY idd;

    //    pefile->setFileName(sFileName);

    nOffset=pefile->getDataDirectoriesOffset();
    nSize=pefile->getDataDirectoriesSize();

    ui->lineEditOffset->setText(QString("%1").arg(nOffset));
    ui->lineEditSize->setText(QString("%1").arg(nSize));

    pefile->getOptionalHeader_DataDirectory(0,&idd);

    lineeditEXPORTAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditEXPORTSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(0))
    {
        pushButtonEXPORTExtra->setEnabled(true);
        pushButtonEXPORTHex->setEnabled(true);
    }
    else
    {
        pushButtonEXPORTExtra->setEnabled(false);
        pushButtonEXPORTHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(1,&idd);

    lineeditIMPORTAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditIMPORTSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(1))
    {
        pushButtonIMPORTExtra->setEnabled(true);
        pushButtonIMPORTHex->setEnabled(true);
    }
    else
    {
        pushButtonIMPORTExtra->setEnabled(false);
        pushButtonIMPORTHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(2,&idd);

    lineeditRESOURCEAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditRESOURCESize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(2))
    {
        pushButtonRESOURCEExtra->setEnabled(true);
        pushButtonRESOURCEHex->setEnabled(true);
    }
    else
    {
        pushButtonRESOURCEExtra->setEnabled(false);
        pushButtonRESOURCEHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(3,&idd);

    lineeditEXCEPTIONAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditEXCEPTIONSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(3))
    {
        pushButtonEXCEPTIONExtra->setEnabled(true);
        pushButtonEXCEPTIONHex->setEnabled(true);
    }
    else
    {
        pushButtonEXCEPTIONExtra->setEnabled(false);
        pushButtonEXCEPTIONHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(4,&idd);

    lineeditSECURITYAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditSECURITYSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if((pefile->isOffsetAndSizeValid(idd.VirtualAddress,idd.Size))&&(idd.VirtualAddress)) // Offset, not RVA!!!
    {
        pushButtonSECURITYExtra->setEnabled(true);
        pushButtonSECURITYHex->setEnabled(true);
    }
    else
    {
        pushButtonSECURITYExtra->setEnabled(false);
        pushButtonSECURITYHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(5,&idd);

    lineeditBASERELOCAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditBASERELOCSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(5))
    {
        pushButtonBASERELOCExtra->setEnabled(true);
        pushButtonBASERELOCHex->setEnabled(true);
    }
    else
    {
        pushButtonBASERELOCExtra->setEnabled(false);
        pushButtonBASERELOCHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(6,&idd);

    lineeditDEBUGAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditDEBUGSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(6))
    {
        pushButtonDEBUGExtra->setEnabled(true);
        pushButtonDEBUGHex->setEnabled(true);
    }
    else
    {
        pushButtonDEBUGExtra->setEnabled(false);
        pushButtonDEBUGHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(7,&idd);

    lineeditCOPYRIGHTAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditCOPYRIGHTSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(7))
    {
        pushButtonCOPYRIGHTExtra->setEnabled(true);
        pushButtonCOPYRIGHTHex->setEnabled(true);
    }
    else
    {
        pushButtonCOPYRIGHTExtra->setEnabled(false);
        pushButtonCOPYRIGHTHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(8,&idd);

    lineeditGLOBALPTRAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditGLOBALPTRSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(8))
    {
        pushButtonGLOBALPTRExtra->setEnabled(true);
        pushButtonGLOBALPTRHex->setEnabled(true);
    }
    else
    {
        pushButtonGLOBALPTRExtra->setEnabled(false);
        pushButtonGLOBALPTRHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(9,&idd);

    lineeditTLSAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditTLSSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(9))
    {
        pushButtonTLSExtra->setEnabled(true);
        pushButtonTLSHex->setEnabled(true);
    }
    else
    {
        pushButtonTLSExtra->setEnabled(false);
        pushButtonTLSHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(10,&idd);

    lineeditLOAD_CONFIGAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditLOAD_CONFIGSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(10))
    {
        pushButtonLOAD_CONFIGExtra->setEnabled(true);
        pushButtonLOAD_CONFIGHex->setEnabled(true);
    }
    else
    {
        pushButtonLOAD_CONFIGExtra->setEnabled(false);
        pushButtonLOAD_CONFIGHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(11,&idd);

    lineeditBOUND_IMPORTAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditBOUND_IMPORTSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(11))
    {
        pushButtonBOUND_IMPORTExtra->setEnabled(true);
        pushButtonBOUND_IMPORTHex->setEnabled(true);
    }
    else
    {
        pushButtonBOUND_IMPORTExtra->setEnabled(false);
        pushButtonBOUND_IMPORTHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(12,&idd);

    lineeditIATAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditIATSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(12))
    {
        pushButtonIATExtra->setEnabled(true);
        pushButtonIATHex->setEnabled(true);
    }
    else
    {
        pushButtonIATExtra->setEnabled(false);
        pushButtonIATHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(13,&idd);

    lineeditDELAY_IMPORTAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditDELAY_IMPORTSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(13))
    {
        pushButtonDELAY_IMPORTExtra->setEnabled(true);
        pushButtonDELAY_IMPORTHex->setEnabled(true);
    }
    else
    {
        pushButtonDELAY_IMPORTExtra->setEnabled(false);
        pushButtonDELAY_IMPORTHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(14,&idd);

    lineeditCOM_DESCRIPTORAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditCOM_DESCRIPTORSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(14))
    {
        pushButtonCOM_DESCRIPTORExtra->setEnabled(true);
        pushButtonCOM_DESCRIPTORHex->setEnabled(true);
    }
    else
    {
        pushButtonCOM_DESCRIPTORExtra->setEnabled(false);
        pushButtonCOM_DESCRIPTORHex->setEnabled(false);
    }

    pefile->getOptionalHeader_DataDirectory(15,&idd);

    lineeditReservedAddress->setText(QString("%1").arg(idd.VirtualAddress,8,16,QChar('0')));
    lineeditReservedSize->setText(QString("%1").arg(idd.Size,8,16,QChar('0')));

    if(pefile->isDirectoryPresent(15))
    {
        pushButtonReservedExtra->setEnabled(true);
        pushButtonReservedHex->setEnabled(true);
    }
    else
    {
        pushButtonReservedExtra->setEnabled(false);
        pushButtonReservedHex->setEnabled(false);
    }

    int nNumberOfRvaAndSizes=pefile->getOptionalHeader_NumberOfRvaAndSizes();

    if(nNumberOfRvaAndSizes<16)
    {
        pushButtonReservedExtra->setEnabled(false);
        pushButtonReservedHex->setEnabled(false);
        lineeditReservedAddress->setEnabled(false);
        lineeditReservedSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<15)
    {
        pushButtonCOM_DESCRIPTORExtra->setEnabled(false);
        pushButtonCOM_DESCRIPTORHex->setEnabled(false);
        lineeditCOM_DESCRIPTORAddress->setEnabled(false);
        lineeditCOM_DESCRIPTORSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<14)
    {
        pushButtonDELAY_IMPORTExtra->setEnabled(false);
        pushButtonDELAY_IMPORTHex->setEnabled(false);
        lineeditDELAY_IMPORTAddress->setEnabled(false);
        lineeditDELAY_IMPORTSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<13)
    {
        pushButtonIATExtra->setEnabled(false);
        pushButtonIATHex->setEnabled(false);
        lineeditIATAddress->setEnabled(false);
        lineeditIATSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<12)
    {
        pushButtonBOUND_IMPORTExtra->setEnabled(false);
        pushButtonBOUND_IMPORTHex->setEnabled(false);
        lineeditBOUND_IMPORTAddress->setEnabled(false);
        lineeditBOUND_IMPORTSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<11)
    {
        pushButtonLOAD_CONFIGExtra->setEnabled(false);
        pushButtonLOAD_CONFIGHex->setEnabled(false);
        lineeditLOAD_CONFIGAddress->setEnabled(false);
        lineeditLOAD_CONFIGSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<10)
    {
        pushButtonTLSExtra->setEnabled(false);
        pushButtonTLSHex->setEnabled(false);
        lineeditTLSAddress->setEnabled(false);
        lineeditTLSSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<9)
    {
        pushButtonGLOBALPTRExtra->setEnabled(false);
        pushButtonGLOBALPTRHex->setEnabled(false);
        lineeditGLOBALPTRAddress->setEnabled(false);
        lineeditGLOBALPTRSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<8)
    {
        pushButtonCOPYRIGHTExtra->setEnabled(false);
        pushButtonCOPYRIGHTHex->setEnabled(false);
        lineeditCOPYRIGHTAddress->setEnabled(false);
        lineeditCOPYRIGHTSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<7)
    {
        pushButtonDEBUGExtra->setEnabled(false);
        pushButtonDEBUGHex->setEnabled(false);
        lineeditDEBUGAddress->setEnabled(false);
        lineeditDEBUGSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<6)
    {
        pushButtonBASERELOCExtra->setEnabled(false);
        pushButtonBASERELOCHex->setEnabled(false);
        lineeditBASERELOCAddress->setEnabled(false);
        lineeditBASERELOCSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<5)
    {
        pushButtonSECURITYExtra->setEnabled(false);
        pushButtonSECURITYHex->setEnabled(false);
        lineeditSECURITYAddress->setEnabled(false);
        lineeditSECURITYSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<4)
    {
        pushButtonEXCEPTIONExtra->setEnabled(false);
        pushButtonEXCEPTIONHex->setEnabled(false);
        lineeditEXCEPTIONAddress->setEnabled(false);
        lineeditEXCEPTIONSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<3)
    {
        pushButtonRESOURCEExtra->setEnabled(false);
        pushButtonRESOURCEHex->setEnabled(false);
        lineeditRESOURCEAddress->setEnabled(false);
        lineeditRESOURCESize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<2)
    {
        pushButtonIMPORTExtra->setEnabled(false);
        pushButtonIMPORTHex->setEnabled(false);
        lineeditIMPORTAddress->setEnabled(false);
        lineeditIMPORTSize->setEnabled(false);
    }

    if(nNumberOfRvaAndSizes<1)
    {
        pushButtonEXPORTExtra->setEnabled(false);
        pushButtonEXPORTHex->setEnabled(false);
        lineeditEXPORTAddress->setEnabled(false);
        lineeditEXPORTSize->setEnabled(false);
    }

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());

    ui->pushButtonApply->setEnabled(false);

    //   emit reloadSignal();

    return true;
}

void DialogPEDirectories::on_checkBoxReadOnly_stateChanged(int arg1)
{
    lineeditEXPORTAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditIMPORTAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditRESOURCEAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditEXCEPTIONAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditSECURITYAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditBASERELOCAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditDEBUGAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditCOPYRIGHTAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditGLOBALPTRAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditTLSAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditLOAD_CONFIGAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditBOUND_IMPORTAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditIATAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditDELAY_IMPORTAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditCOM_DESCRIPTORAddress->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditReservedAddress->setReadOnly(ui->checkBoxReadOnly->checkState());

    lineeditEXPORTSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditIMPORTSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditRESOURCESize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditEXCEPTIONSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditSECURITYSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditBASERELOCSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditDEBUGSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditCOPYRIGHTSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditGLOBALPTRSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditTLSSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditLOAD_CONFIGSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditBOUND_IMPORTSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditIATSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditDELAY_IMPORTSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditCOM_DESCRIPTORSize->setReadOnly(ui->checkBoxReadOnly->checkState());
    lineeditReservedSize->setReadOnly(ui->checkBoxReadOnly->checkState());
}

void DialogPEDirectories::on_pushButtonHex_clicked()
{
    DialogHDE dialoghde(this);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",nOffset,nSize,tr("Directories"));

    dialoghde.exec();
}

void DialogPEDirectories::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogPEDirectories::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPEDirectories::on_pushButtonApply_clicked()
{
    bool bTemp;
    IMAGE_DATA_DIRECTORY idd;

    ui->pushButtonApply->setEnabled(false);

    idd.VirtualAddress=lineeditEXPORTAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditEXPORTSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(0,&idd);

    idd.VirtualAddress=lineeditIMPORTAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditIMPORTSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(1,&idd);

    idd.VirtualAddress=lineeditRESOURCEAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditRESOURCESize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(2,&idd);

    idd.VirtualAddress=lineeditEXCEPTIONAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditEXCEPTIONSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(3,&idd);

    idd.VirtualAddress=lineeditSECURITYAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditSECURITYSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(4,&idd);

    idd.VirtualAddress=lineeditBASERELOCAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditBASERELOCSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(5,&idd);

    idd.VirtualAddress=lineeditDEBUGAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditDEBUGSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(6,&idd);

    idd.VirtualAddress=lineeditCOPYRIGHTAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditCOPYRIGHTSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(7,&idd);

    idd.VirtualAddress=lineeditGLOBALPTRAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditGLOBALPTRSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(8,&idd);

    idd.VirtualAddress=lineeditTLSAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditTLSSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(9,&idd);

    idd.VirtualAddress=lineeditLOAD_CONFIGAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditLOAD_CONFIGSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(10,&idd);

    idd.VirtualAddress=lineeditBOUND_IMPORTAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditBOUND_IMPORTSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(11,&idd);

    idd.VirtualAddress=lineeditIATAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditIATSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(12,&idd);

    idd.VirtualAddress=lineeditDELAY_IMPORTAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditDELAY_IMPORTSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(13,&idd);

    idd.VirtualAddress=lineeditCOM_DESCRIPTORAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditCOM_DESCRIPTORSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(14,&idd);

    idd.VirtualAddress=lineeditReservedAddress->text().toUInt(&bTemp,16);
    idd.Size=lineeditReservedSize->text().toUInt(&bTemp,16);

    pefile->setOptionalHeader_DataDirectory(15,&idd);

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPEDirectories::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogPEDirectories::pushButtonEXPORTExtra_clicked()
{
    DialogPEExport dialogpeexport(this);
    dialogpeexport.setData(pOptions,pefile);

    dialogpeexport.exec();
}
void DialogPEDirectories::pushButtonIMPORTExtra_clicked()
{
    DialogPEImport dialogpeimport(this);
    dialogpeimport.setData(pOptions,pefile);

    dialogpeimport.exec();
}
void DialogPEDirectories::pushButtonRESOURCEExtra_clicked()
{
    DialogPEResource dialogperesource(this);
    dialogperesource.setData(pOptions,pefile);

    dialogperesource.exec();
}
void DialogPEDirectories::pushButtonEXCEPTIONExtra_clicked()
{
}
void DialogPEDirectories::pushButtonSECURITYExtra_clicked()
{
}
void DialogPEDirectories::pushButtonBASERELOCExtra_clicked()
{
    DialogPEBaseReloc dialogbasereloc(this);
    dialogbasereloc.setData(pOptions,pefile);

    dialogbasereloc.exec();
}
void DialogPEDirectories::pushButtonDEBUGExtra_clicked()
{
    DialogPEDebug dialogpedebug(this);
    dialogpedebug.setData(pOptions,pefile);

    dialogpedebug.exec();
}
void DialogPEDirectories::pushButtonCOPYRIGHTExtra_clicked()
{
}
void DialogPEDirectories::pushButtonGLOBALPTRExtra_clicked()
{
}
void DialogPEDirectories::pushButtonTLSExtra_clicked()
{
    DialogPETLS dialogpetls(this);
    dialogpetls.setData(pOptions,pefile);

    dialogpetls.exec();
}
void DialogPEDirectories::pushButtonLOAD_CONFIGExtra_clicked()
{
    DialogPELoadConfig dialogpeloadconfig(this);
    dialogpeloadconfig.setData(pOptions,pefile);

    dialogpeloadconfig.exec();
}
void DialogPEDirectories::pushButtonBOUND_IMPORTExtra_clicked()
{
    DialogPEBoundImport dialogbi(this);
    dialogbi.setData(pOptions,pefile);

    dialogbi.exec();
}
void DialogPEDirectories::pushButtonIATExtra_clicked()
{
}
void DialogPEDirectories::pushButtonDELAY_IMPORTExtra_clicked()
{
}
void DialogPEDirectories::pushButtonCOM_DESCRIPTORExtra_clicked()
{
    DialogPECOM dialogpecom(this);
    dialogpecom.setData(pOptions,pefile);

    dialogpecom.exec();
}
void DialogPEDirectories::pushButtonReservedExtra_clicked()
{
}
void DialogPEDirectories::pushButtonEXPORTHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Export"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonIMPORTHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Import"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonRESOURCEHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_RESOURCE,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Resource"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonEXCEPTIONHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_EXCEPTION,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Exceptions"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonSECURITYHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_SECURITY,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",idd.VirtualAddress,idd.Size,tr("Security")); // Except!!! offset, not RVA

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonBASERELOCHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Basereloc"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonDEBUGHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_DEBUG,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Debug"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonCOPYRIGHTHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_ARCHITECTURE,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Copyright"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonGLOBALPTRHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_GLOBALPTR,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Globalptr"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonTLSHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_TLS,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("TLS"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonLOAD_CONFIGHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Load config"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonBOUND_IMPORTHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Bound import"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonIATHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_IAT,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("IAT"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonDELAY_IMPORTHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Delay import"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonCOM_DESCRIPTORHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("COM descriptor"));

    dialoghde.exec();
}
void DialogPEDirectories::pushButtonReservedHex_clicked()
{
    IMAGE_DATA_DIRECTORY idd;

    DialogHDE dialoghde(this);

    pefile->getOptionalHeader_DataDirectory(15,&idd);

    dialoghde.setData(pOptions,pefile->getFileName(),0,0,"HEX","","",pefile->RVAToOffset(idd.VirtualAddress),idd.Size,tr("Reserved"));

    dialoghde.exec();
}
