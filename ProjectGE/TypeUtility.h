#ifndef TYPE_UTILITY_H
#define TYPE_UTILITY_H
#include "TypeList.h"
#include <type_traits>

namespace GameEngine {
	namespace util {
		template <typename T, typename TList, typename Res = TypeList<>>
		struct removeT_helper;

		template<typename T, typename Res>
		struct removeT_helper<T, TypeList<>, Res> {
			using type = Res;
		};

		template<typename T, typename... Ts, typename... TRes>
		struct removeT_helper<T, TypeList<T, Ts...>, TypeList<TRes...>> :
			removeT_helper<T, TypeList<Ts...>, TypeList<TRes...>>
		{};

		template<typename T, typename T1, typename ...Ts, typename... TRes>
		struct removeT_helper<T, TypeList<T1, Ts...>, TypeList<TRes...>> :
			removeT_helper<T, TypeList<Ts...>, TypeList<TRes..., T1>>
		{};

		template <typename T, typename...Ts> struct RemoveT {
			using type = typename removeT_helper<T, TypeList<Ts...>>::type;
		};
		//----

		template <typename T, typename T2>
		struct is_not_same {
			static constexpr bool value = !std::is_same<T, T2>::value;
		};

		template <typename...> struct uniqueTypes;
		template <>
		struct uniqueTypes<> {
			static constexpr bool value = true;
		};
		template <typename T>
		struct uniqueTypes<T> {
			static constexpr bool value = true;
		};
		template <typename T, typename... Ts>
		struct uniqueTypes<T, Ts...> {
			static constexpr bool value = std::conjunction<uniqueTypes<Ts...>, is_not_same<T, Ts>...>::value;
		};
	}
}
#endif