#ifndef TEXTURED_SPRITE_H
#define TEXTURED_SPRITE_H
#include "Sprite.h"
#include "Texture2D.h"
#include "Shader.h"
#include "TexturedSprite.generated.h"
namespace Core {
	CLASS() TexturedSprite : public Sprite {
		GENERATED_BODY()
	public:
		TexturedSprite();
		TexturedSprite(Texture2D texture, Color color = { 255,255,255,255 });
		TexturedSprite(Texture2D texture, Shader shader, Color color = { 255,255,255,255 });
		virtual ~TexturedSprite();

		void setTexture(Texture2D texture);
		const Texture2D& getTexture() const;

		void setShader(Shader shader);
		const Shader& getShader() const;

	private:
		PROPERTY()
		Texture2D texture;
		PROPERTY()
		Shader shader;
	};
}
#endif