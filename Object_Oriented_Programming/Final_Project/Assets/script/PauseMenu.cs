using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PauseMenu : MonoBehaviour
{
    public static bool isGamePause = false;

    public GameObject background;
    

    void Update()
    {
        //if escape is pressed
        if (Input.GetKeyDown(KeyCode.Escape)) 
        {
            if (isGamePause)
            {
                ResumeGame();
            }
            else 
            {
                PauseGame();
            }
        }
        //already paused and escape is pressed
        if (isGamePause && Input.GetKeyDown(KeyCode.R)) 
        {
            ResumeGame();
        }
        //already paused and game is quit
        if (isGamePause && Input.GetKeyDown(KeyCode.Q))
        {
            QuitGame();
        }
    }

    public void ResumeGame() 
    {
       //turns off background and unfreezes game
        background.SetActive(false);
        Time.timeScale = 1f;
        isGamePause = false;
    }

    public void PauseGame() 
    {
        //turns on background and freezes game
        background.SetActive(true);
        Time.timeScale = 0f;
        isGamePause = true;
    }

    public void QuitGame() 
    {

        Application.Quit();
        
        Debug.Log("Quit");
    }

}
