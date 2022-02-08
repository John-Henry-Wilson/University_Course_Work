using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FootStepScript : MonoBehaviour
{

	public AudioSource footStep;
	private bool IsMoving;

	// Update is called once per frame
	void Update()
	{
		//checks if moving
		if ((Input.GetAxis("Horizontal") != 0f || Input.GetAxis("Vertical") != 0f))
		{
			IsMoving = true;
		}
		else
		{
			IsMoving = false;
		}

		//if moving and footsteps arnt playing play footsteps
		if (IsMoving && !footStep.isPlaying)
		{
			footStep.Play();
		}
		//if not moving stop footsteps
		if (!IsMoving)
		{
			footStep.Stop();
		}
	}



}
