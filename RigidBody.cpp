#include "RigidBody.h"



RigidBody::RigidBody(float width, float height, float depth, float m, float y0)
{
	mass = m;
	g = glm::vec3(0, -9.8f, 0);
	groundY = y0;
	float x = width / 2.0f;
	float y = height / 2.0f;
	float z = depth / 2.0f;
	vertices.push_back(glm::vec3(-x, -y, -z));
	vertices.push_back(glm::vec3(-x, -y, z));
	vertices.push_back(glm::vec3(-x, y, -z));
	vertices.push_back(glm::vec3(-x, y, z));
	vertices.push_back(glm::vec3(x, -y, -z));
	vertices.push_back(glm::vec3(x, -y, z));
	vertices.push_back(glm::vec3(x, y, -z));
	vertices.push_back(glm::vec3(x, y, z));
	model = new Model();
	model->MakeBox(glm::vec3(-x, -y, -z), glm::vec3(x, y, z));
	I0 = glm::mat3(1);
	I0[0][0] = mass * (height * height + depth * depth) / 12.0f;
	I0[1][1] = mass * (width * width + depth * depth) / 12.0f;
	I0[2][2] = mass * (height * height + width * width) / 12.0f;
	Reset();
}


RigidBody::~RigidBody()
{
}

void RigidBody::Update(float time)
{
	force += mass * g;

	// Update position
	momentum += force * time;
	glm::vec3 d = (momentum / mass) * time;
	mtx[3][0] += d.x; // Mtx.d = position
	mtx[3][1] += d.y;
	mtx[3][2] += d.z;
	// Update orientation
	angMomentum += torque * time;
	glm::mat3 rot = glm::mat3(mtx);
	glm::mat3 I = rot * I0 * glm::transpose(rot); // A，I0，AT
	glm::vec3 angVelocity = glm::inverse(I) * angMomentum;
	float angle = glm::length(angVelocity) * time; // magnitude of ω
	glm::vec3 axis = glm::normalize(angMomentum);
	
	if (angle != 0)
		mtx = glm::rotate(mtx, angle, axis);
	
	force = torque = glm::vec3(0);

	rot = glm::mat3(mtx);
	I = rot * I0 * glm::transpose(rot); // A，I0，AT
	
	float lowest = 0;
	glm::vec3 pos = glm::vec3(0);
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 p = glm::vec3(mtx * glm::vec4(vertices[i], 1));
		if (p.y < lowest) {
			lowest = p.y;
			pos = p;
		}
	}
	if (pos.y <= groundY) {
		glm::vec3 r = pos - glm::vec3(mtx[3][0], mtx[3][1], mtx[3][2]);
		glm::vec3 velocity = momentum / mass;
		glm::mat3 inverseM = glm::mat3(1.0f / mass) - Hat(r) * glm::inverse(I) * Hat(r);
		glm::vec3 impulse = glm::inverse(inverseM) * (-velocity - glm::cross(angVelocity, r));
		ApplyForce(impulse / time, pos);
		mtx[3][1] += groundY - pos.y;
	}

	//// collision detection
	//for (int i = 0; i < vertices.size(); i++) {
	//	glm::vec3 pos = glm::vec3(mtx * glm::vec4(vertices[i], 1));
	//	glm::vec3 r = pos - glm::vec3(mtx[3][0], mtx[3][1], mtx[3][2]);
	//	glm::vec3 velocity = momentum / mass;
	//	if (pos.y <= groundY) {
	//		
	//		glm::mat3 inverseM = glm::mat3(1.0f / mass) - Hat(r) * glm::inverse(I) * Hat(r);
	//		glm::vec3 impulse = glm::inverse(inverseM) * (-velocity - glm::cross(angVelocity, r));
	//		ApplyForce(impulse / time, pos);
	//		//ApllyImpulse(impulse, pos);
	//		mtx[3][1] += groundY - pos.y;
	//	}	
	//}

	
	
}

void RigidBody::ApplyForce(glm::vec3 f, glm::vec3 pos)
{
	force += f;
	torque += glm::cross((pos - glm::vec3(mtx[3][0], mtx[3][1], mtx[3][2])), f);
}

void RigidBody::ApllyImpulse(glm::vec3 j, glm::vec3 pos)
{
	//impulse += j;
	//angularImpulse += glm::cross((pos - glm::vec3(mtx[3][0], mtx[3][1], mtx[3][2])), j);
}


void RigidBody::Draw(const glm::mat4 & viewProjMtx, uint shader)
{
	model->Draw(mtx, viewProjMtx, shader, glm::vec3(1,1,1));
}

void RigidBody::Reset()
{
	mtx = glm::mat4(1);
	momentum = glm::vec3(0);
	angMomentum = glm::vec3(rand() % 40 - 20, rand() % 40 - 20, rand() % 40 - 20);
	//angMomentum = glm::vec3(0,0,0);
	force = torque = glm::vec3(0);
	//ApplyForce(vertices[3], glm::vec3(0, 10, 0));
}

void RigidBody::Print(glm::vec3 v)
{
	cout << v.x << " " << v.y << " " << v.z << endl;
}

void RigidBody::Print(glm::mat3 m)
{
	cout << m[0][0] << " " << m[0][1] << " " << m[0][2] << endl;
	cout << m[1][0] << " " << m[1][1] << " " << m[1][2] << endl;
	cout << m[2][0] << " " << m[2][1] << " " << m[2][2] << endl;
}

glm::mat3 RigidBody::Hat(glm::vec3 a)
{
	return glm::mat3(0, a.z, -a.y, -a.z, 0, a.x, a.y, -a.x, 0);
}



