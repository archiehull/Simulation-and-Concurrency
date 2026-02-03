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
		double dx = point.x - m_position.x;
		double dy = point.y - m_position.y;
		double dz = point.z - m_position.z;
		double distSq = dx * dx + dy * dy + dz * dz;
		double rSq = static_cast<double>(m_radius) * static_cast<double>(m_radius);
		return distSq <= rSq;
	}

	// segment-sphere intersection test
	bool Intersects(const Line& line) const override
	{
		// closest point on segment to sphere center
		Vec3 ab = line.b - line.a;
		Vec3 ac = m_position - line.a;
		double abLenSq = LengthSq(ab);
		if (abLenSq == 0.0)
		{
			// degenerate segment: treat as point
			double distSq = LengthSq(m_position - line.a);
			double rSq = static_cast<double>(m_radius) * static_cast<double>(m_radius);
			return distSq <= rSq;
		}

		double t = Dot(ac, ab) / abLenSq;
		if (t < 0.0) t = 0.0;
		else if (t > 1.0) t = 1.0;

		Vec3 closest = line.a + (ab * t);
		double distSq = LengthSq(m_position - closest);
		double rSq = static_cast<double>(m_radius) * static_cast<double>(m_radius);
		return distSq <= rSq;
	}

	float m_radius;
};