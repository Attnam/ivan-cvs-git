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

#ifndef __FELOOPS_H__
#define __FELOOPS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

template <class objectptr, class routine>
void DoIndexParameterRoutine(objectptr O, routine F, int A)
{
  for(int c = 0; c < A; ++c)
    (O->*F)(c);
}

template <bool OrBit, class element, class objectptr, class extractor, class routine>
inline bool CombinePredicates(objectptr O, extractor X, routine F, int A)
{
  for(int c = 0; c < A; ++c)
    {
      element E = (O->*X)(c);

      if(E && (E->*F)() == OrBit)
	return OrBit;
    }

  return !OrBit;
}

template <bool OrBit, class element, class objectptr, class extractor, class routine, class param>
inline bool CombinePredicates(objectptr O, extractor X, routine F, param P, int A)
{
  for(int c = 0; c < A; ++c)
    {
      element E = (O->*X)(c);

      if(E && (E->*F)(P) == OrBit)
	return OrBit;
    }

  return !OrBit;
}

template <class element, class objectptr, class extractor, class routine>
inline void DoForElements(objectptr O, extractor X, routine F, int A)
{
  for(int c = 0; c < A; ++c)
    {
      element E = (O->*X)(c);

      if(E)
        (E->*F)();
    }
}

template <class element, class objectptr, class extractor, class routine, class param>
inline void DoForElements(objectptr O, extractor X, routine F, param P, int A)
{
  for(int c = 0; c < A; ++c)
    {
      element E = (O->*X)(c);

      if(E)
        (E->*F)(P);
    }
}

template <class element, class objectptr, class extractor, class routine>
inline int SumProperties(objectptr O, extractor X, routine F, int A)
{
  int Sum = 0;

  for(int c = 0; c < A; ++c)
    {
      element E = (O->*X)(c);

      if(E)
        Sum += (E->*F)();
    }

  return Sum;
}

template <class element, class objectptr, class extractor, class routine, class param>
inline element FindElement(objectptr O, extractor X, routine F, param P, int A)
{
  for(int c = 0; c < A; ++c)
    {
      element E = (O->*X)(c);

      if(E && (E->*F)(P))
        return E;
    }

  return 0;
}

#endif
