#pragma once

#include "Core.h"
#include "Model.h"

class RigidBody
{
public:
	RigidBody(float x, float y, float z, float m, float y0);
	~RigidBody();

	void Update(float time);
	void ApplyForce(glm::vec3 f, glm::vec3 pos);
	void ApllyImpulse(glm::vec3 j, glm::vec3 pos);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void Reset();

private:
	// constants
	float mass;
	glm::vec3 rotInertia; // Ix, Iy, & Iz from diagonal inertia
	glm::mat3 I0;
	glm::vec3 g;

	// variables
	glm::mat4 mtx; // contains position & orientation
	glm::vec3 momentum;
	glm::vec3 angMomentum;

	// accumulators
	glm::vec3 force;
	glm::vec3 torque;
	//glm::vec3 impulse;
	//glm::vec3 angularImpulse;

	Model* model;
	vector<glm::vec3> vertices;
	float groundY;

	void Print(glm::vec3);
	void Print(glm::mat3);
	glm::mat3 Hat(glm::vec3);
};

