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

#include "service.h"

using namespace KWSDL;

Service::Service()
{
}

Service::Service( const QString &name )
  : mName( name )
{
}

void Service::addPort( const Port &port )
{
  mPorts.insert( port.mName, port );
}

Service::Port Service::port( const QString &name ) const
{
  return mPorts[ name ];
}

Service::Port::List Service::ports() const
{
  return mPorts.values();
}