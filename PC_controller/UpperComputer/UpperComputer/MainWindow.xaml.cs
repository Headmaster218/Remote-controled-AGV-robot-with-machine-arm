using System;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Net;
using System.Text.Json;
using System.Text.Json.Nodes;
using System.Text.Json.Serialization;
using System.Configuration;
using System.IO;
using System.Diagnostics;
using System.Numerics;
using System.Windows.Threading;

namespace UpperComputer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Thread video_thread, pulse_thread;
        private UdpServer udpServer;
        private bool on_running = true;
        private bool connected = false;
        private User user;

        private string last_send_speed;

        private string[] image_buff;
        private int last_buff_size = 10;

        [Serializable]
        private struct Message
        {
            public string type { get; set; }
            public string data { get; set; }
            public int index { get; set; }
            public int max_index { get; set; }
        }

        [Serializable]
        private struct User
        {
            public string ip;
            public int port;
            public bool init;
            public DateTime last_contact_time;
            public User()
            {
                ip = "0.0.0.0";
                port = 0;
                init = false;
                last_contact_time = DateTime.MinValue;
            }
        }


        public MainWindow()
        {
            InitializeComponent();
            video_thread = new Thread(new ThreadStart(video_handle));
            pulse_thread = new Thread(new ThreadStart(pulse_handle));
            image_buff = new string[last_buff_size];
            udpServer = new UdpServer(25539);
            video_thread.Start();
            pulse_thread.Start();
            last_send_speed = SpeedBox.Text;
        }

        private void Close_Click(object sender, RoutedEventArgs e)
        {
            on_running = false;
            udpServer.Close();
            this.Close();
        }

        private void MoveWindows(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                this.DragMove();
            }

        }

        private void Connect_Click(object sender, RoutedEventArgs e)
        {
            string text = AddressBox.Text;
            string[] ip_port = text.Split(':');
            if (ip_port.Length == 1)
            {
                user.ip = ip_port[0];
                user.port = 25560;
                user.init = true;
            }
            if (ip_port.Length == 2)
            {
                user.ip = ip_port[0];
                user.port = int.Parse(ip_port[1]);
                user.init = true;
            }
        }

        private BitmapImage ByteArrayToBitmapImage(byte[] byteArray)
        {
            using (Stream stream = new MemoryStream(byteArray))
            {
                BitmapImage image = new BitmapImage();
                stream.Position = 0;
                image.BeginInit();
                image.CacheOption = BitmapCacheOption.OnLoad;
                image.StreamSource = stream;
                image.EndInit();
                image.Freeze();
                return image;
            }
        }

        private void video_handle()
        {
            while (on_running)
            {
                if (connected && DateTime.Now.Subtract(user.last_contact_time).TotalMilliseconds > 3000)
                {
                    connected = false;
                    Dispatcher.BeginInvoke(() =>
                    {
                        Connect_Label.Content = "未连接";
                    });
                }
                string message, ip;
                int port;
                bool flag = udpServer.GetRecvMessage(out message, out ip, out port);
                if (!flag)
                {
                    Thread.Sleep(1);
                }
                if (ip == user.ip && port == user.port && user.init)
                {
                    if (!connected)
                    {
                        connected = true;
                        Dispatcher.BeginInvoke(() =>
                        {
                            Connect_Label.Content = "已连接";
                        });
                        
                        Message send_message = new Message();
                        send_message.type = "speed";
                        Dispatcher.Invoke(() =>
                        {
                            send_message.data = SpeedBox.Text;
                        });
                        string send_message_str = JsonSerializer.Serialize(send_message);
                        udpServer.SetSendMsg(send_message_str, user.ip, user.port);
                    }
                    user.last_contact_time = DateTime.Now;
                    Message recv_message = JsonSerializer.Deserialize<Message>(message);
                    if (recv_message.type == "video")
                    {
                        if (recv_message.max_index > last_buff_size)
                        {
                            last_buff_size = recv_message.max_index;
                            image_buff = new string[last_buff_size];
                        }
                        image_buff[recv_message.index - 1] = recv_message.data;
                        if (recv_message.index == recv_message.max_index)
                        {
                            string image_str = "";
                            for (int i = 0; i < recv_message.max_index; i++)
                            {
                                image_str += image_buff[i];
                            }
                            try
                            {
                                byte[] image_bytes = Convert.FromBase64String(image_str);
                                Dispatcher.BeginInvoke(() =>
                                {
                                    video.Source = ByteArrayToBitmapImage(image_bytes);
                                });
                            }
                            catch
                            {
                            }
                        }
                    }
                }
            }
        }

        private void Grid_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.W)
            {
                if (connected)
                {
                    Message message = new Message();
                    message.type = "control";
                    message.data = "front";
                    string send_message = JsonSerializer.Serialize(message);
                    udpServer.SetSendMsg(send_message, user.ip, user.port);
                }
            }
            if (e.Key == Key.S)
            {
                if (connected)
                {
                    Message message = new Message();
                    message.type = "control";
                    message.data = "back";
                    string send_message = JsonSerializer.Serialize(message);
                    udpServer.SetSendMsg(send_message, user.ip, user.port);
                }
            }
            if (e.Key == Key.A)
            {
                if (connected)
                {
                    Message message = new Message();
                    message.type = "control";
                    message.data = "left";
                    string send_message = JsonSerializer.Serialize(message);
                    udpServer.SetSendMsg(send_message, user.ip, user.port);
                }
            }
            if (e.Key == Key.D)
            {
                if (connected)
                {
                    Message message = new Message();
                    message.type = "control";
                    message.data = "right";
                    string send_message = JsonSerializer.Serialize(message);
                    udpServer.SetSendMsg(send_message, user.ip, user.port);
                }
            }
            if (e.Key == Key.Up)
            {
                if (connected)
                {
                    Message message = new Message();
                    message.type = "control";
                    message.data = "turn front";
                    string send_message = JsonSerializer.Serialize(message);
                    udpServer.SetSendMsg(send_message, user.ip, user.port);
                }
            }
            if (e.Key == Key.Down)
            {
                if (connected)
                {
                    Message message = new Message();
                    message.type = "control";
                    message.data = "turn back";
                    string send_message = JsonSerializer.Serialize(message);
                    udpServer.SetSendMsg(send_message, user.ip, user.port);
                }
            }
            if (e.Key == Key.Left)
            {
                if (connected)
                {
                    Message message = new Message();
                    message.type = "control";
                    message.data = "turn left";
                    string send_message = JsonSerializer.Serialize(message);
                    udpServer.SetSendMsg(send_message, user.ip, user.port);
                }
            }
            if (e.Key == Key.Right)
            {
                if (connected)
                {
                    Message message = new Message();
                    message.type = "control";
                    message.data = "turn right";
                    string send_message = JsonSerializer.Serialize(message);
                    udpServer.SetSendMsg(send_message, user.ip, user.port);
                }
            }
            if (e.Key == Key.Space)
            {
                if (connected)
                {
                    Message message = new Message();
                    message.type = "control";
                    message.data = "stop";
                    string send_message = JsonSerializer.Serialize(message);
                    udpServer.SetSendMsg(send_message, user.ip, user.port);
                }
            }
        }

        private void textBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            string new_str = "";
            for(int i = 0; i < SpeedBox.Text.Length; i++)
            {
                if (SpeedBox.Text[i] >= '0' && SpeedBox.Text[i] <= '9')
                {
                    new_str += SpeedBox.Text[i];
                }
                if (SpeedBox.Text[i] == '.' || SpeedBox.Text[i] == '-')
                {
                    new_str += SpeedBox.Text[i];
                }
            }
            SpeedBox.Text = new_str;
            SpeedBox.SelectionStart = SpeedBox.Text.Length;
            if (SpeedBox.Text != last_send_speed && connected)
            {
                Message message = new Message();
                message.type = "speed";
                message.data = SpeedBox.Text;
                string send_message = JsonSerializer.Serialize(message);
                udpServer.SetSendMsg(send_message, user.ip, user.port);
            }
        }

        private void video_MouseDown(object sender, MouseButtonEventArgs e)
        {
            video.Focus();
        }

        private void ___blue_png_MouseDown(object sender, MouseButtonEventArgs e)
        {
            ___blue_png.Focus();
        }

        private void pulse_handle()
        {
            while (on_running)
            {
                if (user.init)
                {
                    Message send_message = new Message();
                    send_message.type = "pulse";
                    send_message.data = "";
                    send_message.max_index = -1;
                    send_message.index = -1;
                    string send_str = JsonSerializer.Serialize(send_message);
                    udpServer.SetSendMsg(send_str, user.ip, user.port);
                }
                Thread.Sleep(500);
            }
        }
        
    }
}
