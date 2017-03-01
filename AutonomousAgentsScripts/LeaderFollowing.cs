using UnityEngine;
using System.Collections;

/// <summary>
/// unless specially designated as the leader, this will follow the leader
/// if it is the leader it will wander within 200 units of the origin
/// </summary>
public class LeaderFollowing : Vehicle
{
    //whether this astronaut is a leader or not
    public bool leader;

    //each of the weight values for the steering forces, some are not used
    public float seekWeight = 75.0f;
    public float safeDistance = 10.0f;
    public float avoidWeight = 100.0f;
    public float alignmentWeight = 120.0f;
    public float separateWeight = 120.0f;
    public float wanderWeight = 100.0f;
    public float leaderFollowWeight = 100.0f;

    private Vector3 totalForce;//the total force vector for applying to the controller

    /// <summary>
    /// will be called once when this class is first used
    /// </summary>
    override public  void Start()
    {
        base.Start();////parent's start function
        totalForce = Vector3.zero;//intilializes the total force
    }

    /// <summary>
    /// calculates and adds the steering forces together to be applied to the controller
    /// </summary>
    protected override void CalcSteeringForces()
    {
        totalForce = Vector3.zero;//reset the total force
        if(leader)//if this is the astonaut leader
        {
            totalForce += wanderWeight * Wander();//add a wander force
            if(Vector3.Distance(Vector3.zero,transform.position) > 200)//if the astronaut leader goes a little far, just seek the origin
            {
                totalForce += seekWeight * Seek(Vector3.zero);
            }
        }
        else//not the leader
        {
            totalForce += leaderFollowWeight * FollowLeader(gm.astronautLeader.transform.position, gm.astronautLeader.transform.forward);//follow the leader force
        }
        totalForce = Vector3.ClampMagnitude(totalForce, maxForce);//clamp the total force to the max force value
        ApplyForce(totalForce);//apply that force to the vehicle
    }
}
