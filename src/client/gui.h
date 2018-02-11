#ifndef GUI_H_
#define GUI_H_

#include <vector>
#include <initializer_list>
#include "renderer.h"

namespace GUI {

	// using Point2D = Vec2f;

	class Point2D {
	public:
		float x, y;

		Point2D(): x(0.0f), y(0.0f) {}
		Point2D(int x_, int y_): x(float(x_)), y(float(y_)) {}
		Point2D(float x_, float y_): x(x_), y(y_) {}

		Point2D operator+ (const Point2D& r) const { return Point2D(x + r.x, y + r.y); }
		Point2D operator- (const Point2D& r) const { return Point2D(x - r.x, y - r.y); }
		Point2D operator* (const Point2D& r) const { return Point2D(x * r.x, y * r.y); }
	};

	class Position {
	public:
		Point2D relative, offset;

		Position() = default;
		Position(const Point2D& relative_, const Point2D& offset_): relative(relative_), offset(offset_) {}

		Point2D compute(const Point2D& parentSize) const {
			return parentSize * relative + offset;
		}
	};

	class Control {
	public:
		Position upperLeft, lowerRight;

		Control() = default;
		Control(const Position& ul, const Position& lr): upperLeft(ul), lowerRight(lr) {}

		void addChild(Control* c) { children.push_back(c); }
		void addChild(std::initializer_list<Control*> c) { for (Control* curr: c) children.push_back(curr); }
		virtual void update() {}
		void render(const Point2D& parentSize, const Point2D& parentPos) {
			Point2D ul = upperLeft.compute(parentSize) + parentPos;
			Point2D lr = lowerRight.compute(parentSize) + parentPos;
			renderSelf(ul, lr);
			for (size_t i = 0; i < children.size(); i++) children[i]->render(lr - ul, ul);
		}

	private:
		std::vector<Control*> children;

		virtual void renderSelf(const Point2D&, const Point2D&) {}
	};

	class Form: public Control {
	public:
		Form(): Control(Position(Point2D(0.0f, 0.0f), Point2D(0.0f, 0.0f)), Position(Point2D(1.0f, 1.0f), Point2D(0.0f, 0.0f))) {}
	};

	class Area : public Control {
	public:
		Area(const Position& ul, const Position& lr) : Control(ul, lr) {}
	};

	class Button: public Control {
	public:
		Button(const Position& ul, const Position& lr): Control(ul, lr) {}

	private:
		void renderSelf(const Point2D& ul, const Point2D& lr) override {
			VertexArray va(6, VertexFormat(0, 4, 0, 2));
			va.setColor({ 1.0f, 1.0f, 1.0f, 0.7f });
			va.addVertex({ ul.x, ul.y });
			va.addVertex({ ul.x, lr.y });
			va.addVertex({ lr.x, ul.y });
			va.addVertex({ ul.x, lr.y });
			va.addVertex({ lr.x, lr.y });
			va.addVertex({ lr.x, ul.y });
			VertexBuffer vb(va, false);
			vb.render();
		}
	};
}

#endif
