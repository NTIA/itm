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
        private const string ITM_x86_DLL_NAME = "itm_x86.dll";
        private const string ITM_x64_DLL_NAME = "itm_x64.dll";

        #region 32-Bit P/Invoke Definitions

        [DllImport(ITM_x86_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_P2P_TLS")]
        private static extern int ITM_TLS_x86(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double time, double location,
            double situation, out double A__db, out long warnings);

        [DllImport(ITM_x86_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_P2P_TLS_Ex")]
        private static extern int ITM_TLS_Ex_x86(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double time, double location,
            double situation, out double A__db, out long warnings, out IntermediateValues interValues);

        [DllImport(ITM_x86_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_P2P_CR")]
        private static extern int ITM_CR_x86(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double confidence,
            double reliability, out double A__db, out long warnings);

        [DllImport(ITM_x86_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_P2P_CR_Ex")]
        private static extern int ITM_CR_Ex_x86(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double confidence,
            double reliability, out double A__db, out long warnings, out IntermediateValues interValues);

        [DllImport(ITM_x86_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_AREA_TLS")]
        private static extern int ITM_AREA_TLS_x86(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double time, double location, double situation, out double A__db, out long warnings);

        [DllImport(ITM_x86_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_AREA_TLS_Ex")]
        private static extern int ITM_AREA_TLS_Ex_x86(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double time, double location, double situation, out double A__db, out long warnings,
            out IntermediateValues interValues);

        [DllImport(ITM_x86_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_AREA_CR")]
        private static extern int ITM_AREA_CR_x86(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double confidence, double reliability, out double A__db, out long warnings);

        [DllImport(ITM_x86_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_AREA_CR_Ex")]
        private static extern int ITM_AREA_CR_Ex_x86(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double confidence, double reliability, out double A__db, out long warnings, out IntermediateValues interValues);

        #endregion

        #region 64-Bit P/Invoke Definitions

        [DllImport(ITM_x64_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_P2P_TLS")]
        private static extern int ITM_TLS_x64(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double time, double location,
            double situation, out double A__db, out long warnings);

        [DllImport(ITM_x64_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_P2P_TLS_Ex")]
        private static extern int ITM_TLS_Ex_x64(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double time, double location,
            double situation, out double A__db, out long warnings, out IntermediateValues interValues);

        [DllImport(ITM_x64_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_P2P_CR")]
        private static extern int ITM_CR_x64(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double confidence,
            double reliability, out double A__db, out long warnings);

        [DllImport(ITM_x64_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_P2P_CR_Ex")]
        private static extern int ITM_CR_Ex_x64(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double confidence,
            double reliability, out double A__db, out long warnings, out IntermediateValues interValues);

        [DllImport(ITM_x64_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_AREA_TLS")]
        private static extern int ITM_AREA_TLS_x64(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double time, double location, double situation, out double A__db, out long warnings);

        [DllImport(ITM_x64_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_AREA_TLS_Ex")]
        private static extern int ITM_AREA_TLS_Ex_x64(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double time, double location, double situation, out double A__db, out long warnings,
            out IntermediateValues interValues);

        [DllImport(ITM_x64_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_AREA_CR")]
        private static extern int ITM_AREA_CR_x64(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double confidence, double reliability, out double A__db, out long warnings);

        [DllImport(ITM_x64_DLL_NAME, CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi, EntryPoint = "ITM_AREA_CR_Ex")]
        private static extern int ITM_AREA_CR_Ex_x64(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double confidence, double reliability, out double A__db, out long warnings, out IntermediateValues interValues);

        #endregion

        private delegate int ITM_TLS_Delegate(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double time, double location,
            double situation, out double A__db, out long warnings);

        private delegate int ITM_TLS_Ex_Delegate(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double time, double location,
            double situation, out double A__db, out long warnings, out IntermediateValues interValues);

        private delegate int ITM_CR_Delegate(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double confidence,
            double reliability, out double A__db, out long warnings);

        private delegate int ITM_CR_Ex_Delegate(double h_tx__meter, double h_rx__meter, [MarshalAs(UnmanagedType.LPArray)] double[] pfl,
            int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma, int mdvar, double confidence,
            double reliability, out double A__db, out long warnings, out IntermediateValues interValues);

        private delegate int ITM_AREA_TLS_Delegate(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double time, double location, double situation, out double A__db, out long warnings);

        private delegate int ITM_AREA_TLS_Ex_Delegate(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double time, double location, double situation, out double A__db, out long warnings,
            out IntermediateValues interValues);

        private delegate int ITM_AREA_CR_Delegate(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double confidence, double reliability, out double A__db, out long warnings);

        private delegate int ITM_AREA_CR_Ex_Delegate(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double confidence, double reliability, out double A__db, out long warnings, out IntermediateValues interValues);

        private static ITM_TLS_Delegate ITM_TLS_Invoke;
        private static ITM_TLS_Ex_Delegate ITM_TLS_Ex_Invoke;
        private static ITM_CR_Delegate ITM_CR_Invoke;
        private static ITM_CR_Ex_Delegate ITM_CR_Ex_Invoke;
        private static ITM_AREA_TLS_Delegate ITM_AREA_TLS_Invoke;
        private static ITM_AREA_TLS_Ex_Delegate ITM_AREA_TLS_Ex_Invoke;
        private static ITM_AREA_CR_Delegate ITM_AREA_CR_Invoke;
        private static ITM_AREA_CR_Ex_Delegate ITM_AREA_CR_Ex_Invoke;

        static ITM()
        {
            if (Environment.Is64BitProcess)
            {
                ITM_TLS_Invoke = ITM_TLS_x64;
                ITM_TLS_Ex_Invoke = ITM_TLS_Ex_x64;
                ITM_CR_Invoke = ITM_CR_x64;
                ITM_CR_Ex_Invoke = ITM_CR_Ex_x64;
                ITM_AREA_TLS_Invoke = ITM_AREA_TLS_x64;
                ITM_AREA_TLS_Ex_Invoke = ITM_AREA_TLS_Ex_x64;
                ITM_AREA_CR_Invoke = ITM_AREA_CR_x64;
                ITM_AREA_CR_Ex_Invoke = ITM_AREA_CR_Ex_x64;
            }
            else
            {
                ITM_TLS_Invoke = ITM_TLS_x86;
                ITM_TLS_Ex_Invoke = ITM_TLS_Ex_x86;
                ITM_CR_Invoke = ITM_CR_x86;
                ITM_CR_Ex_Invoke = ITM_CR_Ex_x86;
                ITM_AREA_TLS_Invoke = ITM_AREA_TLS_x86;
                ITM_AREA_TLS_Ex_Invoke = ITM_AREA_TLS_Ex_x86;
                ITM_AREA_CR_Invoke = ITM_AREA_CR_x86;
                ITM_AREA_CR_Ex_Invoke = ITM_AREA_CR_Ex_x86;
            }
        }

        #region Point-to-Point TLS

        /// <summary>
        /// Point-to-point mode functionality, with variability specified with time/location/situation (TLS)
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="pfl">Terrain data, in PFL format</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="time">Time percentage</param>
        /// <param name="location">Location percentage</param>
        /// <param name="situation">Situation percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <returns>Error code</returns>
        public static int ITM_P2P_TLS(double h_tx__meter, double h_rx__meter, double[] pfl, int climate, double N_0, double f__mhz,
            int pol, double epsilon, double sigma, int mdvar, double time, double location, double situation,
            out double A__db, out long warnings)
        {
            return ITM_TLS_Invoke(h_tx__meter, h_rx__meter, pfl, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar, time,
                location, situation, out A__db, out warnings);
        }

        /// <summary>
        /// Point-to-point mode functionality, with variability specified with time/location/situation (TLS)
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="pfl">Terrain data, in PFL format</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="time">Time percentage</param>
        /// <param name="location">Location percentage</param>
        /// <param name="situation">Situation percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <param name="interValues">Struct of intermediate values</param>
        /// <returns>Error code</returns>
        public static int ITM_P2P_TLS_Ex(double h_tx__meter, double h_rx__meter, double[] pfl, int climate, double N_0, double f__mhz,
            int pol, double epsilon, double sigma, int mdvar, double time, double location, double situation,
            out double A__db, out long warnings, out IntermediateValues interValues)
        {
            return ITM_TLS_Ex_Invoke(h_tx__meter, h_rx__meter, pfl, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar, time,
                location, situation, out A__db, out warnings, out interValues);
        }

        /// <summary>
        /// Point-to-point mode functionality, with variability specified with time/location/situation (TLS)
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="pfl">Terrain data, in PFL format</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="time">Time percentage</param>
        /// <param name="location">Location percentage</param>
        /// <param name="situation">Situation percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <returns>Error code</returns>
        public static int ITM_P2P_TLS(double h_tx__meter, double h_rx__meter, double[] pfl, Climate climate, double N_0, double f__mhz,
            Polarization pol, double epsilon, double sigma, int mdvar, double time, double location, double situation,
            out double A__db, out Warnings warnings)
        {
            int rtn = ITM_P2P_TLS(h_tx__meter, h_rx__meter, pfl, (int)climate, N_0, f__mhz, (int)pol, epsilon, sigma, mdvar, time,
                location, situation, out A__db, out long warns);

            warnings = (Warnings)warns;
            return rtn;
        }

        /// <summary>
        /// Point-to-point mode functionality, with variability specified with time/location/situation (TLS)
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="pfl">Terrain data, in PFL format</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="time">Time percentage</param>
        /// <param name="location">Location percentage</param>
        /// <param name="situation">Situation percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <param name="interValues">Struct of intermediate values</param>
        /// <returns>Error code</returns>
        public static int ITM_P2P_TLS_Ex(double h_tx__meter, double h_rx__meter, double[] pfl, Climate climate, double N_0, double f__mhz,
            Polarization pol, double epsilon, double sigma, int mdvar, double time, double location, double situation,
            out double A__db, out Warnings warnings, out IntermediateValues interValues)
        {
            int rtn = ITM_P2P_TLS_Ex(h_tx__meter, h_rx__meter, pfl, (int)climate, N_0, f__mhz, (int)pol, epsilon, sigma, mdvar, time,
                location, situation, out A__db, out long warns, out interValues);

            warnings = (Warnings)warns;
            return rtn;
        }

        #endregion

        #region Point-to-Point CR

        /// <summary>
        /// Point-to-point mode functionality, with variability specified with confidence/reliability (CR)
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="pfl">Terrain data, in PFL format</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="confidence">Confidence percentage</param>
        /// <param name="reliability">Reliability percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <returns>Error code</returns>
        public static int ITM_P2P_CR(double h_tx__meter, double h_rx__meter, double[] pfl, int climate, double N_0, double f__mhz,
            int pol, double epsilon, double sigma, int mdvar, double confidence, double reliability, out double A__db, out long warnings)
        {
            return ITM_CR_Invoke(h_tx__meter, h_rx__meter, pfl, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar,
                confidence, reliability, out A__db, out warnings);
        }

        /// <summary>
        /// Point-to-point mode functionality, with variability specified with confidence/reliability (CR)
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="pfl">Terrain data, in PFL format</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="confidence">Confidence percentage</param>
        /// <param name="reliability">Reliability percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <param name="interValues">Struct of intermediate values</param>
        /// <returns>Error code</returns>
        public static int ITM_P2P_CR_Ex(double h_tx__meter, double h_rx__meter, double[] pfl, int climate, double N_0, double f__mhz,
            int pol, double epsilon, double sigma, int mdvar, double confidence, double reliability, out double A__db, out long warnings,
            out IntermediateValues interValues)
        {
            return ITM_CR_Ex_Invoke(h_tx__meter, h_rx__meter, pfl, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar,
                confidence, reliability, out A__db, out warnings, out interValues);
        }

        /// <summary>
        /// Point-to-point mode functionality, with variability specified with confidence/reliability (CR)
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="pfl">Terrain data, in PFL format</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="confidence">Confidence percentage</param>
        /// <param name="reliability">Reliability percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <returns>Error code</returns>
        public static int ITM_P2P_CR(double h_tx__meter, double h_rx__meter, double[] pfl, Climate climate, double N_0,
            double f__mhz, Polarization pol, double epsilon, double sigma, int mdvar, double confidence, double reliability,
            out double A__db, out Warnings warnings)
        {
            int rtn = ITM_P2P_CR(h_tx__meter, h_rx__meter, pfl, (int)climate, N_0, f__mhz, (int)pol, epsilon, sigma, mdvar,
                confidence, reliability, out A__db, out long warns);

            warnings = (Warnings)warns;
            return rtn;
        }

        /// <summary>
        /// Point-to-point mode functionality, with variability specified with confidence/reliability (CR)
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="pfl">Terrain data, in PFL format</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="confidence">Confidence percentage</param>
        /// <param name="reliability">Reliability percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <param name="interValues">Struct of intermediate values</param>
        /// <returns>Error code</returns>
        public static int ITM_P2P_CR_Ex(double h_tx__meter, double h_rx__meter, double[] pfl, Climate climate, double N_0,
            double f__mhz, Polarization pol, double epsilon, double sigma, int mdvar, double confidence, double reliability,
            out double A__db, out Warnings warnings, out IntermediateValues interValues)
        {
            int rtn = ITM_P2P_CR_Ex(h_tx__meter, h_rx__meter, pfl, (int)climate, N_0, f__mhz, (int)pol, epsilon, sigma, mdvar,
                confidence, reliability, out A__db, out long warns, out interValues);

            warnings = (Warnings)warns;
            return rtn;
        }

        #endregion

        #region Area TLS

        /// <summary>
        /// Area mode functionality
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="tx_site_criteria">Siting criteria of the TX</param>
        /// <param name="rx_site_criteria">Siting criteria of the RX</param>
        /// <param name="d__km">Path distance, in km</param>
        /// <param name="delta_h__meter">Terrain irregularity parameter</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="time">Time percentage</param>
        /// <param name="location">Location percentage</param>
        /// <param name="situation">Situation percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <returns>Error code</returns>
        public static int ITM_AREA_TLS(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double time, double location, double situation, out double A__db, out long warnings)
        {
            return ITM_AREA_TLS_Invoke(h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km, delta_h__meter,
                climate, N_0, f__mhz, pol, epsilon, sigma, mdvar, time, location, situation, out A__db, out warnings);
        }

        /// <summary>
        /// Area mode functionality
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="tx_site_criteria">Siting criteria of the TX</param>
        /// <param name="rx_site_criteria">Siting criteria of the RX</param>
        /// <param name="d__km">Path distance, in km</param>
        /// <param name="delta_h__meter">Terrain irregularity parameter</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="time">Time percentage</param>
        /// <param name="location">Location percentage</param>
        /// <param name="situation">Situation percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <returns>Error code</returns>
        public static int ITM_AREA_TLS(double h_tx__meter, double h_rx__meter, SitingCriteria tx_site_criteria,
            SitingCriteria rx_site_criteria, double d__km, double delta_h__meter, Climate climate, double N_0,
            double f__mhz, Polarization pol, double epsilon, double sigma, int mdvar, double time, double location,
            double situation, out double A__db, out Warnings warnings)
        {
            int rtn = ITM_AREA_TLS(h_tx__meter, h_rx__meter, (int)tx_site_criteria, (int)rx_site_criteria, d__km, delta_h__meter,
                (int)climate, N_0, f__mhz, (int)pol, epsilon, sigma, mdvar, time, location, situation, out A__db, out long warns);

            warnings = (Warnings)warns;
            return rtn;
        }

        /// <summary>
        /// Area mode functionality
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="tx_site_criteria">Siting criteria of the TX</param>
        /// <param name="rx_site_criteria">Siting criteria of the RX</param>
        /// <param name="d__km">Path distance, in km</param>
        /// <param name="delta_h__meter">Terrain irregularity parameter</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="time">Time percentage</param>
        /// <param name="location">Location percentage</param>
        /// <param name="situation">Situation percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <param name="interValues">Struct of intermediate values</param>
        /// <returns>Error code</returns>
        public static int ITM_AREA_TLS_Ex(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double time, double location, double situation, out double A__db, out long warnings,
            out IntermediateValues interValues)
        {
            return ITM_AREA_TLS_Ex_Invoke(h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km, delta_h__meter,
                climate, N_0, f__mhz, pol, epsilon, sigma, mdvar, time, location, situation, out A__db, out warnings,
                out interValues);
        }

        /// <summary>
        /// Area mode functionality
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="tx_site_criteria">Siting criteria of the TX</param>
        /// <param name="rx_site_criteria">Siting criteria of the RX</param>
        /// <param name="d__km">Path distance, in km</param>
        /// <param name="delta_h__meter">Terrain irregularity parameter</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="time">Time percentage</param>
        /// <param name="location">Location percentage</param>
        /// <param name="situation">Situation percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <param name="interValues">Struct of intermediate values</param>
        /// <returns>Error code</returns>
        public static int ITM_AREA_TLS_Ex(double h_tx__meter, double h_rx__meter, SitingCriteria tx_site_criteria,
            SitingCriteria rx_site_criteria, double d__km, double delta_h__meter, Climate climate, double N_0,
            double f__mhz, Polarization pol, double epsilon, double sigma, int mdvar, double time, double location,
            double situation, out double A__db, out Warnings warnings, out IntermediateValues interValues)
        {
            int rtn = ITM_AREA_TLS_Ex(h_tx__meter, h_rx__meter, (int)tx_site_criteria, (int)rx_site_criteria, d__km,
                delta_h__meter, (int)climate, N_0, f__mhz, (int)pol, epsilon, sigma, mdvar, time, location, situation,
                out A__db, out long warns, out interValues);

            warnings = (Warnings)warns;
            return rtn;
        }

        #endregion

        #region Area CR

        /// <summary>
        /// Area mode functionality
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="tx_site_criteria">Siting criteria of the TX</param>
        /// <param name="rx_site_criteria">Siting criteria of the RX</param>
        /// <param name="d__km">Path distance, in km</param>
        /// <param name="delta_h__meter">Terrain irregularity parameter</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="confidence">Confidence percentage</param>
        /// <param name="reliability">Reliability percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <returns>Error code</returns>
        public static int ITM_AREA_CR(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double confidence, double reliability, out double A__db, out long warnings)
        {
            return ITM_AREA_CR_Invoke(h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km, delta_h__meter,
                climate, N_0, f__mhz, pol, epsilon, sigma, mdvar, confidence, reliability, out A__db, out warnings);
        }

        /// <summary>
        /// Area mode functionality
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="tx_site_criteria">Siting criteria of the TX</param>
        /// <param name="rx_site_criteria">Siting criteria of the RX</param>
        /// <param name="d__km">Path distance, in km</param>
        /// <param name="delta_h__meter">Terrain irregularity parameter</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="confidence">Confidence percentage</param>
        /// <param name="reliability">Reliability percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <returns>Error code</returns>
        public static int ITM_AREA_CR(double h_tx__meter, double h_rx__meter, SitingCriteria tx_site_criteria,
            SitingCriteria rx_site_criteria, double d__km, double delta_h__meter, Climate climate, double N_0,
            double f__mhz, Polarization pol, double epsilon, double sigma, int mdvar, double confidence, double reliability,
            out double A__db, out Warnings warnings)
        {
            int rtn = ITM_AREA_CR(h_tx__meter, h_rx__meter, (int)tx_site_criteria, (int)rx_site_criteria, d__km, delta_h__meter,
                (int)climate, N_0, f__mhz, (int)pol, epsilon, sigma, mdvar, confidence, reliability, out A__db, out long warns);

            warnings = (Warnings)warns;
            return rtn;
        }

        /// <summary>
        /// Area mode functionality
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="tx_site_criteria">Siting criteria of the TX</param>
        /// <param name="rx_site_criteria">Siting criteria of the RX</param>
        /// <param name="d__km">Path distance, in km</param>
        /// <param name="delta_h__meter">Terrain irregularity parameter</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="confidence">Confidence percentage</param>
        /// <param name="reliability">Reliability percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <param name="interValues">Struct of intermediate values</param>
        /// <returns>Error code</returns>
        public static int ITM_AREA_CR_Ex(double h_tx__meter, double h_rx__meter, int tx_site_criteria, int rx_site_criteria,
            double d__km, double delta_h__meter, int climate, double N_0, double f__mhz, int pol, double epsilon, double sigma,
            int mdvar, double confidence, double reliability, out double A__db, out long warnings,
            out IntermediateValues interValues)
        {
            return ITM_AREA_CR_Ex_Invoke(h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km, delta_h__meter,
                climate, N_0, f__mhz, pol, epsilon, sigma, mdvar, confidence, reliability, out A__db, out warnings,
                out interValues);
        }

        /// <summary>
        /// Area mode functionality
        /// </summary>
        /// <param name="h_tx__meter">Structural height of the TX, in meters</param>
        /// <param name="h_rx__meter">Structural height of the RX, in meters</param>
        /// <param name="tx_site_criteria">Siting criteria of the TX</param>
        /// <param name="rx_site_criteria">Siting criteria of the RX</param>
        /// <param name="d__km">Path distance, in km</param>
        /// <param name="delta_h__meter">Terrain irregularity parameter</param>
        /// <param name="climate">Radio climate</param>
        /// <param name="N_0">Refractivity, in N-Units</param>
        /// <param name="f__mhz">Frequency, in MHz</param>
        /// <param name="pol">Polarization</param>
        /// <param name="epsilon">Relative permittivity</param>
        /// <param name="sigma">Conductivity</param>
        /// <param name="mdvar">Mode of variability</param>
        /// <param name="confidence">Confidence percentage</param>
        /// <param name="reliability">Reliability percentage</param>
        /// <param name="A__db">Basic transmission loss, in dB</param>
        /// <param name="warnings">Warning flags</param>
        /// <param name="interValues">Struct of intermediate values</param>
        /// <returns>Error code</returns>
        public static int ITM_AREA_CR_Ex(double h_tx__meter, double h_rx__meter, SitingCriteria tx_site_criteria,
            SitingCriteria rx_site_criteria, double d__km, double delta_h__meter, Climate climate, double N_0,
            double f__mhz, Polarization pol, double epsilon, double sigma, int mdvar, double confidence, double reliability,
            out double A__db, out Warnings warnings, out IntermediateValues interValues)
        {
            int rtn = ITM_AREA_CR_Ex(h_tx__meter, h_rx__meter, (int)tx_site_criteria, (int)rx_site_criteria, d__km,
                delta_h__meter, (int)climate, N_0, f__mhz, (int)pol, epsilon, sigma, mdvar, confidence, reliability,
                out A__db, out long warns, out interValues);

            warnings = (Warnings)warns;
            return rtn;
        }

        #endregion
    }
}
