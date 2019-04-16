#ifndef INTERACTABLE_H
#define INTERACTABLE_H

namespace Core {
	namespace ui {
		typedef void(*LeftClickPressFun)();
		typedef void(*LeftClickReleaseFun)();
		typedef void(*RightClickPressFun)();
		typedef void(*RightClickReleaseFun)();
		typedef void(*MiddleClickPressFun)();
		typedef void(*MiddleClickReleaseFun)();
		typedef void(*DoubleClickFun)();
		typedef void(*HoverOverFun)();
		typedef void(*HoverOutFun)();
		typedef void(*ScrollFun)(float, float);

		class Interactable {
		protected:
			Interactable() {
				// These function pointers are not always nullptr by default for some reason.
				leftClickPressFun		= nullptr;
				leftClickReleaseFun		= nullptr;
				rightClickPressFun		= nullptr;
				rightClickReleaseFun	= nullptr;
				middleClickPressFun		= nullptr;
				middleClickReleaseFun	= nullptr;
				doubleClickFun			= nullptr;
				hoverOverFun			= nullptr;
				hoverOutFun				= nullptr;
				scrollFun				= nullptr;
			}
		public:
			virtual ~Interactable() {}
		public:
			// UI
			virtual void onLeftClickPressed()					{ if (leftClickPressFun)leftClickPressFun(); }
			virtual void onLeftClickReleased()					{ if (leftClickReleaseFun)leftClickReleaseFun(); }
			virtual void onRightClickPressed()					{ if (rightClickPressFun)rightClickPressFun(); }
			virtual void onRightClickReleased()					{ if (rightClickReleaseFun)rightClickReleaseFun(); }
			virtual void onMiddleClickPressed()					{ if (middleClickPressFun)middleClickPressFun(); }
			virtual void onMiddleClickReleased()				{ if (middleClickReleaseFun)middleClickReleaseFun(); }
			virtual void onDoubleClick()						{ if (doubleClickFun)doubleClickFun(); }
			virtual void onHoverover()							{ if (hoverOverFun)hoverOverFun(); }
			virtual void onHoverout()							{ if (hoverOutFun)hoverOutFun(); }
			virtual void onScroll(float xoffset, float yoffset) { if (scrollFun)scrollFun(xoffset, yoffset); }

			void  setTimeSinceLastClick(float time)	{ timeSinceLastClick = time; }
			float getTimeSinceLastClick()			{ if (doubleClickFun) return timeSinceLastClick; return 0.0f; }
		protected:
			// Callback function set
			void setLeftClickPressedCallback(LeftClickPressFun fun)			{ leftClickPressFun		= fun; }
			void setLeftClickReleasedCallback(LeftClickReleaseFun fun)		{ leftClickReleaseFun	= fun; }
			void setRightClickPressedCallback(RightClickPressFun fun)		{ rightClickPressFun	= fun; }
			void setRightClickReleasedCallback(RightClickReleaseFun fun)	{ rightClickReleaseFun	= fun; }
			void setMiddleClickPressedCallback(MiddleClickPressFun fun)		{ middleClickPressFun	= fun; }
			void setMiddleClickReleasedCallback(MiddleClickReleaseFun fun)	{ middleClickReleaseFun	= fun; }
			void setDoubleClickCallback(DoubleClickFun fun)					{ doubleClickFun		= fun; }
			void setHoveroverCallback(HoverOverFun fun)						{ hoverOverFun			= fun; }
			void setHoveroutCallback(HoverOutFun fun)						{ hoverOutFun			= fun; }
			void setScrollCallback(ScrollFun fun)							{ scrollFun				= fun; }

		private:
			//Callback function pointers
			LeftClickPressFun		leftClickPressFun;
			LeftClickReleaseFun		leftClickReleaseFun;
			RightClickPressFun		rightClickPressFun;
			RightClickReleaseFun	rightClickReleaseFun;
			MiddleClickPressFun		middleClickPressFun;
			MiddleClickReleaseFun	middleClickReleaseFun;
			DoubleClickFun			doubleClickFun;
			HoverOverFun			hoverOverFun;
			HoverOutFun				hoverOutFun;
			ScrollFun				scrollFun;

			float timeSinceLastClick;
		};
	}
}
#endif