
/******************************************************************************
* MODULE     : block.cpp
* DESCRIPTION: A block of Scheme data
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
                   2023  Darcy Shen
                   2023  Charonxin
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "block.hpp"
#include "analyze.hpp"

/******************************************************************************
 * Handling escape characters
 ******************************************************************************/

static int UNKNOWN= 1;
static int TUPLE  = 245;

void
unslash (string& s, string& r, int& i, int& r_index, int end_index) {
  char ch= s[i];
  while (i < end_index) {
    if ((ch == '\\') && ((i + 1) < end_index)) {
      i++;
      ch= s[i];
      switch (ch) {
      case '0':
        r[r_index]= ((char) 0);
        r_index++;
        break;
      case 'n':
        r[r_index]= '\n';
        r_index++;
        break;
      case 't':
        r[r_index]= '\t';
        r_index++;
        break;
      default:
        r[r_index]= ch;
        r_index++;
      }
    }
    else {
      r[r_index]= ch;
      r_index++;
    }
    i++;
    ch= s[i];
  }
}

/******************************************************************************
 * Converting strings to scheme trees
 ******************************************************************************/

static bool
is_spc (char c) {
  return (c == ' ') || (c == '\t') || (c == '\n');
}

static bool
ends_with_paren (char c) {
  return (c == ' ') || (c == '\t') || (c == '\n') || (c == '(') || (c == ')');
};

static scheme_tree
string_to_scheme_tree (string& s, int& i) {
  int lth= N (s);
  for (; i < lth; i++)
    switch (s[i]) {

    case ' ':
    case '\t':
    case '\n':
      break;
    case '(': {
      scheme_tree p (TUPLE);
      i++;
      while (true) {
        while (is_spc (s[i]) && (i < lth))
          i++;
        if ((i == lth) || (s[i] == ')')) break;
        p << string_to_scheme_tree (s, i);
      }
      if (i < lth) i++;
      return p;
    }

    case '\'':
      i++;
      return scheme_tree (TUPLE, tree ("\'"), string_to_scheme_tree (s, i));

    case '\"': { // "
      i++;
      int  end_index= i;
      char ch       = s[end_index];
      while (!(ch == '\"') && end_index < lth) {
        if ((ch == '\\') && (end_index < lth - 1)) end_index++;
        end_index++;
        ch= s[end_index];
      }
      const int r_size      = 1; // N ("\"");
      int       quoted_index= r_size;
      string    quoted (r_size + end_index - i);
      quoted[0]= '"';
      unslash (s, quoted, i, quoted_index, end_index);
      if (i < lth) i++;
      quoted->resize (quoted_index + 1);
      quoted[quoted_index]= '"';
      return scheme_tree (quoted);
    }

    case ';':
      while ((i < lth) && (s[i] != '\n'))
        i++;
      break;

    default: {
      int  end_index= i;
      char ch       = s[end_index];
      while (!(ends_with_paren (ch)) && end_index < lth) {
        if ((ch == '\\') && (end_index < lth - 1)) end_index++;
        end_index++;
        ch= s[end_index];
      }
      const int r_size     = 0; // empty string
      int       token_index= r_size;
      string    token (r_size + end_index - i);
      unslash (s, token, i, token_index, end_index);
      token->resize (token_index);
      return scheme_tree (token);
    }
    }

  return scheme_tree ("");
}

scheme_tree
string_to_scheme_tree (string s) {
  s    = replace (s, "\015", "");
  int i= 0;
  return string_to_scheme_tree (s, i);
}

scheme_tree
block_to_scheme_tree (string s) {
  scheme_tree p (TUPLE);
  int         i= 0, lth= N (s);
  while ((i < lth) && (is_spc (s[i]) || s[i] == ')'))
    i++;
  while (i < lth) {
    p << string_to_scheme_tree (s, i);
    while ((i < lth) && (is_spc (s[i]) || s[i] == ')'))
      i++;
  }
  return p;
}

/******************************************************************************
 * Handling escape characters
 ******************************************************************************/

string
slash (string s) {
  int    i, n= N (s);
  string r;
  for (i= 0; i < n; i++)
    switch (s[i]) {
    case '(':
    case ')':
    case ' ':
    case '\'':
      if ((n < 2) || (s[0] != '\042') || (s[n - 1] != '\042')) r << "\\";
      r << s[i];
      break;
    case '\\':
      r << '\\' << s[i];
      break;
    case '\042':
      if (((i == 0) && (s[n - 1] == '\042')) ||
          ((i == (n - 1)) && (s[0] == '\042')))
        r << s[i];
      else r << "\\" << s[i];
      break;
    case ((char) 0):
      r << "\\0";
      break;
    case '\t':
      r << "\\t";
      break;
    case '\n':
      r << "\\n";
      break;
    default:
      r << s[i];
    }
  return r;
}

/******************************************************************************
 * Converting scheme trees to strings
 ******************************************************************************/

inline bool
is_tuple (tree t) {
  return t->op == TUPLE;
}

inline bool
is_tuple (tree t, const char* s, int n) {
  return (t->op == TUPLE) && (N (t) == (n + 1)) && (t[0] == s);
}

static void
scheme_tree_to_string (string& out, scheme_tree p) {
  if (!is_tuple (p)) {
    string s= p->label;
    if (is_quoted (s)) out << scm_quote (raw_unquote (s));
    else out << slash (s);
  }
  else {
    if (is_tuple (p, "\'", 1)) {
      out << "\'";
      scheme_tree_to_string (out, p[1]);
    }
    else {
      int i, n= N (p);
      out << "(";
      for (i= 0; i < n; i++) {
        if (i > 0) out << " ";
        scheme_tree_to_string (out, p[i]);
      }
      out << ")";
    }
  }
}

string
scheme_tree_to_string (scheme_tree p) {
  string out;
  scheme_tree_to_string (out, p);
  return out;
}

string
scheme_tree_to_block (scheme_tree p) {
  string out;
  int    i, n= N (p);
  for (i= 0; i < n; i++)
    out << scheme_tree_to_string (p[i]) << "\n";
  return out;
}
