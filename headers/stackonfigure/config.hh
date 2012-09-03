/* This file is part of StacKonfigure.
 *
 * StacKonfigure is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * StacKonfigure is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with StacKonfigure.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright © 2012 Krzysztof Mędrzycki
 */

#pragma once

#include <string>
#include <fstream>

#include <stackonfigure/value.hh>

namespace sc
{

class Config
{
public:
  Config  ();

  void  read  ( const char * file );
  void  save  ( const char * file );

  inline bool get ( const std::string& path, double& d )      const { return _root->get ( path, d ); }
  inline bool get ( const std::string& path, bool& b )        const { return _root->get ( path, b ); }
  inline bool get ( const std::string& path, long long& l )   const { return _root->get ( path, l ); }
  inline bool get ( const std::string& path, std::string& s ) const { return _root->get ( path, s ); }

  inline bool exists  ( const std::string& path ) { return _root->exists ( path ); }

  inline Value& operator[]  ( const std::string& path ) const { return (*_root)[path]; }
  inline Value& operator[]  ( const unsigned int id )   const { return (*_root)[id]; }
  inline Value& root  ()  { return *_root; }

private:
  Value       * _root;
  int           current, line;
  std::fstream  file;

  void        skipws    ();
  void        next      ();
  void        exportfn  ( Value *, int=-1 );
  void        error     ( std::string err );
  void        indent    ( int );
  std::string scanident ();
  std::string scanstr   ();
};

}
