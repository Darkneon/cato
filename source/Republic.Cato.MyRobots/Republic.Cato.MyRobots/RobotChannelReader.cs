using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Xml.Linq;

namespace Republic.Cato.MyRobots
{
    public class RobotChannelReader
    {

        /// <summary>
        /// The URI from which the feeds will be retrieved
        /// </summary>
        public string ServiceUri { get; private set;  }

        /// <summary>
        /// The name of the channel from which feeds will be retrieved
        /// </summary>
        public string ChannelName { get; private set; }


        /// <summary>
        /// The API key for the robot
        /// </summary>
        public string Key { get; set; }


        /// <summary>
        /// Retrieves a feed from the Web service
        /// </summary>
        /// <param name="serviceUri">the service URI</param>
        /// <param name="channelName">The name of the channel</param>
        /// <param name="key">The key</param>
        public RobotChannelReader(string serviceUri, string channelName, string key)
        {
            if (String.IsNullOrEmpty(serviceUri))
            {
                throw new ArgumentException("'serviceUri' is required");
            }

            if (String.IsNullOrEmpty(channelName))
            {
                throw new ArgumentException("'channelName' is required");
            }

            if (String.IsNullOrEmpty(key))
            {
                throw new ArgumentException("'key' is required");
            }

            ServiceUri = serviceUri;
            ChannelName = channelName;
            Key = key;
        }


        /// <summary>
        /// Retrieves a robot channel from a Web service
        /// </summary>
        public RobotChannel ReadChannel()
        {
            string effectiveUrl = string.Format(ServiceUri.ToString(), ChannelName, Key);
            var webClient = new WebClient();           
            using (var stream = webClient.OpenRead(effectiveUrl))
            {
                var channelDocument = XDocument.Load(stream);
                return LoadChannel(channelDocument);           
            }

        }

        private RobotChannel LoadChannel(XDocument document)
        {
            var channelElement = document.Root;

            IDictionary<string, string> fieldMap =
                (from element in channelElement.Elements()
                 where element.Name.LocalName.StartsWith("field")
                 select element).ToDictionary(el => el.Name.LocalName, el => el.Value); 
                
            return new RobotChannel()
            {
                Name = channelElement.Element("name").Value,                
                Created = DateTime.Parse(channelElement.Element("created-at").Value),
                Updated = DateTime.Parse(channelElement.Element("updated-at").Value),
                Id = Convert.ToInt32(channelElement.Element("id").Value),
                LastEntryId = Convert.ToInt32(channelElement.Element("last-entry-id").Value),
                Description = channelElement.Element("description").Value,
                Feeds = 
                    from feedElement in channelElement.Element("feeds").Elements("feed")
                    select ReadFeed(feedElement, fieldMap),
            };
        }

        private RobotFeed ReadFeed(XElement feedElement, IDictionary<string, string> fieldMap)
        {
            return new RobotFeed()
            {
                Created = DateTime.Parse(feedElement.Element("created-at").Value),
                EntryId = Convert.ToInt32(feedElement.Element("entry-id").Value),
                Values = fieldMap.ToDictionary(entry => entry.Value, entry => feedElement.Element(entry.Key).Value),                   
            };
        }
        

    }
}
