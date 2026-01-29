#pragma once
#include <cmath>
#include <iostream>

struct Vec3
{
	double x, y, z;
};

class Sphere
{
public:
	Sphere(const Vec3& center, double radius) : m_center(center), m_radius(radius), m_radiusSq (radius * radius) {}

	bool IsInside(const Vec3& point) const
	{
		double dx = point.x - m_center.x;
		double dy = point.y - m_center.y;
		double dz = point.z - m_center.z;
		double distanceSq = dx * dx + dy * dy + dz * dz;

		std::cout << "\n\nDistance squared: " << distanceSq << ", Radius squared: " << m_radiusSq << "\n" << std::endl;

		return distanceSq <= m_radiusSq;
	}

private:
	Vec3 m_center;
	double m_radius;
	double m_radiusSq;
};