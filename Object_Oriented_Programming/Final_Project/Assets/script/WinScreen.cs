using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WinScreen : MonoBehaviour
{
    public GameObject screenofwin;   

    // Update is called once per frame
    void Update()
    {
        if (ScoreUpdate.keys == 4)
        {
            
            Time.timeScale = 0;
            Debug.Log("WIN");
            screenofwin.SetActive(true);


        }
    }
}
