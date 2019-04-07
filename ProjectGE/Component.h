#ifndef COMPONENT_H
#define COMPONENT_H
#include "Serializable.h"
#include "Entity.h"
#include "Handle.h"

#include <array>

typedef std::size_t ComponentTypeID;

#define IMPLEMENT_COMPONENT_TYPE(componentTypeID)								\
public:																			\
	static constexpr ComponentTypeID TYPE_ID = componentTypeID;					\
	virtual const ComponentTypeID getTypeID() const { return TYPE_ID; }			\
	virtual bool equalType(ComponentTypeID typeID) const {						\
		for (const ComponentTypeID& id : equalTypeList) {						\
			if (typeID == id) return true;										\
		}																		\
		return false;															\
	}																	

#define REGISTER_COMPONENT_TYPE_WITH_BASE(componentTypeID, BASE)				\
IMPLEMENT_COMPONENT_TYPE(componentTypeID)										\
protected:																		\
	static constexpr auto equalTypeList = append(BASE::equalTypeList, TYPE_ID);

#define REGISTER_COMPONENT_TYPE(componentTypeID)								\
IMPLEMENT_COMPONENT_TYPE(componentTypeID)										\
protected:																		\
	static constexpr std::array<ComponentTypeID, 1> equalTypeList{ TYPE_ID };

namespace Core {
	template <typename T, std::size_t N, std::size_t... I>
	constexpr std::array<T, N + 1>
		append_aux(std::array<T, N> a, T t, std::index_sequence<I...>) {
		return std::array<T, N + 1>{ t, a[I]... };
	}
	template <typename T, std::size_t N>
	constexpr std::array<T, N + 1> append(std::array<T, N> a, T t) {
		return append_aux(a, t, std::make_index_sequence<N>());
	}
}

namespace Core {
	class Component : public Serializable {
	protected:
		Component();
	public:
		virtual ~Component();

		virtual const ComponentTypeID	getTypeID() const = 0;
		/* Returns true if this component derives from a Component Type with the same typeID as the parameter */
		virtual bool					equalType(ComponentTypeID typeID) const = 0;

		Handle							getOwner();
		void							setOwner(Handle owner);

		virtual Handle*					getParent();
		virtual Handle*					getChild(std::size_t index);
		virtual std::size_t				getChildCount();
		/* Marks the Component for destruction. */
		void							destroy();
		/* Looks if the Component is marked for destruction. */
		bool							isDestroyed();

		// The components that need this will override
		virtual void					init() {};	// Called after setOwner()
		virtual void					end() {};	// Called before component values are copied in a move

		// Serializable
		virtual void					serialize(std::ostream& os) const;
		virtual void					deserialize(std::istream& is);

	protected:
		Handle owner;

	private:
		bool destruct = false;
	};
}

//std::istream& operator>> (std::istream& is, Component::componentTypes& e);
#endif