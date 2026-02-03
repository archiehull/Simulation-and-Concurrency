#pragma once
#include "Collider.h"
#include <cmath>

class Sphere : public Collider
{
public:
	// radius stored as float per request
	Sphere(const Vec3& center, float radius)
		: Collider(center), m_radius(radius) {
	}

	// point-inside-sphere test
	bool IsInside(const Vec3& point) const override
	{
		const double rSq = static_cast<double>(m_radius) * static_cast<double>(m_radius);
		return DistanceSqToPoint(point) <= rSq + EPS;
	}

	// segment-sphere intersection test
	bool Intersects(const Line& line) const override
	{
		Vec3 closest = ClosestPointOnSegment(line, m_position);
		const double rSq = static_cast<double>(m_radius) * static_cast<double>(m_radius);
		return LengthSq(m_position - closest) <= rSq + EPS;
	}

	// Sphere-sphere collision: true if distance between centers <= sum of radii
	bool CollideWith(const Sphere& other) const
	{
		double rSum = static_cast<double>(m_radius) + static_cast<double>(other.m_radius);
		return DistanceSqToPoint(other.m_position) <= (rSum * rSum) + EPS;
	}

	float m_radius;

private:
	static constexpr double EPS = 1e-9;

	double DistanceSqToPoint(const Vec3& p) const
	{
		double dx = p.x - m_position.x;
		double dy = p.y - m_position.y;
		double dz = p.z - m_position.z;
		return dx * dx + dy * dy + dz * dz;
	}

	static Vec3 ClosestPointOnSegment(const Line& seg, const Vec3& point)
	{
		Vec3 a = seg.a;
		Vec3 b = seg.b;
		Vec3 ab = b - a;
		double abLenSq = LengthSq(ab);
		if (abLenSq <= 0.0)
			return a; // degenerate segment -> return endpoint

		double t = Dot(point - a, ab) / abLenSq;
		if (t < 0.0) t = 0.0;
		else if (t > 1.0) t = 1.0;
		return a + (ab * t);
	}
};