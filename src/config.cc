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
 */

#include "config.hh"

#include <stdexcept>
#include <fstream>
#include <stack>
#include <sstream>

namespace sc
{

Config::Config ()
{
  _root = new Value ();
  _root->_type = Value::t_group;
  _root->_name = "root";
}

void Config::read ( const char * fname )
{
  file.open ( fname, std::fstream::in );

  std::stack <Value*> stack;
  Value * v = _root;
  Value * g = _root;

  while ( not file.eof () and file.good () )
  {
    skipws ();
    if ( file.eof () ) break;

    if ( current == ':' )
    {
      skipws ();
      v->_name = scanident ();
    }
    else if ( current == '(' )
    {
      stack.push ( g );
      v = new Value ();
      v->_type = Value::t_group;
      g->vals.push_back ( v );
      g = v;
      stack.push ( v );
    }
    else if ( current == ')' )
    {
      v = stack.top ();
      stack.pop ();
      g = stack.top ();
      stack.pop ();
    }
    else if ( current == '"' )
    {
      next ();
      v = new Value ();
      v->_type = Value::t_string;
      v->val_s = scanstr ();
      g->vals.push_back ( v );
    }
    else if ( isdigit ( current ) )
    {
      std::string numval;
      bool doubleval = false;
      do
      {
        numval += current;
        if ( current == '.' )
        {
          if ( doubleval ) throw new std::runtime_error ( "allready one decimal dot in number!" );
          doubleval = true;
        }
        next ();
      } while ( isdigit ( current ) or current == '.' );
      std::istringstream iss ( numval );
      v = new Value ();
      if ( doubleval )
      {
        iss >> v->val_d;
        v->_type = Value::t_float;
      }
      else
      {
        iss >> v->val_l;
        v->_type = Value::t_number;
      }
      g->vals.push_back ( v );
    }
    else
    {
      std::string ident = scanident ();
      v = new Value ();
      v->_type = Value::t_boolean;
           if ( ident == "true" )  v->val_b = true;
      else if ( ident == "false" ) v->val_b = false;
      else throw new std::runtime_error ( "unexcepted " + ident );
      g->vals.push_back ( v );
    }
  }

  file.close ();
}

std::string Config::scanident ()
{
  std::string ret;
  do
  {
    ret += current;
    next ();
  } while ( isalnum ( current ) );
  return ret;
}

std::string Config::scanstr ()
{
  std::string ret;
  while ( current != '"' )
  {
    ret += current;
    next ();
  }
  return ret;
}

void Config::next ()
{
  if ( file.eof () ) throw new std::runtime_error ( "eof reached." );
  current = file.get ();
  if ( current == '#' )
    do current = file.get () ; while ( current != '\n' );
}

void Config::skipws ()
{
  do next (); while ( isspace ( current ) );
}

void Config::save ( const char * fname )
{
  file.open ( fname, std::fstream::out );

  exportfn ( _root );

  file.close ();
}

void Config::exportfn ( Value * g )
{
  if ( g != _root )
  {
    file << "( ";
    if ( g->name () != "" )
      file << ':' << g->name ();
    file << '\n';
  }

  Value * v = g;
  for ( int i=0 ; i<g->length () ; ++i )
  {
    v = &((*g)[i]);

    if ( v->_type == Value::t_boolean )
    {
      if ( v->val_b )
        file << "true ";
      else
        file << "false ";
    }
    else if ( v->_type == Value::t_group )
    {
      exportfn ( v );
      continue;
    }
    else if ( v->_type == Value::t_number )
      file << v->val_l << ' ';
    else if ( v->_type == Value::t_float )
      file << v->val_d << ' ';
    else if ( v->_type == Value::t_string )
      file << '"' << v->val_s << "\" ";

    if ( v->name () != "" )
      file << ':' << v->name () << '\n';
  }
  if ( g != _root )
    file << ")\n";
}

}
