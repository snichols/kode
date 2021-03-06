/*
    This file is part of KXForms.

    Copyright (c) 2007 Andre Duffeck <aduffeck@suse.de>

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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include "../guielement.h"

#include <QWidget>

class QPushButton;
class QEventLoop;

namespace KXForms {

class Editor;
class FormGui;

class EditorWidget : public QWidget
{
  Q_OBJECT
  public:
    EditorWidget( Editor *e, FormGui *parent = 0 );

    void setGuiElements( const GuiElement::List &list );

    GuiElement *hoveredElement() { return mHoveredElement; }

    GuiElement *selectElement( Editor::SelectionMode sm );

    void setInEdit( bool b );

  public Q_SLOTS:
    void showActionMenu();
    void showHints();
    void editDefaults();

  protected:
    void init();

    void mouseMoveEvent( QMouseEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void paintEvent( QPaintEvent *event );

    void drawInterface( QPainter *p, const QRect &, GuiElement *e );
    void highlightElement( QPainter *p, const QRect &, GuiElement *e );
    void targetElement( QPainter *p, const QRect &, GuiElement *e );
    void printMessage( QPainter *p, const QRect &, const QString &msg );
    void drawGlobalInterface( QPainter *p );
    void drawGroups( QPainter *p );
    void drawWidgetFrames( QPainter *p );

  private:
    Editor *mEditor;
    FormGui *mGui;
    QMap< GuiElement *, QRect > mElementMap;
    QMap< QString, QRect > mGroupMap;

    QWidget *mInterfaceWidget;
    QPushButton *mEditButton;
    QPushButton *mShowHintsButton;
    QPushButton *mEditDefaultsButton;
    QPushButton *mSaveHintsButton;
    QPushButton *mSaveHintsAsButton;
    QPushButton *mExitButton;

    GuiElement *mHoveredElement;
    GuiElement *mActiveElement;

    QEventLoop *mEventLoop;

    bool mInSelection;
    Editor::SelectionMode mSelectionMode;
    bool mInEdit;
    bool mInDrag;
    QPoint mDragPoint;
    GuiElement *mDraggedElement;
};

}
#endif
