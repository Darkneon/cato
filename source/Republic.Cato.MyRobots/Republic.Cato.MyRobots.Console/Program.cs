using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Configuration;

namespace Republic.Cato.MyRobots.Console
{
    using Console = System.Console;
    class Program
    {
        static void Main(string[] args)
        {
            /*
            var reader = new RobotChannelReader("http://bots.myrobots.com/channels/{0}/feed.xml?key={1}", "67", "B94DD85F4ED8430B");
            var channel = reader.ReadChannel();
            Console.WriteLine("Name: {0}", channel.Name);
            Console.WriteLine("Created: {0}", channel.Description);
            Console.WriteLine("Feeds:");

            foreach (var feed in channel.Feeds)
            {
                Console.WriteLine("Created: {0}", feed.Created);
                Console.WriteLine("Entry ID: {0}", feed.EntryId);
                Console.WriteLine();
                Console.WriteLine("Values:");
                foreach (var field in feed.Values)
                {
                    Console.WriteLine("{0} = {1}", field.Key, field.Value);
                }
                Console.WriteLine();
            }*/
            var notification = new SMSNotification();
            notification.PhoneNumber = Properties.Settings.Default.PhoneNumber;
            notification.GatewayEmail = Properties.Settings.Default.GatewayEmail;
            notification.Content = "Test";
            notification.Send();
            
        }
    }
}
