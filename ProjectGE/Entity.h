#ifndef ENTITY_H
#define ENTITY_H
#include <cstddef>

namespace Core {
	/* An object with an ID. An entity may be linked with one or more components. */
	class Entity {
	public:
		static constexpr std::size_t INVALID_ID = 0;

		Entity(std::size_t entityID);
		Entity(const Entity& other) {
			id = other.id;
		}
		Entity();
		~Entity();

		void setID(std::size_t entityID);
		std::size_t getID() const;

		bool operator<(const Entity& rhs) const
		{
			return id < rhs.id;
		}
		bool operator==(const Entity& rhs) const
		{
			return id == rhs.id;
		}
	private:
		std::size_t id;
	};
}
#endif