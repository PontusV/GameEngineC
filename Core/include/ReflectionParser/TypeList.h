#ifndef TYPE_LIST_H
#define TYPE_LIST_H

namespace Mirror {
	template <typename... Ts> struct TypeList {
		typedef TypeList<Ts...> type;
	};
	template <> struct TypeList<> {}; //Empty
	template <typename... Ts, typename T>
	struct TypeList<TypeList<Ts...>, T> {
		typedef TypeList<Ts..., T> type;
	};
}
#endif