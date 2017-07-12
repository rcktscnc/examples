using System;
using System.Text;
using System.Windows;
using System.Net.Sockets;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for ConnectionWindow.xaml
    /// </summary>
    public partial class ConnectionWindow : Window
    {
        public ConnectionWindow()
        {
            InitializeComponent();
        }

        public void button_connect(object sender, RoutedEventArgs e)
        {

            if (text_box_nickname.Text.Length < 3 ||
                text_box_nickname.Text.Length > 12 ||
                text_box_nickname.Text.Length == 0 ||
                text_box_nickname.Text.Contains(" "))
            {
                text_box_nickname.Text = "invalid name";
                return;
            }

            try
            {
                Socket to_server = new Socket(AddressFamily.InterNetwork,
                    SocketType.Stream, ProtocolType.Tcp);

                Socket from_server = new Socket(AddressFamily.InterNetwork,
                    SocketType.Stream, ProtocolType.Tcp);

                Socket from_server_users = new Socket(AddressFamily.InterNetwork,
                    SocketType.Stream, ProtocolType.Tcp);

                to_server.Connect(text_box_ip.Text, Int32.Parse(text_box_port.Text));
                from_server.Connect(text_box_ip.Text, Int32.Parse(text_box_port.Text));
                from_server_users.Connect(text_box_ip.Text, Int32.Parse(text_box_port.Text));

                byte[] buf = Encoding.UTF8.GetBytes(text_box_nickname.Text + "\0");

                to_server.Send(BitConverter.GetBytes(buf.Length));
                to_server.Send(buf);

                MainWindow mw = new MainWindow(to_server, from_server,
                    from_server_users, text_box_nickname.Text);

                mw.Show();
                this.Close();
            }
            catch (Exception ex)
            {

            }
        }
    }
} // namespace
