#ifndef __ATTRIB_H__
#define __ATTRIB_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "ivandef.h"

class attribute
{
 public:
  attribute(uchar Value) : Value(Value) { }
  uchar GetValue() const () { return Value; }
  void SetValue(uchar What) { Value = What; }
  void EditValue(char What) { Value += What; }
  uchar GetExperience() const () { return Experience; }
  void SetExperience(uchar What) { Experience = What; }
  void EditExperience(char What) { Experience += What; }
  bool IsReadyForIncrease() const { return Experience >= long(Value) * 1000; }
  bool IsReadyForDecrease() const { return Experience <= (long(Value) - 100) * 250; }
 private:
  uchar Value;
  long Experience;
};

#endif
