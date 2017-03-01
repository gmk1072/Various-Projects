using UnityEngine;
using System.Collections;

/// <summary>
/// this will seek points designated by the seekertarget
/// acts as a flock
/// </summary>
public class Seeker : Vehicle
{
    public static GameObject seekerTarget;//holds the target that this class as a whole will seek
    public static bool done;//if the seekers are done going after the astronauts

    //force weighting
    public float seekWeight = 75.0f;
    public float safeDistance = 10.0f;
    public float avoidWeight = 100.0f;
    public float alignmentWeight = 120.0f;
    public float separateWeight = 120.0f;

    private Vector3 totalForce;//the total force vector for applying to the controller
    public static bool collectedAstro;//if they are carrying an astronaut

    /// <summary>
    /// will be called once when this class is first used
    /// </summary>
    override public void Start()
    {
        base.Start();//parent's start function
        totalForce = Vector3.zero;//intilializes the total force
    }

    /// <summary>
    /// calculates and adds the steering forces together to be applied to the controller
    /// </summary>
    protected override void CalcSteeringForces()
    {
        totalForce = Vector3.zero;//resets the steering force
        if (!done)//if the seekers are not done yet
        {
            totalForce += seekWeight * Seek(seekerTarget.transform.position);//seek the target
            totalForce += alignmentWeight * Align(gm.Direction);//align with the direction of the flock
            totalForce += separateWeight * Separate();//separate among the flock
            totalForce += Cohere();//cohere with the flock

        }
        else//the seeker are done
        {
            totalForce += .25f * seekWeight * Seek(gm.mothership.transform.position - gm.mothership.transform.forward * 20);//seek the mothership position minus a little
            totalForce += alignmentWeight * Align(gm.Direction);//align with the direction of the flock
            totalForce += 1.75f * Separate() * separateWeight;//separate among the flock
            totalForce += .75f * Cohere();//cohere with the flock
            maxForce = gm.mothership.GetComponent<PathFollower>().maxForce;//travel at the motherships maxforce
            maxSpeed = gm.mothership.GetComponent<PathFollower>().maxSpeed;//travel at the motherships maxspeed
        }
        for (int i = 0; i < gm.Obstacles.Length; i++)//for each of the obstacles
        {
            totalForce += avoidWeight * AvoidObstacle(gm.Obstacles[i], safeDistance);//add an avoidance weight if there is any
        }
        totalForce = Vector3.ClampMagnitude(totalForce, maxForce);//clamp the total force to 
        ApplyForce(totalForce);//apply the total force to the vehicle
    }
}
