using System.Runtime.InteropServices;
using UnityEngine;
using Unity.Jobs;
using static Networking;
using static Networking.PResult;

public class NetworkControl : MonoBehaviour
{
    SocketData sock;
    IPEndpointData endp;
    NetworkServerJob jobServer;
    NetworkClientJob jobClient;
    JobHandle hnd;
    static bool up, down, left, right;

    [HideInInspector] public static bool close;

    [StructLayout(LayoutKind.Sequential)]
    struct vec3
    {
        public float x, y, z;
        public vec3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        public static vec3 operator -(vec3 v1, vec3 v2) => new vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);

        public float length() => Mathf.Sqrt(x * x + y * y + z * z);

        public static float dist(vec3 v1, vec3 v2) => (v1 - v2).length();


        public override string ToString() => "(" + x + ", " + y + ", " + z + ")";
    }

    void PrintError(object msg) => Debug.LogError(msg);
    public struct NetworkServerJob : IJob
    {
        public SocketData sock;
        public IPEndpointData endp;

        public void Execute()
        {
            int size = 0, dump = 0;
            vec3 dat = new vec3();
            for (; ; )
            {
                bool printit = true;

                if (recvFromPacket(ref sock, ref size, 4, ref dump, ref endp) == P_GenericError)
                {
                    printit = false; Debug.LogError(getLastNetworkError());

                }



                if (recvFromPacket(ref sock, ref dat, size, ref dump, ref endp) == P_GenericError)
                {
                    printit = false; Debug.LogError(getLastNetworkError());
                }

                if (printit)
                    print(dat);
                //}

                Debug.LogError(getLastNetworkError());

                if (NetworkControl.close)
                    break;
            }
            //  gotsomething = true;

        }
    }
    public struct NetworkClientJob : IJob
    {
        //int size, dump;
        public SocketData sock;
        public IPEndpointData endp;
        public void Execute()
        {

            for (; ; )
            {
                string tmp = "hello?";
                sendToPacket(ref sock, ref tmp, ref endp);
                if (NetworkControl.close)
                    break;
            }
            //  gotsomething = true;

        }
    }


    private void Awake()
    {
    
        //UDP Client
        shutdownNetwork();//just incase there was some sort of error last time
        Networking.initNetwork();
        endp = createIPEndpointData("10.150.32.203", 8888);
        sock = initSocketData();

        if (createSocket(ref sock, SocketType.UDP) == P_GenericError)
            Debug.LogError(getLastNetworkError());

        if (connectEndpoint(ref endp, ref sock) == P_GenericError)
            Debug.LogError(getLastNetworkError());

        jobClient = new NetworkClientJob()
        {
            sock = this.sock,
            endp = this.endp
        };
        hnd = jobClient.Schedule();//schedules the job to start asynchronously like std::detach in c++

    }

    private void Update()
    {
        if (Input.GetKey(KeyCode.A))
            left = true;
        else left = false;

        if (Input.GetKey(KeyCode.W))
            up = true;
        else
            up = false;

        if (Input.GetKey(KeyCode.S))
            down = true;
        else down = false;

        if (Input.GetKey(KeyCode.D))
            right = true;
        else right = false;
    }

    private void OnApplicationQuit()
    {
        close = true;


        closeSocket(ref sock);
        shutdownNetwork();
        hnd.Complete();//should be the same as thread::join in c++
    }
}
