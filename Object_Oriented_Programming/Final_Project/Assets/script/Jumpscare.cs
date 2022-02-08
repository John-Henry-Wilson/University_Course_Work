using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Jumpscare : MonoBehaviour
{

    public GameObject jumpscareObject;

    void Start()
    {
        //despawns jumpscare object
        jumpscareObject.SetActive(false);
    }

    void OnTriggerEnter(Collider player)
    {
        //spawns on trigger and destroys trigger as to not respawn
        if (player.tag == "Player")
        {
            jumpscareObject.SetActive(true);
            StartCoroutine(DestroyObject());
        }
    }
    IEnumerator DestroyObject()
    {
        yield return new WaitForSeconds(2f);
        Destroy(jumpscareObject);
        Destroy(gameObject);
    }
}
