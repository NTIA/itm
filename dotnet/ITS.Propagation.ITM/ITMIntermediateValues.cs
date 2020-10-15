using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
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
        /// Intermediate values from ITM
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct IntermediateValues
        {
            /// <summary>
            /// Terminal horizon angles, in radians
            /// </summary>
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public double[] theta_hzn__rad;

            /// <summary>
            /// Terminal horizon distances, in meters
            /// </summary>
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public double[] d_hzn__meter;

            /// <summary>
            /// Terminal effective heights, in meters
            /// </summary>
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
            public double[] h_e__meter;

            /// <summary>
            /// Surface refractivity, in N-Units
            /// </summary>
            public double N_s;

            /// <summary>
            /// Terrain irregularity parameter, in meters
            /// </summary>
            public double delta_h__meter;

            /// <summary>
            /// Reference attenuation, in dB
            /// </summary>
            public double A_ref__db;

            /// <summary>
            /// Free space basic transmission loss, in dB
            /// </summary>
            public double A_fs__db;

            /// <summary>
            /// Path distance, in km
            /// </summary>
            public double d__km;

            /// <summary>
            /// Mode of propagation
            /// </summary>
            public ModeOfPropagation Mode;
        }
    }
}
