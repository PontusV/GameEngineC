#ifndef INPUT_FIELD_H
#define INPUT_FIELD_H

#include "Selectable.h"
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
	CLASS() InputField : public Selectable {
		GENERATED_BODY()
	public:
		InputField();
		~InputField();


		void awake() override;
		void update(float deltaTime) override;
		void onSelect() override;
		void onDeselect() override;
		void onMouseDrag(float mouseX, float mouseY) override;
		void onDoubleClick() override;

		void setText(std::wstring text);
		void setText(std::string text);

		std::wstring getText();
		std::wstring getHighlightedText();
		
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
		/* Calls the assigned onSubmit. */
		void submit();
		/* Moves the selectedTextIndex one step to the left, if possible. */
		void traverseLeft();
		/* Moves the selectedTextIndex one step to the right, if possible. */
		void traverseRight();
		/* Highlight all text inside the inputField. */
		void selectAll();
		/* Writes text at the selected index. */
		void write(std::wstring input);

		/* Returns true if text was deleted. */
		bool deleteHighlightedText();
		/* Updates the size and position of the text highlight. */
		void updateTextHighlight();

		void showMark();
		void hideMark();

		void showHighlight();
		void hideHighlight();

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