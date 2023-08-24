
/******************************************************************************
* MODULE     : url.hpp
* DESCRIPTION: unified resource location handling
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#ifndef URL_H
#define URL_H

#include "tree.hpp"

#define URL_SYSTEM 0
#define URL_UNIX 1
#define URL_STANDARD 2
#define URL_CLEAN_UNIX 3

#define URL_TUPLE 245

/******************************************************************************
* The url data type
******************************************************************************/

struct url_rep: concrete_struct {
  tree t;
  inline url_rep (tree t2): t (t2) {}
};

class url {
  CONCRETE(url);
private:
  url (tree t): rep (tm_new<url_rep> (t)) {}
public:
  url ();
  url (const char* name);
  url (string name);
  url (string dir, string name);
  inline bool operator == (url u) { return rep->t == u->t; }
  inline bool operator != (url u) { return rep->t != u->t; }
  inline url operator [] (int i) { return url (rep->t[i]); }
  friend url as_url (tree t);
};
CONCRETE_CODE(url);

tm_ostream& operator << (tm_ostream& out, url u);
inline url as_url (tree t) { return url(t); }
string as_string (url u, int type= URL_SYSTEM);
inline tree as_tree (url u) { return tree (u->t); }
inline string as_system_string (url u) { return as_string (u, URL_SYSTEM); }
inline string as_unix_string (url u) { return as_string (u, URL_UNIX); }
inline string as_standard_string (url u) { return as_string (u,URL_STANDARD); }

/******************************************************************************
* url constructors
******************************************************************************/

url url_none (); 
inline url url_here () { return as_url (tree (".")); }
inline url url_parent () { return as_url (tree ("..")); }
inline url url_ancestor () { return as_url (tree ("...")); }
inline url url_pwd () { return url_system ("$PWD"); }

url url_root (string protocol);       // root url
url url_ramdisc (string contents);    // ramdisc with contents contents
url url_wildcard ();                  // any url
url url_wildcard (string name);       // string with * wildcards

url operator * (url u1, url u2);      // concatenation of url with rootless url
url operator * (url u1, const char* name);
url operator * (url u1, string name);
url url_concat (url u1, url u2);
url operator | (url u1, url u2);      // disjunction of urls like in file paths
url url_or (url u1, url u2);

inline url url_parent (url u) { return u * url_parent (); }

/******************************************************************************
* predicates
******************************************************************************/

bool is_none (url u);
bool is_here (url u);
bool is_parent (url u);
bool is_ancestor (url u);
bool is_atomic (url u);
bool is_concat (url u);
bool is_or (url u);
bool is_root (url u);
bool is_root (url u, string s);
bool is_root_web (url u);
bool is_root_tmfs (url u);
bool is_root_blank (url u);
bool is_wildcard (url u);
bool is_wildcard (url u, int n);
bool is_pseudo_atomic (url u);

bool is_rooted (url u);
bool is_rooted (url u, string protocol);
bool is_rooted_web (url u);
bool is_rooted_tmfs (url u);
bool is_rooted_tmfs (url u, string sub_protocol);
bool is_rooted_blank (url u);
bool is_name (url u);
bool is_rooted_name (url u);
bool is_ramdisc (url u);

/******************************************************************************
* operations on urls
******************************************************************************/

string get_root (url u);             // get root
url    unroot (url u);               // remove root
url    reroot (url u, string s);     // reroot using new protocol

#endif // defined URL_H
