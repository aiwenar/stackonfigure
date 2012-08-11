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
  int           current;
  std::fstream  file;

  void        skipws    ();
  void        next      ();
  void        exportfn  ( Value * );
  std::string scanident ();
  std::string scanstr   ();
};

}
