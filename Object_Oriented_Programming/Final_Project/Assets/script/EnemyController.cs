using System.Collections;
using System.Collections.Generic;
using UnityEngine.AI;
using UnityEngine;

public class EnemyController : MonoBehaviour
{

    Transform target;
    NavMeshAgent agent;
    FieldOfView fov;

    bool running;

    public Transform[] objs = new Transform[2];
    public float speed;
    int state = 0;
    Vector3[] positions = new Vector3[2];

    void Start()
    {
        target = PlayerManager.instance.player.transform;
        agent = GetComponent<NavMeshAgent>();
        fov = GetComponent<FieldOfView>();

        for (int i = 0; i < objs.Length; i++)
        {
            positions[i] = objs[i].position;
        }
        if(ScoreUpdate.keys == 0)
        {
            StartCoroutine("LoopMove1");
        }
        


    }

    IEnumerator LoopMove1()
    {
        running = true;
        while (true)
        {
            for (int i = 0; i < objs.Length; i++)
            {

                int cur = state + i; // "i" is a current object. i+state makes for us a number of the position that the object must go to.

                if (cur > objs.Length - 1)
                {
                    cur -= objs.Length; // if cur is higher than the number of objects, then we loop it by decreasing by the number of the objects.
                }


                if (objs[i].position != positions[cur])
                {

                    // If the current object hasnt reached the destination yet, then we move it towards it
                    objs[i].position = Vector3.MoveTowards(objs[i].position, positions[cur], speed * Time.deltaTime);

                }
                else
                {

                    // else we chose next destination point
                    state++;

                }
            }

            // If state exeedes the number of objects, then we zero it
            if (state >= objs.Length)
                state = 0;

            yield return null;
        }
    }

        void Update()
    {

        if (!running)
        {
            if (fov.getSeePlayer())
            {
                agent.SetDestination(target.position);
                FaceTarget();
            }
            else
            {
                agent.SetDestination(transform.position);
            }
        }
        
       
    }

    void FaceTarget()
    {
        Vector3 direction = (target.position - transform.position).normalized;
        Quaternion lookRotation = Quaternion.LookRotation(new Vector3(direction.x, 0, direction.z));
        transform.rotation = Quaternion.Slerp(transform.rotation, lookRotation, Time.deltaTime * 5f);

    }
}
