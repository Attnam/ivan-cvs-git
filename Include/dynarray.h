#ifndef __DYNARRAY_H__
#define __DYNARRAY_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

template <class Type, class SizeType = ushort> class dynarray
{
public:
	dynarray();
	dynarray(const dynarray<Type, SizeType>*);
	~dynarray();
	void Add(Type);
	void Add(const dynarray<Type, SizeType>*);
	void Put(Type, SizeType);
	Type Remove(SizeType);
	Type& Access(SizeType) const;
	SizeType Length() const;
	Type& operator[](SizeType) const;
	Type& operator<<(Type&);
	SizeType Search(Type) const;
	void Resize(SizeType);
private:
	Type* Data;
	SizeType Elements;
};

template <class Type, class SizeType> inline dynarray<Type, SizeType>::dynarray() : Data(0), Elements(0)
{
}

template <class Type, class SizeType> inline dynarray<Type, SizeType>::dynarray(const dynarray<Type, SizeType>* Array) : Data(Array->Data), Elements(Array->Elements)
{
}

template <class Type, class SizeType> inline dynarray<Type, SizeType>::~dynarray()
{
	delete [] Data;
}

template <class Type, class SizeType> inline Type& dynarray<Type, SizeType>::Access(SizeType Index) const
{
	return Data[Index];
}

template <class Type, class SizeType> inline SizeType dynarray<Type, SizeType>::Length() const
{
	return Elements;
}

template <class Type, class SizeType> inline void dynarray<Type, SizeType>::Add(Type Element)
{
	Type* TempData = new Type[Elements + 1];

	for(SizeType c = 0; c < Elements; ++c)
		TempData[c] = Data[c];

	delete [] Data;

	Data = TempData;

	Data[Elements] = Element;

	++Elements;
}

template <class Type, class SizeType> inline void dynarray<Type, SizeType>::Add(const dynarray<Type, SizeType>* DynArray)
{
	SizeType NewElements = Elements + DynArray->Elements;

	Type* TempData = new Type[NewElements];

	for(SizeType c = 0; c < Elements; ++c)
		TempData[c] = Data[c];

	delete [] Data;

	Data = TempData;

	for(SizeType c = Elements; c < NewElements; ++c)
		Data[c] = DynArray->Data[c];

	Elements = NewElements;
}

template <class Type, class SizeType> inline void dynarray<Type, SizeType>::Put(Type Element, SizeType Position)
{
	Type* TempData = new Type[Elements + 1];

	{
	for(SizeType c = 0; c < Position; ++c)
		TempData[c] = Data[c];
	}

	TempData[Position] = Element;

	for(SizeType c = Position; c < Elements; ++c)
		TempData[c + 1] = Data[c];

	delete [] Data;

	Data = TempData;

	++Elements;
}

template <class Type, class SizeType> inline Type dynarray<Type, SizeType>::Remove(SizeType Index)
{
	Type Backup = Data[Index];

	Type* TempData;

	if(Elements - 1)
	{
		TempData = new Type[Elements - 1];

		{
			for(SizeType c = 0; c < Index; ++c)
				TempData[c] = Data[c];
		}

		for(SizeType c = Index + 1; c < Elements; ++c)
			TempData[c - 1] = Data[c];
	}
	else
		TempData = 0;

	delete [] Data;

	Data = TempData;

	Elements--;

	return Backup;
}

template <class Type, class SizeType> inline Type& dynarray<Type, SizeType>::operator[](SizeType Index) const
{
	return Access(Index);
}

template <class Type, class SizeType> inline Type& dynarray<Type, SizeType>::operator<<(Type& Element)
{
	Add(Element);

	return Element;
}

template <class Type, class SizeType> inline SizeType dynarray<Type, SizeType>::Search(Type Element) const
{
	for(SizeType c = 0; c < Elements; ++c)
		if(Data[c] == Element)
			return c;

	return -1;
}

template <class Type, class SizeType> inline void dynarray<Type, SizeType>::Resize(SizeType NewSize)
{
	if(NewSize < Elements)
	{
		Type* TempData = new Type[NewSize];

		for(SizeType c = 0; c < NewSize; ++c)
			TempData[c] = Data[c];

		delete [] Data;

		Data = TempData;

		Elements = NewSize;
	}
}

#endif
