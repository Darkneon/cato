using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Republic.Cato.MyRobots
{

    /// <summary>
    /// Represents a robot channel
    /// </summary>
    public class RobotChannel
    {
        /// <summary>
        /// The name of the robot / channel
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// The time when the channel was first created
        /// </summary>
        public DateTime Created { get; set; }

        /// <summary>
        /// The time when the channel was last updated
        /// </summary>
        public DateTime Updated { get; set; }

        /// <summary>
        /// The channel ID
        /// </summary>
        public int Id { get; set; }

        /// <summary>
        /// The ID of the last entry
        /// </summary>
        public int LastEntryId { get; set; }

        /// <summary>
        /// The robot / channel description
        /// </summary>
        public string Description { get; set; }

        /// <summary>
        /// A collection of feeds (i.e. data points)
        /// </summary>
        public IEnumerable<RobotFeed> Feeds { get; set; }

    }
}
