/***************************************************************************
 *   Copyright (C) 2006-2013 by Artur Kozioł                               *
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

#ifndef COMMONINC_H
#define COMMONINC_H

#include<QString>
#include<QRegExp>

#define TABBED_MODE              0x01
#define SHOW_FILENAME            0x02
#define SHOW_FILEPATH            0x04
#define SHOW_PROGTITLE           0x08


#define MODE_AUTO                0x00
#define MODE_OKUMA               0x02
#define MODE_FANUC               0x03
#define MODE_HEIDENHAIN          0x04
#define MODE_SINUMERIK           0x05
#define MODE_SINUMERIK_840       0x06
#define MODE_PHILIPS             0x07
#define MODE_HEIDENHAIN_ISO      0x08
#define MODE_TOOLTIPS            0x09
#define MODE_LINUXCNC            0x0A


struct _h_colors
{
   int commentColor;
   int gColor;
   int mColor;
   int nColor;
   int lColor;
   int fsColor;
   int dhtColor;
   int rColor;
   int macroColor;
   int keyWordColor;
   int progNameColor;
   int operatorColor;
   int zColor;
   int aColor;
   int bColor;
   int highlightMode;

};

//**************************************************************************************************
//
//**************************************************************************************************

struct _editor_properites
{
   bool isRedo, isUndo, isSel;
   bool readOnly;
   bool ins;
   bool modified;
   int cursorPos;
   QString lastDir, dotAdr, i2mAdr;
   QString fileName;
   bool atEnd, dotAfter;
   int dotAftrerCount;
   int i2mprec;
   bool inch;
   QString fontName;
   int fontSize;
   bool intCapsLock;
   bool syntaxH;
   _h_colors hColors;
   QByteArray geometry;
   bool maximized;
   bool underlineChanges;
   int lineColor;
   int underlineColor;
   int windowMode;
   QString calcBinary;
   bool clearUndoHistory;
   bool clearUnderlineHistory;
   bool editorToolTips;
   bool defaultReadOnly;
   QStringList extensions;
   QString saveExtension;
   QString saveDirectory;
   int defaultHighlightMode;
   bool startEmpty;
   QString curFile;  // replace by filename
   QString curFileInfo;
};



#endif
