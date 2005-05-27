/*
    This file is part of KDE.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

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

#include "port.h"

using namespace KWSDL;

Port::Operation::Operation()
{
}

Port::Operation::Operation( const QString &name, const QString &input, const QString &output )
  : mName( name ), mInput( input ), mOutput( output )
{
}

Port::Port()
{
}

Port::Port( const QString &name )
  : mName( name )
{
}

void Port::addOperation( const Operation &operation )
{
  mOperations.append( operation );
}

Port::Operation Port::operation( const QString &name ) const
{
  Operation::List::ConstIterator it;
  for ( it = mOperations.begin(); it != mOperations.end(); ++it )
    if ( (*it).name() == name )
      return *it;

  return Operation();
}

Port::Operation::List Port::operations() const
{
  return mOperations;
}