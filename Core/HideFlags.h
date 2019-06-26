#ifndef HIDE_FLAGS_H
#define HIDE_FLAGS_H
namespace Core {
	enum class HideFlags {
		None				= 0x01,
		HideInHierarchy		= 0x02,
		HideInInspector		= 0x04,
		DontSaveInEditor	= 0x08,
		NotEditable			= 0x10,
		DontSaveInBuild		= 0x20,
		DontSave			= 0x40,
		HideAndDontSave		= 0x80
	};
}

inline bool operator==(Core::HideFlags left, Core::HideFlags right) {
	return static_cast<int>(left)& static_cast<int>(right);
}
inline bool operator!=(Core::HideFlags left, Core::HideFlags right) {
	return !(static_cast<int>(left) & static_cast<int>(right));
}
inline Core::HideFlags operator|(Core::HideFlags left, Core::HideFlags right) {
	return static_cast<Core::HideFlags>(static_cast<int>(left) | static_cast<int>(right));
}
#endif