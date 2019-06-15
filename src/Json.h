/*
 *
 *  Copyright ( c ) 2019
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  ( at your option ) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JSON_H
#define JSON_H

#include "json.h"

#include <QString>
#include <QStringList>
#include <QFile>

namespace sirikali
{
	class json
	{
	public:
		enum class type{ PATH,CONTENTS } ;

		json( const QString& e,type s )
		{
			if( s == type::PATH ){

				QFile file( e ) ;

				if( file.open( QIODevice::ReadOnly ) ){

					m_json = nlohmann::json::parse( file.readAll().constData() ) ;
				}
			}else{
				m_json = nlohmann::json::parse( e.toStdString() ) ;
			}
		}
		json()
		{
		}
		template< typename T >
		T get( const char * key ) const
		{
			return m_json[ key ].get< T >() ;
		}
		QStringList getStringList( const char * key ) const
		{
			QStringList s ;

			const auto e = m_json[ key ].get< std::vector< std::string > >() ;

			for( const auto& it : e ){

				s.append( it.c_str() ) ;
			}

			return s ;
		}
		QString getString( const char * key ) const
		{
			return m_json[ key ].get< std::string >().c_str() ;
		}
		QByteArray getByteArray( const char * key ) const
		{
			return this->getString( key ).toLatin1() ;
		}
		bool getBool( const char * key ) const
		{
			return m_json[ key ].get< bool >() ;
		}
		bool getInterger( const char * key ) const
		{
			return m_json[ key ].get< int >() ;
		}
		double getDouble( const char * key ) const
		{
			return m_json[ key ].get< double >() ;
		}
		json& operator[]( const char * key )
		{
			m_key = key ;
			return *this ;
		}
		void operator=( const QString& value )
		{
			m_json[ m_key ] = value.toStdString() ;
		}
		void operator=( const QByteArray& value )
		{
			m_json[ m_key ] = value.constData() ;
		}
		void operator=( const QStringList& value )
		{
			std::vector< std::string > m ;

			for( const auto& it : value ){

				m.emplace_back( it.toStdString() ) ;
			}

			m_json[ m_key ] = m ;
		}
		template< typename T >
		void operator=( const T& value )
		{
			m_json[ m_key ] = value ;
		}
		bool toFile( const QString& path,int indent = 8 ) const
		{
			QFile file( path ) ;

			if( file.open( QIODevice::WriteOnly ) ){

				file.write( m_json.dump( indent ).data() ) ;

				return true ;
			}else{
				return false ;
			}
		}
		QByteArray structure( int indent = 8 ) const
		{
			return m_json.dump( indent ).c_str() ;
		}
		QStringList getTags( const char * tag )
		{
			QStringList s ;

			for( const auto& it : m_json ){

				auto e = it.find( tag ) ;

				if( e != it.end() ){

					s.append( QString::fromStdString( e.value() ) ) ;
				}
			}

			return s ;
		}
	private:
		const char * m_key ;
		nlohmann::json m_json ;
	};
}

#endif //JSON_H