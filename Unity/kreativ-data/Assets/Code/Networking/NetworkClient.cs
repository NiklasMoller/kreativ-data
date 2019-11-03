using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SocketIO;

public class NetworkClient : SocketIOComponent
{


    [Header("Network Client")]
    [SerializeField]
    private Transform networkContainer;

    private Dictionary<string, GameObject> serverObjects;

    private ColorChanger myCube;


    // Start is called before the first frame update
    public override void Start()
    {
        base.Start(); //Always on top of this scope

        initialize();
        setupEvents();

        myCube = GameObject.FindObjectOfType<ColorChanger>();


        
    }

    // Update is called once per frame
    public override void Update()
    {
        base.Update();
    }

    private void initialize()
    {
        serverObjects = new Dictionary<string, GameObject>();
    }

    private void setupEvents()
    {
        On("open", (E) =>
        {
            Debug.Log("Connection made to server");
        });


        On("OSC", (E) =>
        {
        //Debug.Log(E.data.ToString());
            Debug.Log("Got OSC and updating color");
            myCube.OnOSC();
       
        });


    }

}
