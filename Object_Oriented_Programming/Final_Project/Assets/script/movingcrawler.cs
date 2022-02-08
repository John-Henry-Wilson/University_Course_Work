using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class movingcrawler : MonoBehaviour
{
    public Transform target;
    public float speed;
    void Update()
    {
        //moves crawler from original position to target position 
        float step = speed * Time.deltaTime;
        transform.position = Vector3.MoveTowards(transform.position, target.position, step);
    }
}
