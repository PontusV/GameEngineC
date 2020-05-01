#include "InputField.h"
#include "input/Input.h"
#include "components/graphics/Text.h"
#include "components/RectTransform.h"
#include "components/graphics/RectSprite.h"
#include "engine/ResourceManager.h"
#include "utils/Clipboard.h"
#include "maths/MatrixTransform.h"
#include <GLFW/glfw3.h>

using namespace Core;

InputField::InputField()
{
}


InputField::~InputField()
{
}


void InputField::start() {
	RectTransform* rect = owner.getComponent<RectTransform>();
	EntityHandle ownerHandle = owner;
	if (rect) {
		inputText = createEntity(ownerHandle.getEntityName() + "_Text");
		inputText.setParent(owner);

		RectTransform* textRect = inputText.addComponent(RectTransform(rect->getSize().x / 2, rect->getSize().y / 2, 0, 0, rect->getZ() + 0.01f, Alignment::CENTER));
		Text* textComponent = inputText.addComponent(Text(filterText(initText), "resources/fonts/segoeui.ttf", 15, Color(0, 0, 0)));

		textMark = createEntity(ownerHandle.getEntityName() + "_Mark",
			RectSprite(Color(markColor.r, markColor.g, markColor.b, 0)),
			RectTransform(0, 0, 0, 0, textRect->getZ(), Alignment::LEFT)
		);
		textMark.setEntityHideFlags(HideFlags::HideInInspector | HideFlags::HideAndDontSave);
		textMark.setParent(inputText);

		textHighlight = createEntity(ownerHandle.getEntityName() + "_Highlight",
			RectSprite(Color(highlightColor.r, highlightColor.g, highlightColor.b, 0)),
			RectTransform(0, 0, 0, 0, textRect->getZ(), Alignment::LEFT)
		);
		textHighlight.setEntityHideFlags(HideFlags::HideInInspector | HideFlags::HideAndDontSave);
		textHighlight.setParent(inputText);
	}
}

void InputField::onDestroy() {
	destroyEntity(inputText);
	destroyEntity(textMark);
	destroyEntity(textHighlight);
}

void InputField::submit() {
	Text* textComponent = inputText.getComponent<Text>();
	std::wstring input = textComponent->getText();
	textComponent->setText(input);
	onSubmit.invoke(input);
}

void InputField::onMouseDrag(float mouseX, float mouseY) {
	// Calculate selected text index
	RectTransform* textRect = inputText.getComponent<RectTransform>();
	Text* textComponent = inputText.getComponent<Text>();
	selectedTextIndex = getTextIndexAtPosition(input->getMousePosition(), textRect, textComponent);
	updateTextHighlight();
}

void InputField::update(float deltaTime) {
	static constexpr float typeInitDelay = 0.5f;
	static constexpr float typeDelay = 0.05f;
	static constexpr float markBlinkTime = 1.5f;

	if (!selected) return;
	// TODO: Home & End. Shift + Home & Shift + End
	// Select all (ctrl + a)
	if (input->getKeyDown(GLFW_KEY_LEFT_CONTROL) && input->getKeyPressed(GLFW_KEY_A)) {
		selectAll();
	}
	else if (input->getKeyDown(GLFW_KEY_LEFT_CONTROL) && input->getKeyPressed(GLFW_KEY_C)) {
		std::wstring copy = getHighlightedText();
		if (!copy.empty())
			Clipboard::copy(copy);
	}
	else if (input->getKeyDown(GLFW_KEY_LEFT_CONTROL) && input->getKeyPressed(GLFW_KEY_V)) {
		std::wstring paste = Clipboard::paste();
		std::wcout << paste << std::endl;
		write(paste);
	}
	else if (input->getKeyDown(GLFW_KEY_LEFT_CONTROL) && input->getKeyPressed(GLFW_KEY_X)) {
		std::wstring copy = getHighlightedText();
		if (!copy.empty()) {
			Clipboard::copy(copy);
			deleteHighlightedText();
		}
	}

	// Traversal with arrow-keys
	if (input->getKeyPressed(GLFW_KEY_LEFT)) {
		leftTime = typeInitDelay;
		traverseLeft();
	}
	if (input->getKeyDown(GLFW_KEY_LEFT)) {
		if (leftTime <= 0) {
			leftTime += typeDelay;
			traverseLeft();
		}
		leftTime -= deltaTime;
	}
	if (input->getKeyPressed(GLFW_KEY_RIGHT)) {
		rightTime = typeInitDelay;
		traverseRight();
	}
	if (input->getKeyDown(GLFW_KEY_RIGHT)) {
		if (rightTime <= 0) {
			rightTime += typeDelay;
			traverseRight();
		}
		rightTime -= deltaTime;
	}

	// Delete deletion
	if (input->getKeyPressed(GLFW_KEY_DELETE)) {
		deleteText();
		deletionTime = typeInitDelay;
	}
	if (input->getKeyDown(GLFW_KEY_DELETE)) {
		if (deletionTime <= 0) {
			deleteText();
			deletionTime += typeDelay;
		}
		deletionTime -= deltaTime;
	}

	// Backspace deletion
	if (input->getKeyPressed(GLFW_KEY_BACKSPACE)) {
		backSpace();
		backspaceTime = typeInitDelay;
	}
	if (input->getKeyDown(GLFW_KEY_BACKSPACE)) {
		if (backspaceTime <= 0) {
			backSpace();
			backspaceTime += typeDelay;
		}
		backspaceTime -= deltaTime;
	}

	if (input->getKeyPressed(GLFW_KEY_ENTER)) {
		submit();
	}

	// Text Mark (Blinking)
	markTime += deltaTime;
	if (markTime > markBlinkTime) markTime = 0.0f;
	if (markTime < markBlinkTime / 2 && selectedTextIndex == pressTextIndex) {
		// Show
		if (markIsHidden)
			showMark();
		// Update position
		float markXPos = getXPosOfTextIndex(selectedTextIndex, inputText.getComponent<RectTransform>(), inputText.getComponent<Text>());
		textMark.getComponent<RectTransform>()->setLocalPosition(Vector2(markXPos, 0));
	}
	else {
		// Hide
		if (!markIsHidden)
			hideMark();
	}
}

void InputField::onSelect() {
	selected = true;
	pressTextIndex = getTextIndexAtPosition(input->getMousePosition(), inputText.getComponent<RectTransform>(), inputText.getComponent<Text>());
	selectedTextIndex = pressTextIndex;
	markTime = 0.0f;
	showMark();
	showHighlight();
	input->focusKeyboard(TComponentHandle<InputField>(this));
}

void InputField::onDeselect() {
	selected = false;
	pressTextIndex = 0;
	selectedTextIndex = 0;
	hideMark();
	hideHighlight();
	input->clearFocusKeyboard();
}

bool InputField::deleteHighlightedText() {
	Text* textComponent = inputText.getComponent<Text>();
	if (!textComponent) return false;

	std::size_t startIndex = getSelectedStartIndex();
	std::size_t endIndex = getSelectedEndIndex();
	if (startIndex != endIndex) {
		std::wstring currentText = textComponent->getText();
		std::wstring newText = currentText.substr(0, startIndex);
		if (endIndex < currentText.size())
			newText += currentText.substr(endIndex, currentText.size() - endIndex);
		// Update variables
		textComponent->setText(newText);
		selectedTextIndex = startIndex;
		pressTextIndex = selectedTextIndex;
		updateTextHighlight();
		return true;
	}
	return false;
}

void InputField::backSpace() {
	Text* textComponent = inputText.getComponent<Text>();
	if (!textComponent) return;

	std::wstring currentText = textComponent->getText();
	if (currentText.size() > 0) {
		std::wstring newText = L"";
		if (!deleteHighlightedText() && selectedTextIndex > 0) {
			// Delete at selected area
			newText = currentText.substr(0, selectedTextIndex - 1);
			if (selectedTextIndex < currentText.size())
				newText += currentText.substr(selectedTextIndex, currentText.size() - selectedTextIndex);
			selectedTextIndex--;
			// Update variables
			textComponent->setText(newText);
			if (selectedTextIndex > newText.size())
				selectedTextIndex = newText.size();
			pressTextIndex = selectedTextIndex;
			updateTextHighlight();
			onChange.invoke(newText);
		}
	}
}

void InputField::deleteText() {
	Text* textComponent = inputText.getComponent<Text>();
	if (!textComponent) return;

	std::wstring currentText = textComponent->getText();
	if (currentText.size() > 0) {
		std::wstring newText = L"";
		if (!deleteHighlightedText() && selectedTextIndex < currentText.size()) {
			// Delete at selected area
			newText = currentText.substr(0, selectedTextIndex);
			if (selectedTextIndex < currentText.size())
				newText += currentText.substr(selectedTextIndex + 1, currentText.size() - selectedTextIndex);
			// Update variables
			textComponent->setText(newText);
			if (selectedTextIndex > newText.size())
				selectedTextIndex = newText.size();
			pressTextIndex = selectedTextIndex;
			updateTextHighlight();
			onChange.invoke(newText);
		}
	}
}

std::size_t InputField::getTextIndexAtPosition(const Vector2& position, RectTransform * textRect, Text * textComponent) {
	Vector2 localPos = textRect->getWorldToLocalMatrix() * position - textRect->getLocalPosition().x + textComponent->getSize().x * textRect->getPivot().x;
	TextData2D data = ResourceManager::getInstance().createText(textComponent->getText(), textComponent->getFont());
	std::size_t index = 0;
	for (const CharTexture2D& texture : data.textures) {
		if (localPos.x <= texture.offset.x) return index;
		index++;
	}
	return index; // Returns end of text
}

float InputField::getXPosOfTextIndex(std::size_t index, RectTransform * textRect, Text * textComponent) {
	TextData2D data = ResourceManager::getInstance().createText(textComponent->getText(), textComponent->getFont());
	std::size_t i = 0;
	if (index < data.textures.size()) {
		return data.textures[index].offset.x
			- textComponent->getSize().x * textRect->getPivot().x;
	}
	else if (index >= data.textures.size() && data.textures.size() > 0) {
		return data.textures[data.textures.size() - 1].offset.x + data.textures[data.textures.size() - 1].texture.size.x
			- textComponent->getSize().x * textRect->getPivot().x;
	}
	else {
		return -textComponent->getSize().x * textRect->getPivot().x;
	}
}

void InputField::updateTextHighlight() {
	RectTransform* textRect = inputText.getComponent<RectTransform>();
	Text* textComponent = inputText.getComponent<Text>();
	if (textRect == nullptr || textComponent == nullptr) return;

	// Calculate Highlight size & position
	std::size_t startIndex = getSelectedStartIndex();
	std::size_t endIndex = getSelectedEndIndex();
	float startX = getXPosOfTextIndex(startIndex, textRect, textComponent);
	float endX = getXPosOfTextIndex(endIndex, textRect, textComponent);
	// Assign new values to Highlight entity
	RectTransform * hRect = textHighlight.getComponent<RectTransform>();
	hRect->setLocalPosition(Vector2(startX, 0));
	hRect->setSize(Vector2(endX - startX, textComponent->getSize().y + 4));
}

void InputField::traverseLeft() {
	if (selectedTextIndex > 0) {
		selectedTextIndex--;
	}
	if (!input->getKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		pressTextIndex = selectedTextIndex;
	}
	updateTextHighlight();
	markTime = 0.0f;
}

void InputField::traverseRight() {
	std::wstring text = inputText.getComponent<Text>()->getText();
	if (selectedTextIndex < text.size()) {
		selectedTextIndex++;
	}
	if (!input->getKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		pressTextIndex = selectedTextIndex;
	}
	updateTextHighlight();
	markTime = 0.0f;
}

std::size_t InputField::getSelectedStartIndex() {
	return pressTextIndex > selectedTextIndex ? selectedTextIndex : pressTextIndex;
}

std::size_t InputField::getSelectedEndIndex() {
	return pressTextIndex < selectedTextIndex ? selectedTextIndex : pressTextIndex;
}

void InputField::showMark() {
	textMark.getComponent<RectSprite>()->setColor(markColor);
	textMark.getComponent<RectTransform>()->setSize(Vector2(1, inputText.getComponent<Text>()->getSize().y + 4));
	markIsHidden = false;
}

void InputField::hideMark() {
	textMark.getComponent<RectSprite>()->setColor(Color(markColor.r, markColor.g, markColor.b, 0));
	textMark.getComponent<RectTransform>()->setSize(Vector2(0, 0));
	markIsHidden = true;
}

void InputField::showHighlight() {
	textHighlight.getComponent<RectSprite>()->setColor(highlightColor);
}

void InputField::hideHighlight() {
	textHighlight.getComponent<RectSprite>()->setColor(Color(highlightColor.r, highlightColor.g, highlightColor.b, 0));
	textHighlight.getComponent<RectTransform>()->setSize(Vector2(0, 0));
}

void InputField::setText(std::wstring text) {
	Text* textComponent = inputText.getComponent<Text>();
	if (textComponent) {
		textComponent->setText(text);
		if (selected) {
			if (selectedTextIndex > text.size())
				selectedTextIndex = text.size();
			pressTextIndex = selectedTextIndex;
			updateTextHighlight();
		}
	}
	else {
		initText = text;
	}
}

void InputField::setText(std::string text) {
	setText(std::wstring(text.begin(), text.end()));
}

std::wstring InputField::filterText(const std::wstring& text) {
	if (contentType == ContentType::Standard) return text;
	std::wstring newText;
	bool decimal = false;
	for (const wchar_t& chr : text) {
		if (contentType == ContentType::Integer) {
			if (isdigit(chr)) { // Range between 0-9
				newText += chr;
			}
		}
		else if (contentType == ContentType::Decimal) {
			if (chr == '.') {
				if (decimal) continue;
				decimal = true;
				newText += chr;
			}
			else if (isdigit(chr)) { // Range between 0-9
				newText += chr;
			}
		}
		else if (contentType == ContentType::Alphanumeric) {
			if (isalnum(chr)) { // Range between 0-9 A-Z
				newText += chr;
			}
		}
	}
	return newText;
}

void InputField::onDoubleClick() {
	selectAll();
}

void InputField::selectAll() {
	std::wstring currentText = inputText.getComponent<Text>()->getText();
	pressTextIndex = 0;
	selectedTextIndex = currentText.size();
	updateTextHighlight();
}

void InputField::write(std::wstring input) {
	if (!input.empty()) {
		Text* textComponent = inputText.getComponent<Text>();
		if (textComponent) {
			deleteHighlightedText(); // Deletes any marked text
			std::wstring currentText = textComponent->getText();

			if (contentType == ContentType::Decimal && currentText.find('.') != std::wstring::npos) {
				input.erase(std::remove(input.begin(), input.end(), '.'), input.end());
			}
			if (!input.empty()) {
				std::wstring beginText = currentText.substr(0, selectedTextIndex);
				std::wstring endText = L"";
				if (selectedTextIndex < currentText.size())
					endText = currentText.substr(selectedTextIndex, currentText.size() - selectedTextIndex);
				std::wstring newText = filterText(beginText + input + endText);

				if (characterLimit != 0 && characterLimit < currentText.size()) {
					newText = newText.substr(0, characterLimit);
				}

				textComponent->setText(newText);
				selectedTextIndex += input.size();
				pressTextIndex = selectedTextIndex;
				onChange.invoke(newText);
			}
		}
	}
}

std::wstring InputField::getText() {
	return inputText.getComponent<Text>()->getText();
}

std::wstring InputField::getHighlightedText() {
	std::size_t start = getSelectedStartIndex();
	std::size_t end = getSelectedEndIndex();
	return getText().substr(start, end - start);
}