#include "Serializer.h"

namespace Core {
	namespace details {
		template<>
		void serialize<std::string>(const std::string& value, SerializationDetails& details) {
			std::size_t size = value.size();
			serialize(size, details);
			details.os.write(value.c_str(), sizeof(char) * size);
		}

		template<>
		void serialize<std::wstring>(const std::wstring& value, SerializationDetails& details) {
			std::size_t size = value.size();
			serialize(size, details);
			details.os.write((char*)value.c_str(), sizeof(wchar_t) * size);
		}

		template<>
		void details::serialize<Mirror::Property>(const Mirror::Property& value, SerializationDetails& details) {
			serialize(value.name, details);
			serialize(value.type.name, details);
			serialize(value.type.size, details);
		}

		template<>
		void serialize<Entity>(const Entity& value, SerializationDetails& details) {
			std::size_t entityID = value.getID();
			if (details.entityRemapInfo) {
				auto it = std::find_if(details.entityRemapInfo->begin(), details.entityRemapInfo->end(), [&entityID](const std::pair<std::size_t, std::string>& pair) { return pair.first == entityID; });
				if (it != details.entityRemapInfo->end()) {
					bool entityMapped = true;
					serialize(entityMapped, details);
					std::string guid = it->second;
					serialize(guid, details);
				}
				else {
					bool entityMapped = false;
					serialize(entityMapped, details);
					serialize(entityID, details);
					//std::cout << "Serializer::serialize::ERROR Unable to serialize Entity due to missing remap for Entity with ID: " << value.getID() << std::endl;
					//throw std::invalid_argument("Serializer::serialize::ERROR Unable to serialize Entity due to missing remap");
				}
			}
			else {
				bool entityMapped = false;
				serialize(entityMapped, details);
				serialize(entityID, details);
				//std::cout << "Serializer::serialize::ERROR Unable to serialize Entity without EntityRemapSaveInfo" << std::endl;
				//throw std::invalid_argument("Serializer::serialize::ERROR Unable to serialize Entity without EntityRemapSaveInfo");
			}
		}

		template<>
		void serialize<ComponentHandle>(const ComponentHandle& value, SerializationDetails& details) {
			serialize(value.getOwner(), details);
			// Serializes name of component instead of typeID
			std::string typeName = Mirror::getName(value.getComponentTypeID());
			serialize(typeName, details);
		}

		template<>
		void deserialize<std::string>(std::string& value, DeserializationDetails& details) {
			std::size_t size;
			deserialize(size, details);
			value.resize(size);
			details.is.read(value.data(), sizeof(char) * size);
		}

		template<>
		void deserialize<std::wstring>(std::wstring& value, DeserializationDetails& details) {
			std::size_t size;
			deserialize(size, details);
			wchar_t* buffer = new wchar_t[size];
			details.is.read((char*)buffer, sizeof(wchar_t) * size);
			value = buffer;
			delete[] buffer;
		}

		template<>
		void deserialize<Mirror::Property>(Mirror::Property& value, DeserializationDetails& details) {
			deserialize(value.name, details);
			deserialize(value.type.name, details);
			deserialize(value.type.size, details);
		}

		template<>
		void deserialize<Entity>(Entity& value, DeserializationDetails& details) {
			bool entityMapped;
			deserialize(entityMapped, details);
			if (!entityMapped) {
				std::size_t entityID;
				deserialize(entityID, details);
				value.setID(entityID);
				return;
			}
			std::string entityGUID;
			deserialize(entityGUID, details);
			if (details.entityRemapInfo) {
				auto it = std::find_if(details.entityRemapInfo->begin(), details.entityRemapInfo->end(), [&entityGUID](const std::pair<std::string, std::size_t>& pair) { return pair.first == entityGUID; });
				if (it != details.entityRemapInfo->end()) {
					std::size_t entityID = it->second;
					value.setID(entityID);
				}
				else {
					value.setID(0);
					//std::cout << "Serializer::deserialize::ERROR Failed to load Entity with GUID: '" << entityGUID << "'. Expected remap but none was found for the Entity" << std::endl;
					//throw std::invalid_argument("Serializer::deserialize::ERROR Failed to load Entity due to missing remap in EntityRemapLoadInfo");
				}
			}
			else {
				std::cout << "Serializer::deserialize::ERROR Unable to deserialize Entity without EntityRemapLoadInfo" << std::endl;
				throw std::invalid_argument("Serializer::serialize::ERROR Unable to serialize Entity without EntityRemapLoadInfo");
			}
		}

		template<>
		void deserialize<ComponentHandle>(ComponentHandle& value, DeserializationDetails& details) {
			Handle handle;
			std::string typeName;
			deserialize(handle, details);
			deserialize(typeName, details);
			std::size_t typeID = Mirror::getTypeID(typeName);

			value.setOwner(handle);
			value.setComponentTypeID(typeID);
		}

		void skipPropertiesImpl(DeserializationDetails& details) {
			std::size_t count;
			details::deserialize(count, details);
			for (std::size_t i = 0; i < count; i++) {
				details::SerializedPropertyDetails prop;
				details::deserialize(prop.name, details);
				details::deserialize(prop.typeName, details);
				details::deserialize(prop.typeSize, details);
				details::deserialize(prop.dataSize, details);
				details.is.seekg(prop.dataSize, std::ios::cur);
			}
		}

		void skipProperties(details::DeserializationDetails& details) {
			details::skipPropertiesImpl(details);
			std::size_t continueCount;
			details::deserialize(continueCount, details);
			for (std::size_t i = 0; i < continueCount; i++) {
				std::string name = std::string();
				details::deserialize(name, details);
				skipProperties(details);
			}
		}
	}
}