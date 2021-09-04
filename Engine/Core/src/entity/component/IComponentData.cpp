#include "IComponentData.h"
#include <iostream>

using namespace Core;

IComponentData::IComponentData() {}

IComponentData::~IComponentData() {}

IComponentData::IComponentData(const IComponentData& other) {
}

IComponentData& IComponentData::operator=(const IComponentData& other) {
	return *this;
}