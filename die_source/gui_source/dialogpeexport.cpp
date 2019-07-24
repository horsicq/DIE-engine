#include "dialogpeexport.h"
#include "ui_dialogpeexport.h"

DialogPEExport::DialogPEExport(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPEExport)
{
    ui->setupUi(this);

    actCopyAsString=new QAction("", this);
    connect(actCopyAsString,SIGNAL(triggered()),this,SLOT(copyAsString()));

    ui->lineEditDllName->setHex(false);

    ui->tableWidgetHeader->setRowCount(11);
    ui->tableWidgetHeader->setColumnCount(3);

    for(int i=0; i<11; i++)
    {
        ui->tableWidgetHeader->setRowHeight(i,20);
    }

    QTableWidgetItem *newItem;

    lineEditCharacteristics=new QLineEditX;
    lineEditTimeDateStamp=new QLineEditX;
    lineEditMajorVersion=new QLineEditX;
    lineEditMinorVersion=new QLineEditX;
    lineEditName=new QLineEditX;
    lineEditBase=new QLineEditX;
    lineEditNumberOfFunctions=new QLineEditX;
    lineEditNumberOfNames=new QLineEditX;
    lineEditAddressOfFunctions=new QLineEditX;
    lineEditAddressOfNames=new QLineEditX;
    lineEditAddressOfNameOrdinals=new QLineEditX;

    lineEditCharacteristics->setAlignment(Qt::AlignCenter);
    lineEditTimeDateStamp->setAlignment(Qt::AlignCenter);
    lineEditMajorVersion->setAlignment(Qt::AlignCenter);
    lineEditMinorVersion->setAlignment(Qt::AlignCenter);
    lineEditName->setAlignment(Qt::AlignCenter);
    lineEditBase->setAlignment(Qt::AlignCenter);
    lineEditNumberOfFunctions->setAlignment(Qt::AlignCenter);
    lineEditNumberOfNames->setAlignment(Qt::AlignCenter);
    lineEditAddressOfFunctions->setAlignment(Qt::AlignCenter);
    lineEditAddressOfNames->setAlignment(Qt::AlignCenter);
    lineEditAddressOfNameOrdinals->setAlignment(Qt::AlignCenter);

    lineEditCharacteristics->setInputMask("HHHHHHHH");
    lineEditTimeDateStamp->setInputMask("HHHHHHHH");
    lineEditMajorVersion->setInputMask("HHHH");
    lineEditMinorVersion->setInputMask("HHHH");
    lineEditName->setInputMask("HHHHHHHH");
    lineEditBase->setInputMask("HHHHHHHH");
    lineEditNumberOfFunctions->setInputMask("HHHHHHHH");
    lineEditNumberOfNames->setInputMask("HHHHHHHH");
    lineEditAddressOfFunctions->setInputMask("HHHHHHHH");
    lineEditAddressOfNames->setInputMask("HHHHHHHH");
    lineEditAddressOfNameOrdinals->setInputMask("HHHHHHHH");

    newItem = new QTableWidgetItem;
    newItem->setText("Characteristics");
    ui->tableWidgetHeader->setItem(0,0,newItem);
    ui->tableWidgetHeader->item(0,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText("TimeDateStamp");
    ui->tableWidgetHeader->setItem(1,0,newItem);
    ui->tableWidgetHeader->item(1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText("MajorVersion");
    ui->tableWidgetHeader->setItem(2,0,newItem);
    ui->tableWidgetHeader->item(2,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText("MinorVersion");
    ui->tableWidgetHeader->setItem(3,0,newItem);
    ui->tableWidgetHeader->item(3,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText("Name");
    ui->tableWidgetHeader->setItem(4,0,newItem);
    ui->tableWidgetHeader->item(4,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText("Base");
    ui->tableWidgetHeader->setItem(5,0,newItem);
    ui->tableWidgetHeader->item(5,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText("NumberOfFunctions");
    ui->tableWidgetHeader->setItem(6,0,newItem);
    ui->tableWidgetHeader->item(6,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText("NumberOfNames");
    ui->tableWidgetHeader->setItem(7,0,newItem);
    ui->tableWidgetHeader->item(7,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText("AddressOfFunctions");
    ui->tableWidgetHeader->setItem(8,0,newItem);
    ui->tableWidgetHeader->item(8,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText("AddressOfNames");
    ui->tableWidgetHeader->setItem(9,0,newItem);
    ui->tableWidgetHeader->item(9,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    newItem = new QTableWidgetItem;
    newItem->setText("AddressOfNameOrdinals");
    ui->tableWidgetHeader->setItem(10,0,newItem);
    ui->tableWidgetHeader->item(10,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->tableWidgetHeader->setCellWidget(0,1,lineEditCharacteristics);
    ui->tableWidgetHeader->setCellWidget(1,1,lineEditTimeDateStamp);
    ui->tableWidgetHeader->setCellWidget(2,1,lineEditMajorVersion);
    ui->tableWidgetHeader->setCellWidget(3,1,lineEditMinorVersion);
    ui->tableWidgetHeader->setCellWidget(4,1,lineEditName);
    ui->tableWidgetHeader->setCellWidget(5,1,lineEditBase);
    ui->tableWidgetHeader->setCellWidget(6,1,lineEditNumberOfFunctions);
    ui->tableWidgetHeader->setCellWidget(7,1,lineEditNumberOfNames);
    ui->tableWidgetHeader->setCellWidget(8,1,lineEditAddressOfFunctions);
    ui->tableWidgetHeader->setCellWidget(9,1,lineEditAddressOfNames);
    ui->tableWidgetHeader->setCellWidget(10,1,lineEditAddressOfNameOrdinals);

    ui->tableWidgetHeader->resizeColumnsToContents();

    connect(lineEditCharacteristics,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditTimeDateStamp,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMajorVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditMinorVersion,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditName,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditBase,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditNumberOfFunctions,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditNumberOfNames,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditAddressOfFunctions,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditAddressOfNames,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));
    connect(lineEditAddressOfNameOrdinals,SIGNAL(textChanged(QString)),this,SLOT(applyEnable()));

    //    QFont font("MS Shell Dlg 2", 10, QFont::Bold);
    actEdit=new QAction(tr("Edit"), this);
    //    actEdit->setFont(font);

    connect(actEdit,SIGNAL(triggered()),this,SLOT(edit()));

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPEExport::~DialogPEExport()
{
    emit reset();
    //    scan->_stop();
    //    while(!scan->isCompleted())
    //    {
    //        //qDebug("buzy");
    //    }


    //    delete scan;

    delete ui;
}

bool DialogPEExport::reload()
{

    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditDllName->setToolTip(tr("Dll name"));

        ui->progressBar->setToolTip(tr("Export progress"));
    }

    lineEditCharacteristics->setText(QString("%1").arg(pefile->getExport_Characteristics(),8,16,QChar('0')));
    lineEditTimeDateStamp->setText(QString("%1").arg(pefile->getExport_TimeDateStamp(),8,16,QChar('0')));
    lineEditMajorVersion->setText(QString("%1").arg(pefile->getExport_MajorVersion(),4,16,QChar('0')));
    lineEditMinorVersion->setText(QString("%1").arg(pefile->getExport_MinorVersion(),4,16,QChar('0')));
    lineEditName->setText(QString("%1").arg(pefile->getExport_Name(),8,16,QChar('0')));
    lineEditBase->setText(QString("%1").arg(pefile->getExport_Base(),8,16,QChar('0')));
    lineEditNumberOfFunctions->setText(QString("%1").arg(pefile->getExport_NumberOfFunctions(),8,16,QChar('0')));
    lineEditNumberOfNames->setText(QString("%1").arg(pefile->getExport_NumberOfNames(),8,16,QChar('0')));
    lineEditAddressOfFunctions->setText(QString("%1").arg(pefile->getExport_AddressOfFunctions(),8,16,QChar('0')));
    lineEditAddressOfNames->setText(QString("%1").arg(pefile->getExport_AddressOfNames(),8,16,QChar('0')));
    lineEditAddressOfNameOrdinals->setText(QString("%1").arg(pefile->getExport_AddressOfNameOrdinals(),8,16,QChar('0')));

    ui->lineEditDllName->setText(pefile->getExport_NameAsString());

    QStringList labels;

    labels.append("Ordinal");
    labels.append("RVA");
    labels.append("Function Name");

    if(pefile->getExport_NumberOfFunctions()<0xFFFF)
    {
        ui->tableWidgetFunctions->setColumnCount(3);
        ui->tableWidgetFunctions->setHorizontalHeaderLabels(labels);
        ui->tableWidgetFunctions->setRowCount(pefile->getExport_NumberOfFunctions());
        ui->tableWidgetFunctions->setColumnWidth(0,50);
        ui->tableWidgetFunctions->setColumnWidth(1,70);
        ui->tableWidgetFunctions->setColumnWidth(2,340);

        scan= new ThreadExport;

        QThread* thread =new QThread;
        scan->moveToThread(thread);

        scan->setData(pefile);

        connect(this, SIGNAL(reset()), scan, SLOT(_stop()),Qt::DirectConnection);
        connect(thread, SIGNAL(started()), scan, SLOT(process()));
        connect(scan, SIGNAL(_finished()), thread, SLOT(quit()));
        connect(scan, SIGNAL(_finished()), scan, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        connect(scan,SIGNAL(setProgressBar(int,int)),this,SLOT(setProgressBar(int,int)));
        connect(scan,SIGNAL(appendRow(int,QString,QString,QString)),this,SLOT(appendRow(int,QString,QString,QString)));


        thread->start(QThread::LowestPriority);

        ui->tableWidgetFunctions->setSortingEnabled(false);
    }

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPEExport::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

void DialogPEExport::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPEExport::on_pushButtonApply_clicked()
{
    bool bTemp;

    ui->pushButtonApply->setEnabled(false);

    pefile->setExport_Characteristics(lineEditCharacteristics->text().toUInt(&bTemp,16));
    pefile->setExport_TimeDateStamp(lineEditTimeDateStamp->text().toUInt(&bTemp,16));
    pefile->setExport_MajorVersion(lineEditMajorVersion->text().toUInt(&bTemp,16));
    pefile->setExport_MinorVersion(lineEditMinorVersion->text().toUInt(&bTemp,16));
    pefile->setExport_Name(lineEditName->text().toUInt(&bTemp,16));
    pefile->setExport_Base(lineEditBase->text().toUInt(&bTemp,16));
    pefile->setExport_NumberOfFunctions(lineEditNumberOfFunctions->text().toUInt(&bTemp,16));
    pefile->setExport_NumberOfNames(lineEditNumberOfNames->text().toUInt(&bTemp,16));
    pefile->setExport_AddressOfFunctions(lineEditAddressOfFunctions->text().toUInt(&bTemp,16));
    pefile->setExport_AddressOfNames(lineEditAddressOfNames->text().toUInt(&bTemp,16));
    pefile->setExport_AddressOfNameOrdinals(lineEditAddressOfNameOrdinals->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}

void DialogPEExport::on_checkBoxReadOnly_stateChanged(int arg1)
{
    lineEditCharacteristics->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditTimeDateStamp->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditMajorVersion->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditMinorVersion->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditName->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditBase->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditNumberOfFunctions->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditNumberOfNames->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditAddressOfFunctions->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditAddressOfNames->setReadOnly(ui->checkBoxReadOnly->isChecked());
    lineEditAddressOfNameOrdinals->setReadOnly(ui->checkBoxReadOnly->isChecked());
}

void DialogPEExport::applyEnable()
{
    ui->pushButtonApply->setEnabled(true);
}

void DialogPEExport::edit()
{
    DialogPEExportFunction dialogpeexportfunction(this);
    unsigned int nRow=ui->tableWidgetFunctions->selectedItems().at(0)->row();

    QString sNumber=ui->tableWidgetFunctions->item(nRow,0)->data(Qt::UserRole).toString();
    int nNumber=sNumber.toUInt();
    //    nNumber+=pefile->getExport_Base();

    dialogpeexportfunction.setData(pOptions,pefile,nNumber);

    dialogpeexportfunction.exec();
}

void DialogPEExport::on_tableWidgetFunctions_customContextMenuRequested(const QPoint &pos)
{
    if(!ui->tableWidgetFunctions->selectedItems().count())
    {
        return;
    }

    QMenu menu;

    menu.addAction(actCopyAsString);
    actCopyAsString->setText(tr("Copy as \"%1\"").arg(ui->tableWidgetFunctions->selectedItems().at(2)->text()));

    menu.addSeparator();

    menu.addAction(actEdit);

    QPoint pos2=pos;
    pos2.setY(pos2.ry()+20);
    menu.exec(ui->tableWidgetFunctions->mapToGlobal(pos2));
}
void DialogPEExport::setProgressBar(int nMax, int nValue)
{
    ui->progressBar->setMaximum(nMax);
    ui->progressBar->setValue(nValue);

    if(nMax==nValue)
    {
        ui->tableWidgetFunctions->setSortingEnabled(true);
        ui->tableWidgetFunctions->sortByColumn(0,Qt::AscendingOrder);
    }
}
void DialogPEExport::appendRow(int index,QString sOrdinal,QString sAddress,QString sName)
{
    QTableWidgetItem *newItem;

    newItem = new QTableWidgetItem;

    newItem->setText(sOrdinal);
    newItem->setData(Qt::UserRole,index);

    ui->tableWidgetFunctions->setItem(index,0,newItem);
    ui->tableWidgetFunctions->item(index,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


    newItem = new QTableWidgetItem;

    newItem->setText(sAddress);
    ui->tableWidgetFunctions->setItem(index,1,newItem);
    ui->tableWidgetFunctions->item(index,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    //    QLineEditX *lineEdit=new QLineEditX;
    //    lineEdit->setHex(false);
    //    lineEdit->setReadOnly(true);

    //    lineEdit->setText(sName);

    //    ui->tableWidgetFunctions->setIndexWidget(ui->tableWidgetFunctions->model()->index(index,2),lineEdit);

    newItem = new QTableWidgetItem;

    newItem->setText(sName);
    ui->tableWidgetFunctions->setItem(index,2,newItem);
    ui->tableWidgetFunctions->item(index,2)->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    ui->tableWidgetFunctions->setRowHeight(index,20);
}
void DialogPEExport::copyAsString()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString("%1").arg(ui->tableWidgetFunctions->selectedItems().at(2)->text()));
}
