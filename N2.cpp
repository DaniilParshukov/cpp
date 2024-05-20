#include <iostream>
#include <optional>
#include <cassert>


struct Point {
    double x, y;
};

class Line {
    double a_, b_;

public:
    Line(double a, double b): a_(a), b_(b) {}

    Line(Point p1, Point p2) {
        a_ = (p1.y - p2.y) / (p1.x - p2.x);
        b_ = p1.y - a_ * p1.x;
    }

    std::optional<Point> intersection(const Line& other) const {
        if (a_ == other.a_) {
            return {};
        }
        double x = (other.b_ - b_) / (a_ - other.a_);
        double y = x * a_ + b_;
        return Point{x, y};
    }

    Line perpendicular(const Point& point) const {
        double a = -1 / a_;
        double b = point.y - a * point.x;
        return Line{a, b};
    }

    double a() const {
        return a_;
    }

    double b() const {
        return b_;
    }
};

void test1() {
    assert(0 == Line(0, 0).intersection(Line(Point{1, 1}, Point{3, 3})).value().x);
}

void test2() {
    assert(false == Line(1, 1).intersection(Line(1, 2)).has_value());
}

void test3() {
    Line line = Line(0.5, 0).perpendicular(Point{1, 2});
    assert(-2 == line.a());
    assert(4 == line.b());
}

void test4() {
    Line line = Line(Point{1, 3}, Point{3, 1}).perpendicular(Point{3, 1});
    assert(1 == line.a());
    assert(-2 == line.b());
}

int main() {
    test1();
    test2();
    test3();
    test4();
}