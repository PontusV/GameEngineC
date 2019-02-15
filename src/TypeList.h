#ifndef TYPE_LIST_H
#define TYPE_LIST_H

namespace GameEngine {
	template <typename... Ts> struct TypeList {
		typedef TypeList<Ts...> type;
	};
	template <> struct TypeList<> {}; //Empty
	template <typename... Ts, typename T>
	struct TypeList<TypeList<Ts...>, T> {
		typedef TypeList<Ts..., T> type;
	};
}

#define TYPE_LIST GameEngine::TypeList<>
#endif