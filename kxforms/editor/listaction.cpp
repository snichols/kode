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

#include "listaction.h"

#include "editor.h"
#include "editorwidget.h"
#include "../hints.h"
#include "../guielement.h"
#include "../list.h"

#include <kdebug.h>
#include <klocale.h>
#include <kdialog.h>

#include <QCheckBox>
#include <QVBoxLayout>

using namespace KXForms;

class ListActionDialog : public KDialog
{
  public:
    ListActionDialog( const QString &caption, QWidget *parent );

    bool showHeader();
    void setShowHeader( bool a ) { mChkHeader->setChecked( a ); }
    bool showFilter();
    void setShowFilter( bool a ) { mChkFilter->setChecked( a ); }

  private:
    QCheckBox *mChkHeader;
    QCheckBox *mChkFilter;
};

ListActionDialog::ListActionDialog( const QString &caption, QWidget *parent )
: KDialog( parent )
{
  setCaption( caption );

  setButtons( KDialog::Ok | KDialog::Cancel );
  setDefaultButton( KDialog::Ok );
  showButtonSeparator( true );

  QWidget *page = new QWidget(this);
  setMainWidget(page);

  QVBoxLayout *topLayout = new QVBoxLayout( page );	
  topLayout->addSpacing( spacingHint() );

  mChkHeader = new QCheckBox( i18n("Show Headers"), page );
  topLayout->addWidget( mChkHeader );

  mChkFilter = new QCheckBox( i18n("Show Filterline"), page );
  topLayout->addWidget( mChkFilter );
}

bool ListActionDialog::showHeader()
{
  return mChkHeader->checkState() == Qt::Checked;
}

bool ListActionDialog::showFilter()
{
  return mChkFilter->checkState() == Qt::Checked;
}







ListAction::ListAction( Editor *e)
: EditorAction( e )
{
}

ListAction::~ListAction()
{
}

void ListAction::perform( GuiElement *e )
{
  kDebug() ;
  editor()->beginEdit();

  List::ListProperties prop;
  if( List *l = dynamic_cast< List *>( e ) )
    prop = l->listProperties();

  ListActionDialog *dlg = new ListActionDialog( i18n("Edit %1", e->ref().toString()), e->widget() );
  dlg->setShowHeader( prop.showHeader );
  dlg->setShowFilter( prop.showFilter );
  if( dlg->exec() == QDialog::Accepted ) {

    Hint h;
    h.setRef( e->id() );
    h.setValue( Hint::ListShowHeader, dlg->showHeader() ? "true" : "false" );
    h.setValue( Hint::ListShowSearch, dlg->showFilter() ? "true" : "false" );
    emit hintGenerated( h );
  }
  dlg->deleteLater();

  editor()->finishEdit();
}

#include "listaction.moc"
