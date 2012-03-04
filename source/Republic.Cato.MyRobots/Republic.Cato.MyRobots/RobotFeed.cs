using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Republic.Cato.MyRobots
{
    /// <summary>
    /// Represents a single data point in a robot feed
    /// </summary>
    public class RobotFeed
    {
        /// <summary>
        /// The date and time when the data point was recorded
        /// </summary>
        public DateTime Created { get; set; }

        /// <summary>
        /// A unique ID for this data point
        /// </summary>
        public int EntryId { get; set; }

        /// <summary>
        /// Custom field values
        /// </summary>
        public IDictionary<string, string> Values { get; set; }

        public T GetValue<T>(string name)
        {
            if (!Values.ContainsKey(name)) 
            {
                throw new ArgumentException(String.Format("Field {0} does not exist in the feed"));
            }

            return (T) Convert.ChangeType(Values[name], typeof(T));
        }
    }
}
