/***************************************************************************
 *   Copyright (C) 2006-2015 by Artur Kozioł                               *
 *   artkoz78@gmail.com                                                    *
 *                                                                         *
 *   This file is part of EdytorNC.                                        *
 *                                                                         *
 *   EdytorNC is free software; you can redistribute it and/or modify      *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/



#include "serialtransmission.h"
#include <QtSerialPort/QSerialPortInfo>


static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");


SPConfigDialog::SPConfigDialog(QWidget *parent, QString confName, Qt::WindowFlags f) : QDialog(parent, f)
{
   setupUi(this);
   setAttribute(Qt::WA_DeleteOnClose);
   setWindowTitle(tr("Serial port configuration"));
   setModal(true);

   configName = confName;

   baudGroup = new QButtonGroup(this);
   baudGroup->setExclusive(true);
   baudGroup->addButton(b3CheckBox, QSerialPort::Baud1200);
   baudGroup->addButton(b4CheckBox, QSerialPort::Baud2400);
   baudGroup->addButton(b5CheckBox, QSerialPort::Baud4800);
   baudGroup->addButton(b6CheckBox, QSerialPort::Baud9600);
   baudGroup->addButton(b7CheckBox, QSerialPort::Baud19200);
   baudGroup->addButton(b8CheckBox, QSerialPort::Baud38400);
   baudGroup->addButton(b9CheckBox, QSerialPort::Baud57600);
   baudGroup->addButton(b11CheckBox, QSerialPort::Baud115200);



   dataBitsGroup = new QButtonGroup(this);
   dataBitsGroup->setExclusive(true);
   dataBitsGroup->addButton(d1CheckBox, QSerialPort::Data5);
   dataBitsGroup->addButton(d2CheckBox, QSerialPort::Data6);
   dataBitsGroup->addButton(d3CheckBox, QSerialPort::Data7);
   dataBitsGroup->addButton(d4CheckBox, QSerialPort::Data8);


   stopBitsGroup = new QButtonGroup(this);
   stopBitsGroup->setExclusive(true);
   stopBitsGroup->addButton(s1CheckBox, QSerialPort::OneStop);
   stopBitsGroup->addButton(s2CheckBox, QSerialPort::TwoStop);


   parityGroup = new QButtonGroup(this);
   parityGroup->setExclusive(true);
   parityGroup->addButton(p1CheckBox, QSerialPort::NoParity);
   parityGroup->addButton(p2CheckBox, QSerialPort::OddParity);
   parityGroup->addButton(p3CheckBox, QSerialPort::EvenParity);



   flowCtlGroup = new QButtonGroup(this);
   flowCtlGroup->setExclusive(true);
   flowCtlGroup->addButton(f1CheckBox, QSerialPort::NoFlowControl);
   flowCtlGroup->addButton(f2CheckBox, QSerialPort::HardwareControl);
   flowCtlGroup->addButton(f3CheckBox, QSerialPort::SoftwareControl);


#ifdef Q_OS_WIN32
   browseButton->setEnabled(false);
#else
   connect(browseButton, SIGNAL(clicked()), SLOT(browseButtonClicked()));
#endif


   connect(browse1PushButton, SIGNAL(clicked()), SLOT(browse1ButtonClicked()));
   connect(browse2PushButton, SIGNAL(clicked()), SLOT(browse2ButtonClicked()));
   connect(browse3PushButton, SIGNAL(clicked()), SLOT(browse3ButtonClicked()));

   connect(saveButton, SIGNAL(clicked()), SLOT(saveButtonClicked()));
   connect(saveCloseButton, SIGNAL(clicked()), SLOT(saveCloseButtonClicked()));
   connect(deleteButton, SIGNAL(clicked()), SLOT(deleteButtonClicked()));
   connect(closeButton, SIGNAL(clicked()), SLOT(closeButtonClicked()));

   connect(flowCtlGroup, SIGNAL(buttonReleased(int)), SLOT(flowCtlGroupReleased()));

   loadSettings();
   flowCtlGroupReleased();

   connect(configNameBox, SIGNAL(currentIndexChanged(int)), SLOT(changeSettings()));
   connect(portNameComboBox, SIGNAL(currentIndexChanged(QString)), SLOT(portNameComboBoxIndexChanged(QString)));

   setResult(QDialog::Rejected);
}

//**************************************************************************************************
//
//**************************************************************************************************

SPConfigDialog::~SPConfigDialog()
{

}

//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::flowCtlGroupReleased()
{
   if(f2CheckBox->isChecked())
   {
      startDelaySpinBox->setEnabled(false);
      startDelaySpinBox->setValue(0);
   }
   else
   {
      startDelaySpinBox->setEnabled(true);
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::browseButtonClicked()
{
   QString fileName = QFileDialog::getOpenFileName(
                         this,
                         tr("Select serial port device"),
                         portNameComboBox->currentText(),
                         tr("All files (*)"));

   QFileInfo file(fileName);

   if((file.exists()) && (file.isReadable()))
   {
      portNameComboBox->addItem(fileName);
      portNameComboBox->setCurrentIndex(portNameComboBox->count() - 1);
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::saveButtonClicked()
{
    QStringList list;
    QString item, curItem;

    QSettings settings("EdytorNC", "EdytorNC");

    settings.beginGroup("SerialPortConfigs");

    curItem = configNameBox->currentText();

#ifndef Q_OS_WIN32
    list.clear();
    list.prepend(portNameComboBox->currentText());
    for(int i = 0; i <= portNameComboBox->count(); i++)
    {
       item = portNameComboBox->itemText(i);
       if(!item.isEmpty())
         if(!list.contains(item))
           list.prepend(item);
    };

    while(list.size() > 64)
    {
       list.removeLast();
    };
    settings.setValue("PortNameList", list);
#endif


    list.clear();
    list.prepend(configNameBox->currentText());
    for(int i = 0; i <= configNameBox->count(); i++)
    {
       item = configNameBox->itemText(i);
       if(!item.isEmpty())
         if(!list.contains(item))
           list.prepend(item);
    };

    while(list.size() > 64)
    {
       list.removeLast();
    };
    list.sort();
    settings.setValue("SettingsList", list);
    settings.setValue("CurrentSerialPortSettings", configNameBox->currentText());


    settings.beginGroup(configNameBox->currentText());

    settings.setValue("PortName", portNameComboBox->currentText());
    settings.setValue("BaudRate", baudGroup->checkedId());
    settings.setValue("DataBits", dataBitsGroup->checkedId());
    settings.setValue("StopBits", stopBitsGroup->checkedId());
    settings.setValue("Parity", parityGroup->checkedId());
    settings.setValue("FlowControl", flowCtlGroup->checkedId());
    settings.setValue("SendAtEnd", eotInput->text());
    settings.setValue("SendAtBegining", stInput->text());
    settings.setValue("LineDelay", delayDoubleSpinBox->value());
    settings.setValue("Xon", xonInput->text());
    settings.setValue("Xoff", xoffInput->text());
    settings.setValue("DeleteControlChars", deleteControlChars->isChecked());
    settings.setValue("RemoveEmptyLines", removeEmptyLines->isChecked());
    settings.setValue("RemoveBefore",removeBefore->isChecked());
    //settings.setValue("StartAfterXONCTS", startAfterXONCTS->isChecked());
    settings.setValue("SendingStartDelay", startDelaySpinBox->value());
    settings.setValue("DoNotShowProgressInEditor", doNotShowProgressInEditor->isChecked());
    settings.setValue("RecieveTimeout", recieveTimeoutSpinBox->value());
    settings.setValue("EndOfBlockLF", endOfBlockLF->isChecked());
    settings.setValue("RemoveSpaceEOB", removeSpaceEOB->isChecked());



    settings.endGroup();
    settings.endGroup();


    configNameBox->clear();
    configNameBox->addItems(list);
    int id = configNameBox->findText(curItem);
    if(id >= 0)
       configNameBox->setCurrentIndex(id);

}


//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::changeSettings()
{
    int id;
    QString item, port;

    QSettings settings("EdytorNC", "EdytorNC");

    settings.beginGroup("SerialPortConfigs");

    settings.beginGroup(configNameBox->currentText());

#ifdef Q_OS_WIN32
    port = "COM1";
#else
    port = "/dev/ttyUSB0";
#endif

    item = settings.value("PortName", port).toString();
    id = portNameComboBox->findText(item);
    if(id == -1)
    {
        portNameComboBox->addItems(QStringList(item));
        id = portNameComboBox->findText(item);
    };
    portNameComboBox->setCurrentIndex(id);
    portNameComboBoxIndexChanged(item);

    id = settings.value("BaudRate", QSerialPort::Baud9600).toInt();
    switch(id)
    {
           case QSerialPort::Baud1200   : b3CheckBox->setChecked(true);
                break;
           case QSerialPort::Baud2400   : b4CheckBox->setChecked(true);
                break;
           case QSerialPort::Baud4800   : b5CheckBox->setChecked(true);
                break;
           case QSerialPort::Baud9600   : b6CheckBox->setChecked(true);
                break;
           case QSerialPort::Baud19200  : b7CheckBox->setChecked(true);
                break;
           case QSerialPort::Baud38400  : b8CheckBox->setChecked(true);
                break;
           case QSerialPort::Baud57600  : b9CheckBox->setChecked(true);
                break;
           case QSerialPort::Baud115200 : b11CheckBox->setChecked(true);
                break;
    };

    id = settings.value("DataBits", QSerialPort::Data8).toInt();
    switch(id)
    {
           case QSerialPort::Data5    : d1CheckBox->setChecked(true);
                break;
           case QSerialPort::Data6    : d2CheckBox->setChecked(true);
                break;
           case QSerialPort::Data7    : d3CheckBox->setChecked(true);
                break;
           case QSerialPort::Data8    : d4CheckBox->setChecked(true);
                break;
    };

    id = settings.value("StopBits", QSerialPort::TwoStop).toInt();
    switch(id)
    {
           case QSerialPort::OneStop    : s1CheckBox->setChecked(true);
                break;
           case QSerialPort::TwoStop    : s2CheckBox->setChecked(true);
                break;
    };

    id = settings.value("Parity", QSerialPort::NoParity).toInt();
    switch(id)
    {
           case QSerialPort::NoParity   : p1CheckBox->setChecked(true);
                break;
           case QSerialPort::OddParity  : p2CheckBox->setChecked(true);
                break;
           case QSerialPort::EvenParity : p3CheckBox->setChecked(true);
                break;
    };

    id = settings.value("FlowControl", QSerialPort::HardwareControl).toInt();
    switch(id)
    {
           case QSerialPort::NoFlowControl      : f1CheckBox->setChecked(true);
                break;
           case QSerialPort::HardwareControl    : f2CheckBox->setChecked(true);
                break;
           case QSerialPort::SoftwareControl    : f3CheckBox->setChecked(true);
                break;
    };

    eotInput->setText(settings.value("SendAtEnd", "").toString());
    stInput->setText(settings.value("SendAtBegining", "").toString());
    xonInput->setText(settings.value("Xon", "17").toString());
    xoffInput->setText(settings.value("Xoff", "19").toString());
    delayDoubleSpinBox->setValue(settings.value("LineDelay", 0).toDouble());
    deleteControlChars->setChecked(settings.value("DeleteControlChars", true).toBool());
    removeEmptyLines->setChecked(settings.value("RemoveEmptyLines", true).toBool());
    removeBefore->setChecked(settings.value("RemoveBefore", false).toBool());
    //startAfterXONCTS->setChecked(settings.value("StartAfterXONCTS", true).toBool());
    startDelaySpinBox->setValue(settings.value("SendingStartDelay", 0).toInt());
    doNotShowProgressInEditor->setChecked(settings.value("DoNotShowProgressInEditor", false).toBool());
    recieveTimeoutSpinBox->setValue(settings.value("RecieveTimeout", 0).toInt());
    endOfBlockLF->setChecked(settings.value("EndOfBlockLF", false).toBool());
    removeSpaceEOB->setChecked(settings.value("RemoveSpaceEOB", false).toBool());

    settings.endGroup();
    settings.endGroup();
}

//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::loadSettings()
{
    int id;
    QStringList list;

    QSettings settings("EdytorNC", "EdytorNC");

    settings.beginGroup("SerialPortConfigs");

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        list.append(info.portName());
    };

    list.removeDuplicates();
    list.sort();
    portNameComboBox->clear();
    portNameComboBox->addItems(list);


    configNameBox->clear();
    list = settings.value("SettingsList", tr("Default")).toStringList();
    list.sort();
    configNameBox->addItems(list);
    //item = settings.value("CurrentSerialPortSettings", tr("Default")).toString();
    id = configNameBox->findText(configName);
    configNameBox->setCurrentIndex(id);
    settings.endGroup();

    changeSettings();
}

//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::portNameComboBoxIndexChanged(QString name)
{
    QString description;
    QString manufacturer;
    QString serialNumber;

    QSerialPortInfo *info = new QSerialPortInfo(name);

    description = info->description();
    manufacturer = info->manufacturer();
    serialNumber = info->serialNumber();

    portInfoListWidget->clear();
    //portInfoListWidget->addItem(tr("Port name:          %1").arg(info->portName()));
    portInfoListWidget->addItem(tr("Description       : %1").arg((!description.isEmpty() ? description : blankString)));
    portInfoListWidget->addItem(tr("Manufacturer      : %1").arg((!manufacturer.isEmpty() ? manufacturer : blankString)));
    portInfoListWidget->addItem(tr("SerialNumber      : %1").arg((!serialNumber.isEmpty() ? serialNumber : blankString)));
    portInfoListWidget->addItem(tr("System location   : %1").arg(info->systemLocation()));
    portInfoListWidget->addItem(tr("Vendor identifier : %1").arg((info->vendorIdentifier() ? QString::number(info->vendorIdentifier(), 16) : blankString)));
    portInfoListWidget->addItem(tr("Product identifier: %1").arg((info->productIdentifier() ? QString::number(info->productIdentifier(), 16) : blankString)));

}


//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::deleteButtonClicked()
{

    QSettings settings("EdytorNC", "EdytorNC");

    settings.beginGroup("SerialPortConfigs");
    settings.remove(configNameBox->currentText());
    settings.endGroup();

    int id = configNameBox->findText(configNameBox->currentText());
    configNameBox->removeItem(id);

}

//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::closeButtonClicked()
{
   QSettings settings("EdytorNC", "EdytorNC");
   settings.beginGroup("SerialPortConfigs");
   settings.setValue("CurrentSerialPortSettings", configNameBox->currentText());
   settings.endGroup();

   close();
}

//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::saveCloseButtonClicked()
{
   saveButtonClicked();
   closeButtonClicked();
   accept();
}

//**************************************************************************************************
//
//**************************************************************************************************

QString SPConfigDialog::browseForDir(const QString dir)
{
   QString dirName = QFileDialog::getExistingDirectory(
                         this,
                         tr("Select serial port device"),
                         dir,
                         QFileDialog::ShowDirsOnly);

   QFileInfo file(dirName);

   if((file.exists()) && (file.isReadable()))
   {
      return file.canonicalPath();
   };

   return "";
}

//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::browse1ButtonClicked()
{
   path1LineEdit->setText(browseForDir(path1LineEdit->text()));
}

//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::browse2ButtonClicked()
{
   path2LineEdit->setText(browseForDir(path2LineEdit->text()));
}

//**************************************************************************************************
//
//**************************************************************************************************

void SPConfigDialog::browse3ButtonClicked()
{
   path3LineEdit->setText(browseForDir(path3LineEdit->text()));
}





/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
=========================================================================================

=========================================================================================
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

TransmissionDialog::TransmissionDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
   setupUi(this);
   setAttribute(Qt::WA_DeleteOnClose);
   setWindowTitle(tr("Serial transmission test"));

   comPort = NULL;

   connect(closeButton, SIGNAL(clicked()), SLOT(closeButtonClicked()));

   connect(clearButton, SIGNAL(clicked()), SLOT(clearButtonClicked()));
   connect(configButton, SIGNAL(clicked()), SLOT(configButtonClicked()));

   connect(connectButton, SIGNAL(toggled(bool)), SLOT(connectButtonToggled(bool)));

   connect(setRtsButton, SIGNAL(clicked()), SLOT(setRtsButtonClicked()));
   connect(setDtrButton, SIGNAL(clicked()), SLOT(setDtrButtonClicked()));

   loadSerialConfignames();
   changeSettings();
   connect(configBox, SIGNAL(currentIndexChanged(int)), SLOT(changeSettings()));

   timer = new QTimer(this);
   connect(timer, SIGNAL(timeout()), this, SLOT(updateLeds()));

   connect(sendLineEdit, SIGNAL(returnPressed()), SLOT(sendTextEditChanged()));

   connect(setXonButton, SIGNAL(clicked()), SLOT(setXonButtonClicked()));
   connect(setXoffButton, SIGNAL(clicked()), SLOT(setXoffButtonClicked()));


   connect(textEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(textEditScroll(int)));
   connect(hexTextEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(hexTextEditScroll(int)));


}

//**************************************************************************************************
//
//**************************************************************************************************

TransmissionDialog::~TransmissionDialog()
{

}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::textEditScroll(int pos)
{
   Q_UNUSED(pos);
   hexTextEdit->verticalScrollBar()->setSliderPosition(textEdit->verticalScrollBar()->sliderPosition());
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::hexTextEditScroll(int pos)
{
   Q_UNUSED(pos);
   textEdit->verticalScrollBar()->setSliderPosition(hexTextEdit->verticalScrollBar()->sliderPosition());
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::sendTextEditChanged()
{
   QString tx, ty;
   int i;

   tx = sendLineEdit->text();
   sendLineEdit->clear();

   tx.append("\r\n");

   sendText(tx);

   QTextCursor cr = textEdit->textCursor();
   QTextCharFormat format = cr.charFormat();
   format.setForeground(Qt::black);
   cr.setCharFormat(format);
   cr.insertText(tx);
   textEdit->setTextCursor(cr);

   cr = hexTextEdit->textCursor();
   format = cr.charFormat();
   format.setForeground(Qt::black);
   cr.setCharFormat(format);
   for(i = 0; i < tx.size(); i++)
   {
      ty = QString("%1 ").arg((int)tx.at(i).toLatin1(), 2, 16, (QChar)'0');

      cr.insertText(ty);
   };
   hexTextEdit->setTextCursor(cr);
   hexTextEdit->insertPlainText("\n");

   textEdit->ensureCursorVisible();
   hexTextEdit->ensureCursorVisible();
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::closeButtonClicked()
{
   stop = true;
   qApp->processEvents();
   if(comPort != NULL)
   {
      //comPort->close();
      //delete(comPort);
   };

   close();
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::clearButtonClicked()
{
   textEdit->clear();
   hexTextEdit->clear();
   sendLineEdit->clear();
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::connectButtonToggled(bool tg)
{
   if(tg)
   {

      connectButton->setIcon(QIcon(":/images/connect_established.png"));
      connectButton->setText(tr("&Disconnect"));

      textEdit->clear();
      hexTextEdit->clear();
      sendLineEdit->clear();

      count = 0;

      if(comPort != NULL)
      {
         comPort->reset();
         comPort->close();
         delete comPort;
      };

      comPort = new QSerialPort(portName);
      comPort->clearError();
      comPort->setBaudRate(portSettings.BaudRate);
      comPort->setDataBits(portSettings.DataBits);
      comPort->setParity(portSettings.Parity);
      comPort->setStopBits(portSettings.StopBits);
      comPort->setFlowControl(portSettings.FlowControl);
      if (!comPort->open(QIODevice::ReadWrite))
      {
          showError(comPort->error());
          delete comPort;
          comPort = NULL;
          connectButton->setChecked(false);
          return;
      };

      comPort->clear(QSerialPort::AllDirections);

//      if(portSettings.FlowControl == FLOW_XONXOFF)
//      {
//         comPort->putChar(portSettings.Xon);
//      };

      setDtrButton->setEnabled(true);
      setDtrButton->setChecked(false);
      setRtsButton->setEnabled(true);
      setRtsButton->setChecked(false);

      configBox->setEnabled(false);
      configButton->setEnabled(false);


      bool en = portSettings.FlowControl == QSerialPort::SoftwareControl;
      setXonButton->setEnabled(en);
      setXoffButton->setEnabled(en);

      sendLineEdit->setReadOnly(false);
      sendLineEdit->setFocus(Qt::MouseFocusReason);

      showError(comPort->error());

      stop = false;
      timer->start(20);

   }
   else
   {
      timer->stop();
      //qApp->processEvents();
      connectButton->setIcon(QIcon(":/images/connect_no.png"));
      connectButton->setText(tr("&Connect"));

      stop = true;
      qApp->processEvents();
      setDtrButton->setEnabled(false);
      setRtsButton->setEnabled(false);
      setXonButton->setEnabled(false);
      setXoffButton->setEnabled(false);

      ctsLabel->setEnabled(false);
      dsrLabel->setEnabled(false);
      dcdLabel->setEnabled(false);
      rtsLabel->setEnabled(false);
      dtrLabel->setEnabled(false);

      connectButton->setChecked(false);

      configBox->setEnabled(true);
      configButton->setEnabled(true);

      sendLineEdit->setReadOnly(true);
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::setXonButtonClicked()
{
   if(comPort->isOpen())
     comPort->putChar(portSettings.Xon);
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::setXoffButtonClicked()
{
   if(comPort->isOpen())
     comPort->putChar(portSettings.Xoff);
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::setDtrButtonClicked()
{
   if(comPort->isOpen())
     comPort->setDataTerminalReady(setDtrButton->isChecked());
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::setRtsButtonClicked()
{
   if(comPort->isOpen())
     comPort->setRequestToSend(setRtsButton->isChecked());
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::updateLeds()
{
   QSerialPort::PinoutSignals status;
   bool ok;
   QString tx;
   char ch;
   int i;
   QTextCursor cr;
   QTextCharFormat format;


   timer->stop();
   if(comPort == NULL)
      return;

   if(!comPort->isOpen())
      return;

   while(!stop)
   {

      status = comPort->pinoutSignals();

      ctsLabel->setEnabled(status & QSerialPort::ClearToSendSignal);
      dsrLabel->setEnabled(status & QSerialPort::DataSetReadySignal);
      dcdLabel->setEnabled(status & QSerialPort::DataCarrierDetectSignal);
      rtsLabel->setEnabled(status & QSerialPort::RequestToSendSignal);
      setRtsButton->setChecked(status & QSerialPort::RequestToSendSignal);
      dtrLabel->setEnabled(status & QSerialPort::DataTerminalReadySignal);
      setDtrButton->setChecked(status & QSerialPort::DataTerminalReadySignal);


      i = comPort->bytesAvailable();
      qApp->processEvents();
      if(i > 0)
      {
         ok = comPort->getChar(&ch);
         if(!ok)
         {
            showError(comPort->error());
            return;
         };
         count++;
         errorLabel->setText(tr("Recived: %1 bytes.").arg(count));
         tx = ch;

         qApp->processEvents();

         if(ch != '\r')
         {
            cr = textEdit->textCursor();
            format = cr.charFormat();
            format.setForeground(Qt::blue);
            cr.setCharFormat(format);
            cr.insertText(tx);
            textEdit->setTextCursor(cr);
         };

         tx = QString("%1 ").arg((int)ch, 2, 16, (QChar)'0');
         cr = hexTextEdit->textCursor();
         format = cr.charFormat();
         format.setForeground(Qt::blue);
         cr.setCharFormat(format);
         cr.insertText(tx);
         hexTextEdit->setTextCursor(cr);

         if(ch == '\n')
            hexTextEdit->insertPlainText("\n");

         textEdit->ensureCursorVisible();
         hexTextEdit->ensureCursorVisible();
      }
      else
         if(i < 0)
            showError(comPort->error());

   };
   comPort->close();
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::configButtonClicked()
{
   SPConfigDialog *serialConfigDialog = new SPConfigDialog(this, configBox->currentText());

   if(serialConfigDialog->exec() == QDialog::Accepted)
      changeSettings();
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::changeSettings()
{
    QString port;
    bool ok;

    QSettings settings("EdytorNC", "EdytorNC");

    settings.beginGroup("SerialPortConfigs");

#ifdef Q_OS_WIN32
       port = "COM1";
#else
       port = "/dev/ttyS0";

#endif

    settings.beginGroup(configBox->currentText());

    portName = settings.value("PortName", port).toString();

    portSettings.BaudRate = (QSerialPort::BaudRate) settings.value("BaudRate", QSerialPort::Baud9600).toInt();
    portSettings.DataBits = (QSerialPort::DataBits) settings.value("DataBits", QSerialPort::Data8).toInt();
    portSettings.StopBits = (QSerialPort::StopBits) settings.value("StopBits", QSerialPort::TwoStop).toInt();
    portSettings.Parity = (QSerialPort::Parity) settings.value("Parity", QSerialPort::NoParity).toInt();
    portSettings.FlowControl = (QSerialPort::FlowControl) settings.value("FlowControl", QSerialPort::HardwareControl).toInt();
    portSettings.Xon = settings.value("Xon", "17").toString().toInt(&ok, 10);
    portSettings.Xoff = settings.value("Xoff", "19").toString().toInt(&ok, 10);
    sendAtEnd = settings.value("SendAtEnd", "").toString();
    sendAtBegining = settings.value("SendAtBegining", "").toString();
    lineDelay = settings.value("LineDelay", 0).toDouble();


    settings.endGroup();
    settings.endGroup();
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::loadSerialConfignames()
{
   int id;
    QStringList list;
    QString item;

    QSettings settings("EdytorNC", "EdytorNC");

    settings.beginGroup("SerialPortConfigs");

    configBox->clear();
    list = settings.value("SettingsList", QStringList(tr("Default"))).toStringList();
    list.sort();
    configBox->addItems(list);
    item = settings.value("CurrentSerialPortSettings", tr("Default")).toString();
    id = configBox->findText(item);
    configBox->setCurrentIndex(id);
    settings.endGroup();
}


//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::showError(int error)
{
   switch(error)
   {
   case QSerialPort::NoError                   : errorLabel->setText("No Error has occured");
       break;
   case QSerialPort::DeviceNotFoundError       : errorLabel->setText("Attempting to open an non-existing device");
       break;
   case QSerialPort::PermissionError           : errorLabel->setText("Attempting to open an already opened device by another process or user not having enough permission and credentials to open");
       break;
   case QSerialPort::OpenError                 : errorLabel->setText("Attempting to open an already opened device in this object");
       break;
   case QSerialPort::ParityError               : errorLabel->setText("Parity error detected by the hardware while reading data");
       break;
   case QSerialPort::FramingError              : errorLabel->setText("Framing error detected by the hardware while reading data");
       break;
   case QSerialPort::BreakConditionError       : errorLabel->setText("Break condition detected by the hardware on the input line");
       break;
   case QSerialPort::WriteError                : errorLabel->setText("An I/O error occurred while writing the data");
       break;
   case QSerialPort::ReadError                 : errorLabel->setText("An I/O error occurred while reading the data");
       break;
   case QSerialPort::ResourceError             : errorLabel->setText("An I/O error occurred when a resource becomes unavailable, e.g. when the device is unexpectedly removed from the system");
       break;
   case QSerialPort::UnsupportedOperationError : errorLabel->setText("Receive buffer overflow");
       break;
   case QSerialPort::UnknownError              : errorLabel->setText("An unidentified error occurred");
       break;
   case QSerialPort::TimeoutError              : errorLabel->setText("Transmit buffer overflow");
       break;
   case QSerialPort::NotOpenError              : errorLabel->setText("An operation is executed that can only be successfully performed if the device is open");
       break;
   default                                     : errorLabel->setText("Unknown error");
   }


}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::sendText(QString tx)
{
   int i;
   bool xoffReceived;
   char controlChar;
   int bytesToWrite;

   if(comPort == NULL)
   return;

   if(comPort->isOpen())
   {
      tx.prepend(sendAtBegining);
      tx.append(sendAtEnd);
      if(!tx.contains("\r\n"))
         tx.replace("\n", "\r\n");

      errorLabel->setText(tr("Waiting..."));
      qApp->processEvents();

      i = 0;
      xoffReceived = true;
      while(i < tx.size())
      {
         if(xoffReceived)
            errorLabel->setText(tr("Waiting for a signal readiness..."));
         qApp->processEvents();

         if(stop)
            break;

         if(portSettings.FlowControl == QSerialPort::SoftwareControl)
         {
            controlChar = 0;
            if(comPort->bytesAvailable() > 0)
            {
               comPort->getChar(&controlChar);
            };

            if(controlChar == portSettings.Xoff)
               xoffReceived = true;
            if(controlChar == portSettings.Xon)
               xoffReceived = false;
            //setXoffButton->setChecked(xoffReceived);
            //setXonButton->setChecked(!xoffReceived);
         }
         else
            xoffReceived = false;

         bytesToWrite = comPort->bytesToWrite();

         if((bytesToWrite == 0) && (!xoffReceived))
         {
            if(!comPort->putChar(tx[i].toLatin1()))
               showError(comPort->error());

            errorLabel->setText(tr("Sending byte %1 of %2").arg(i + 1).arg(tx.size()));
            qApp->processEvents();

            if(lineDelay > 0)
            {
               if(tx[i].toLatin1() == '\n')
               {
                  readyCont = false;
                  QTimer::singleShot(int(lineDelay * 1000), this, SLOT(lineDelaySlot()));
                  while(!readyCont)
                  {
                     qApp->processEvents();
                  };
               };
            };

            i++;
         };
      };
   };

}

//**************************************************************************************************
//
//**************************************************************************************************

void TransmissionDialog::lineDelaySlot()
{
   readyCont = true;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
=========================================================================================

=========================================================================================
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

TransProgressDialog::TransProgressDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
   setupUi(this);
   //setWindowTitle(tr("Serial transmission"));

   comPort = NULL;

   canceled = true;

   connect(cancelButton, SIGNAL(clicked()), SLOT(cancelButtonClicked()));
   connect(this, SIGNAL(rejected()), SLOT(cancelButtonClicked()));

   timer = new QTimer(this);
   connect(timer, SIGNAL(timeout()), this, SLOT(updateLeds()));
}

//**************************************************************************************************
//
//**************************************************************************************************

TransProgressDialog::~TransProgressDialog()
{
   timer->stop();
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransProgressDialog::closeEvent(QCloseEvent *event)
{
   timer->stop();
   canceled = true;
   event->accept();
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransProgressDialog::cancelButtonClicked()
{
   timer->stop();
   canceled = true;
   close();
}

//**************************************************************************************************
//
//**************************************************************************************************

bool TransProgressDialog::wasCanceled()
{
   return canceled;
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransProgressDialog::setLabelText(const QString text)
{
   label->setText(text);
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransProgressDialog::setValue(int val)
{
   progressBar->setValue(val);
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransProgressDialog::setRange(int min, int max)
{
   if(max == 0)
   {
      progressBar->hide();
      cancelButton->setText(tr("&Close"));
      cancelButton->setIcon(QIcon(":/images/window-close.png"));
   }
   else
      progressBar->setRange(min, max);

   progressBar->setValue(0);
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransProgressDialog::open(QSerialPort *port)
{
   if(port != NULL)
   {
      canceled = false;
      show();
      comPort = port;
      timer->start(20);
   };
}

//**************************************************************************************************
//
//**************************************************************************************************

void TransProgressDialog::updateLeds()
{
   timer->stop();
   if(comPort == NULL)
      return;

   if(!comPort->isOpen())
      return;

   QSerialPort::PinoutSignals status = comPort->pinoutSignals();
   ctsLabel->setEnabled(status & QSerialPort::ClearToSendSignal);
   dsrLabel->setEnabled(status & QSerialPort::DataSetReadySignal);
   dcdLabel->setEnabled(status & QSerialPort::DataCarrierDetectSignal);
   rtsLabel->setEnabled(status & QSerialPort::RequestToSendSignal);
   dtrLabel->setEnabled(status & QSerialPort::DataTerminalReadySignal);

   timer->start();
}

//**************************************************************************************************
//
//**************************************************************************************************

//void TransmissionDialog::reciveButtonClicked()
//{
//   QString tx;
//   int count, i;
//   bool ok;
//   char ch;
//
//
//   loadConfig();
//   comPort = new QextSerialPort(portName, portSettings);
//   if(comPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered | QIODevice::Truncate))
//     stop = false;
//   else
//   {
//      stop = true;
//      showError(E_INVALID_FD);
//      delete(comPort);
//      return;
//   };
//   comPort->flush();
//   comPort->reset();
//
//   i = configBox->currentIndex();
//   newFile();
//   activeWindow = activeMdiChild();
//   if(!(activeWindow != 0))
//     return;
//   configBox->setCurrentIndex(i);
//
//   receiveAct->setEnabled(false);
//   sendAct->setEnabled(false);
//   QApplication::setOverrideCursor(Qt::BusyCursor);
//
//   QProgressDialog progressDialog(this);
//   progressDialog.setRange(0, 32768);
//   progressDialog.setModal(true);
//   progressDialog.setLabelText(tr("Waiting for data..."));
//   progressDialog.open();
//   qApp->processEvents();
//
//   if(portSettings.FlowControl == FLOW_XONXOFF)
//   {
//      comPort->putChar(portSettings.Xon);
//   };
//
//   tx.clear();
//   while(1)
//   {
//      //progressDialog.setValue(count);
//
//#ifdef Q_OS_UNIX
//      usleep(2000);
//#endif
//
//      i = comPort->bytesAvailable();
//      if(i > 0)
//      {
//         ok = comPort->getChar(&ch);
//         if(!ok)
//         {
//            stop = true;
//            if(portSettings.FlowControl == FLOW_XONXOFF)
//            {
//               comPort->putChar(portSettings.Xoff);
//            };
//            showError(comPort->lastError());
//            break;
//         };
//         count++;
//         errorLabel->setText(tr("Recived: %1 bytes.").arg(count));
//         textEdit->insertPlainText(ch);
//         tx = QString("%1 ").arg(ch, 0, 16);
//         hexTextEdit->insertPlainText(tx.toUpper());
//
//         textEdit->ensureCursorVisible();
//         hexTextEdit->ensureCursorVisible();
//         qApp->processEvents();
//
//      };
//   };
//
//   comPort->close();
//   delete(comPort);
//   stopButton->setEnabled(false);
//   reciveButton->setEnabled(true);
//   sendButton->setEnabled(true);
//   QApplication::restoreOverrideCursor();
//
//
//   //////////////////////////////////////////////////////////////////////
//
//
////   showError(E_NO_ERROR);
////   count = 0;
////   if(comPort->open(QIODevice::ReadOnly))
////     stop = false;
////   else
////   {
////      stop = true;
////      showError(E_INVALID_FD);
////      return;
////   };
////   reciveButton->setEnabled(false);
////   sendButton->setEnabled(false);
////   stopButton->setEnabled(true);
////   QApplication::setOverrideCursor(Qt::BusyCursor);
////
////   while(!stop)
////   {
////      qApp->processEvents();
////
////      if(comPort->bytesAvailable() > 0)
////      {
////         ok = comPort->getChar(&ch);
////         if(!ok)
////         {
////            stop = true;
////            showError(comPort->lastError());
////            break;
////         };
////         count++;
////         errorLabel->setText(tr("Recived: %1 bytes.").arg(count));
////         textEdit->insertPlainText(ch);
////         tx = QString("%1 ").arg(ch, 0, 16);
////         textEdit->insertPlainText(tx.toUpper());
////      };
////
////   };
////
////   comPort->close();
//
////   QApplication::restoreOverrideCursor();
//
//}

//**************************************************************************************************
//
//**************************************************************************************************


//**************************************************************************************************
//
//**************************************************************************************************



//**************************************************************************************************
//
//**************************************************************************************************


//**************************************************************************************************
//
//**************************************************************************************************

