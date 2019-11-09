using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnitySocketIO.Events;
using UnitySocketIO;

public class OnConnectionHandler : MonoBehaviour
{

    public SocketIOController niklasIo;
    public ColorHandler myCube;


        // Start is called before the first frame update
    void Start()
        {

        myCube = GameObject.FindObjectOfType<ColorHandler>();
   

        niklasIo.On("connect", (SocketIOEvent e) =>
            {
                Debug.Log("SocketIO connected");
            });

            niklasIo.On("OSC", (SocketIOEvent e) =>
            {
                Debug.Log("Receievd OSC!");
                myCube.ChangeColor();
             
            });

        niklasIo.Connect();

    }

        // Update is called once per frame
        void Update()
        {

        }
    }
