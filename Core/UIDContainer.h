#ifndef UNIQUE_ID_CONTAINER
#define UNIQUE_ID_CONTAINER
#include <array>
#include <cstddef>
#include <iostream>
#include <stdexcept>
namespace Core {
	template<std::size_t N>
	class UIDContainer {
	public:
		UIDContainer() {
			for (std::size_t i = 0; i < N; i++) {
				container[i] = 0;
			}
		}
		/* Retrieves an ID from the container and returns it. */
		std::size_t getUID() {
			for (std::size_t i = 0; i < N; i++) {
				if (container[i] == 0) {
					container[i] += 1;
					return i;
				}
			}
			throw std::out_of_range("Out of IDs");
		}
		void useID(std::size_t id) {
			container[id] += 1;
		}
		/* Puts the ID back into the container. */
		void recycle(std::size_t id) {
			container[id] -= 1;
		}

	private:
		std::array<std::size_t, N> container;
	};
}
#endif