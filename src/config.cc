#include "config.hh"

#include <stdexcept>
#include <fstream>
#include <stack>
#include <sstream>

namespace sc
{

Config::Config ()
{
  root = new Value ();
  root->_type = Value::t_group;
  root->_name = "root";
}

void Config::read ( const char * fname )
{
  file.open ( fname, std::fstream::in );

  std::stack <Value*> stack;
  Value * v = root;
  Value * g = root;

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

  exportfn ( root );

  file.close ();
}

void Config::exportfn ( Value * g )
{
  if ( g != root )
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
  if ( g != root )
    file << ")\n";
}

}
