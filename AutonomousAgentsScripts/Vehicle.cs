using UnityEngine;
using System.Collections;
using System.Collections.Generic;

[RequireComponent(typeof(CharacterController))]//will require a character controller

/// <summary>
/// the vehicle class that classes such as seeker, path following and leader following extend
/// </summary>
abstract public class Vehicle : MonoBehaviour
{
    CharacterController charControl;//get access to Character Controller component
    protected Vector3 acceleration;//acceleration vector
    protected Vector3 velocity;//velocity vector
    public Vector3 Velocity { get { return velocity; } }//velocity vector accessor
    protected Vector3 desired;//the desired velocity vector
    protected Vector3 steer;//the steer force vector
    public float maxSpeed = 60.0f;//the max speed value
    public float maxForce = 20.0f;//the max force value
    public float radius = 4.0f;//the radius of the vehicle
    public float mass = 1.0f;//the mass(not really used)
    protected GameManager gm;//the game manager object

    /// <summary>
    /// overrode in the child classes
    /// will calculate each of the steering forces to be applied
    /// </summary>
    abstract protected void CalcSteeringForces();

    /// <summary>
    /// will be called once when this class is first used
    /// </summary>
    virtual public void Start()
    {
        gm = GameObject.Find("GameManagerGO").GetComponent<GameManager>();//gets the game manager object
        acceleration = Vector3.zero;//instantiates the acceleration vector
        velocity = transform.forward;//instantiates the velocity vector to which ever the way the vehicle is facing
        charControl = GetComponent<CharacterController>();//gets the character controller component
    }

    /// <summary>
    /// called once per frame
    /// </summary
    protected void Update()
    {
        CalcSteeringForces();//calls the childrens' overriden calcSteeringforces 
        velocity += acceleration * Time.deltaTime;//adds acceleration accounted for frame time to velocity
        velocity = Vector3.ClampMagnitude(velocity, maxSpeed);//clamps velocity to max speed
        if (velocity != Vector3.zero)//if the vehicle is moving
        {
            transform.forward = velocity.normalized;//turns the vehicle to the velocity 
        }
        charControl.Move(velocity * Time.deltaTime);//moves the character controller by the velocity accounted for frame time
        acceleration = Vector3.zero;//resets the acceleration
    }

    /// <summary>
    /// will apply the force to the acceleration
    /// </summary>
    /// <param name="steeringForce">the steering force</param>
    protected void ApplyForce(Vector3 steeringForce)
    {
        acceleration += (steeringForce / mass);//adds the steering force accounting for mass to the acceleration
    }

    /// <summary>
    /// the method that calculates and returns the seek steering force
    /// </summary>
    /// <param name="targetPos">the target that you are seeking</param>
    /// <returns>the force that should be applied in order to approach the target</returns>
    protected Vector3 Seek(Vector3 targetPos)
    {
        desired = targetPos - transform.position;//get the desired velocity
        desired = desired.normalized * maxSpeed;//set the magnitude of desired
        steer = desired - velocity;//find the difference between your current velocity and the desired to see the amount you need to compensate
        return steer;//returns the appropriate force
    }

    /// <summary>
    /// the force to align with a certain direction
    /// </summary>
    /// <param name="direction">the direction you are trying to align with</param>
    /// <returns>the force to align with that direction</returns>
    protected Vector3 Align(Vector3 direction)
    {
        return ((direction * maxSpeed) - velocity);//returns the appropriate force
    }

    /// <summary>
    /// a wander force to attempt to go in a random direction in front of the vehicle
    /// </summary>
    /// <returns>the force to turn to wander in</returns>
    protected Vector3 Wander()
    {
        Vector3 vec = new Vector3(1,0,0);//some unit vector
        vec = Quaternion.Euler(Random.Range(0, 360), Random.Range(0, 360), Random.Range(0, 360)) * Vector3.forward;//rotate that unit vector in some direction
        Vector3 desired =(transform.forward * 8 )+ vec.normalized;//targets that vector added onto a point a little in front of the vehicle
        return desired - velocity;//returns the appropriate force
    }

    /// <summary>
    /// a force to follow a specific target 
    /// </summary>
    /// <param name="leaderPos">the position of the leader you are following</param>
    /// <param name="leaderDirection">the direction of the leader you are following</param>
    /// <returns>the force to follow the targeted leader</returns>
    protected Vector3 FollowLeader(Vector3 leaderPos,Vector3 leaderDirection)
    {
        Vector3 pos = leaderPos + (leaderDirection * -14);//sets the position to follow the position a little behind the leader so as to not crowd the leader
        Vector3 force = Seek(pos);//seek that position
        force += AstroSeparate();//a separation force for the astonauts
        return force;//returns the appropriate force
    }

    /// <summary>
    /// a separate force specific to the astronauts
    /// </summary>
    /// <returns>the separation force to get away from any astronauts that are too close</returns>
    protected Vector3 AstroSeparate()
    {
        float desiredSeparation = radius * 4;//the desired radius for separation
        Vector3 sum = new Vector3();//the sum of the forces
        foreach (GameObject a in gm.Astronauts)//for each astronaut
        {
            float d = Vector3.Distance(transform.position, a.transform.position);//the distance between the current astronaut and the current vehicle
            if (d > 0 && d < desiredSeparation)//if it isn't itself and it is within the desired amount to be separated by
            {
                Vector3 diff = transform.position - a.transform.position;//find the difference between the two 
                diff.Normalize();//normalize it to get the direction
                sum += diff;//add it to the sum
            }
        }
        return sum;//return the appropriate force
    }
    
    /// <summary>
    /// separation force specific to the flock of drones
    /// </summary>
    /// <returns>the force to keep them separated within a reasonable distance</returns>
    protected Vector3 Separate()
    {
        float desiredSeparation = radius * 6;//the desired radius for separation
        Vector3 sum = new Vector3();//the sum of the forces 
        foreach (GameObject g in gm.Flock)//for each flocker
        {
            float d = Vector3.Distance(transform.position, g.transform.position);//the distance between the current flocker and the one selected within the iteration
            if (d > 0 && d < desiredSeparation)//if it isn't itself and it is within the desired amount to be separated by
            {
                Vector3 diff = transform.position - g.transform.position;//the difference between the two
                diff.Normalize();//normalize it to just get the direction
                sum += diff;//add it to the sum
            }
        }
        return sum;//return the appropriate force
    }

    /// <summary>
    /// cohesion force specific to the flock of drones
    /// </summary>
    /// <returns>the force to bring the flock closer together</returns>
    protected Vector3 Cohere()
    {
        float toofar = radius * 9;//the radius that they should start cohering at
        Vector3 sum = new Vector3();//instantiates the sum
        foreach (GameObject g in gm.Flock)//for each flocker
        {
            float d = Vector3.Distance(gm.Centroid, g.transform.position);//the distance between the current flocker and the centroid
            if (d > toofar)//if they are too far
            {
                sum = gm.Centroid - g.transform.position;//the difference between the centroid and the flockers position
                sum.Normalize();//normalize that vector for the direction
            }
        }
        return sum;//returns the appropriate force
    }

    /// <summary>
    /// makes the vehicle direct around the obstalce
    /// </summary>
    /// <param name="ob">the obstacle</param>
    /// <param name="safe">the safe distance that you realize there is an obstacle at</param>
    /// <returns>the force to steer away or around the obstacle</returns>
    protected Vector3 AvoidObstacle(GameObject ob, float safe)
    {
        desired = Vector3.zero;//instantiates the desired vector
        float rad = ob.GetComponent<ObstacleScript>().Radius;//the radius of the obstacle
        Vector3 vecToCenter = ob.transform.position - transform.position;//the distance between the vehicle and the obstacle
        if (vecToCenter.magnitude > safe)//if the magnitude of the distance between the vehicle and the obstacle is larger than the safe distance
        {
            return Vector3.zero;//don't have to steer around
        }
        if (Vector3.Dot(vecToCenter, transform.forward) < 0)//if the obstacle is behind the vehicle
        {
            return Vector3.zero;//don't have to steer around
        }
        if (Mathf.Abs(Vector3.Dot(vecToCenter, transform.right)) > rad + radius)//if the obstacle isn't actually close enough to collide
        {
            return Vector3.zero;//dont' have to steer around
        }
        if (Vector3.Dot(vecToCenter, transform.right) < 0)//if it has passed all of the above, is it to the left
        {
            desired = transform.right * maxSpeed;//steer to the right
        }
        else//its to the right
        {
            desired = transform.right * -maxSpeed;//steer to the left
        }
        if(Vector3.Dot(vecToCenter,transform.up) < 0)//if it is below 
        {
            desired += transform.up * maxSpeed;//steer up
        }
        else//it is above
        {
            desired += transform.up * -maxSpeed;//steer down;
        }
        return desired;//return the appropriate force
    }
}
