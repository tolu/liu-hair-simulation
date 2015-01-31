#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include <math.h>
#include <fstream>
#include "Vector3D.h"
#include <cstdlib>
#include <ctime>


using namespace std;


// class Mass			---> An object to represent a mass
class Mass
{
public:
	float m;									// The mass value
	Vector3D lastPos;
	Vector3D pos;								// Position in space
	Vector3D vel;								// Velocity
	Vector3D force;								// Force applied on this mass at an instance

	Mass(float m)								// Constructor
	{
		this->m = m;
	}

	void applyForce(Vector3D force)
	{
		this->force += force;					// The external force is added to the force of the mass
	}

	void init()
	{
		force.x = 0;
		force.y = 0;
		force.z = 0;
	}

	/*
	  void simulate(float dt) method calculates the new velocity and new position of
	  the mass according to change in time (dt). Here, a simulation method called
	  "The Euler Method" is used. The Euler Method is not always accurate, but it is
	  simple. It is suitable for most of physical simulations that we know in common
	  computer and video games.
	*/
	void simulate(float dt)
	{
	    //spara förra positionen
		lastPos = pos;

		//räkna ut förflyttningen
		vel += (force / m) * dt;
		pos += vel * dt;

	}

};

// class Simulation		---> A container object for simulating masses
class Simulation
{
public:
	int numOfMasses;	// number of masses in this container
	Mass** masses;		// masses are held by pointer to pointer. (Here Mass** represents a 1 dimensional array)

    // Constructor creates masses with mass values m
	Simulation(int numOfMasses, float m)
	{
		this->numOfMasses = numOfMasses;
		masses = new Mass*[numOfMasses];		// Create an array of pointers

		for (int a = 0; a < numOfMasses; ++a){	// We will step to every pointer in the array
			masses[a] = new Mass(m);			// Create a Mass as a pointer and put it in the array
			masses[a]->init();
        }
	}

    // delete the masses created
	virtual void release()
	{
		for (int a = 0; a < numOfMasses; ++a)		// must delete everyone!
		{
			delete(masses[a]);
			masses[a] = NULL;
		}
		delete(masses);
		masses = NULL;
	}

	Mass* getMass(int index)
	{
		if (index < 0 || index >= numOfMasses)		// if the index is not in the array
			return NULL;							// then return NULL

		return masses[index];						// get the mass at the index
	}

    /********** INIT() **/
	virtual void init()								// this method will call the init() method of every mass
	{
		for (int a = 0; a < numOfMasses; ++a)		// We will init() every mass
			masses[a]->init();						// call init() method of the mass
	}
    /********** SOLVE() **/
	virtual void solve()							// no implementation because no forces are wanted in this basic container
	{
                                                    // in advanced containers, this method will be overrided and some forces will act on masses
	}
    /********** SIMULATE() **/
	virtual void simulate(float dt)					// Iterate the masses by the change in time
	{
		for (int a = 0; a < numOfMasses; ++a)		// We will iterate every mass
			masses[a]->simulate(dt);				// Iterate the mass and obtain new position and new velocity
	}
    /********** OPERATE() **/
	virtual void operate(float dt)					// The complete procedure of simulation
	{
		init();										// Step 1: reset forces to zero
		solve();									// Step 2: apply forces
		simulate(dt);								// Step 3: iterate the masses by the change in time
	}
	virtual void initSystem()
	{
	    initSystem();
	}

};

/***************************************************************
  class MassSpringSystem is derived from class Simulation
  It creates masses with preferred weight and binds the first mass to an arbitrary constant point with a spring.
****************************************************************/
class MassSpringSystem : public Simulation
{
public:
	Vector3D connectionPos;													//the arbitrary constant point that the mass is connected
    float length, airFriction, springConstant, inertia, maxLength;


	MassSpringSystem(float springConstant, float initX, float length) : Simulation(10, 0.005f)		//Constructor firstly constructs its super class with 1 mass and 1 kg
	{

		this->springConstant = 3.0;
		this->airFriction = 0.003;                                  //luften friktionskoefficient
		this->maxLength = length;
		this->inertia = (masses[0]->m*maxLength*maxLength)/12.0f;  //tröghetsmoment, från ekv: I = ( m*L^2 ) / 12

		connectionPos = Vector3D(initX, 15.0f, 0.0f);						//set the connectionPos

        masses[0]->pos = connectionPos + Vector3D(maxLength, 0.0f, 0.0f);		//set the position of the mass 10 meters to the right side of the connectionPos
        masses[0]->vel = Vector3D(0.0f, 0.0f, 0.0f);						//set the velocity of the mass to zero
        masses[0]->lastPos = masses[0]->pos;                                //set last position to initial position

        for (int a = 1; a < this->numOfMasses; ++a)
        {
            masses[a]->lastPos = masses[a-1]->pos + Vector3D(maxLength, 0.0f, 0.0f);
            masses[a]->pos = masses[a-1]->pos + Vector3D(maxLength, 0.0f, 0.0f);
            masses[a]->vel = Vector3D(0.0f, 0.0f, 0.0f);

        }

	}

    /** the spring force will be applied here **/
	virtual void solve()
	{

	    /** we apply force to all masses */
		for (int a = 0; a < numOfMasses; ++a)
		{

			Vector3D force, springForce;
            Vector3D springVector;

            /* calculate vector between to masses*/
            if (a == 0){
                springVector = connectionPos - masses[a]->pos;
                //dämpningsmoment för första massan
                springForce += -masses[a]->vel * inertia;
                /** add springforce */
                length = springVector.length();
                springForce += (springVector / length) * ((length - maxLength) * (springConstant));
                //applicera kraften från fjädern
                masses[a]->applyForce(springForce);
            } else {
                springVector = masses[(a-1)]->lastPos - masses[a]->pos;
                /* räkna fram dämpningsmoment mellan massorna*/
                /** se s. 64 i Physics for game developers - dampers*/
                springForce += (masses[(a-1)]->vel - masses[(a)]->vel) * (inertia);
                /** addera fjäderkraften */
                length = springVector.length();
                springForce += (springVector / length) * ((length - maxLength) * (springConstant));
                //aplicera kraften från fjädern på båda massorna som sitter fast i den
                masses[a]->applyForce(springForce);
                masses[a-1]->applyForce(-springForce);
			}


            /* addera gravitationen till kraften **/
			Vector3D gravity = Vector3D(0.0, -10.0, 0.0);
			force += gravity*masses[a]->m;

            /* addera luftmotståndet till kraften **/
            Vector3D airForce;
            airForce = -masses[a]->vel * airFriction;
            force += airForce;

            /* applicera andra krafter på massan **/
			masses[a]->applyForce(force);

            /** ett försök att räkna ut torque som: T = ( R*r ) x ( F ) */
			//Vector3D Inertia = springVector.cross(masses[a]->force);
			//Inertia.unit();
            //masses[a]->applyForce(Inertia*0.1);

            /** ett försök att räkna ut torque som: T = ( J ) * ( vinkel acceleration ) */
            //Vector3D v = springVector;
            //Vector3D u = masses[a]->pos - masses[a]->lastPos;
            //float angle = v.getAngle( u );
            //Vector3D ang = Vector3D(cos(angle), sin(angle), 0.0f);
            //masses[a]->applyForce(ang*0.0001*0.0001);

            /**add some random wind*/
            srand(time(0));
            int var = rand() % 10; // slumptal mellan noll och tio
            Vector3D* wind = new Vector3D(-(float)(var/250.0), 0.0, 0.0);
//            if(var < 4)
//                *wind *= -1;
            masses[a]->applyForce(*wind);

		}
	}
    /** press space in program to reset the simulation**/
	virtual void initSystem(){
	    masses[0]->pos = connectionPos + Vector3D(maxLength, 0.0f, 0.0f);
        masses[0]->vel = Vector3D(0.0f, 0.0f, 0.0f);
        masses[0]->lastPos = masses[0]->pos;

        for (int a = 1; a < this->numOfMasses; ++a)
        {
            masses[a]->lastPos = masses[a-1]->pos + Vector3D(maxLength, 0.0f, 0.0f);
            masses[a]->pos = masses[a-1]->pos + Vector3D(maxLength, 0.0f, 0.0f);
            masses[a]->vel = Vector3D(0.0f, 0.0f, 0.0f);

        }
	}

};

#endif // PHYSICS_H_INCLUDED
