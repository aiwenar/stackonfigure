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

#include <vector>
#include <map>
#include <string>

extern "C" long long int llabs ( long long j );

namespace sc
{

class Value
{
public:
  friend class Config;

  enum Type
  {
    t_float   = 1,
    t_number  = 2,
    t_boolean = 3,
    t_string  = 4,
    t_group   = 5,
    t_none    = 6
  };

  ~Value  ();

  inline  Type        type    ()  { return _type; }
  inline  int         length  ()  { return vals.size (); }
  inline  std::string name    ()  { return std::string ( _name.c_str () ); }
  inline  void        name    ( const std::string& newn )  { _name = newn; }

  Value&  append  ( Type ); // adds new unnamed entry at end of list

  operator  bool        ()  const;
  operator  double      ()  const;
  operator  long long   ()  const;
  operator  std::string ()  const;
  inline operator int () const { return operator long long (); }
  inline operator unsigned long long () const  { return llabs ( operator long long () ); }
  inline operator unsigned int () const { return llabs ( operator long long () ); }

  bool  get ( const std::string& path, double& d )      const;
  bool  get ( const std::string& path, bool& b )        const;
  bool  get ( const std::string& path, long long& l )   const;
  bool  get ( const std::string& path, std::string& s ) const;

  inline  bool  exists  ( const std::string& path ) { return find ( path ) != 0; }

  Value&  operator[]  ( const std::string& path )     const;
  Value&  operator[]  ( const unsigned long long l )  const;

  Value&  operator= ( double val );
  Value&  operator= ( bool val );
  Value&  operator= ( long long val );
  Value&  operator= ( std::string& val );

  inline Value& operator= ( int val )           { return this->operator = ( (long long)val ); }
  inline Value& operator= ( const char * val )  { std::string s ( val ); return this->operator = ( s ); }

  Value&  operator+=  ( long long v );

protected:
  Type        _type;
  double      val_d;
  long long   val_l;
  bool        val_b;
  std::string val_s;
  std::string _name;

  Value ( Value& );
  Value ();

  std::vector <Value*>  vals;

  void    assert  ( Type ) const;
  Value * find    ( const std::string& path ) const;
};

}
