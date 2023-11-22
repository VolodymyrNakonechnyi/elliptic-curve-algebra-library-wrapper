#include <iostream>
#include <random>
#include "cmath"
#include <cassert>
#include "string"

typedef struct ECPoint {
    int64_t X = NULL;
    int64_t Y = NULL;
};

ECPoint ECPointGen(int64_t x, int64_t y) { //ECPoint creation
    ECPoint point;
    point.X = x;
    point.Y = y;
    return point;
}

ECPoint BasePointGGet() { //G-generator receiving
    std::random_device rd;
    std::mt19937_64 generator(rd());
    std::uniform_int_distribution<int64_t> distribution(0, std::numeric_limits<int64_t>::max());

    ECPoint point = ECPointGen(
        distribution(generator),
        distribution(generator)
    );

    return point;
}

bool IsOnCurveCheck(ECPoint point, int64_t a, int64_t b, int64_t q) { //DOES P ∈ CURVE?
    int64_t x = point.X;
    int64_t y = point.Y;

    int64_t rightSide = (std::pow(x, 3) + a * x + b);

    return std::pow(y, 2) == (rightSide % q);
}

ECPoint AddECPoints(ECPoint a, ECPoint b) { //P + Q
    int64_t xP, yP, xQ, yQ;
    xP = a.X;
    yP = a.Y;

    xQ = b.X;
    yQ = b.Y;
    int64_t m = ((yP - yQ) / (xP - xQ));

    int64_t xR, yR;

    xR = m * m - xP - xQ;
    yR = yP + m * (xR - xP);

    return ECPointGen(xR, yR);
}

ECPoint DoubleECPoints(ECPoint point, int64_t a) {
    int64_t x1, y1, x3, y3, lambda;

    x1 = point.X;
    y1 = point.Y;

    lambda = (3 * pow(x1, 2) + a) / 2 * y1;

    x3 = std::pow(lambda, 2) - 2 * x1;
    y3 = lambda * (x1 - x3) - y1;

    return ECPointGen(x3, y3);
}

ECPoint ScalarMult(int64_t k, ECPoint point, int64_t a) { //k * P
    ECPoint c = ECPointGen(0, 0);

    if (k <= 0) {
        return c;
    }

    if (k == 1) {
        return point;
    }

    for (int i = 0; i < k - 1; i++) {
        c = DoubleECPoints(point, a);
    }

    return c;
}

std::string ECPointToString(ECPoint point) {
    return "X = " + std::to_string(point.X) + "; Y = " + std::to_string(point.Y) + ";";
}

int64_t extractNumber(std::string s, std::string keyword, std::string sep) {
    size_t start = s.find(keyword);
    size_t end = s.find(sep, start);
    std::string numStr = s.substr(start + keyword.size(), end - (start + keyword.size()));
    return std::stoull(numStr);
}

ECPoint StringToECPoint(const std::string& s) {
    ECPoint result;

    result.X = extractNumber(s, "X = ", ";");
    result.Y = extractNumber(s, "Y = ", ";");

    return result;
}

void PrintECPoint(ECPoint point) {
    std::cout << "Coord x: " << point.X << std::endl;
    std::cout << "Coord y: " << point.Y << std::endl;
}

int main() {
    ECPoint point = BasePointGGet();
    ECPoint customPoint = ECPointGen(2, 1);
    PrintECPoint(point);

    std::cout << "Is on the curve? " << std::endl;
    assert(IsOnCurveCheck(customPoint, 3, 1, 7));

    // Test ECPoint creation
    std::cout << "Test ECPoint creation" << std::endl;
    ECPoint point1 = ECPointGen(2, 3);
    assert(point1.X == 2 && point1.Y == 3 && ("Created points not correct!"));

    // Test G-generator receiving
    std::cout << "Test G-generator receiving" << std::endl;
    ECPoint point2 = BasePointGGet();
    assert(point2.X != NULL && point2.Y != NULL && ("Created points not correct!"));

    // Test AddECPoints
    std::cout << "Test AddECPoints" << std::endl;
    ECPoint addPoint1 = ECPointGen(5, 12);
    ECPoint addPoint2 = ECPointGen(2, 6);

    ECPoint point3 = AddECPoints(addPoint1, addPoint2);

    std::cout << point3.X << std::endl;
    std::cout << point3.Y << std::endl;

    assert((point3.X == -3 && point3.Y == -4) && ("Adding EC points doesn't work correctly!"));

    // Test DoubleECPoints
    std::cout << "Test DoubleECPoints" << std::endl;
    ECPoint point4 = DoubleECPoints(point1, 1);  // Replace 1 with your actual coefficient a
    PrintECPoint(point4);
    assert((point4.X == 357 && point4.Y == -6748) && ("Doubling of EC points doesn't work correctly!A"));

    // Test ScalarMult
    std::cout << "Test ScalarMult" << std::endl;
    ECPoint point5 = ScalarMult(3, point1, 3);
    PrintECPoint(point5);
    assert((point5.X == 480 && point5.Y == -10519) && ("Scalar multiplication EC points doesn't work correctly!"));

    // Test ECPointToString and StringToECPoint
    std::cout << "Test ECPointToString and StringToECPoint" << std::endl;
    std::string pointString = ECPointToString(point1);
    ECPoint parsedPoint = StringToECPoint(pointString);
    assert(parsedPoint.X == point1.X && parsedPoint.Y == point1.Y);

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
