#pragma once
class Vec2
{
public:
	Vec2(void);
	Vec2(float nx, float ny);
	~Vec2(void);
	
	float x;
	float y;

	float Length();
	void Normalise();
	void SetLength(float l);
	
	// Arithmetic operators
	Vec2 operator +(const Vec2 &v) const { return Vec2(x + v.x, y + v.y); }
	Vec2 operator -(const Vec2 &v) const { return Vec2(x - v.x, y - v.y); }
	Vec2 operator *(const Vec2 &v) const { return Vec2(x * v.x, y * v.y); }
	Vec2 operator /(const Vec2 &v) const { return Vec2(x / v.x, y / v.y); }
	
	Vec2 operator *(const float &f) const { return Vec2(x * f, y * f); }
	Vec2 operator /(const float &f) const { return Vec2(x / f, y / f); }

	// Static inline methods
	static Vec2 Add(Vec2 a, Vec2 b) { return Vec2(a.x+b.x, a.y+b.y); }
	static Vec2 Sub(Vec2 a, Vec2 b) { return Vec2(a.x-b.x, a.y-b.y); }
	static Vec2 Mul(Vec2 a, Vec2 b) { return Vec2(a.x*b.x, a.y*b.y); }
	static Vec2 Mul(Vec2 a, float n) { return Vec2(a.x*n, a.y*n); }
	static Vec2 Div(Vec2 a, Vec2 b) { return Vec2(a.x/b.x, a.y/b.y); }
};

