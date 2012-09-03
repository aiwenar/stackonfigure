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

#include "value.hh"

#include <stdexcept>
#include <sstream>

namespace sc
{

Value::~Value ()
{
  for ( Value * v : vals )
    delete v;
}

Value::Value ()
: _type ( t_none )
, _name ()
, vals  ()
{
}

Value::Value ( Value & )
{}

//
// casters
//

Value::operator bool () const
{
  assert ( t_boolean );
  return val_b;
}

Value::operator double () const
{
  assert ( t_float );
  return val_d;
}

Value::operator long long () const
{
  assert ( t_number );
  return val_l;
}

Value::operator std::string () const
{
  assert ( t_string );
  return val_s;
}

//
// getters
//

bool Value::get ( const std::string& path, bool& b ) const
{
  Value * v = this->find ( path );
  if ( v == 0 ) return false;
  v->assert ( t_boolean );
  b = *v;
  return true;
}

bool Value::get ( const std::string& path, double& d ) const
{
  Value * v = find ( path );
  if ( v == 0 ) return false;
  v->assert ( t_float );
  d = *v;
  return true;
}

bool Value::get ( const std::string& path, long long& l ) const
{
  Value * v = find ( path );
  if ( v == 0 ) return false;
  v->assert ( t_number );
  l = *v;
  return true;
}

bool Value::get ( const std::string& path, std::string& s) const
{
  Value * v = find ( path );
  if ( v == 0 ) return false;
  v->assert ( t_string );
  s = v->val_s;
  return true;
}

Value& Value::operator[] ( const std::string& path ) const
{
  Value * v = find ( path );
  if ( v == 0 ) throw std::runtime_error ( "cannot find " + path );
  return *v;
}

Value& Value::operator [] ( const unsigned long long l ) const
{
  if ( l > vals.size () ) throw std::out_of_range ( "Value[]");
  return *vals[l];
}

//
// setters
//

Value& Value::operator= ( double val )
{
  assert ( t_float );
  val_d = val;
  return *this;
}

Value& Value::operator= ( bool val )
{
  assert ( t_boolean );
  val_b = val;
  return *this;
}

Value& Value::operator= ( long long val )
{
  assert ( t_number );
  val_l = val;
  return *this;
}

Value& Value::operator= ( std::string& val )
{
  assert ( t_string );
  val_s = val;
  return *this;
}

Value& Value::operator += ( long long val )
{
  assert ( t_number );
  val_l += val;
  return *this;
}

//
// misc
//

const char * types_names[] = {
  "invalid!", "float", "number", "boolean", "string", "group", "none"
};

void Value::assert ( Type t ) const
{
  if ( t != _type ) throw std::runtime_error ( std::string ( "Bad type (" ) + types_names[t] + " ~= " + types_names[_type] + ")." );
}

std::string scanident ( const std::string& str, unsigned int * i )
{
  std::string ret;
  while ( isalnum ( str[*i] ) or str[*i] == '_' )
  {
    ret += str[*i];
    ++*i;
  }
  return ret;
}

int scannum ( const std::string& str, unsigned int * i )
{
  std::string nums;
  while ( isdigit ( str[*i] ) )
  {
    nums += str[*i];
    ++*i;
  }
  std::istringstream iss ( nums );
  int ret;
  iss >> ret;
  return ret;
}

Value * findval ( const std::vector <Value*>& vals, const std::string& name )
{
  for ( Value * val : vals )
    if ( val->name () == name )
      return val;
  return 0;
}

Value& Value::append ( Type t )
{
  Value * v = new Value ();
  v->_type = t;
  vals.push_back ( v );
  return *v;
}

Value * Value::find ( const std::string& path ) const
{
  Value * v = (Value*) this;
  unsigned int i = 0;
  std::string ident;
  while ( i < path.size () and path[i] != '\0' )
  {
    if ( v == 0 )
      return 0;
    v->assert ( t_group );
    if ( path[i] == '.' )
    {
      ++i;
      ident = scanident ( path, &i );
      v = findval ( v->vals, ident );
    }
    else if ( path[i] == '[' )
    {
      ++i;
      int num = scannum ( path, &i );
      ++i;
      v = v->vals[num];
    }
    else throw std::runtime_error ( std::string ( "invalid path `" ) + path[i] + "`." );
  }
  return v;
}

}
