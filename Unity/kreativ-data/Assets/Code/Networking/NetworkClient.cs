using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using SocketIO;

public class NetworkClient : SocketIOComponent
{

    Material m_Material;

    [Header("Network Client")]
    [SerializeField]
    private Transform networkContainer;

    private Dictionary<string, GameObject> serverObjects;

    // Start is called before the first frame update
    public override void Start()
    {
        base.Start();
        //Fetch the Material from the Renderer of the GameObject
        m_Material = GetComponent<Renderer>().material;
        initialize();
        setupEvents();
        
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
        Debug.Log(E.data.ToString());
            // Change the Color of the GameObject when the mouse hovers over it
            m_Material.color = Color.red;
        });


    }

}
