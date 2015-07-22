using System;
using System.Collections.Generic;
using System.Text;
using STEPNCLib;
using System.Net;
using System.Net.Sockets;
using System.Configuration;
using System.IO;

namespace Automate_Raw_Workpiece
{
    class Runner
    {
        //outputs a workpiece step file
        //a rawpiece step file
        //and a new stepnc file
        static void Main(string[] args)
        {
            System.Diagnostics.Process computeBoundingBox = new System.Diagnostics.Process();
            computeBoundingBox.StartInfo.FileName = "C:\\Users\\wamsej\\Joel\\RPI\\Internships\\STEP Tools\\Projects\\Compute Bounding Box Project\\Debug\\Compute Bounding Box Project.exe";

            short port_number = 5001;
            string inputStepNCFileName = "test_nokia_cell_phone.238";
            string outputWorkpieceFileName = "workpiece_" + inputStepNCFileName;
            outputWorkpieceFileName = outputWorkpieceFileName.Replace(".238", "");
            string finalFileName = "rawpieceadded_" + inputStepNCFileName;
            AptStepMaker asm1 = new AptStepMaker();
            asm1.Open238(inputStepNCFileName);
            long ID = asm1.GetCurrentWorkpiece();
            asm1.ExportWorkpiece(ID, outputWorkpieceFileName);
            //eventually get rid of this line
            Console.WriteLine(Dns.GetHostEntry("localhost").AddressList[0]);
            IPAddress ipAddress = Dns.GetHostEntry("localhost").AddressList[0];
            TcpListener listener = new TcpListener(ipAddress, port_number);
            listener.Start();

            computeBoundingBox.Start();
            //wait until a request has been made
            while (!listener.Pending());
            Socket soc = listener.AcceptSocket(); //blocks
            //eventually get rid of this line
            Console.WriteLine("Got here (connected)");
            NetworkStream s = new NetworkStream(soc);
            StreamWriter sw = new StreamWriter(s);
            //sw.WriteLine(IPAddress.HostToNetworkOrder((long)outputWorkpieceFileName.Length));
            Console.WriteLine(outputWorkpieceFileName.Length);
            byte[] myWriteBuffer = BitConverter.GetBytes(outputWorkpieceFileName.Length);
            //Array.Reverse(myWriteBuffer);
            s.Write(myWriteBuffer, 0, myWriteBuffer.Length);
            sw.AutoFlush = true;
            sw.WriteLine(outputWorkpieceFileName);
            StreamReader sr = new StreamReader(s);
            string rawMaterialName = sr.ReadLine();
            s.Close();
            soc.Close();

            Console.WriteLine(rawMaterialName);
            Console.ReadLine();
            asm1.Rawpiece(rawMaterialName);
            asm1.SaveAsP21(finalFileName);
        }
    }
}
