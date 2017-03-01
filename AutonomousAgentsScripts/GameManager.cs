
using UnityEngine;
using System.Collections;

//add using System.Collections.Generic; to use the generic list format
using System.Collections.Generic;

/// <summary>
/// This managers parts of the program that overarch or connect multiple classes and objects
/// it manages the flock and instantiates most, if not all, of the objects in the scene
/// it holds each of the prefabs used and acts and as an overall place to put any code that is too general for a class by itself
/// </summary>
public class GameManager : MonoBehaviour
{
    public GameObject drone;//holds the currently accessed drone object
    public GameObject astronaut;//holds the currently accessed astronaut object
    public GameObject mothership;//holds the mothership, there is only one
    public GameObject astronautLeader;//holds the astronaut leader,the one wandering not leader following, there is only one

    //each of the prefabs for the drone, astronaut, the three obstacles and the mothership.
    public GameObject dronePrefab;
    public GameObject astronautPrefab;
    public GameObject obstaclePrefab1;
    public GameObject obstaclePrefab2;
    public GameObject obstaclePrefab3;
    public GameObject mothershipPrefab;

    public GameObject[] Obstacles { get { return obstacles; } }//accessor for the obstacles array
    private GameObject[] obstacles;//the actual obstacles array
    private Vector3 centroid;//the centroid of the flock of drones
    private Vector3 direction;//the average direction of the flock of drones

    //each of the accessors for the centroid and direciton of the flock
    public Vector3 Centroid { get { return centroid; } }
    public Vector3 Direction { get { return direction; } }

    //the flock list and its accessor
    private List<GameObject> flock;
    public List<GameObject> Flock { get { return flock; } }

    //the astronaut list and its accessor
    private List<GameObject> astronauts;
    public List<GameObject> Astronauts { get { return astronauts; } }

    protected GameManager gm;//the game manager
    protected SmoothFollow cam;//the camera's smoothfollow to set its target and access it
    Transform[] cameraTargets;//list of transforms to store each of the possible targets for the camera to follow
    int cameraCount;//the counter that tracks which camera target it is at
    protected Transform leaderPos;//the transform for following the astronaut leader and such
    private int numberFlockers;//the number of flockers in the list to easily keep track of when trying to find average direciton and position

    //the empty game objects used to hold each of the layers of asteroids
    private GameObject astLayer1;
    private GameObject astLayer2;
    private GameObject astLayer3;

    /// <summary>
    /// will be called once when this class is first used
    /// </summary>
    void Start()
    {
        //getting the game manager, camera smooth follow, asteroid layers, and astronaut leader position objects
        gm = GameObject.Find("GameManagerGO").GetComponent<GameManager>();
        cam = GameObject.Find("Main Camera").GetComponent<SmoothFollow>();
        leaderPos = GameObject.Find("LeaderPos").transform;
        astLayer1 = GameObject.Find("Asteroid Layer 1");
        astLayer2 = GameObject.Find("Asteroid Layer 2");
        astLayer3 = GameObject.Find("Asteroid Layer 3");

        Vector3 pos = new Vector3();//vector that is used for position numerous times
        cameraTargets = new Transform[3];//array of the different camera targets for following
        cameraTargets[2] = gm.transform;//sets the gamemager which will follow the centroid of the flock of drones as one of the camera targets

        pos = new Vector3(136, 50, 585);//starting position for the mothership
        mothership = (GameObject)Instantiate(mothershipPrefab, pos, Quaternion.identity);//makes the mothership
        cameraTargets[0] = mothership.transform;//sets the mothership as the next target for the camera to follow
        cam.target = mothership.transform;//starts the camera off with following the mothership when you first play

        astronauts = new List<GameObject>();//sets up astronauts, there will be 20 that follow the leader
        for (int i = 0; i < 20; i++)
        {
            pos = Vector3.zero;//sets pos to the origin
            pos += new Vector3(Random.Range(-5, 5), -50, Random.Range(-5, 5));//they are placed randomly around pos, with a -50 in the y
            astronaut = (GameObject)Instantiate(astronautPrefab, pos, Quaternion.identity);//makes an astronaut
            astronauts.Add(astronaut);//adds that astronaut to the list to keep track of
        }

        pos = Vector3.zero;//resets pos to the origin
        pos += new Vector3(Random.Range(-5, 5), -50, Random.Range(-5, 5));//it is placed randomly around pos, with a -50 in the y
        astronautLeader = (GameObject)Instantiate(astronautPrefab, pos, Quaternion.identity);//instantiates the leader
        astronautLeader.transform.Rotate(new Vector3(Random.Range(0, 360), Random.Range(0, 360), Random.Range(0, 360)));//rotates in a random direction to randomize the movement direction
        astronautLeader.GetComponent<LeaderFollowing>().leader = true;//this astronaut is the leader
        astronautLeader.GetComponent<LeaderFollowing>().maxSpeed *= 1.05f;//makes this astronaut a little faster since it is wandering and not seeking

        cameraTargets[1] = leaderPos;//makes the leader pos one of the smooth follow camera targets

        flock = new List<GameObject>();//sets up the flock for creation
        for (int i = 0; i < 40; i++)//there is 40 of them in the flock
        {
            pos = gm.transform.position;//sets pos to the game managers position
            pos += new Vector3(Random.Range(-5, 5), Random.Range(-5, 5), Random.Range(-5, 5));//randomly placed around that pos
            drone = (GameObject)Instantiate(dronePrefab, pos, Quaternion.identity);//instantiates a drone
            flock.Add(drone);//adds that drone to the flock
            numberFlockers++;//increases the amount of flockers to keep track for later
        }
        Seeker.seekerTarget = astronautLeader;//the static target is being set to the astronaut leader

        obstacles = new GameObject[300];//there is 300 obstacles
        for (int i = 0; i < 100; i++)
        {
            pos = astLayer1.transform.position;
            pos += new Vector3(Random.Range(-200, 200), Random.Range(-200, 200), Random.Range(-200, 200));//this is between -200 and 200
            Quaternion rot = Quaternion.Euler(Random.Range(0, 360), Random.Range(0, 360), Random.Range(0, 360));//rotates it
            obstacles[i] = (GameObject)Instantiate(obstaclePrefab1, pos, rot);//instantiates and adds it to the array
            obstacles[i].transform.parent = astLayer1.transform;//parents it under its layer
        }
        for (int i = 0; i < 100; i++)
        {
            pos = astLayer2.transform.position;
            pos += new Vector3(Random.Range(-300, 300), Random.Range(-300, 300), Random.Range(-300, 300));//this is between -300 and 300
            Quaternion rot = Quaternion.Euler(Random.Range(0, 360), Random.Range(0, 360), Random.Range(0, 360));//rotates it
            obstacles[i+100] = (GameObject)Instantiate(obstaclePrefab2, pos, rot);//instantiates and adds it to the array
            obstacles[i + 100].transform.parent = astLayer2.transform;//parents it under its layer
        }
        for (int i = 0; i < 100; i++)
        {
            pos = astLayer3.transform.position;
            pos += new Vector3(Random.Range(-250, 250), Random.Range(-250, 250), Random.Range(-250, 250));//this is between -250 and 250
            Quaternion rot = Quaternion.Euler(Random.Range(0, 360), Random.Range(0, 360), Random.Range(0, 360));//rotates it
            obstacles[i + 200] = (GameObject)Instantiate(obstaclePrefab3, pos, rot);//instantiates and adds it to the array
            obstacles[i + 200].transform.parent = astLayer3.transform;//parents it under its layer
        }
    }

    /// <summary>
    /// called once per frame
    /// </summary>
    void Update()
    {
        if (Seeker.collectedAstro)//if the drones have collected there astronaut they should now target the mothership to drop it off
        {
            Seeker.seekerTarget = mothership;
        }
        else//otherwise, they have not yet collected their astronaut
        {
            if (astronauts.Count == 0 && Seeker.done)//if there are no more astronauts in the list and the seekers are done(the leader is collected as well), then just target the mothership
            {
                Seeker.seekerTarget = mothership;
            }
            else//they aren't done and the count isn't 0, then target the astronaut leader
            {
                Seeker.seekerTarget = astronautLeader;
            }
        }

        foreach (GameObject f in flock)//for all of the flockers do this
        {
            float dist = Vector3.Distance(Seeker.seekerTarget.transform.position, f.transform.position);//dist is the distance between the seekers' target and the current flockers
            if (dist < 15f)//if that distance is less than 15
            {
                if (!Seeker.collectedAstro)//flockers have not collected their astronaut yet
                {
                    if (astronauts.Count != 0)//there are still astronaut left in the list
                    {
                        GameObject.Destroy(astronauts[astronauts.Count - 1]);//destroy one of the astronauts
                        astronauts.RemoveAt(astronauts.Count - 1);//remove it from the list
                    }
                    else if(astronauts.Count ==0 && !Seeker.collectedAstro)//the seekers have not collected an astronaut but there are none left in the list
                    {
                        GameObject.Destroy(astronautLeader);//destroy the leader
                        Seeker.done = true;//the seekers are now done and should just keep following the mother ship
                    }
                    Seeker.collectedAstro = true;//they have now collected an astronaut(even if it is the last it will still always follow the mothership because of the done bool)

                }
                else//flockers have collected their astronaut
                {
                    Seeker.collectedAstro = !Seeker.collectedAstro;//they have now dropped off the astronaut at the mothership and should now try to go back unless they are done
                }
            }
            CalcFlockDirection();//calls the calcFlockDirection method below which finds the average direction for the game manager to hold
            CalcCentroid();//calls the calcCentroid method below which finds the average position for the game manager to follow and hold
        }
        if (!Seeker.done)//if the seekers aren't done(there is still a leader)
        {
            leaderPos.position = astronautLeader.transform.position + (astronautLeader.transform.forward * -14);//the astronaut leader position = the actual position of the leader minus a little so the other astronauts don't crowd around the actual leader
            leaderPos.transform.forward = astronautLeader.transform.forward;//gets the direction for leaderPos
            leaderPos.rotation = Quaternion.Euler(new Vector3(astronautLeader.transform.rotation.x, astronautLeader.transform.rotation.y, astronautLeader.transform.rotation.z));//rotates the leaderPos accordingly
        }
        else//the seekers are done
        {
            cameraTargets[1] = cameraTargets[0];//sets the camera target for the leader pos to the mothership
        }
        if (Input.GetKeyDown(KeyCode.RightArrow))//if you press the right arrow on the keyboard
        {
            cameraCount++;//increases the cameracount variable
            if (cameraCount == cameraTargets.Length)//if the cameracount is too high brings it back around to the beginning to wrap it
            {
                cameraCount = 0;
            }
            cam.transform.position = cameraTargets[cameraCount].position;//sets the camera position to its target to make it more instantaneous
            cam.target = cameraTargets[cameraCount];//sets the camera target to the one designated by cameracount
        }
        if (Input.GetKeyDown(KeyCode.LeftArrow))//if you press the left arrow on the keyboard
        {
            cameraCount--;//decreases the cameracount variable
            if (cameraCount < 0)//if the cameracount is too low brings it back around to the end to wrap it
            {
                cameraCount = cameraTargets.Length - 1;
            }
            cam.transform.position = cameraTargets[cameraCount].position;//sets the camera position to its target to make it more instantaneous
            cam.target = cameraTargets[cameraCount];//sets the camera target to the one designated by cameracount
        }
    }

    /// <summary>
    /// calculates the centroid of the flock
    /// </summary>
    private void CalcCentroid()
    {

        Vector3 total = new Vector3();//sets up a totatl vector to store the total position
        for (int i = 0; i < numberFlockers; i++)//for each of the flockers
        {
            total += flock[i].transform.position;//adds to the total the position of the current flocker
        }
        total /= numberFlockers;//divides the total by the number of flockers to find the average position
        centroid = total;//sets the centroid
        gm.transform.position = centroid;//sets the position of the game manager to the centroid
    }

    /// <summary>
    /// calculates the average direction of the flock
    /// </summary>
    private void CalcFlockDirection()
    {
        Vector3 direction2 = new Vector3();//sets up a direction vector fo the totoal direction
        for (int i = 0; i < numberFlockers; i++)//for each of the flockers
        {
            direction2 += flock[i].transform.forward;//adds each of the flockers forward vectors to the direction vector
        }
        direction2.Normalize();//normalizes the direction vector
        direction = direction2;//sets the average direction to the calculated direction
        gm.transform.forward = direction;//sets the game manager's direction to the average direction of the flock.
    }
}
