//**********************************************************************
//
//     HALL A C++/ROOT Parity Analyzer  Pan           
//
//       TaFileName.cc  (implementation)
//
// Author:  R. Holmes <http://mepserv.phy.syr.edu/~rsholmes>, A. Vacheret <http://www.jlab.org/~vacheret>, R. Michaels <http://www.jlab.org/~rom>
// @(#)pan/src:$Name$:$Id$
//
////////////////////////////////////////////////////////////////////////
//
// Provides methods to generate Pan-standard filenames.
//
////////////////////////////////////////////////////////////////////////

#include "TaFileName.hh"
#include <strstream>
#include <ctime>

#ifndef NODICT
ClassImp(TaFileName)
#endif

string TaFileName::fgAnaStr = "";
string TaFileName::fgBaseName = "";

// Constructors/destructors/operators

TaFileName::TaFileName (const string s, 
			const string com = "", 
			const string suf = "")
{
  // Construct a Pan standard filename for a file of type s (which may
  // be "coda" for CODA data files, "db" for run-specific ASCII
  // database files, "dbdef" for generic ASCII database files, "root"
  // for ROOT files, or "output" for general output files) with
  // additional comment tag com (for "root" or "output" files only)
  // and suffix suf (for "output" files only).  
  //
  //  Filenames/paths generated are as follows. (In the following,
  //  environment variables are enclosed within $().)
  //
  // File type "coda":
  //
  //   $(PAN_CODA_FILE_PATH)/$(PAN_FILE_PREFIX)_XXXX.$(PAN_CODA_FILE_SUFFIX)
  //
  //   where XXXX is the (4-digit) run number.
  //
  // File type "db": 
  //
  //   $(PAN_DB_FILE_PATH)/$(PAN_FILE_PREFIX)_XXXX.$(PAN_DB_FILE_SUFFIX)
  //
  // File type "dbdef":
  //
  //   $(PAN_DB_FILE_PATH)/control.$(PAN_DB_FILE_SUFFIX)
  //
  // File type "root"
  //
  //   $(PAN_ROOT_FILE_PATH)/$(PAN_FILE_PREFIX)_XXXX_ZZZZ.$(PAN_ROOT_FILE_SUFFIX)
  //
  // or
  //
  //   $(PAN_ROOT_FILE_PATH)/$(PAN_FILE_PREFIX)_XXXX_ZZZZ_WWWW.$(PAN_ROOT_FILE_SUFFIX)
  //
  //   where ZZZZ = analysis type (not necessarily 4 characters).
  //   WWWW = whatever descriptive tag the programmer wants (not
  //   necessarily 4 characters), as given in the string com.
  //
  // File type "output"
  //
  //   $(PAN_OUTPUT_FILE_PATH)/$(PAN_FILE_PREFIX)_XXXX_ZZZZ_WWWW.VVV
  //
  //   as above, where VVV is whatever suffix the programmer wants,
  //   e.g. '.txt', as given in the string suf.
  //
  // All the environment variables have default values to use in case
  // they're undefined:
  //
  //   $PAN_FILE_PREFIX           parityYY  where YY = last 2 digits of year
  //   $PAN_CODA_FILE_SUFFIX      dat
  //   $PAN_CODA_FILE_PATH        .
  //   $PAN_DB_FILE_SUFFIX        db
  //   $PAN_DB_FILE_PATH          ./db (for "db" type) or . (for "dbdef")
  //   $PAN_ROOT_FILE_PATH        .
  //   $PAN_ROOT_FILE_SUFFIX      root
  //   $PAN_OUTPUT_FILE_PATH      .

  string path (".");
  string tags ("");
  string suffix ("");
  string base = fgBaseName;

  if (fgBaseName == "")
    Setup (0, "");

  if (s == "coda")
    {
      suffix = GetEnvOrDef ("PAN_CODA_FILE_SUFFIX", "dat");
      path = GetEnvOrDef ("PAN_CODA_FILE_PATH", ".");
    }
  else if (s == "db")
    {
      suffix = GetEnvOrDef ("PAN_DB_FILE_SUFFIX", "db");
      path = GetEnvOrDef ("PAN_DB_FILE_PATH", "./db");
    }
  else if (s == "dbdef")
    {
      suffix = GetEnvOrDef ("PAN_DB_FILE_SUFFIX", "db");
      path = GetEnvOrDef ("PAN_DB_FILE_PATH", ".");
      base = "control";
    }
  else if (s == "root")
    {
      suffix = GetEnvOrDef ("PAN_ROOT_FILE_SUFFIX", "root");
      path = GetEnvOrDef ("PAN_ROOT_FILE_PATH", ".");
    }
  else if (s == "output")
    {
      suffix = suf;
      path = GetEnvOrDef ("PAN_OUTPUT_FILE_PATH", ".");
    }
  else
    {
      clog << "TaFileName::TaFileName ERROR: Unknown filename type " << s << endl;
      fFileName = "ERROR";
      return;
    }

  if (s == "root" || s == "output")
    {
      if (fgAnaStr != "")
	tags += string ("_") + fgAnaStr;
      if (com != "")
	tags += string ("_") + com;
    }

  fFileName = path + string("/") + base + tags + string(".") + suffix;
}

TaFileName::TaFileName (const TaFileName& fn)
{
  fFileName = fn.fFileName;
}

TaFileName&
TaFileName::operator= (const TaFileName& fn)
{
  if (this != &fn)
    fFileName = fn.fFileName;
  return *this;
}

// Major functions

void 
TaFileName::Setup (RunNumber_t r, string a)
{
  // Prepare for future creation of filenames by storing the run
  // number r and analysis type a, and getting the file prefix from
  // $(PAN_FILE_PREFIX) or building it by appending last 2 digits of
  // the year to "parity".  Yes, we have a year 2100 problem.  This
  // should be called before any filenames are constructed, perhaps
  // repeatedly as the run number and analysis type become known.

  string runstr ("xxxx");
  if (r != 0)
    {
      char temp[20];
      sprintf (temp, "%4.4d", r);
      runstr = temp;
    }
  string prefix (GetEnvOrDef ("PAN_FILE_PREFIX", ""));
  if (prefix == "")
    {
      time_t t;
      string ystr;
      if (time (&t) == time_t (-1))
	{
	  clog << "TaFileName::Setup ERROR: Can't get time" << endl;
	  ystr = "xx";
	}
      else
	{
	  tm* lt = gmtime(&t);
	  char yc[2];
	  sprintf (yc, "%2.2d", (lt->tm_year) % 10);
	  ystr = yc;
	}
      prefix = "parity";
      prefix += ystr;
    }
  fgBaseName = prefix + string ("_") + runstr;
  fgAnaStr = a;
}

// Private member functions


// Non member functions

string
GetEnvOrDef (string e, const string d)
{
  // Return value of environment variable e, or default d.

  char *env = getenv (e.c_str());
  if (env == 0)
    return d;
  else
    return env;
}