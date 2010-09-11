/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "templatetag.h"

#include "exception.h"
#include "grantlee_tags_p.h"
#include "parser.h"

TemplateTagNodeFactory::TemplateTagNodeFactory()
{

}

Node* TemplateTagNodeFactory::getNode( const QString &tagContent, Parser *p ) const
{
  QStringList expr = smartSplit( tagContent );
  expr.takeAt( 0 );
  if ( expr.size() <= 0 ) {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "'templatetag' statement takes one argument" ) );
  }

  QString name = expr.first();

  if ( !TemplateTagNode::isKeyword( name ) ) {
    throw Grantlee::Exception( TagSyntaxError, QLatin1String( "Not a template tag" ) );
  }

  return new TemplateTagNode( name, p );
}


TemplateTagNode::TemplateTagNode( const QString &name, QObject *parent )
    : Node( parent )
{
  m_name = name;
}

static QHash<QString, QString> getKeywordMap()
{
  QHash<QString, QString> map;
  map.insert( QLatin1String( "openblock" ), BLOCK_TAG_START );
  map.insert( QLatin1String( "closeblock" ), BLOCK_TAG_END );
  map.insert( QLatin1String( "openvariable" ), VARIABLE_TAG_START );
  map.insert( QLatin1String( "closevariable" ), VARIABLE_TAG_END );
  map.insert( QLatin1String( "openbrace" ), QChar::fromLatin1( '{' ) );
  map.insert( QLatin1String( "closebrace" ), QChar::fromLatin1( '}' ) );
  map.insert( QLatin1String( "opencomment" ), COMMENT_TAG_START );
  map.insert( QLatin1String( "closecomment" ), COMMENT_TAG_END );
  return map;
}

bool TemplateTagNode::isKeyword( const QString &name )
{
  static QHash<QString, QString> map = getKeywordMap();
  return map.contains( name );
}

void TemplateTagNode::render( OutputStream *stream, Context *c )
{
  Q_UNUSED( c )
  static QHash<QString, QString> map = getKeywordMap();
  ( *stream ) << map.value( m_name );
}

