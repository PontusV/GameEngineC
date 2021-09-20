#ifndef BATCH_CONFIG_H
#define BATCH_CONFIG_H

#include <vector>
#include <stdexcept>

#define MAX_TEXTURE_IDS 32

namespace Core {
	class ConstBatchConfig {
	public:
		ConstBatchConfig(const unsigned int& textureID, const unsigned int& shaderID) : textureID(textureID), shaderID(shaderID) {}

		const unsigned int&	textureID;
		const unsigned int&	shaderID;
	};

	class BatchConfig {
	public:
		inline bool compatible(const ConstBatchConfig& other) {
			if (equal(other)) {
				if (other.textureID == 0) //No texture
					return true;
				for (unsigned int& textureID : textureIDs) {
					if (textureID == other.textureID)
						return true;
				}
				if (textureIDs.size() < MAX_TEXTURE_IDS) {
					textureIDs.push_back(other.textureID);
					return true;
				}
			}
			return false;
		}

		inline bool equal(const ConstBatchConfig& other) {
			return (shaderID == other.shaderID);
		}

		BatchConfig(const unsigned int& textureID = 0, const unsigned int& shaderID = 0) : shaderID(shaderID) {
			textureIDs.reserve(MAX_TEXTURE_IDS);
		}
		BatchConfig(const ConstBatchConfig& other) {
			textureIDs.push_back(other.textureID);
			shaderID = other.shaderID;
		}

		inline std::size_t getTextureSlot(const unsigned int& id) {
			for (std::size_t i = 0; i < textureIDs.size();i++) {
				if (textureIDs[i] == id)
					return i+1;
			}
			//std::cout << "BatchConfig::getTextureSlot::ERROR This TextureID is not contained in the TextureSlot array (textureIDs)." << std::endl;
			throw std::invalid_argument("BatchConfig::getTextureSlot::ERROR This TextureID is not contained in the TextureSlot array (textureIDs).");
		}

		unsigned int				shaderID;
		std::vector<unsigned int>	textureIDs;
	};
}
#endif