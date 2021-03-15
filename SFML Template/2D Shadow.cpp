#include "2D Shadow.h"

void Line::display(sf::RenderWindow &thatWindow)
{
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(PointsA.x, PointsA.y);
	line[1].position = sf::Vector2f(PointsB.x, PointsB.y);
	thatWindow.draw(line);
}

sf::Vector2f Line::GetPointA()
{
	return PointsA;
}

sf::Vector2f Line::GetPointB()
{
	return PointsB;
}

void Line::SetPointA(sf::Vector2f& newPoint)
{
	PointsA = newPoint;
}

void Line::SetPointB(sf::Vector2f& newPoint)
{
	PointsB = newPoint;
}

sf::Vector2f Shadow::IntersectionClass::getIntersection(Line ray, Line segments)
{
	// RAY in parametric: Point + Direction*T1
	float r_px = ray.GetPointA().x;
	float r_py = ray.GetPointA().y;
	float r_dx = ray.GetPointB().x - ray.GetPointA().x;
	float r_dy = ray.GetPointB().y - ray.GetPointA().y;

	// SEGMENT in parametric: Point + Direction*T2
	float s_px = segments.GetPointA().x;
	float s_py = segments.GetPointA().y;
	float s_dx = segments.GetPointB().x - segments.GetPointA().x;
	float s_dy = segments.GetPointB().y - segments.GetPointA().y;

	// Are they parallel? If so, no intersect
	float r_mag = sqrt(r_dx*r_dx + r_dy * r_dy);
	float s_mag = sqrt(s_dx*s_dx + s_dy * s_dy);
	if (r_dx / r_mag == s_dx / s_mag && r_dy / r_mag == s_dy / s_mag) { // Directions are the same.
		return sf::Vector2f();
	}

	// SOLVE FOR T1 & T2
	// r_px+r_dx*T1 = s_px+s_dx*T2 && r_py+r_dy*T1 = s_py+s_dy*T2
	// ==> T1 = (s_px+s_dx*T2-r_px)/r_dx = (s_py+s_dy*T2-r_py)/r_dy
	// ==> s_px*r_dy + s_dx*T2*r_dy - r_px*r_dy = s_py*r_dx + s_dy*T2*r_dx - r_py*r_dx
	// ==> T2 = (r_dx*(s_py-r_py) + r_dy*(r_px-s_px))/(s_dx*r_dy - s_dy*r_dx)
	float T2 = (r_dx*(s_py - r_py) + r_dy * (r_px - s_px)) / (s_dx*r_dy - s_dy * r_dx);
	float T1 = (s_px + s_dx * T2 - r_px) / r_dx;

	// Must be within parametic whatevers for RAY/SEGMENT
	if (T1 < 0) return sf::Vector2f();
	if (T2 < 0 || T2>1) return sf::Vector2f();

	// Return the POINT OF INTERSECTION
	param = T1;
	return sf::Vector2f(r_px + r_dx * T1, r_py + r_dy * T1);
}

void Shadow::Point::display(sf::RenderWindow & thatWindow)
{
	sf::CircleShape ball(4);
	ball.setOrigin(2, 2);
	ball.setPosition(p_pos);
	thatWindow.draw(ball);
}

void Shadow::points(std::vector<Line> segs)
{
	pointsVector.clear();
	for (unsigned int i = 0; i < segs.size(); i++) {
		pointsVector.push_back(segs[i].GetPointA()); pointsVector.push_back(segs[i].GetPointB());
	}
}

void Shadow::update(sf::Vector2i Mousepos)
{
	mousepos = Mousepos;
	points(segments);

	uniqueAngles.clear();
	for (unsigned int j = 0; j < pointsVector.size(); j++) {
		float angle = atan2(pointsVector[j].y - mousepos.y, pointsVector[j].x - mousepos.x);
		uniqueAngles.push_back(angle - 0.00005);
		uniqueAngles.push_back(angle);
		uniqueAngles.push_back(angle + 0.00005);
	}

	std::sort(uniqueAngles.begin(), uniqueAngles.end());
	uniqueAngles.erase(unique(uniqueAngles.begin(), uniqueAngles.end()), uniqueAngles.end());

	intersects.clear();
	for (int j = 0; j < uniqueAngles.size(); j++) {

		float angle = uniqueAngles[j];

		// Calculate dx & dy from angle
		float dx = cosf(angle);
		float dy = sinf(angle);

		// Ray from center of screen to mouse
		Line ray(mousepos.x, mousepos.y, mousepos.x + dx, mousepos.y + dy);

		// Find CLOSEST intersection
		IntersectionClass closestIntersect(Line(0, 0, 0, 0), Line(0, 0, 0, 0)); closestIntersect.IntersectionVector = sf::Vector2f(); closestIntersect.param = 0; closestIntersect.angle = 0;
		for (int i = 0; i < segments.size(); i++) {
			IntersectionClass intersect(ray, segments[i]);
			if ((intersect.IntersectionVector.x + intersect.IntersectionVector.y) == NULL) continue;
			if (closestIntersect.IntersectionVector == sf::Vector2f() || intersect.param < closestIntersect.param) {
				closestIntersect = intersect;
			}
		}

		closestIntersect.angle = angle;
		// Add to list of intersects
		intersects.push_back(closestIntersect);
	}
}

void Shadow::draw(sf::RenderWindow &thatWindow)
{
	sf::VertexArray shape(sf::TriangleFan, intersects.size() + 1);
	shape[0].position = (sf::Vector2f(mousepos.x, mousepos.y));
	shape[0].color = (sf::Color(255, 255, 255));
	shape[intersects.size()].position = (intersects[0].IntersectionVector);
	shape[intersects.size()].color = (sf::Color(255, 255, 255));

	for (int j = 1; j < intersects.size(); j++) {
		shape[j].position = (intersects[j].IntersectionVector);
		shape[j].color = (sf::Color(255, 255, 255));
	}

	thatWindow.draw(shape);
}

void Shadow::debugPoints(sf::RenderWindow &thatWindow)
{
	for (int j = 0; j < intersects.size(); j++) {
		Point points(intersects[j].IntersectionVector);
		points.display(thatWindow);
	}
}

void Shadow::debugLines(sf::RenderWindow &thatWindow)
{
	for (int j = 0; j < intersects.size(); j++) {
		Line laser(mousepos.x, mousepos.y, intersects[j].IntersectionVector.x, intersects[j].IntersectionVector.y);
		laser.display(thatWindow);
	}
}