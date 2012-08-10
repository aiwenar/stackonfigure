#pragma once

#include <string>
#include <fstream>

#include <stackonfigured/value.hh>

namespace sc
{

class Config
{
public:
  Config  ();

  void  read  ( const char * file );
  void  save  ( const char * file );

  inline bool get ( const std::string& path, double& d )      const { return root->get ( path, d ); }
  inline bool get ( const std::string& path, bool& b )        const { return root->get ( path, b ); }
  inline bool get ( const std::string& path, long long& l )   const { return root->get ( path, l ); }
  inline bool get ( const std::string& path, std::string& s ) const { return root->get ( path, s ); }

  inline bool exists  ( const std::string& path ) { return root->exists ( path ); }

  inline Value& operator[]  ( const std::string& path ) const { return (*root)[path]; }
  inline Value& operator[]  ( const unsigned int id )   const { return (*root)[id]; }

private:
  Value       * root;
  int           current;
  std::fstream  file;

  void        skipws    ();
  void        next      ();
  void        exportfn  ( Value * );
  std::string scanident ();
  std::string scanstr   ();
};

}
