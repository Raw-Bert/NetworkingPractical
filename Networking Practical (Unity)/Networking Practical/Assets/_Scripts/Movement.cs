using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Movement : MonoBehaviour
{
    public int speed;
    public int rotateSpeed;


    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        float verticalMove = Input.GetAxis("Vertical") * speed * Time.deltaTime;
        
        float rotate = Input.GetAxis("Horizontal") * rotateSpeed * Time.deltaTime;
    
        transform.Translate(0, 0, verticalMove);
        transform.Rotate(0, rotate, 0);

        
    }
}
