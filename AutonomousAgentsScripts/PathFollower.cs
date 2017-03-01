using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/// <summary>
/// the mothership is the only path follower
/// this is simple path following of seeking the next point designated by the list
/// </summary>
public class PathFollower : Vehicle
{
    public GameObject seekerTarget;//the seeking target
    public int currentTarget;//the current target marked as int 
    public float seekWeight = 75.0f;//weighting for the seek force
    private Vector3 totalForce;//the total force vector for applying to the controller
    List<Vector3> path;//the list of points making up the path

    /// <summary>
    /// will be called once when this class is first used
    /// </summary>
    override public void Start()
    {
        base.Start();//parent's start function
        totalForce = Vector3.zero;//intilializes the total force
        path = new List<Vector3>();//intializes the path list
        
        //sets up and adds each of the points along the path
        path.Add(new Vector3(( 273), (50), ( 575)));//1
        path.Add(new Vector3(( 547), (50), ( 301)));//2
        path.Add(new Vector3(( 600), (50), (   0)));//3
        path.Add(new Vector3(( 547), (50), (-246)));//4
        path.Add(new Vector3(( 273), (50), (-521)));//5
        path.Add(new Vector3((   0), (50), (-600)));//6
        path.Add(new Vector3((-275), (50), (-521)));//7
        path.Add(new Vector3((-549), (50), (-246)));//8
        path.Add(new Vector3((-600), (50), (   0)));//9
        path.Add(new Vector3((-549), (50), ( 301)));//10
        path.Add(new Vector3((-275), (50), ( 575)));//11
        path.Add(new Vector3((   0), (50), ( 600)));//12

        currentTarget = 0;//sets the first target
    }

    /// <summary>
    /// calculates and adds the steering forces together to be applied to the controller
    /// </summary>
    protected override void CalcSteeringForces()
    {
        totalForce = Vector3.zero;//resets the total force
        if (Vector3.Distance(transform.position, path[currentTarget]) < 20)//if the distance to the next point is small enought
        {
            if (currentTarget == path.Count - 1)//if the current target is the last
            {
                currentTarget = 0;//sets it back to zero
            }
            else//its not too high
            {
                currentTarget++;//increase it to the next one
            }
        }
        totalForce += seekWeight * Seek(path[currentTarget]);//add the seek force to the total force
        totalForce = Vector3.ClampMagnitude(totalForce, maxForce);//clamp the total force to the max force value
        ApplyForce(totalForce);//apply the total force to the vehicle
    }
}
