/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

#ifndef __ILOOPS_H__
#define __ILOOPS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "feloops.h"
#include "char.h"

template <class charptr, class routine>
inline void DoForBodyParts(charptr C, routine F)
{
  DoForElements<bodypart*>(C, &character::GetBodyPart, F, C->GetBodyParts());
}

template <class charptr, class routine, class param>
inline void DoForBodyParts(charptr C, routine F, param P)
{
  DoForElements<bodypart*>(C, &character::GetBodyPart, F, P, C->GetBodyParts());
}

template <bool OrBit, class routine>
inline bool CombineBodyPartPredicates(const character* C, routine F)
{
  return CombinePredicates<OrBit, const bodypart*>(C, &character::GetBodyPart, F, C->GetBodyParts());
}

template <bool OrBit, class routine, class param>
inline bool CombineBodyPartPredicates(const character* C, routine F, param P)
{
  return CombinePredicates<OrBit, const bodypart*>(C, &character::GetBodyPart, F, P, C->GetBodyParts());
}

template<class routine>
inline int SumBodyPartProperties(const character* C, routine F)
{
  return SumProperties<const bodypart*>(C, &character::GetBodyPart, F, C->GetBodyParts());
}

template <class charptr, class routine>
inline void DoForEquipments(charptr C, routine F)
{
  DoForElements<item*>(C, &character::GetEquipment, F, C->GetEquipments());
}

template <class charptr, class routine, class param>
inline void DoForEquipments(charptr C, routine F, param P)
{
  DoForElements<item*>(C, &character::GetEquipment, F, P, C->GetEquipments());
}

template <bool OrBit, class routine>
inline bool CombineEquipmentPredicates(const character* C, routine F)
{
  return CombinePredicates<OrBit, const item*>(C, &character::GetEquipment, F, C->GetEquipments());
}

template <bool OrBit, class routine, class param>
inline bool CombineEquipmentPredicates(const character* C, routine F, param P)
{
  return CombinePredicates<OrBit, const item*>(C, &character::GetEquipment, F, P, C->GetEquipments());
}

template <class routine, class param>
inline item* FindEquipment(const character* C, routine F, param P)
{
  return FindElement<item*>(C, &character::GetEquipment, F, P, C->GetEquipments());
}

#endif
