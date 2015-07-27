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


#ifndef SERIALTRANSMISSIONDIALOG_H
#define SERIALTRANSMISSIONDIALOG_H


#include <QtWidgets>
#include <QtSerialPort/QSerialPort>

#include "serialtransmission.h"
#include "commoninc.h"

#include "ui_serialtransmissiondialog.h"

class SerialTransmissionDialog : public QDialog, private Ui::SerialTransmissionDialog
{
    Q_OBJECT

public:
    SerialTransmissionDialog(QWidget *parent = 0);
    ~SerialTransmissionDialog();
    QString receiveData(QString *readData, QString configName);
    void sendData(QString dataToSend, QString configName);

public slots:
    bool wasCanceled();



protected:
    void closeEvent(QCloseEvent *event);

signals :



private slots:
    void cancelButtonClicked();
    void updateLeds();
    void sendStartDelayTimeout();
    void stopButtonClicked();
    void autoClose();
    void showSerialPortError(QSerialPort::SerialPortError error);
    void serialPortBytesWritten(qint64 bytes);
    void serialPortReadyRead();
    void serialPortRequestToSend(bool set);
    void lineDelaySlot();


private:
    void setRange(int min, int max);
    void setValue(int val);
    void setLabelText(const QString text);
    void loadConfig(QString configName);
    QString guessFileName(QString *text, bool byProgNum);
    QString saveDataToFile(QString *text);
    void writeLog(QString msg, QString timeStamp);


    QSerialPort serialPort;
    bool canceled;
    PortSettings portSettings;
    QByteArray serialPortReadBuffer;
    QStringList serialPortWriteBuffer;
    QTimer *updateLedsTimer;
    QTimer *autoCloseTimer;
    qint64 bytesWritten;
    qint64 noOfBytes;
    QStringList::iterator it;
    bool prevXoffReceived;
    int autoCloseCountner;
    int autoCloseCountnerReloadValue;
    bool guessFileNameByProgName;
    bool createLogFile;
    bool renameIfExists;
    bool removeLetters;
    bool appendExt;

    bool stop;
    QString portName, sendAtEnd, sendAtBegining;
    int baudRate;
    int dataBits;
    int stopBits;
    int parity;
    int flowControl;
    double lineDelay;
    bool readyCont;
    bool xoffReceived;
    bool deleteControlChars;
    bool removeEmptyLines;
    bool removeBefore;
    bool endOfBlockLF;
    bool removeSpaceEOB;
    int sendStartDelay;
    int recieveTimeout;

    bool autoSave;
    QString savePath;
    QString saveExt;

    QFile logFile;


};

#endif // SERIALTRANSMISSIONDIALOG_H
