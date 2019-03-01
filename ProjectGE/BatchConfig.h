#ifndef BATCH_CONFIG_H
#define BATCH_CONFIG_H

#include "glad/glad.h"
#include <vector>

#define MAX_TEXTURE_IDS 32

namespace GameEngine {
	class ConstBatchConfig {
	public:
		ConstBatchConfig(const int layer, const float& rotation, const GLuint& textureID, const GLuint& shaderID) : layer(layer), rotation(rotation), textureID(textureID), shaderID(shaderID) {}

		const int layer;
		const float& rotation;
		const GLuint& textureID;
		const GLuint& shaderID;
	};

	class BatchConfig {
	public:
		inline bool compatible(const ConstBatchConfig& other) {
			if (equal(other)) {
				if (other.textureID == 0) //No texture
					return true;
				for (GLuint& textureID : textureIDs) {
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
			return (layer == other.layer && rotation == other.rotation && shaderID == other.shaderID);
		}

		BatchConfig(const int layer = 0, const float& rotation = 0, const GLuint& textureID = 0, const GLuint& shaderID = 0) : layer(layer), rotation(rotation), shaderID(shaderID) {
			textureIDs.reserve(MAX_TEXTURE_IDS);
		}
		BatchConfig(const ConstBatchConfig& other) {
			layer = other.layer;
			rotation = other.rotation;
			textureIDs.push_back(other.textureID);
			shaderID = other.shaderID;
		}

		inline std::size_t getTextureSlot(GLuint& id) {
			for (std::size_t i = 0; i < textureIDs.size();i++) {
				if (textureIDs[i] == id)
					return i+1;
			}
			throw std::invalid_argument("This TextureID is not contained in the TextureSlot array (textureIDs).");
		}

		int layer;
		float rotation;
		std::vector<GLuint> textureIDs;
		GLuint shaderID;
	};
}
#endif