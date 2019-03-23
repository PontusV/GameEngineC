#ifndef HANLDE_H
#define HANLDE_H

#include <stdint.h>
#include <tuple>

namespace Core {
	class Handle {
	public:
		Handle();
		Handle(uint32_t index, uint32_t counter, uint32_t type);
		Handle(uint32_t values);
		~Handle();

		const uint32_t getIndex() const;
		const uint32_t getCounter() const; //How many different things this handle has identified (used to see if handle has been changed to point to something else)
		const uint32_t getType() const;

		void addToCounter();

		operator uint32_t() const;

		bool operator <(const Handle& rhs) const
		{
			return std::tie(index, counter, type) < std::tie(rhs.index, rhs.counter, rhs.type);
		}
	private:
		uint32_t index : 12;	//Index of array holding HandleEntrys
		uint32_t counter : 15;	//Defines how many times the HandleEntry pointed by this object has been used
		uint32_t type : 5;		//Defines what type of object the HandleEntry is connected to
	};
}
#endif