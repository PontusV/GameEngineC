#ifndef BATCH_CONFIG_H
#define BATCH_CONFIG_H

#include "glad/glad.h"
#include <vector>

#define MAX_TEXTURE_IDS 32

namespace Core {
	class ConstBatchConfig {
	public:
		ConstBatchConfig(const float& rotation, const GLuint& textureID, const GLuint& shaderID, bool clipEnabled, const glm::vec4& drawRect) : rotation(rotation), textureID(textureID), shaderID(shaderID), clipEnabled(clipEnabled), drawRect(drawRect) {}

		const float&		rotation;
		const GLuint&		textureID;
		const GLuint&		shaderID;
		bool				clipEnabled;
		const glm::vec4&	drawRect;
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
			if (clipEnabled) {
				if (!other.clipEnabled) return false;
				return (rotation == other.rotation && shaderID == other.shaderID && drawRect == other.drawRect);
			}
			else {
				if (other.clipEnabled) return false;
				return (rotation == other.rotation && shaderID == other.shaderID);
			}
		}

		BatchConfig(const float& rotation = 0, const GLuint& textureID = 0, const GLuint& shaderID = 0) : rotation(rotation), shaderID(shaderID) {
			textureIDs.reserve(MAX_TEXTURE_IDS);
		}
		BatchConfig(const ConstBatchConfig& other) {
			textureIDs.push_back(other.textureID);
			rotation	= other.rotation;
			shaderID	= other.shaderID;
			clipEnabled	= other.clipEnabled;
			drawRect	= other.drawRect;
		}

		inline std::size_t getTextureSlot(const GLuint& id) {
			for (std::size_t i = 0; i < textureIDs.size();i++) {
				if (textureIDs[i] == id)
					return i+1;
			}
			throw std::invalid_argument("This TextureID is not contained in the TextureSlot array (textureIDs).");
		}

		float				rotation;
		GLuint				shaderID;
		glm::vec4			drawRect;
		bool				clipEnabled;
		std::vector<GLuint>	textureIDs;
	};
}
#endif