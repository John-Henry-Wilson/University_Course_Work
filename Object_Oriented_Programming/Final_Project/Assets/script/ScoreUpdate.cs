using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ScoreUpdate : MonoBehaviour
{
    public static int keys;
    public GameObject keyText;

    public static bool secondspawn = false;
    public static bool thirdspawn = false;


    // Start is called before the first frame update
    void Update()
    {
           
            //updating ui
            keyText.GetComponent<Text>().text = "Keys: " + keys + "/4";

            
            
    }
}
