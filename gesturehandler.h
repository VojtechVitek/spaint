/***************************************************************************
 *   Copyright (C) 2010 Brno University of Technology,                     *
 *   Faculty of Information Technology                                     *
 *   Author(s): Marek Vantuch    <xvantu00 AT stud.fit.vutbr.cz>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef GESTUREHANDLER_H
#define GESTUREHANDLER_H

#include <QObject>
#include <QPainterPath>
#include <QPair>
#include <QList>
#include <QString>
#include <QIcon>
#include "canvas.h"
#include "canvasmgr.h"
#include "mousegesturerecognizer.h"

namespace Gesture
{

typedef enum Type {
   Pen      = 0,
   Brush    = 1,
   Eraser   = 2,
   Clear    = 3,
   FColor   = 4,
   BColor   = 5,
}gestureType;

typedef QPair<QString, QIcon> typeData;

class Handler : public MouseGestureRecognizer
{
   Q_OBJECT

   public:
   Handler(QObject* parent = 0);
   ~Handler();
   bool observe(CanvasMgr* cm);
   DirectionList getGesture(gestureType type);              //gets direction list of gesture
   void setGesture(gestureType type, DirectionList dl);     //changes directions of gesture
   void resetGesture(gestureType type);                      //set gestures directions to default

   //converting string <-> DirectionList
   DirectionList strToDl (QString str);
   QString dlToStr (DirectionList dl);
   typeData& getTypeData(gestureType type);                      //returns label and icon for type
   unsigned getTypeCount();                                 //returns count of defined types

   public slots:
   bool start();              //start/stop handling gestures
   bool stop();
   void handleGesture(QPainterPath gesture);   //procedure which takes care of incoming gesture
   void handleCanvas(Canvas* cnvs);            //connecting with newly created canvas
   void letCanvasGo(Canvas* cnvs);             //disconnecting from destroyed canvas

   protected:
   void initializeGestures(void);               //sets all gestures either from settings or from default values
   void uninitializeGestures(void);             //saves all changed gestures to settings

   protected slots:
   void debugGesture(int code);

   signals:
   void somethingChanged();


   private:
   class Private;
   Private* d;
};

}
#endif // GESTUREHANDLER_H
