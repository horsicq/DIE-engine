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
#include "dialogpesectioncharacteristics.h"
#include "ui_dialogpesectioncharacteristics.h"

DialogPESectionCharacteristics::DialogPESectionCharacteristics(QWidget *parent) :
    DialogPEGeneric(parent),
    ui(new Ui::DialogPESectionCharacteristics)
{
    ui->setupUi(this);

    bEdited=true;

    ui->comboBoxALIGN->addItem("");
    ui->comboBoxALIGN->addItem("ALIGN_1BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_2BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_4BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_8BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_16BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_32BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_64BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_128BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_256BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_512BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_1024BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_2048BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_4096BYTES");
    ui->comboBoxALIGN->addItem("ALIGN_8192BYTES");

    ui->lineEditSectionSectionCharacteristics->setInputMask("HHHHHHHH");

    connect(this,SIGNAL(checked()),this,SLOT(checkbox_reload()));

    ui->checkBoxReadOnly->setChecked(true);

    ui->pushButtonCancel->setShortcut(QKeySequence(__KeySequence_quit));
    ui->pushButtonApply->setShortcut(QKeySequence(__KeySequence_save));
    ui->pushButtonOK->setShortcut(QKeySequence(__KeySequence_saveandquit));
}

DialogPESectionCharacteristics::~DialogPESectionCharacteristics()
{
    delete ui;
}

void DialogPESectionCharacteristics::setData(__DIE_OPTIONS *pOptions,PEFile *pefile, int nSection)
{
    this->nSection=nSection;

    DialogPEGeneric::setData(pOptions,pefile);
}

void DialogPESectionCharacteristics::on_pushButtonOK_clicked()
{
    if(ui->pushButtonApply->isEnabled())
    {
        on_pushButtonApply_clicked();
    }

    on_pushButtonCancel_clicked();
}

bool DialogPESectionCharacteristics::reload()
{
    if(pOptions->bShowTooltips)
    {
        ui->pushButtonOK->setToolTip(tr("Quit and apply changes"));
        ui->pushButtonCancel->setToolTip(tr("Quit without changes"));
        ui->pushButtonApply->setToolTip(tr("Apply changes"));
        ui->checkBoxReadOnly->setToolTip(tr("Set/unset read-only mode"));
        ui->lineEditSectionSectionCharacteristics->setToolTip(tr("Section Characteristics"));

        ui->checkBoxCNT_CODE->setToolTip(tr("Section contains code"));
        ui->checkBoxCNT_INITIALIZED_DATA->setToolTip(tr("Section contains initialized data"));
        ui->checkBoxCNT_UNINITIALIZED_DATA->setToolTip(tr("Section contains uninitialized data"));
        ui->checkBoxLNK_INFO->setToolTip(tr("Section contains comments or some other type of information"));
        ui->checkBoxLNK_REMOVE->setToolTip(tr("Section contents will not become part of image"));
        ui->checkBoxLNK_COMDAT->setToolTip(tr("Section contents comdat"));
        ui->checkBoxNO_DEFER_SPEC_EXC->setToolTip(tr("Reset speculative exceptions handling bits in the TLB entries for this section"));
        ui->checkBoxGPREL->setToolTip(tr("Section content can be accessed relative to GP"));
        ui->checkBoxMEM_PURGEABLE->setToolTip(tr("Purgeable"));
        ui->checkBoxMEM_LOCKED->setToolTip(tr("Locked"));
        ui->checkBoxMEM_PRELOAD->setToolTip(tr("Preload"));
        ui->checkBoxLNK_NRELOC_OVFL->setToolTip(tr("Section contains extended relocations"));
        ui->checkBoxMEM_DISCARDABLE->setToolTip(tr("Section can be discarded"));
        ui->checkBoxMEM_NOT_CACHED->setToolTip(tr("Section is not cachable"));
        ui->checkBoxMEM_NOT_PAGED->setToolTip(tr("Section is not pageable"));
        ui->checkBoxMEM_SHARED->setToolTip(tr("Section is shareable"));
        ui->checkBoxMEM_EXECUTE->setToolTip(tr("Section is executable"));
        ui->checkBoxMEM_READ->setToolTip(tr("Section is readable"));
        ui->checkBoxMEM_WRITE->setToolTip(tr("Section is writeable"));

        ui->comboBoxALIGN->setToolTip(tr("Set align"));
    }

    bEdited=false;

    ui->lineEditSectionSectionCharacteristics->setText(QString("%1").arg(pefile->getSection_Characteristics(nSection),8,16,QChar('0')));

    bEdited=true;

    ui->checkBoxReadOnly->setEnabled(!pefile->isReadOnly());
    ui->pushButtonApply->setEnabled(false);

    return true;
}

void DialogPESectionCharacteristics::on_checkBoxReadOnly_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    ui->lineEditSectionSectionCharacteristics->setReadOnly(ui->checkBoxReadOnly->isChecked());
    ui->comboBoxALIGN->setEnabled(!ui->checkBoxReadOnly->isChecked());
    ui->groupBox->setEnabled(!ui->checkBoxReadOnly->isChecked());
}

void DialogPESectionCharacteristics::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogPESectionCharacteristics::on_pushButtonApply_clicked()
{
    bool bTemp;
    ui->pushButtonApply->setEnabled(false);

    pefile->setSection_Characteristics(nSection,ui->lineEditSectionSectionCharacteristics->text().toUInt(&bTemp,16));

    ui->checkBoxReadOnly->setCheckState(Qt::Unchecked);

    emit reloadSignal();
}
void DialogPESectionCharacteristics::on_lineEditSectionSectionCharacteristics_textChanged(const QString &arg1)
{
    bool bTemp;
    unsigned int nCharacteristics=0;

    nCharacteristics=arg1.toUInt(&bTemp,16);

    ui->checkBoxCNT_CODE->setChecked(nCharacteristics&S_IMAGE_SCN_CNT_CODE);
    ui->checkBoxCNT_INITIALIZED_DATA->setChecked(nCharacteristics&S_IMAGE_SCN_CNT_INITIALIZED_DATA);
    ui->checkBoxCNT_UNINITIALIZED_DATA->setChecked(nCharacteristics&S_IMAGE_SCN_CNT_UNINITIALIZED_DATA);
    ui->checkBoxLNK_INFO->setChecked(nCharacteristics&S_IMAGE_SCN_LNK_INFO);
    ui->checkBoxLNK_REMOVE->setChecked(nCharacteristics&S_IMAGE_SCN_LNK_REMOVE);
    ui->checkBoxLNK_COMDAT->setChecked(nCharacteristics&S_IMAGE_SCN_LNK_COMDAT);
    ui->checkBoxNO_DEFER_SPEC_EXC->setChecked(nCharacteristics&S_IMAGE_SCN_NO_DEFER_SPEC_EXC);
    ui->checkBoxGPREL->setChecked(nCharacteristics&S_IMAGE_SCN_GPREL);
    ui->checkBoxMEM_PURGEABLE->setChecked(nCharacteristics&S_IMAGE_SCN_MEM_PURGEABLE);
    ui->checkBoxMEM_LOCKED->setChecked(nCharacteristics&S_IMAGE_SCN_MEM_LOCKED);
    ui->checkBoxMEM_PRELOAD->setChecked(nCharacteristics&S_IMAGE_SCN_MEM_PRELOAD);
    ui->checkBoxLNK_NRELOC_OVFL->setChecked(nCharacteristics&S_IMAGE_SCN_LNK_NRELOC_OVFL);
    ui->checkBoxMEM_DISCARDABLE->setChecked(nCharacteristics&S_IMAGE_SCN_MEM_DISCARDABLE);
    ui->checkBoxMEM_NOT_CACHED->setChecked(nCharacteristics&S_IMAGE_SCN_MEM_NOT_CACHED);
    ui->checkBoxMEM_NOT_PAGED->setChecked(nCharacteristics&S_IMAGE_SCN_MEM_NOT_PAGED);
    ui->checkBoxMEM_SHARED->setChecked(nCharacteristics&S_IMAGE_SCN_MEM_SHARED);
    ui->checkBoxMEM_EXECUTE->setChecked(nCharacteristics&S_IMAGE_SCN_MEM_EXECUTE);
    ui->checkBoxMEM_READ->setChecked(nCharacteristics&S_IMAGE_SCN_MEM_READ);
    ui->checkBoxMEM_WRITE->setChecked(nCharacteristics&S_IMAGE_SCN_MEM_WRITE);

    nCharacteristics=(nCharacteristics&S_IMAGE_SCN_ALIGN_MASK)>>20;

    ui->comboBoxALIGN->setCurrentIndex(nCharacteristics);


    ui->pushButtonApply->setEnabled(true);
}

void DialogPESectionCharacteristics::on_comboBoxALIGN_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxCNT_CODE_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxCNT_INITIALIZED_DATA_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxCNT_UNINITIALIZED_DATA_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxLNK_INFO_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxLNK_REMOVE_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxLNK_COMDAT_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxNO_DEFER_SPEC_EXC_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxGPREL_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxMEM_PURGEABLE_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxMEM_LOCKED_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxMEM_PRELOAD_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxLNK_NRELOC_OVFL_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxMEM_DISCARDABLE_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxMEM_NOT_CACHED_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxMEM_NOT_PAGED_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxMEM_SHARED_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxMEM_EXECUTE_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxMEM_READ_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::on_checkBoxMEM_WRITE_stateChanged(int arg1)
{
    Q_UNUSED(arg1)

    emit checked();
}

void DialogPESectionCharacteristics::checkbox_reload()
{
    unsigned int nCharacteristics=0;

    if(bEdited)
    {
        if(ui->checkBoxCNT_CODE->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_CNT_CODE;
        }

        if(ui->checkBoxCNT_INITIALIZED_DATA->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_CNT_INITIALIZED_DATA;
        }

        if(ui->checkBoxCNT_UNINITIALIZED_DATA->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_CNT_UNINITIALIZED_DATA;
        }

        if(ui->checkBoxLNK_INFO->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_LNK_INFO;
        }

        if(ui->checkBoxLNK_REMOVE->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_LNK_REMOVE;
        }

        if(ui->checkBoxLNK_COMDAT->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_LNK_COMDAT;
        }

        if(ui->checkBoxNO_DEFER_SPEC_EXC->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_NO_DEFER_SPEC_EXC;
        }

        if(ui->checkBoxGPREL->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_GPREL;
        }

        if(ui->checkBoxMEM_PURGEABLE->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_MEM_PURGEABLE;
        }

        if(ui->checkBoxMEM_LOCKED->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_MEM_LOCKED;
        }

        if(ui->checkBoxMEM_PRELOAD->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_MEM_PRELOAD;
        }

        if(ui->checkBoxLNK_NRELOC_OVFL->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_LNK_NRELOC_OVFL;
        }

        if(ui->checkBoxMEM_DISCARDABLE->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_MEM_DISCARDABLE;
        }

        if(ui->checkBoxMEM_NOT_CACHED->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_MEM_NOT_CACHED;
        }

        if(ui->checkBoxMEM_NOT_PAGED->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_MEM_NOT_PAGED;
        }

        if(ui->checkBoxMEM_SHARED->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_MEM_SHARED;
        }

        if(ui->checkBoxMEM_EXECUTE->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_MEM_EXECUTE;
        }

        if(ui->checkBoxMEM_READ->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_MEM_READ;
        }

        if(ui->checkBoxMEM_WRITE->isChecked())
        {
            nCharacteristics+=S_IMAGE_SCN_MEM_WRITE;
        }

        nCharacteristics+=((ui->comboBoxALIGN->currentIndex())<<20);

        ui->lineEditSectionSectionCharacteristics->setText(QString("%1").arg(nCharacteristics,8,16,QChar('0')));
    }
}
