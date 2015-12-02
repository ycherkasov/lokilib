#pragma once
#include "Typelist.h"

//Reference version

/************************************************************************************
// class template GenData
// Iteratates a Typelist, and invokes the functor GenFunc<T>
// for each type in the list, passing a functor along the way.
// The functor is designed to be an insertion iterator which GenFunc<T>
// can use to output information about the types in the list.
//

Example Use

template<typename T>
struct ExtractDataType
	{
	some_type operator()()
		{
		return create_value_from_type<T>;
		}
	};

Loki::IterateTypes<parameter_tl, ExtractDataType> gendata;
std::vector<some_type> stuff;
gendata(std::back_inserter(stuff));
*******************************************************************************/
namespace Loki
{
	namespace TL
		{
		template<typename T>
		struct nameof_type
			{
			const char* operator()()
				{
				return typeid(T).name();
				}
			};
		template<typename T>
		struct sizeof_type
			{
			size_t operator()()
				{
				return sizeof(T);
				}
			};
    template <class TList, template <class> class GenFunc>
    struct IterateTypes;
     
    template <class T1, class T2, template <class> class GenFunc>
    struct IterateTypes<Typelist<T1, T2>, GenFunc>
    {
    typedef IterateTypes<T1, GenFunc> head_t;
    head_t head;
    typedef IterateTypes<T2, GenFunc> tail_t;
    tail_t tail;
    template<class II>
    void operator()(II ii)
        {
        head.operator()(ii);
		tail.operator()(ii);
		}
    };
     
    template <class AtomicType, template <class> class GenFunc>
    struct IterateTypes
    {
    template<class II>
    void operator()(II ii)
        {
        GenFunc<AtomicType> genfunc;
        *ii = genfunc();
        ++ii; //Is this even needed?
        }
    };
    
    template <template <class> class GenFunc>
    struct IterateTypes<NullType, GenFunc>
    {
    template<class II>
    void operator()(II ii)
        {}
    };
    
	template<typename Types, template <class> class UnitFunc, typename II>
	void iterate_types(II ii)
		{
		Loki::TL::IterateTypes<Types, UnitFunc> it;
		it(ii);
		}
	}//ns TL
}//ns Loki

