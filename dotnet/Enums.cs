using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ITS.Propagation
{
    /// <summary>
    /// The Irregular Terrain Model (ITM)
    /// </summary>
    public static partial class ITM
    {
        /// <summary>
        /// Polarization
        /// </summary>
        public enum Polarization : int
        {
            /// <summary>
            /// Horizontal Polarization
            /// </summary>
            Horizontal = 0,

            /// <summary>
            /// Vertical Polarization
            /// </summary>
            Vertical = 1
        }

        /// <summary>
        /// ITM Radio Climate
        /// </summary>
        public enum Climate : int
        {
            /// <summary>
            /// Equatorial
            /// </summary>
            Equatorial = 1,

            /// <summary>
            /// Continental Subtropical
            /// </summary>
            ContinentalSubtropical = 2,

            /// <summary>
            /// Maritime Subtropical
            /// </summary>
            MaritimeSubtropical = 3,

            /// <summary>
            /// Desert
            /// </summary>
            Desert = 4,

            /// <summary>
            /// Continetal Temperate
            /// </summary>
            ContinentalTemperate = 5,

            /// <summary>
            /// Maritime Temperate over Land
            /// </summary>
            MaritimeTemperateOverLand = 6,

            /// <summary>
            /// Maritime Temperate over Sea
            /// </summary>
            MaritimeTemperateOverSea = 7
        }

        /// <summary>
        /// ITM Warning Flags
        /// </summary>
        [Flags]
        public enum Warnings : long
        {
            /// <summary>
            /// No warnings
            /// </summary>
            NoWarnings = 0x0000,

            /// <summary>
            /// TX terminal height is near its limits
            /// </summary>
            TxTerminalHeight = 0x0001,

            /// <summary>
            /// RX terminal height is near its limits
            /// </summary>
            RxTerminalHeight = 0x0002,

            /// <summary>
            /// Frequency is near its limits
            /// </summary>
            Frequency = 0x0004,

            /// <summary>
            /// Path distance is near its upper limit
            /// </summary>
            PathDistanceTooBig1 = 0x0008,

            /// <summary>
            /// Path distance is large - care must be taken with result
            /// </summary>
            PathDistanceTooBig2 = 0x0010,

            /// <summary>
            /// Path distance is near its lower limit
            /// </summary>
            PathDistanceTooSmall1 = 0x0020,

            /// <summary>
            /// Path distance is small - care must be taken with result
            /// </summary>
            PathDistanceTooSmall2 = 0x0040,

            /// <summary>
            /// TX horizon angle is large - small angle approximations could break down
            /// </summary>
            TxHorizonAngle = 0x0080,

            /// <summary>
            /// RX horizon angle is large - small angle approximations could break down
            /// </summary>
            RxHorizonAngle = 0x0100,

            /// <summary>
            /// TX horizon distance is less than 1/10 of the smooth earth horizon distance
            /// </summary>
            TxHorizonDistance1 = 0x0200,

            /// <summary>
            /// RX horizon distance is less than 1/10 of the smooth earth horizon distance
            /// </summary>
            RxHorizonDistance1 = 0x0400,

            /// <summary>
            /// TX horizon distance is greater than 3 times the smooth earth horizon distance
            /// </summary>
            TxHorizonDistance2 = 0x0800,

            /// <summary>
            /// RX horizon distance is greater than 3 times the smooth earth horizon distance
            /// </summary>
            RxHorizonDistance2 = 0x1000,

            /// <summary>
            /// One of the provided variabilities is located far in the tail of its distribution
            /// </summary>
            ExtremeVariabilities = 0x2000
        }

        /// <summary>
        /// Terminal Siting Criteria
        /// </summary>
        public enum SitingCriteria : int
        {
            /// <summary>
            /// Random
            /// </summary>
            Random = 0,

            /// <summary>
            /// Careful
            /// </summary>
            Careful = 1,

            /// <summary>
            /// Very careful
            /// </summary>
            VeryCareful = 2
        }

        /// <summary>
        /// The Mode of Propagation
        /// </summary>
        public enum ModeOfPropagation : int
        {
            /// <summary>
            /// Not Set
            /// </summary>
            NotSet = 0,

            /// <summary>
            /// Line of Sight
            /// </summary>
            LineOfSight = 1,

            /// <summary>
            /// Diffraction
            /// </summary>
            Diffraction = 2,

            /// <summary>
            /// Troposcatter
            /// </summary>
            Troposcatter = 3
        }
    }
}
