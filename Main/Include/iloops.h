/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should included
 *  with this file for more details
 *
 */

#ifndef __ILOOPS_H__
#define __ILOOPS_H__

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

template <truth OrBit, class routine>
inline truth CombineBodyPartPredicates(const character* C, routine F)
{
  return CombinePredicates<OrBit, const bodypart*>(C, &character::GetBodyPart, F, C->GetBodyParts());
}

template <truth OrBit, class routine, class param>
inline truth CombineBodyPartPredicates(const character* C, routine F, param P)
{
  return CombinePredicates<OrBit, const bodypart*>(C, &character::GetBodyPart, F, P, C->GetBodyParts());
}

template <class routine>
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

template <truth OrBit, class routine>
inline truth CombineEquipmentPredicates(const character* C, routine F)
{
  return CombinePredicates<OrBit, const item*>(C, &character::GetEquipment, F, C->GetEquipments());
}

template <truth OrBit, class routine, class param>
inline truth CombineEquipmentPredicates(const character* C, routine F, param P)
{
  return CombinePredicates<OrBit, const item*>(C, &character::GetEquipment, F, P, C->GetEquipments());
}

template <class routine, class param>
inline item* FindEquipment(const character* C, routine F, param P)
{
  return FindElement<item*>(C, &character::GetEquipment, F, P, C->GetEquipments());
}

#endif
