using System;
using System.Text;
using System.Windows;
using System.Windows.Input;
using System.Net.Sockets;
using System.Threading;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Thread _receiver_thread;
        private Thread _receiver_thread_users;

        private Socket _to_server;
        private Socket _from_server;
        private Socket _from_server_users;

        private string _user_name;

        private enum Output { MESSAGES, USERS };

        public MainWindow(Socket to_server, Socket from_server,
            Socket from_server_users, string user_name)
        {
            InitializeComponent();

            MouseDown += mouse_down_drag;

            text_box_input.Focus();

            _to_server = to_server;
            _from_server = from_server;
            _from_server_users = from_server_users;
            _user_name = user_name;
        
            try
            {
                _receiver_thread =
                    new Thread(() => receiver(_from_server, Output.MESSAGES));
                _receiver_thread.IsBackground = true;
                _receiver_thread.Start();

                _receiver_thread_users =
                    new Thread(() => receiver(_from_server_users, Output.USERS));
                _receiver_thread_users.IsBackground = true;
                _receiver_thread_users.Start();
            }
            catch (Exception ex)
            {
                text_box_output.AppendText("\n" + ex.Message);
            }
        }

        private void message_dispatcher(Output output, byte[] buf_msg)
        {
            // Removes last byte received -- last byte was \0 (null character).
            Array.Resize(ref buf_msg, buf_msg.Length - 1);

            if (output == Output.MESSAGES)
            {
                Dispatcher.BeginInvoke(new Action(() => {
                    text_box_output.AppendText("\n" +
                        DateTime.Now.ToString("HH:mm:ss - ") +
                        Encoding.UTF8.GetString(buf_msg));

                    text_block_output_scroll.ScrollToBottom();
                }));
            }

            if (output == Output.USERS)
            {
                Dispatcher.BeginInvoke(new Action(() => {
                    text_block_users.Text =
                        Encoding.UTF8.GetString(buf_msg);
                }));
            }
        }

        private void receiver(Socket source, Output output)
        {
            while (true)
            {
                try
                {
                    byte[] buf_msg_size = new byte[4];

                    source.Receive(buf_msg_size, buf_msg_size.Length,
                        SocketFlags.None);
                
                    byte[] buf_msg =
                        new byte[BitConverter.ToUInt32(buf_msg_size, 0)];
                
                    source.Receive(buf_msg, buf_msg.Length, SocketFlags.None);

                    message_dispatcher(output, buf_msg);
                }
                catch (Exception ex)
                {
                    Dispatcher.BeginInvoke(new Action(() => {
                        text_box_output.AppendText("\n" + ex.Message);
                    }));

                    return;
                }
            }
        }

        private void sender()
        {
            if (text_box_input.Text.Length == 0)
            {
                text_box_input.Focus();
                return;
            }
            
            try
            {
                string msg = _user_name + ":\n" + text_box_input.Text + "\0";

                byte[] buf = Encoding.UTF8.GetBytes(msg);

                _to_server.Send(BitConverter.GetBytes(buf.Length));
                _to_server.Send(buf);

                text_box_input.Clear();
                text_box_input.Focus();
            }
            catch (Exception ex)
            {
                text_box_output.AppendText("\n" + ex.Message);
            }
        }

        private void button_send_click(object sender, RoutedEventArgs e)
        {
            this.sender();
        }

        private void text_box_input_keydown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
            {
                this.sender();
            }
        }

        private void menu_exit(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void mouse_down_drag(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                DragMove();
        }

        private void test1(object sender, RoutedEventArgs e)
        {
            _from_server.Close();
        }
    }
} // namespace