using System;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Numerics;
using System.Text;
using System.Threading;


namespace Net
{
    class UDPMessage
    {
        public UDPMessage(string message, string ip, int port)
        {
            this.Message = message;
            this.ip = ip;
            this.port = port;
        }

        public string Message { get; set; }
        public string ip { get; set; }
        public int port { get; set; }

        public int index { get; set; }
    }

    class UdpServer
    {
        private Socket server;
        private Thread recv_thread, send_thread;
        private Mutex recv_lock = new Mutex();
        private Mutex send_lock = new Mutex();
        private Queue recv_message;
        private Queue send_meassage;
        private bool on_running = true;

        public UdpServer() {; }

        public UdpServer(int port)
        {
            server = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            server.Bind(new IPEndPoint(IPAddress.Parse("0.0.0.0"), port));               //UDP通讯时，必须绑定自己的IP和端口号
            recv_message = new Queue();
            send_meassage = new Queue();
            recv_thread = new Thread(this.ReciveMsgThread);
            recv_thread.Start();
            send_thread = new Thread(this.SendMsgThread);
            send_thread.Start();
        }

        ~UdpServer()
        {
            Close();
        }
        
        /// <summary>
        /// 接收发送给本机ip对应端口号的数据报
        /// </summary>
        private void ReciveMsgThread()
        {
            while (on_running)
            {
                EndPoint point = new IPEndPoint(IPAddress.Any, 0);
                byte[] buffer = new byte[1024 * 64];
                try
                {
                    int length = server.ReceiveFrom(buffer, ref point);
                    string message = Encoding.UTF8.GetString(buffer, 0, length);
                    recv_lock.WaitOne();
                    recv_message.Enqueue(new UDPMessage(message, (point as IPEndPoint).Address.ToString(), (point as IPEndPoint).Port));
                    if(recv_message.Count > 10)
                    {
                        recv_message.Dequeue();
                    }
                    recv_lock.ReleaseMutex();
                }
                catch
                {
                    continue;
                }
            }
        }

        /// <summary>
        /// 给指定ip的主机端口发送数据报
        /// </summary>
        private void SendMsgThread()
        {
            while (on_running)
            {
                send_lock.WaitOne();
                while (send_meassage.Count > 0)
                {
                    UDPMessage msg = send_meassage.Dequeue() as UDPMessage;
                    byte[] buffer = Encoding.UTF8.GetBytes(msg.Message);
                    try
                    {
                        server.SendTo(buffer, new IPEndPoint(IPAddress.Parse(msg.ip), msg.port));
                    }
                    catch
                    {
                        continue;
                    }
                }
                send_lock.ReleaseMutex();
                Thread.Sleep(1);
            }
        }

        public bool GetRecvMessage(out string msg, out string ip, out int port)
        {
            if (recv_message.Count > 0)
            {
                recv_lock.WaitOne();
                UDPMessage message = recv_message.Dequeue() as UDPMessage;
                recv_lock.ReleaseMutex();
                msg = message.Message;
                ip = message.ip;
                port = message.port;
                return true;
            }
            else {
                msg = "";
                ip = "0.0.0.0";
                port = 0;
                return false;
            }
        }

        public void SetSendMsg(string msg, string ip, int port)
        {
            send_lock.WaitOne();
            send_meassage.Enqueue(new UDPMessage(msg, ip, port));
            if(send_meassage.Count > 50)
            {
                send_meassage.Dequeue();
            }
            send_lock.ReleaseMutex();
        }

        public void Close()
        {
            server.Close();
            on_running = false;
        }
    }
}