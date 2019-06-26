#ifndef INPUT_FIELD_H
#define INPUT_FIELD_H

#include "Behaviour.h"
#include "Color.h"
#include "ComponentFunctionHandle.h"
#include <string>
#include <cstddef>
#include <glm/glm.hpp>
#include "InputField.InputField.generated.h"

namespace Core {

	class RectTransform;
	class Text;

	/* A Text Field that is modifiable by the user. */
	CLASS() InputField : public Behaviour {
		GENERATED_BODY()
	public:
		InputField();
		~InputField();


		void awake() override;
		void update(float deltaTime) override;
		void onMouseButtonPressedAsButton(int buttoncode, int mods) override;
		void onMouseButtonPressed(int buttoncode, int mods) override;
		void onMouseDrag(float mouseX, float mouseY) override;

		void setText(std::wstring text);
		void setText(std::string text);

		// Selectable
		void select();
		void deselect();
		
		enum class ContentType {
			Standard,
			Integer,
			Decimal,
			Alphanumeric
		};
	public:
		ContentType contentType = ContentType::Standard;
		PROPERTY()
		std::wstring initText;
		PROPERTY()
		ComponentFunctionHandle<std::wstring> onSubmit;
		PROPERTY()
		std::size_t characterLimit = 255;
		PROPERTY()
		Color markColor = Color(0, 0, 0, 255);
		PROPERTY()
		Color highlightColor = Color(100, 100, 255, 255);

	private:
		void backSpace();
		void deleteText();
		void submit();

		/* Returns true if text was deleted. */
		bool deleteHighlightedText();
		/* Updates the size and position of the text highlight. */
		void updateTextHighlight();

		void showMark();
		void hideMark();

		void showHighlight();
		void hideHighlight();

		/* Moves the selectedTextIndex one step to the left, if possible. */
		void traverseLeft();
		/* Moves the selectedTextIndex one step to the right, if possible. */
		void traverseRight();

		float getXPosOfTextIndex(std::size_t index, RectTransform* textRect, Text* textComponent);
		std::size_t getTextIndexAtPosition(const glm::vec2& position, RectTransform* textRect, Text* textComponent);
		std::size_t getSelectedStartIndex();
		std::size_t getSelectedEndIndex();

		/* Filters the given text to fit the contentType and returns the result. */
		std::wstring filterText(const std::wstring& text);
		bool hasDecimalSymbol(std::wstring text);

	private:
		EntityHandle inputText;
		EntityHandle textMark;
		EntityHandle textHighlight;

		bool selected = false;
		bool markIsHidden = true;
		std::size_t selectedTextIndex;
		std::size_t pressTextIndex;

		float markTime = 0.0f;
		float backspaceTime = 0.0f;
		float deletionTime = 0.0f;
		float leftTime = 0.0f;
		float rightTime = 0.0f;
	};
}
#endif