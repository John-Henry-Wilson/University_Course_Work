using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ObjectPickup : MonoBehaviour
{
    public GameObject theEnemy;

    


    // Start is called before the first frame update
    void OnTriggerEnter(Collider collider)
    {
        //checks if object is colliding with player
        if (collider.gameObject.tag == "Player")
        {
            //updates score and destroys item
            ScoreUpdate.keys += 1;
            Destroy(gameObject);
            print("destroyed");
        }

        //if score is now 2, spawns new monster
        if (ScoreUpdate.keys == 2 && !ScoreUpdate.secondspawn)
        {
            Instantiate(theEnemy, new Vector3(8, 0, 22), Quaternion.Euler(0f, 90f, 0f) );
            ScoreUpdate.secondspawn = true;
        }
        //if score is now 3, spawns new monster
        if (ScoreUpdate.keys == 3 && !ScoreUpdate.thirdspawn)
        {
            Instantiate(theEnemy, new Vector3(-7, 0, -41), Quaternion.identity);
            ScoreUpdate.thirdspawn = true;
        }

    }
}
