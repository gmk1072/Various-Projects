using UnityEngine;
using System.Collections;

public class Spin : MonoBehaviour
{
    private Vector3 rotationAxis = Vector3.up;//will rotate around the y axis 
    public float rotationSpeed = 5.0f;//the speed to rotate at

    /// <summary>
    /// will be called once when this class is first used
    /// </summary>
    void Start()
    {

    }

    /// <summary>
    /// called once per frame
    /// </summary>
    void Update()
    {
        transform.Rotate(rotationAxis, rotationSpeed * Time.deltaTime);//rotates at the speed accounting for frame time
    }
}
