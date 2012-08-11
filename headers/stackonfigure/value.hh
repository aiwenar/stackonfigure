#pragma once

#include <vector>
#include <map>
#include <string>

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
  inline  std::string name    ()  { return _name; }
  inline  void        name    ( const std::string& newn )  { _name = newn; }

  Value&  append  ( Type ); // adds new unnamed entry at end of list

  operator  bool        ()  const;
  operator  double      ()  const;
  operator  long long   ()  const;
  operator  std::string ()  const;

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
  Value&  operator= ( int val ) { return this->operator = ( (long long)val ); }
  Value&  operator= ( std::string& val );

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
