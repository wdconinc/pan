#ifndef PAN_TaEvent
#define PAN_TaEvent

//**********************************************************************
//
//     HALL A C++/ROOT Parity Analyzer  Pan           
//
//           TaEvent.hh  (interface)
//
// Author:  R. Holmes <http://mepserv.phy.syr.edu/~rsholmes>, A. Vacheret <http://www.jlab.org/~vacheret>, R. Michaels <http://www.jlab.org/~rom>, K.Paschke
//
////////////////////////////////////////////////////////////////////////
//
//    An event of pure data, un-spoilt by the vagaries of simulation. Inherits from VaEvent.
//
////////////////////////////////////////////////////////////////////////

#define TAEVENT_VERBOSE 1

#include "VaEvent.hh"
#include "Rtypes.h"
#include "PanTypes.hh"
#include "DevTypes.hh"
#include <map>
#include <vector>
#include <iterator>
#include <string>
#include <utility>

class TaDevice;
class TTree;
class TaCutList;
class TaLabelledQuantity;
class TaRun;
class TaDataBase;

class TaEvent : public VaEvent {

public:

  // Constructors/destructors/operators
  TaEvent();
  virtual ~TaEvent();
  TaEvent(const TaEvent &ev);
  TaEvent& operator=(const TaEvent &ev);
  TaEvent& CopyInPlace (const TaEvent& rhs);

  // Major functions
  ErrCode_t RunInit(const TaRun& run);    // initialization at start of run
  void Decode( TaDevice& devices );             // decode the event 

private:

  // Private methods

  // Constants

  // Static members

  // Data members

#ifndef NODICT
ClassDef(TaEvent,0)  // An event
#endif

};

#endif
