#include <iostream>


class Point {
    double x_, y_;
    
public:
    Point(double x, double y) {
        x_ = x;
        y_ = y;
    }

    double x() const {
        return x_;
    }

    double y() const {
        return y_;
    }
};

class Line {
    double a_, b_;

public:
    Line(double a, double b): a_(a), b_(b) {}

    Line(Point p1, Point p2) {
        a_ = (p1.y() - p2.y()) / (p1.x() - p2.x());
        b_ = p1.y() - a_ * p1.x();
    }

    Point* intersection(const Line& other) const {
        double x = (other.b_ - this->b_) / (this->a_ - other.a_);
        double y = x * this->a_ + this->b_;
        return new Point(x, y);
    }

    Line* perpendicular(const Point& point) const {
        double a = -1 / a_;
        double b = point.y() - a * point.x();
        return new Line(a, b);
    }

    double a() const {
        return a_;
    }

    double b() const {
        return b_;
    }
};

int main() {

}