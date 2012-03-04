using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Mail;

namespace Republic.Cato.MyRobots
{
    public class SMSNotification
    {
        public string PhoneNumber { get; set; }
        public string GatewayEmail { get; set; }
        public string Content { get; set; }

        public void Send()
        {
            var message = new MailMessage();
            message.Body = Content;
            message.IsBodyHtml = false;
            message.To.Add(String.Format(GatewayEmail, PhoneNumber));

            using (var smtpClient = new SmtpClient())
            {
                smtpClient.Send(message);
            };
        }
    }
}
