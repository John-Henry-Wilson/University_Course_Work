using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class keyscare : MonoBehaviour
{
    //all the blood images
    public GameObject jumpscareObject1;
    public GameObject jumpscareObject2;
    public GameObject jumpscareObject3;
    public GameObject jumpscareObject4;
    public GameObject jumpscareObject5;
    public GameObject jumpscareObject6;
    public GameObject jumpscareObject7;
    public GameObject jumpscareObject8;
    public GameObject jumpscareObject9;

    void Start()
    {
        //despawns them upon game start
        jumpscareObject1.SetActive(false);
        jumpscareObject2.SetActive(false);
        jumpscareObject3.SetActive(false);
        jumpscareObject4.SetActive(false);
        jumpscareObject5.SetActive(false);
        jumpscareObject6.SetActive(false);
        jumpscareObject7.SetActive(false);
        jumpscareObject8.SetActive(false);
        jumpscareObject9.SetActive(false);

    }


    void OnTriggerEnter(Collider collider)
    {
        //spawns them on trigger
        if (collider.gameObject.tag == "Player")
        {
            jumpscareObject1.SetActive(true);
            jumpscareObject2.SetActive(true);
            jumpscareObject3.SetActive(true);
            jumpscareObject4.SetActive(true);
            jumpscareObject5.SetActive(true);
            jumpscareObject6.SetActive(true);
            jumpscareObject7.SetActive(true);
            jumpscareObject8.SetActive(true);
            jumpscareObject9.SetActive(true);
        }

    }

    

}
