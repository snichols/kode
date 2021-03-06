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

#include "editor.h"
#include "editorwidget.h"
#include "editoraction.h"
#include "changelabelaction.h"
#include "appearanceaction.h"
#include "listaction.h"
#include "positionaction.h"
#include "layoutstyleaction.h"
#include "groupaction.h"
#include "readonlyaction.h"
#include "inputtypeaction.h"

#include "../manager.h"
#include "../guielement.h"

#include <kactionmenu.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmenu.h>
#include <kapplication.h>
#include <kmessagebox.h>
#include <kfiledialog.h>

#include <QEvent>
#include <QEventLoop>

using namespace KXForms;

Editor::Editor( Manager *m)
: mEventLoop( new QEventLoop( this ) ), mEditMode( false ), mInEdit( false ),
  mEditorWidget( 0 ), mManager( m )
{
  setupActions();
}

Editor::~Editor()
{
  qDeleteAll(mActions);
}

void Editor::setupActions()
{
  EditorAction *a;

  a = new ChangeLabelAction( this );
  connect( a, SIGNAL(hintGenerated( const Hint & )), 
      SLOT(applyHint( const Hint & )) );
  mActions[ "edit_label" ] = a;

  a = new AppearanceAction( this );
  connect( a, SIGNAL(hintGenerated( const Hint & )), 
      SLOT(applyHint( const Hint & )) );
  mActions[ "edit_appearance" ] = a;

  a = new ListAction( this );
  connect( a, SIGNAL(hintGenerated( const Hint & )), 
      SLOT(applyHint( const Hint & )) );
  mActions[ "edit_list" ] = a;

  a = new PositionAction( this );
  connect( a, SIGNAL(hintGenerated( const Hint & )), 
      SLOT(applyHint( const Hint & )) );
  mActions[ "edit_position" ] = a;

  a = new LayoutStyleAction( this );
  connect( a, SIGNAL(hintGenerated( const Hint & )), 
      SLOT(applyHint( const Hint & )) );
  mActions[ "edit_layoutstyle" ] = a;

  a = new GroupAction( this );
  connect( a, SIGNAL(hintGenerated( const Hint & )), 
      SLOT(applyHint( const Hint & )) );
  mActions[ "edit_group" ] = a;

  a = new ReadOnlyAction( this );
  connect( a, SIGNAL(hintGenerated( const Hint & )), 
      SLOT(applyHint( const Hint & )) );
  mActions[ "edit_readonly" ] = a;

  a = new InputTypeAction( this );
  connect( a, SIGNAL(hintGenerated( const Hint & )), 
      SLOT(applyHint( const Hint & )) );
  mActions[ "edit_inputtype" ] = a;
}

KActionMenu *Editor::actionMenu( GuiElement *e )
{
  KActionMenu *menu = new KActionMenu( this );
  bool needSeparator = false;
  if( e->actionTypes() & Editor::CommonActions ) {
    needSeparator = true;
    KAction *titleAction = new KAction( i18n("Change Label"), menu );
    titleAction->setData( "edit_label" );
    QObject::connect( titleAction, SIGNAL(triggered(bool)), SLOT( actionTriggered() ) );
    menu->addAction( titleAction );


    KAction *layoutStyleAction = new KAction( i18n("Change Layout Style"), menu );
    layoutStyleAction->setData( "edit_layoutstyle" );
    QObject::connect( layoutStyleAction, SIGNAL(triggered(bool)), SLOT( actionTriggered() ) );
    menu->addAction( layoutStyleAction );

    KAction *readonlyAction = new KAction( i18n("Change ReadOnly Mode"), menu );
    readonlyAction->setData( "edit_readonly" );
    QObject::connect( readonlyAction, SIGNAL(triggered(bool)), SLOT( actionTriggered() ) );
    menu->addAction( readonlyAction );


    KAction *positionAction = new KAction( i18n("Change Position"), menu );
    positionAction->setData( "edit_position" );
    QObject::connect( positionAction, SIGNAL(triggered(bool)), SLOT( actionTriggered() ) );
    menu->addAction( positionAction );


    KAction *groupAction = new KAction( i18n("Change Group"), menu );
    groupAction->setData( "edit_group" );
    QObject::connect( groupAction, SIGNAL(triggered(bool)), SLOT( actionTriggered() ) );
    menu->addAction( groupAction );
  }

  if( e->actionTypes() & Editor::AppearanceActions ) {
    if( needSeparator )
      menu->addSeparator();
    needSeparator = true;

    KAction *appearanceAction = new KAction( i18n("Change Appearance"), menu );
    appearanceAction->setData( "edit_appearance" );
    QObject::connect( appearanceAction, SIGNAL(triggered(bool)), SLOT( actionTriggered() ) );
    menu->addAction( appearanceAction );
  }

  if( e->actionTypes() & Editor::ListActions ) {
    if( needSeparator )
      menu->addSeparator();
    needSeparator = true;

    KAction *listAction = new KAction( i18n("Change List Properties"), menu );
    listAction->setData( "edit_list" );
    QObject::connect( listAction, SIGNAL(triggered(bool)), SLOT( actionTriggered() ) );
    menu->addAction( listAction );
  }

  if( e->actionTypes() & Editor::InputActions ) {
    if( needSeparator )
      menu->addSeparator();
    needSeparator = true;

    KAction *typeAction = new KAction( i18n("Change Input Type"), menu );
    typeAction->setData( "edit_inputtype" );
    QObject::connect( typeAction, SIGNAL(triggered(bool)), SLOT( actionTriggered() ) );
    menu->addAction( typeAction );
  }

  connect( menu->menu(), SIGNAL(aboutToHide()), menu, SLOT(deleteLater())) ;
  return menu;
}

void Editor::registerElement( GuiElement *element )
{
  kDebug() <<"Registered element" << element->ref().toString();
  mElements.append( element );
}

void Editor::setEditMode( bool enabled )
{
  kDebug() <<"Setting editmode to" << enabled;
  mEditMode = enabled;


  if( enabled ) {
    FormGui *w = mManager->currentGui();
    GuiElement::List list = w->elements();
    mEditorWidget = new EditorWidget( this, w );
    mEditorWidget->setGuiElements( list );
    mEditorWidget->show();
  } else {
    mEditorWidget->hide();
    mEditorWidget->deleteLater();
  }
}

void Editor::toggleEditMode()
{
  setEditMode( !mEditMode );
}

void Editor::actionTriggered()
{
  QAction *action = dynamic_cast<QAction *>( sender() );
  if( !action )
    return;

  performAction( action->data().toString(), mEditorWidget->hoveredElement() );
}

void Editor::performAction( const QString &actionId, GuiElement *e )
{
  kDebug() <<"Performing action" << actionId;

  EditorAction *a = mActions[ actionId ];
  if( !a )
    return;

  a->perform( e );
}

void Editor::applyHint( const Hint &h )
{
  Hints hints;
  hints.insertHint( h );
  applyHints( hints );
}

void Editor::applyHints( const Hints &h )
{
  kDebug() ;

  mHints.dump();
  mHints.merge( h );
  mHints.dump();

  mElements.clear();
  mEditMode = false;
  emit hintsChanged( mHints );
}

GuiElement *Editor::selectWidget( SelectionMode sm )
{
  kDebug() ;
  return mEditorWidget->selectElement( sm );
}

void Editor::beginEdit()
{
  mInEdit = true;
  mEditorWidget->setInEdit( true );
}

void Editor::finishEdit()
{
  mInEdit = false;
  mEditorWidget->setInEdit( false );
}

void Editor::saveHints()
{
  if( mHintsUrl.isEmpty() || !mHintsUrl.isValid() ) {
    KMessageBox::sorry( mEditorWidget, i18n("Invalid Url '%1'.",
        mHintsUrl.prettyUrl() ) );
    return;
  } else {
    QFile file( mHintsUrl.path() );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
      KMessageBox::sorry( mEditorWidget, i18n("Can not open '%1'.",
        mHintsUrl.prettyUrl() ) );
      return;
    }
    file.write( mHints.toXml().toLatin1() );
    file.close();
  }
}

void Editor::saveHintsAs()
{
  KUrl url = KFileDialog::getSaveUrl(  KUrl(), QString(), mEditorWidget,
    i18n("Select Hints File") );
  if( !url.isEmpty() && url.isValid() ) {
    mHintsUrl = url;
    saveHints();
  } else  {
    KMessageBox::sorry( mEditorWidget, i18n("Invalid Url '%1'.",
        url.prettyUrl() ) );
  }
}

#include "editor.moc"
