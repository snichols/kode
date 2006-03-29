/*
    This file is part of KXForms.

    Copyright (c) 2005,2006 Cornelius Schumacher <schumacher@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/
#ifndef GUIHANDLERFLAT_H
#define GUIHANDLERFLAT_H

#include "guihandler.h"
#include "formgui.h"

#include <QObject>
#include <QStack>

class QWidget;
class QBoxLayout;
class QStackedWidget;
class QPushButton;

namespace KXForms {

class Manager;
class Form;
class FormGui;

class BreadCrumbNavigator : public QFrame
{
    Q_OBJECT
  public:
    BreadCrumbNavigator( QWidget *parent );

    void push( FormGui * );
    FormGui *pop();

    FormGui *last() const;
    int count() const;

  protected:
    void updateLabel();

  private:
    QStack<FormGui *> mHistory;

    QLabel *mLabel; 
};

class GuiHandlerFlat : public QObject, public GuiHandler
{
    Q_OBJECT
  public:
    GuiHandlerFlat( Manager * );

    QWidget *createRootGui( QWidget *parent );
    void createGui( const Reference &ref, QWidget *parent );

  protected:
    FormGui *createGui( Form *form, QWidget *parent );

  protected slots:
    void goBack();

  private:
    QWidget *mMainWidget;
    
    BreadCrumbNavigator *mBreadCrumbNavigator;
    QStackedWidget *mStackWidget;
    QPushButton *mBackButton;
};

}

#endif